//
//  GlanceMask.h
//  Glance_iOS
//
//  Created by Ankit Desai on 3/29/21.
//  Copyright © 2021 Glance Networks, Inc. All rights reserved.
//

@interface GlanceMask : NSObject

-(id)init:(int)left top:(int)top width:(int)width height:(int)height label:(NSString*)label;

-(int)left;
-(int)top;
-(int)width;
-(int)height;
-(NSString *)label;
@end
