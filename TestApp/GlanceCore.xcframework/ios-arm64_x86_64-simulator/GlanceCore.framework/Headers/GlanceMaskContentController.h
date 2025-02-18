//
//  GlanceMaskContentController.h
//  Glance_iOS
//
//  Created by Kyle Shank on 5/12/21.
//  Copyright © 2021 Glance Networks, Inc. All rights reserved.
//

#import <WebKit/WebKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface GlanceMaskContentController : WKUserContentController

-(id)init:(NSString*)querySelectors;
-(id)init:(NSString*)querySelectors labels:(NSString*)labels;
-(id)initWithArray:(NSArray<NSString*>*)querySelectors;
-(id)initWithArray:(NSArray<NSString*>*)querySelectors labels:(NSArray<NSString*>*)labels;

-(id)initWithUserContentController:(WKUserContentController*)userContentController;
-(id)initWithUserContentController:(WKUserContentController*)userContentController querySelectors:(NSString*)querySelectors;
-(id)initWithUserContentController:(WKUserContentController*)userContentController querySelectors:(NSString *)querySelectors labels:(NSString*)labels;
-(id)initWithUserContentControllerAndArray:(WKUserContentController*)userContentController querySelectors:(NSArray<NSString*>*)querySelectors;
-(id)initWithUserContentControllerAndArray:(WKUserContentController*)userContentController querySelectors:(NSArray<NSString*>*)querySelectors labels:(NSArray<NSString*>*)labels;

-(void)setWebView:(WKWebView*)webView;

@end

NS_ASSUME_NONNULL_END
