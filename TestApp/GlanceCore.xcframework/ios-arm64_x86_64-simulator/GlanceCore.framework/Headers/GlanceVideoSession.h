//
//  GlanceVideoSession.h
//  GlanceFramework
//
//  Created by Ankit Desai on 9/19/19.
//  Copyright © 2019 Glance. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <GlanceCore/Glance_iOS.h>

@class GlanceVideoSession;

@interface ConnectError : NSException

-(id)init:(NSString*)name message:(NSString*)message;

@end

@protocol GlanceVideoSessionDelegate <NSObject>

@optional

-(void)glanceVideoSessionDidConnectVideoSource: (GlanceVideoSession*) session;
-(void)glanceVideoSessionDidFailToConnectVideoSource: (GlanceVideoSession*) session error:(NSError*)error NS_SWIFT_NAME(glanceVideoSessionDidFailToConnectVideoSource(_:error:));
-(void)glanceVideoSessionDidDisconnectVideoSource: (GlanceVideoSession*) session;
-(void)glanceVideoSessionDidStartVideoCapture: (GlanceVideoSession*) session;
-(void)glanceVideoSessionDidConnectStreamer: (GlanceVideoSession*) session;
-(void)glanceVideoSessionDidFailConnectStreamer: (GlanceVideoSession*) session error:(NSError*)error;
-(void)glanceVideoSessionDidDisconnnectStreamer: (GlanceVideoSession*) session;
-(void)glanceVideoSessionWillStartStreaming: (GlanceVideoSession*) session;
-(void)glanceVideoSessionWillStopStreaming: (GlanceVideoSession*) session;
-(void)glanceVideoSessionWillChangeQuality: (GlanceVideoSession*) session;
-(void)glanceVideoSessionDidStart: (GlanceVideoSession*) session;
-(void)glanceVideoSessionDidEnd: (GlanceVideoSession*) session;
-(void)glanceVideoSessionInvitation: (GlanceVideoSession*) session sessiontype:(NSString*)sessiontype username:(NSString*)username sessionkey:(NSString*)sesionkey;

#if TARGET_OS_IPHONE
-(void)sessionUICameraManagerDidCaptureImage:(UIImage*)image;
#endif

@end

@interface GlanceVideoSession : NSObject

@property (strong) AVCaptureSession* captureSession;

#if TARGET_OS_MAC
+(void)allowScreenCaptureDevices;
#endif

+(void)setServer:(NSString*)server;

-(id)initWithGroupId:(int)groupId delegate:(id<GlanceVideoSessionDelegate>) delegate;
-(id)initWithUser:(GlanceUser*)user delegate:(id<GlanceVideoSessionDelegate>) delegate;

-(void)start: (GlanceStartParams*)params;
-(void)restart: (GlanceStartParams*)params;
-(void)showDisplay:(GlanceDisplayParams*)displayParams;
-(BOOL)isStarted;
-(void)stop;
-(void)end;
-(void)slower;
-(void)pause;
-(void)unpause;

@end
