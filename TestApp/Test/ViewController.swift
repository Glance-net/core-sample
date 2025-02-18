//
//  ViewController.swift
//  Test
//
//  Created by Felipe Melo on 14/09/23.
//

import UIKit
import GlanceCore

class ViewController: UIViewController {
    
    enum Constants {
        static let buttonSize: CGFloat = 50
    }
    
    lazy var stackView: UIStackView = {
        let stackView = UIStackView()
        stackView.translatesAutoresizingMaskIntoConstraints = false
        stackView.axis = .vertical
        stackView.spacing = 8
        
        return stackView
    }()
    
    lazy var startRandomScreenshareButton: UIButton = {
        buildButton(for: "Start Random Session")
    }()
    
    lazy var startScreenshareButton: UIButton = {
        buildButton(for: "Start Session")
    }()
    
    lazy var startRandomTwoWayVideoButton: UIButton = {
        buildButton(for: "Start Random Two Way Video Session")
    }()
    
    lazy var startTwoWayVideoButton: UIButton = {
        buildButton(for: "Start Two Way Video Session")
    }()
    
    lazy var startPresenceButton: UIButton = {
        buildButton(for: "Start Presence")
    }()
    
    private var params = GlanceVisitorInitParams()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        initialize()
        setupUI()
    }
    
    func setupUI() {
        view.addSubview(stackView)
        
        stackView.addArrangedSubview(startRandomScreenshareButton)
        stackView.addArrangedSubview(startScreenshareButton)
        stackView.addArrangedSubview(startRandomTwoWayVideoButton)
        stackView.addArrangedSubview(startTwoWayVideoButton)
        stackView.addArrangedSubview(startPresenceButton)
        
        NSLayoutConstraint.activate([
            stackView.centerYAnchor.constraint(equalTo: self.view.centerYAnchor),
            stackView.centerXAnchor.constraint(equalTo: self.view.centerXAnchor),
            stackView.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 24),
            stackView.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -24),
            startRandomScreenshareButton.heightAnchor.constraint(equalToConstant: Constants.buttonSize),
            startScreenshareButton.heightAnchor.constraint(equalToConstant: Constants.buttonSize),
            startRandomTwoWayVideoButton.heightAnchor.constraint(equalToConstant: Constants.buttonSize),
            startTwoWayVideoButton.heightAnchor.constraint(equalToConstant: Constants.buttonSize),
            startPresenceButton.heightAnchor.constraint(equalToConstant: Constants.buttonSize),
        ])
        
        let randomScreenshareGesture = UITapGestureRecognizer(target: self, action: #selector(startRandomSession))
        startRandomScreenshareButton.addGestureRecognizer(randomScreenshareGesture)
        
        let screenshareGesture = UITapGestureRecognizer(target: self, action: #selector(startSession))
        startScreenshareButton.addGestureRecognizer(screenshareGesture)
        
        let randomTwoWayVideoGesture = UITapGestureRecognizer(target: self, action: #selector(startRandomTwoWayVideoSession))
        startRandomTwoWayVideoButton.addGestureRecognizer(randomTwoWayVideoGesture)
        
        let twoWayVideoGesture = UITapGestureRecognizer(target: self, action: #selector(startTwoWayVideoSession))
        startTwoWayVideoButton.addGestureRecognizer(twoWayVideoGesture)
        
        let presenceGesture = UITapGestureRecognizer(target: self, action: #selector(startPresence))
        startPresenceButton.addGestureRecognizer(presenceGesture)
    }
    
    @objc func startRandomSession() {
        params.video = false
        params.visitorid = nil
        // Glance.initVisitor(params: params)
        // Glance.startSession
    }
    
    @objc func startSession() {
        params.video = false
        params.visitorid = nil
        // Glance.initVisitor(params: params)
        // Glance.startSession
    }
    
    @objc func startRandomTwoWayVideoSession() {
        params.video = true
        params.visitorid = nil
        // Glance.initVisitor(params: params)
        // Glance.startSession
    }
    
    @objc func startTwoWayVideoSession() {
        params.video = true
        params.visitorid = nil
        // Glance.initVisitor(params: params)
        // Glance.startSession
    }
    
    @objc func startPresence() {
        params.visitorid = "666666"
        params.screenshare = true
        // Glance.initVisitor(params: params)
        // Glance.startSession
    }
    
    private func buildButton(for title: String) -> UIButton {
        let button = UIButton(type: .system)
        button.setTitle(title, for: .normal)
        button.backgroundColor = .systemBlue
        button.tintColor = .white
        button.layer.cornerRadius = 12
        
        return button
    }
    
}

extension ViewController {
    func initialize() {
        // Change to your credentials
        params = GlanceVisitorInitParams()
        params.groupid = 21489
        params.visitorid = "666666"
        params.screenshare = true
        params.cameras = "\"front\",\"back\""
        params.site = "production"
        params.displayParams = GlanceDisplayParams()
        
        
        GlanceVisitor.add(GlanceManager.shared)
        GlanceVisitor.setCustomViewerDelegate(GlanceManager.shared)
        GlanceManager.shared.params = params
        
        DispatchQueue.main.async {
            GlanceCore.setup(self.params)
            // Using GlanceCore can lead to unexpected behaviours since the new API should've been used, in that case, might be needed to use Glance instead of GlanceCore
            // Glance.initVisitor(params: self.params)
        }
    }
}

final class GlanceManager: NSObject, GlanceVisitorDelegate, SessionUIDelegate, GlanceCustomViewerDelegate {
    static let shared = GlanceManager()
    var webSocket = GlanceWebSocket()
    var params = GlanceVisitorInitParams()
    var glanceStartParams = GlanceStartParams()
    
    func glanceVisitorEvent(_ event: GlanceEvent!) {
        switch event.code {
        case EventVisitorInitialized:
            DispatchQueue.main.async { [weak self] in
                guard let self else { return }
                GlancePresenceVisitor.connect(self, maxAttempts: -1)
                self.webSocket.setup(startParams: self.params)
            }
        case EventPresenceConnected:
            GlancePresenceVisitor.presence(["url": "root view"])
        case EventPresenceDisconnected:
            print("disconnected!")
        case EventPresenceShowTerms:
            self.webSocket.send(message: .terms, data: ["status": "displayed"])
            self.webSocket.send(message: .terms, data: ["status": "accepted"])
            self.webSocket.send(message: .visitorsessionstart)
            
            glanceStartParams?.presenceStart = true
            glanceStartParams?.showTerms = true
            glanceStartParams?.key = self.params.visitorid
            glanceStartParams?.displayParams = self.params.displayParams
            glanceStartParams?.displayParams?.displayName = ""
            
            let videoMode = event.properties["video"] as? String ?? ""
            glanceStartParams?.video = if videoMode == "large" {
                VideoLargeMultiway
            } else if videoMode != "videooff" && videoMode != "off" && videoMode != "" {
                VideoSmallMultiway
            } else {
                VideoOff
            }
            
            if let glanceStartParams {
                DispatchQueue.main.async {
                    GlanceCore.startSession(glanceStartParams)
                    // Using GlanceCore can lead to unexpected behaviours since the new API should've been used, in that case, might be needed to use Glance instead of GlanceCore
                    // Glance.startSession(params: glanceStartParams)
                }
            }
            
        default:
            return
        }
    }
    
     func sessionUIVoiceAuthenticationRequired() {}
     func sessionUIVoiceAuthenticationFailed() {}
     func sessionUIDidError(_ error: Error!) {}
     func sessionUIDialogAccepted() {}
     func sessionUIDialogCancelled() {}
     func glanceViewerDidStart(_ glanceView: UIView, size: CGSize) {}
     func glanceViewerDidStop(_ glanceView: UIView) {}
     func glanceViewerIsStopping(_ glanceView: UIView) {}
     func glanceViewerIsStarting(_ glanceView: UIView) {}
}
