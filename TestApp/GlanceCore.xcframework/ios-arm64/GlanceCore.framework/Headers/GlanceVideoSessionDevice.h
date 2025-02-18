//
//  GlanceVideoSessionDevice.h
//  GlanceFramework
//
//  Created by Ankit Desai on 9/23/19.
//  Copyright © 2019 Glance. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface GlanceVideoSessionDevice : NSObject

@property (retain) AVCaptureDevice* videoDevice;
@property (retain) AVCaptureDeviceFormat* videoFormat;

-(id)init: (NSString*)name width:(int)width height:(int)height;
-(id)init: (AVCaptureDevice*)videoDevice videoFormat:(AVCaptureDeviceFormat*)videoFormat;
-(AVCaptureDeviceFormat*)getVideoFormat;
-(NSString*)modelID;
-(NSString*)localizedName;
-(void)resetDeviceFormat:(int)width height:(int)height;
+(CMVideoDimensions)getVideoDimensions:(AVCaptureDeviceFormat*)format;
+(CMVideoDimensions)getVideoDimensionsFromFormatDescription:(CMFormatDescriptionRef)formatDescription;

@end

@interface GlanceVideoSessionDeviceNotFoundError : NSException

-(id)init:(NSString*)name;

@end
