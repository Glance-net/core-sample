//
//  SessionUICameraManager.h
//  Glance_iOS
//
//  Created by Ankit Desai on 8/5/22.
//  Copyright © 2022 Glance Networks, Inc. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@class SessionUICameraManager;

static NSNotificationName const _Nonnull GlanceNoCameraPermission = @"noCamera";
static NSNotificationName const _Nonnull GlanceGoToSettings = @"goToSettings";

@protocol SessionUICameraManagerDelegate <NSObject>
-(void)sessionUICameraManagerDidStartVideoCapture:(nonnull SessionUICameraManager*)instance;
-(void)sessionUICameraManagerDidStopVideoCapture:(nonnull SessionUICameraManager*)instance;
@end

@interface SessionUICameraManager : NSObject

-(nullable instancetype)initWithDelegate:(id<SessionUICameraManagerDelegate> _Nonnull)delegate;

-(void)startCaptureSession;
-(void)stopCaptureSession;
-(void)endCaptureSession;

- (void) checkCameraStatus:(void (^_Nonnull)(void))doWork;
@property (strong) AVCaptureSession* _Nullable captureSession;

@end
