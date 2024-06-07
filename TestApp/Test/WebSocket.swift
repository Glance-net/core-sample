//
//  GlanceWebSocket.swift
//
//
//  Created by Felipe Melo on 19/01/24.
//

import Foundation
import GlanceCore

final class GlanceWebSocket: NSObject {
    
    public var isConnected = false
    
    private var webSocket : URLSessionWebSocketTask?
    private var initParams: GlanceVisitorInitParams?
    private var receiveThread: DispatchWorkItem?
    private var reconnectThread: DispatchWorkItem?
    
    func setup(startParams: GlanceVisitorInitParams) {
        self.initParams = startParams
        
        disconnect()
        connect()
    }
    
    func connect() {
        //Session
        let session = URLSession(configuration: .default, delegate: self, delegateQueue: OperationQueue())
        
        //Server API
        if let url = URL(string: "wss://\(GlancePresenceVisitor.getPresenceServer() ?? "")/visitorws") {
            //Socket
            webSocket = session.webSocketTask(with: url)
            
            //Connect and handles handshake
            webSocket?.resume()
        }
    }
    
    func disconnect() {
        DispatchQueue.global().async { [weak self] in
            self?.receiveThread?.cancel()
            self?.receiveThread = nil
            
            self?.closeSession()
            self?.webSocket = nil
            self?.isConnected = false
            self?.reconnectThread?.cancel()
        }
    }
    
    func reconnect() {
        connect()
        
        reconnectThread = DispatchWorkItem{ [weak self] in
            guard let self else { return }
            if !self.isConnected {
                self.reconnect()
            }
        }
        
        if let reconnectThread {
            DispatchQueue.global().asyncAfter(deadline: .now() + 10, execute: reconnectThread)
        }
    }
    
    func convertToDictionary(text: String) -> [String: Any]? {
        if let data = text.data(using: .utf8) {
            do {
                return try JSONSerialization.jsonObject(with: data, options: []) as? [String: Any]
            } catch {
                print(error.localizedDescription)
            }
        }
        return nil
    }
}

extension GlanceWebSocket: URLSessionWebSocketDelegate {
    func receive(){
        /*
         - Create a workItem
         - Add it to the Queue
         */
        
        receiveThread = DispatchWorkItem{ [weak self] in
            
            self?.webSocket?.receive(completionHandler: { [weak self] result in
                GlancePresenceVisitor.connect(GlanceManager.shared, maxAttempts: -1)
                switch result {
                case .success(let message):
                    
                    switch message {
                        
                    case .data(let data):
                        print("Data received \(data)")
                        
                    case .string(let strMessage):
                        print("String received \(strMessage)")
                        var dict = self?.convertToDictionary(text: strMessage)
                        
                        var data = ""
                        if let data1 = (dict?["data"] as? [String: String])?.description {
                            data = data1
                        } else if var data2 = (dict?["data"] as? [String: Any]) {
                            let args = (data2["args"] as? [String: String])?.description
                            data2["args"] = args
                            data = data2.description
                        }
                        data = data.replacingOccurrences(of: "[", with: "{")
                        data = data.replacingOccurrences(of: "]", with: "}")
                        dict?["data"] = data
                        
                        if let type = dict?["type"] {
                            dict?["mtype"] = type
                        }
                        
                        GlancePresenceVisitor.handleMessage(dict)
                        
                    default:
                        break
                    }
                    
                case .failure(let error):
                    return
                }
                
                self?.receive()
            })
        }
        
        if let receiveThread {
            DispatchQueue.global().asyncAfter(deadline: .now(), execute: receiveThread)
        }
    }
    
    //MARK: Send
    func send(message: WSMessage, data: [String: String] = [:]) {
        /*
         - Create a workItem
         - Add it to the Queue
         */
        
        let workItem = DispatchWorkItem {
            if let initParams = self.initParams {
                let data = Data(message.buildMessage(initParams: initParams, data: data).utf8)
                let wsMessage = URLSessionWebSocketTask.Message.data(data)
                self.webSocket?.send(wsMessage, completionHandler: { error in
                    if error != nil {
                        print(error)
                    }
                })
            }
        }
        
        DispatchQueue.global().asyncAfter(deadline: .now(), execute: workItem)
    }
    
    //MARK: Close Session
    @objc func closeSession() {
        webSocket?.cancel(with: .goingAway, reason: "You've Closed The Connection".data(using: .utf8))
    }
    
    //MARK: URLSESSION Protocols
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didOpenWithProtocol protocol: String?) {
        isConnected = true
        reconnectThread?.cancel()
        receive()
        send(message: .connect)
    }
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didCloseWith closeCode: URLSessionWebSocketTask.CloseCode, reason: Data?) {
        if isConnected {
            isConnected = false
            reconnect()
        }
    }
}

enum WSMessage: String {
    case connect
    case terms
    case visitorsessionstart
    
    func buildMessage(initParams: GlanceVisitorInitParams, data: [String: String] = [:]) -> String {
        guard let visitorid = initParams.visitorid else { return "" }
        var message = ""
        switch self {
        case .terms:
            message = "{\"groupid\":\(initParams.groupid), \"type\": \"\(self.rawValue)\", \"version\":\"6.8.6.0\", \"visitorid\":\"\(visitorid)\""
        default:
            message = "{\"groupid\":\(initParams.groupid), \"type\": \"\(self.rawValue)\", \"version\":\"6.8.6.0\", \"visitorid\":\"\(visitorid)\""
        }
        
        if !data.isEmpty {
            var data = data.description
            data = data.replacingOccurrences(of: "[", with: "{")
            data = data.replacingOccurrences(of: "]", with: "}")
            
            message += ", \"data\":\(data.description)"
        }
        
        message += "}"
        
        return message
    }
}
