//  Copyright (c) 2014-2017 Glance Networks, Inc. All rights reserved.
//
//  Glance iOS Framework - Glance_iOS.h
//
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <GlanceCore/GlanceMask.h>

@protocol GlancePresenceDialogDelegate
-(void)glancePresenceDialogDidTapNo;
-(void)glancePresenceDialogDidTapYes;
-(void)glancePresenceDialogDidTapTerms;
@end

//! Project version number for Glance_iOS.
FOUNDATION_EXPORT double Glance_iOSVersionNumber;

//! Project version string for Glance_iOS.
FOUNDATION_EXPORT const unsigned char Glance_iOSVersionString[];

// Events -----------------------------------------------------------------------------------------

typedef enum EventCode : NSUInteger
{
    EventNone
    ,EventInvalidParameter      ///< An invalid input parameter was passed into an API method
    ,EventInvalidState          ///< There was an attempt to invoke an Action that was not valid in current state
    ,EventLoginSucceeded        ///< User login succeeded
    ,EventLoginFailed           ///< User login failed
    ,EventPrivilegeViolation    ///< There was an attempt to do something that is not permitted by the User's privileges
    ,EventInvalidWebserver      ///< Attempt to direct the client to connect to a webserver that the client has not been configured to allow
    ,EventUpgradeAvailable      ///< An upgrade to the client library is available
    ,EventUpgradeRequired       ///< An upgrade to the client library is required
    ,EventCompositionDisabled   ///< Desktop composition was disabled (Windows Vista only)
    ,EventConnectedToSession    ///< Client successfully connected to a session, or reconnected after changing roles (display to viewer or vice versa)
    ,EventSwitchingToView       ///< A viewer participant in the session has started showing his screen, so the display is switching to view
    ,EventStartSessionFailed    ///< An attempt to start a new session was unsuccessful
    ,EventJoinFailed            ///< An attempt to join a session was unsuccessful
    ,EventSessionEnded          ///< Session endded
    ,EventFirstGuestSession     ///< A guest's first-ever session just ended, prompt user to uninstall
    ,EventTunneling             ///< Connection is tunneling
    ,EventRestartRequired       ///< To complete the driver install, a machine restart is required
    ,EventConnectionWarning     ///< Connectivity has been lost
    ,EventClearWarning          ///< Connectivity has been restored
    ,EventGuestCountChange      ///< The guest count changed
    ,EventViewerClose           ///< User clicked the viewer window close button
    ,EventActionsChange         ///< The set of allowed actions changed (NOT IMPLEMENTED)
    ,EventDriverInstallError    ///< There was an error installing Glance SpeedBoost.
    ,EventRCDisabled            ///< Remote control has been disabled.
    ,EventDeviceDisconnected    ///< Captured (video) device disconnected.  Can be reconnected and session continued.
    ,EventDeviceReconnected     ///< Captured (video) device reconnected.
    ,EventGesture               ///< A gesture was received.
    ,EventException             ///< Unexpected exception
    ,EventScreenshareInvitation ///< A screenshare session invitation received from another session participant
    ,EventMessageReceived       ///< A user message was received from another session participant
    ,EventChildSessionStarted   ///< A child session started.
    ,EventChildSessionEnded     ///< A child session ended.
    ,EventJoinChildSessionFailed///< An invitation was received to join a child session was received but the session could not be found
    ,EventVisitorInitialized    ///< Visitor API successfully initialized
    ,EventPresenceConnected     ///< Visitor connected to presence service
    ,EventPresenceConnectFail   ///< Visitor failed to connect to presence service
    ,EventPresenceShowTerms     ///< The agent signaled the visitor to show terms and conditions
    ,EventPresenceStartSession  ///< The agent signaled the visitor to start a session
    ,EventPresenceSignal        ///< The agent signaled the visitor
    ,EventPresenceBlur          ///< Another application connected to presence with the same visitor id
    ,EventPresenceSendFail      ///< Failed to send a message on the presence connection due to connection drop
    ,EventPresenceNotConfigured ///< Presence is not configured or not enabled for the group
    , EventPresenceDisconnected ///< The application has disconnected from the presence service
    , EventPresenceStartVideo    ///< The agent signaled the visitor to start a video session
    , EventVisitorVideoRequested ///< The agent has requested visitor video access
} EventCode;

typedef enum EventType : NSUInteger
{
    EventTypeNone,
    EventInfo,
    EventWarning,
    EventError,
    EventAssertFail
} EventType;

@interface GlanceEvent: NSObject
{
    EventCode           code;
    EventType           type;
    NSString        *   message;
    NSDictionary    *   properties;
}
@property (readonly) EventCode           code;
@property (readonly) EventType           type;
@property (readonly, retain) NSString        * _Null_unspecified   message;
@property (readonly, retain) NSDictionary    * _Nonnull  properties;

- (id)initWithCode:(EventCode)c type:(EventType)t message:(NSString *)msg properties:(NSDictionary *)props;
- (NSString *_Nonnull)getProperty:(NSString *_Nonnull)key;
@end


@interface GlanceBase : NSObject
{
    id      evtTarget;
    SEL     evtSelector;
}

- (void)onEventNotifyTarget:(id)target selector:(SEL)selector;

@end


// User Settings and Privileges -------------------------------------------------------------------

@interface GlanceSettings : GlanceBase
{
    void * gs;
}
- (id)init;
- (void)dealloc;

- (void)set:(NSString *)name value:(NSString *)value;
- (NSString *)get:(NSString *)name;

@end

// Settings
extern NSString * const kGlanceSettingGlanceServer;
extern NSString * const kGlanceSettingGlanceAddress;
extern NSString * const kGlanceSettingPassword;
extern NSString * const kGlanceSettingSessionKey;
extern NSString * const kGlanceSettingForceAJAX;
extern NSString * const kGlanceSettingEncrypt;
extern NSString * const kGlanceSettingEnableShowback;
extern NSString * const kGlanceSettingGIFlags;
extern NSString * const kGlanceSettingGlanceGroup;
extern NSString * const kGlanceSettingEnableAgentVideo;

// User and Group level privileges
extern NSString * const kGlancePrivRCAllowed;
extern NSString * const kGlancePrivReverseAllowed;
extern NSString * const kGlancePrivShowbackAllowed;
extern NSString * const kGlancePrivSetGIFlagsAllowed;
extern NSString * const kGlancePrivKeylessAllowed;
extern NSString * const kGlancePrivVideoAllowed;
extern NSString * const kGlancePrivForceEncryption;
extern NSString * const kGlancePrivNumGuestsAllowed;
extern NSString * const kGlancePrivShowAllowed;

// User Subscription attributes
extern NSString * const kGlanceSubIsTrial;
extern NSString * const kGlanceSubPlanTitle;
extern NSString * const kGlanceSubPlanTimeout;


// User  ------------------------------------------------------------------------------------------

@interface GlanceCredentials : NSObject
{
@public
    NSString *  username;
    int         partnerId;
    NSString *  partnerUserId;
    NSString *  loginkey;
    NSString *  gssnid;
}
@property (retain) NSString * username;
@property int partnerId;
@property (retain) NSString * partnerUserId;
@property (retain) NSString * loginkey;
@property (retain) NSString * gssnid;

- (id)  init;
- (void)dealloc;

@end

@interface GlanceUser : GlanceBase
{
    void * user;
}

- (id)  init;
- (void)dealloc;

- (void)            authenticate;
- (void)            authenticateWithUsername:(NSString *)username password:(NSString *)password;
- (void)            authenticateWithUsername:(NSString *)username key:(NSString *)loginkey;
- (void)            authenticateWithPartnerId:(int)partnerId userId:(NSString *)partnerUserId key:(NSString* )loginkey;
- (bool)            authenticated;
- (NSString *)      getAccountSetting:(NSString *)name;
- (void)            logout;
- (GlanceCredentials *)getCredentialsValidForSeconds:(int)requiredTime error:(NSString **)err;

@end

// Session Parameters and Info --------------------------------------------------------------------

@interface GlanceDisplayParams : NSObject
{
@public
    NSString *              displayName; // Either a number, or the name of a video input device
    float                   scale;
    int                     captureWidth;
    int                     captureHeight;
    bool                    video;
}
@property (retain) NSString * displayName;
@property float scale;
@property int captureWidth;
@property int captureHeight;

- (id)init;
- (void)dealloc;
@end


typedef enum GuestInfoFlags : UInt32
{
    GuestNameRequired  = 0x0200,
    GuestEmailRequired = 0x0400,
    GuestPhoneRequired = 0x0800,
    GuestNameHidden    = 0x1000,
    GuestEmailHidden   = 0x2000,
    GuestPhoneHidden   = 0x4000
} GuestInfoFlags;


@interface GlanceWebViewMask : NSObject

@property (retain) NSString * querySelector;
@property (retain) NSString * label;

-(id)initWithQuerySelector:(NSString*)qs;
-(id)initWithQuerySelector:(NSString*)qs andLabel:(NSString*)label;
-(void)dealloc;
@end

typedef enum GlanceVideoMode: NSUInteger
{
    VideoOff,
    VideoSmallVisitor,
    VideoLargeVisitor,
    VideoSmallMultiway,
    VideoLargeMultiway
} GlanceVideoMode;

@interface GlanceStartParams : NSObject
{
@public
    unsigned long           mainCallId;
    short                   maxGuests;
    bool                    show;
    unsigned long           guestInfoFlags;
    bool                    encrypt;
    NSString *              key;
    GlanceDisplayParams *   displayParams;
    bool                    requestRC;
    bool                    instantJoin;
    bool                    forceTunnel;
    bool                    viewerCloseable;
    bool                    reportErrors;
    bool                    persist;
    bool                    paused;
    bool                    presenceStart;
    // SessionUI specific start params, which do not require _Glance::StartParams implementation:
    bool                    showTerms;
    GlanceVideoMode         video;
    NSString *              termsUrl;
}

@property          unsigned long            mainCallId;
@property          short                    maxGuests;
@property          bool                     show;
@property          unsigned long            guestInfoFlags;
@property (retain) NSString *               key;
@property (retain) GlanceDisplayParams *    displayParams;
@property          bool                     requestRC;
@property          bool                     reportErrors;
@property          bool                     persist;
@property          bool                     paused;
@property          bool                     presenceStart;
// SessionUI specific start params, which do not require _Glance::StartParams implementation:
// There are 3 ways to currently start a session
// 1) User initiates via the application, shown start dialog, must accept to start
// 2) Presence initiated via agent, user shown start dialog, must accept to start
// 3) Presence directly starts via agent, user NOT shown start dialog, session starts immediately
// Both cases 2) & 3) will return presenceStart to be true, but showTerms allows us to differentiate between the two
// Why do we need to differentiate? Case 2) operates via the normal flow.
// Case 3) requires us to NOT show the initial start dialog, but show the allow visitor video dialog after the agent has joined
@property          bool                     showTerms;
@property          GlanceVideoMode          video;
@property (retain) NSString *               termsUrl;

- (id)init;
- (void)dealloc;
@end

@interface GlanceJoinParams : NSObject
{
@public
    unsigned long           guestID;
    bool                    decline;        // join only for the purpose of notifying host of decline
    NSString *              guestName;
    NSString *              guestPhone;
    NSString *              guestEmail;
    bool                    forceTunnel;
    bool                    viewerCloseable;
}
@property          unsigned long    guestID;
@property          bool             decline;
@property          bool             forceTunnel;
@property          bool             viewerCloseable;
@property (retain) NSString *       guestName;
@property (retain) NSString *       guestPhone;
@property (retain) NSString *       guestEmail;

- (id)init;
- (void)dealloc;
@end

@interface GlanceSessionInfo : NSObject
{
@public
    unsigned long   callId;
    int             nGuests;
    NSString *      glanceAddress;
    NSString *      key;
    NSString *      hostName;
    bool            isGuest;
    bool            isReverse;
    bool            rcEnabled;
    bool            sbEnabled;
    bool            isPaused;
    bool            isConnected;
    bool            isShowing;
    bool            isViewing;
    NSString *      displayName;
    NSString *      offerURL;
}
@property (readonly) unsigned long   callId;
@property (readonly) int             nGuests;
@property (readonly) NSString *      glanceAddress;
@property (readonly) NSString *      key;
@property (readonly) NSString * _Nonnull      hostName;
@property (readonly) bool            isGuest;
@property (readonly) bool            isReverse;
@property (readonly) bool            rcEnabled;
@property (readonly) bool            sbEnabled;
@property (readonly) bool            isPaused;
@property (readonly) bool            isConnected;
@property (readonly) bool            isShowing;
@property (readonly) bool            isViewing;
@property (readonly) NSString * _Nullable      displayName;
@property (readonly) NSString * _Nullable      offerURL;
@end


typedef enum GlanceAction : NSUInteger
{
    ActionNone
    ,ActionStartShow                ///< Start a session to show screen
    ,ActionStartView                ///< Start a session to view screen
    ,ActionShowDisplay              ///< Show a different screen or device
    ,ActionPause                    ///< Pause a session which is currently showing.
    ,ActionUnpause                  ///< Unpause a session
    ,ActionFreeze                   ///< Freeze a session which is currently showing.
    ,ActionUnfreeze                 ///< Unfreeze a session
    ,ActionEnableRemoteControl      ///< Enable remote control, allowing other side to control screen
    ,ActionDisableRemoteControl     ///< Disable remote control
    ,ActionEnableShowback           ///< Enable showback for the session, allowing guests to show their screens
    ,ActionDisableShowback          ///< Disable showback
    ,ActionEnableGestures           ///< Enable gesturing for the session, allowing guests to draw on the display side screen
    ,ActionDisableGestures          ///< Disable gesturing
    ,ActionEnd                      ///< End the session
} GlanceAction;

typedef NS_ENUM(NSInteger, SessionStartDialogUIViewState) {
    SessionStartDialogUIViewStateHidden,
    SessionStartDialogUIViewStateAppShareOnly,
    SessionStartDialogUIViewStateAppShareAndVideo,
    SessionStartDialogUIViewStateTerms,
    SessionStartDialogUIViewStateInQueue,
    SessionStartDialogUIViewStateInQueueAndCode,
    SessionStartDialogUIViewStateInQueueVideoAndCode,
    SessionStartDialogUIViewStateInSessionAddVideo,
    SessionStartDialogUIViewStateEndSession
};

// REMOVE
@protocol SessionStartDialogUIViewDelegate
-(void)sessionStartDialogDidTapAccept:(BOOL)visitorVideoEnabled;
-(void)sessionStartDialogDidTapCancel;
-(void)sessionStartDialogDidTapTerms;
-(void)sessionStartDialogDidEndSession;
-(void)sessionStartDialogDidAddVideo:(BOOL)visitorVideoEnabled;
@end

@protocol GlanceSDKDelegate
-(void)sessionConnected:(NSString*)sessionKey;
-(void)receivedSessionCode:(NSString*)sessionCode;
-(void)sessionAgentConnected;
-(void)sessionEnded;
-(void)pausedVisitorVideo;
-(void)unpausedVisitorVideo;

-(void)receivedVideoCaptureImage:(UIImage*)image;
-(void)didStartVideoCapture:(AVCaptureSession*)captureSession;
-(void)didStopVideoCapture:(AVCaptureSession*)captureSession;

-(void)didStartAgentViewer:(UIView*)glanceAgentViewer;
-(void)didStopAgentViewer;

//-(void)start:(GlanceStartParams*)sparams groupId:(int)groupId server:(NSString*)glanceServer;
//-(void)showStartDialog:(GlanceStartParams*)sparams delegate:(id<SessionStartDialogUIViewDelegate>)delegate;
//-(void)setStartDialogDelegate:(id<SessionStartDialogUIViewDelegate>)delegate;
//-(void)hideStartDialog;
//-(void)showAddVideoStartDialog;
//-(void)showStartDialogWithState:(SessionStartDialogUIViewState)state;
//-(void)processUserMessage:(NSDictionary*)properties;
//
//-(void)sessionConnected:(NSString*)sessionKey;
//-(void)sessionAgentConnected:(BOOL)hideDialog;
//-(void)sessionAgentConnected;
//-(void)sessionEnded;
//-(void)pauseVisitorVideo;
//-(void)unpauseVisitorVideo;
@end

typedef enum GlanceDisplayType :NSUInteger { Monitor, Device } GlanceDisplayType;

// Sessions ---------------------------------------------------------------------------------------

///**
// * A delegate to manage the agent viewer video experience.
// */
@protocol GlanceCustomViewerDelegate

/**
 * Called when the agent viewer starts with a supplied UIView and size.
 *
 * @param glanceView    UIView displaying agent video.  Add this view to your interface.
 * @param size          Preferred size of the UIView
 */
-(void) glanceViewerDidStart:(UIView*_Nonnull)glanceView size:(CGSize)size;

/**
 * Called when the agent viewer has stopped
 *
 * @param glanceView    UIView displaying agent video.  Remove this view from your interface.
 */
-(void) glanceViewerDidStop:(UIView*_Nonnull)glanceView;

/**
 * Called when the agent viewer is stopping. Can be one of two situations: agent is pausing video, or ending video.
 * If ending, this will be followed by glanceViewerDidStop delegate method
 *
 * @param glanceView    UIView displaying agent video.  Remove this view from your interface.
 */
-(void) glanceViewerIsStopping:(UIView*_Nonnull)glanceView;

/**
 * Called when the agent viewer is starting. Can be one of two situations: agent is unpausing video, or starting new video.
 * If starting new, this will be followed by glanceViewerDidStart delegate method
 *
 * @param glanceView    UIView displaying agent video.  Add this view to your interface.
 */
-(void) glanceViewerIsStarting:(UIView*_Nonnull)glanceView;
@end

@interface GlanceSession : GlanceBase
{
}

- (id)      init;
- (void)    dealloc;

// Display information
+ (int)                 getDisplayCount;         // total number of currently connected monitors and devices
+ (int)                 getMainMonitor;
+ (void)                identifyMonitors;       // draws a number on each monitor (@@@efh not implemented on Mac)
+ (GlanceDisplayType)   getTypeOfDisplay:(int)n;
+ (NSString *)          getNameOfDisplay:(int)n;

- (bool)                canInvokeAction:(GlanceAction)action;
- (void)                invokeAction:(GlanceAction)action;
- (GlanceSessionInfo *) getSessionInfo;
- (void)                end;

- (void)                showDisplay:(GlanceDisplayParams *)dParams;
@end


@interface GlanceHostSession : GlanceSession
{
    void * hs;
}

- (id)  initWithUser:(GlanceUser *)u;
- (void)dealloc;

- (void)                show:(GlanceStartParams *)sp;
- (void)                view:(GlanceStartParams *)sp;

- (void)                setMaskedViews:(NSSet *)masked;
- (void)                addMaskedViews:(NSSet *)masked;
- (void)                addMaskedView:(UIView *)v;
- (void)                removeMaskedView:(UIView *)v;
- (void)                maskKeyboard:(BOOL)maskKeyboard;

@end

@interface GlanceGuestSession : GlanceSession
{
    void * gs;
}

@property CGSize maxWindowSize;
@property UIViewContentMode contentMode;

- (void)dealloc;
- (void)setCustomViewerDelegate:(id<GlanceCustomViewerDelegate>_Nonnull)delegate;
- (GlanceSessionInfo *)lookup:(NSString*_Nonnull)address key:(NSString*)key;
- (void)join:(NSString*)address key:(NSString*_Nonnull)key joinParams:(GlanceJoinParams*)joinParams;
@end


#define GLANCE_WINDOW_TAG 1167

@class GlanceVideoDefaultUI;
@protocol GlanceDefaultUIDelegate; // forward declarations
@protocol GlanceVideoDefaultUIDelegate;
@protocol GlanceStartVideoDefaultUIViewDelegate;


// Implement the following 2 protocols and assign the object to
@protocol GlanceStartVideoDefaultUIViewDelegate
-(void)glanceStartVideoDefaultUIDidTapCancel;
-(void)glanceStartVideoDefaultUIDidTapAccept;
-(void)glanceStartVideoDefaultUIDidTapTerms;
-(BOOL)glanceStartVideoDefaultUIDisplayTerms;
@end

@protocol GlanceStartDefaultUIViewDelegate
-(void)glanceStartDefaultUIDidTapCancel;
-(void)glanceStartDefaultUIDidTapAccept;
-(void)glanceStartDefaultUIDidTapTerms;
-(BOOL)glanceStartDefaultUIDisplayPhone;
-(BOOL)glanceStartDefaultUIDisplayHeadset;
-(BOOL)glanceStartDefaultUIDisplayDraw;
-(BOOL)glanceStartDefaultUIDisplayVideo;
-(BOOL)glanceStartDefaultUIDisplayTerms;
@end

@protocol SessionStartDialogUIViewDelegate;
@class AVCaptureSession;

// SessionUIViewDelegate implements the SessionUI.  To implement your own UI, build an object that conforms to this protocol
@protocol SessionUIViewDelegate
@required
- (void)sessionUIViewAgentVisitorViewUpdated:(BOOL)paused;
- (void)sessionUIViewToggleVideoTapped;
- (void)sessionUIViewEndSessionTapped;
- (void)sessionUIViewBackgroundBlurTapped;
- (void)sessionUIViewFlipCameraTapped;
- (void)sessionUIViewLocationUpdated:(NSString *_Nonnull)location;
- (void)sessionUIViewVisibilityUpdated:(NSString *_Nonnull)visibility;
- (void)sessionUIViewAgentVisitorSizeUpdated:(NSString *_Nonnull)size width:(int)width height:(int)height;
- (void)sessionUIViewDeviceRotated:(NSString *_Nonnull)orientation width:(int)width height:(int)height;
- (void)sessionUICameraSwitched:(NSString *_Nonnull)camera width:(int)width height:(int)height;
@end

@protocol GlanceUIViewDelegate; // used to allow GlanceSDK to call into the VideoDefaultUI if supplied.


typedef NS_ENUM(NSInteger, GlanceUIViewState) {
    GlanceUIViewStateHidden,
    GlanceUIViewStateSessionKey,
    GlanceUIViewStateConnecting,
    GlanceUIViewStateOpen,
    GlanceUIViewStateMinimized,
    GlanceUIViewStateVideoOpen,
    GlanceUIViewStateVideoMinimized
};
@protocol GlanceUIViewDelegate
@required
-(void)glanceUIViewDidTransitionToState:(GlanceUIViewState)state;
-(void)glanceUIViewDidClose;
@end

@protocol SessionUIDelegate <NSObject>

-(void)sessionUIVoiceAuthenticationRequired;
-(void)sessionUIVoiceAuthenticationFailed;
-(void)sessionUIDidError:(NSError*)error;
-(void)sessionUIDialogAccepted;
-(void)sessionUIDialogCancelled;
@end

@protocol GlanceVideoDefaultUIDelegate <NSObject>

-(void)glanceVideoDefaultUIDidError:(NSError*)error;
-(void)glanceVideoDefaultUIDialogAccepted;
-(void)glanceVideoDefaultUIDialogCancelled;
@end


@protocol GlanceDefaultUIDelegate <NSObject>

-(void)glanceDefaultUIVoiceDidAuthenticate:(NSDictionary*)dict;
-(void)glanceDefaultUIVoiceAuthenticationRequired;
-(void)glanceDefaultUIVoiceAuthenticationFailed;
-(void)glanceDefaultUIDialogAccepted;
-(void)glanceDefaultUIDialogCancelled;
@end

/**
 * GlanceVisitorDelegate
 *
 * A delegate for the GlanceVisitor interface to recieve events like session start, session ended and failures.
 */
@protocol GlanceVisitorDelegate
-(void)glanceVisitorEvent:(GlanceEvent*)event;
@end

/**
 * GlanceVisitorInitParams
 *
 * Parameters to the GlanceVisitor init method
 */
@interface GlanceVisitorInitParams : NSObject
{
@public
    int         groupid;    // Glance provided group ID integer associated with your account.
    NSString *  site;       // "staging" or "production"
    NSString *  token;      // Glance provided token string. _optional_
    NSString *  visitorid;
    NSString *  name;       // Name _optional_ Maximum length is 63 characters
    NSString *  email;      // Email address _optional_ Maximum length is 127 characters
    NSString *  phone;      // Phone number _optional_ Maximum length is 31 characters
    bool        screenshare;// Set to true if application is capable of starting screenshare via presence
    bool        video;      // Set to true if application is capable of starting video via presence
    NSString *  cameras;    // List of allowed cameras for starting video session via presence
    GlanceDisplayParams *   displayParams;

  //  _Glance::EventHandler * glanceVisitorHandleEvent;  // passed in instead of being defined in Glance_iOS EdC
}
@property int                   groupid;
@property (retain) NSString *   site;
@property (retain) NSString *   token;
@property (retain) NSString *   visitorid;
@property (retain) NSString *   name;
@property (retain) NSString *   email;
@property (retain) NSString *   phone;
@property bool                  screenshare;
@property bool                  video;
@property (retain) NSString *   cameras;
@property (retain) GlanceDisplayParams *    displayParams;

- (id)init:(int)groupid;
- (void)dealloc;
@end

/**
 * GlanceCore
 *
 * A utility class to interact with the Glance Visitor SDK.
 * There can only be one Visitor session at a time with this SDK.
 */

@interface GlanceCore : NSObject
+(void)setup:(GlanceVisitorInitParams *)visitorInitParams;
+(void)startSession:(GlanceStartParams *)startParams;
+(void)startVideoCall;
+(void)startVideoSession:(NSString *)videoSessionProperties sessionType:(NSString *)sessionType sessionKey:(NSString *)sessionKey;
+(void)endSession;
+(void)endVideoSession;
+(unsigned long)getCallId;
+(void)pause:(BOOL)pause;
+(void)freeze:(BOOL)freeze;
+(void)setContentMode:(UIViewContentMode)contentMode;
+(void)addMaskedView:(UIView *)view withLabel:(NSString*)label;
+(void)removeMaskedView:(UIView*)view;
+(void)addWebViewMask:(GlanceMask*)mask;
+(void)replaceWebViewMasks:(NSSet*)masks;
+(void)releaseVisitor;
+(void)maskKeyboard:(BOOL)maskKeyboard;
+(void)setMaskedKeyboardLabel:(NSString*)label;
+(void)sendUserMessage:(NSString*)messageName properties:(NSString*)messageProperties;
+(void)setUserState:(NSString*)messageName properties:(NSString*)messageProperties;
@end
/**
 * GlanceVisitor
 *
 * A utility class to interact with the Glance Visitor SDK.
 * There can only be one Visitor session at a time with this SDK.
 */
@interface GlanceVisitor : NSObject
/**
 * Call this once to initialize the SDK. You must pass your groupID obtained from Glance.  The token parameter is reserved for future use, pass an empty string now.  You may optionally pass the visitor’s name, email or phone or any strings useful to you.  These will be saved in the Glance database and you may obtain these session attributes via reports or API calls. The visitorid param is only required if subsequently connecting to presence
 *
 * **Init may be called again to supply or change a visitorid, but the gropuId may not be changed
 *
 * @param groupId   Glance provided group ID integer associated with your account. _required_
 * @param token     Glance provided token string. _optional_
 * @param name      Name _optional_ Maximum length is 63 characters
 * @param email     Email address _optional_ Maximum length is 127 characters
 * @param phone     Phone number _optional_ Maximum length is 31 characters
 * @param visitorid Visitor id _optional_ Maximum length is 100 characters. Required for presence, optional otherwise
 */
+(void)init:(int)groupId token:(NSString*_Nullable)token name:(NSString*_Nullable)name email:(NSString*_Nullable)email phone:(NSString*_Nullable)phone;
+(void)init:(int)groupId token:(NSString*_Nullable)token name:(NSString*_Nullable)name email:(NSString*_Nullable)email phone:(NSString*_Nullable)phone visitorid:(NSString*_Nullable)visitorid;
+(void)init:(GlanceVisitorInitParams *_Nonnull)visitorInitParams;

/**
 * The method initVistor is the same as init, it can be used from Swift 5 where "init" is reserved.
 Call initVisitor once to initialize the SDK. You must pass your groupID obtained from Glance.  The token parameter is reserved for future use, pass an empty string now.  You may optionally pass the visitor’s name, email or phone or any strings useful to you.  These will be saved in the Glance database and you may obtain these session attributes via reports or API calls. The visitorid param is only required if subsequently connecting to presence
 *
 * **Init may be called again to supply or change a visitorid, but the gropuId may not be changed
 *
 * @param groupId   Glance provided group ID integer associated with your account. _required_
 * @param token     Glance provided token string. _optional_
 * @param name      Name _optional_ Maximum length is 63 characters
 * @param email     Email address _optional_ Maximum length is 127 characters
 * @param phone     Phone number _optional_ Maximum length is 31 characters
 * @param visitorid Visitor id _optional_ Maximum length is 100 characters. Required for presence, optional otherwise
 */
+(void)initVisitor:(int)groupId token:(NSString*_Nullable)token name:(NSString* _Nullable)name email:(NSString* _Nullable)email phone:(NSString*_Nullable)phone;
+(void)initVisitor:(int)groupId token:(NSString*_Nullable)token name:(NSString*_Nullable)name email:(NSString*_Nullable)email phone:(NSString* _Nullable)phone visitorid:(NSString*_Nonnull)visitorid;
+(void)initVisitor:(GlanceVisitorInitParams *_Nonnull)visitorInitParams;
/**
 * **Finish using the SDK and release any resources.  Your application should not call this method it is automatically called on UIApplicationWillTerminateNotification.  It is only needed for unit tests or other scenarios that where this notification is not sent.
 */
+(void)releaseVisitor;


// This connects the GlanceSDK with the supplied defaultUI (SessionUI)
+(void)setGlanceDelegate:(id<GlanceSDKDelegate>_Nullable) delegate;

// This connects the GlanceSDK with the supplied defaultUI for video
+(void)setVideoDefaultUISurrogateDelegate:(id<GlanceStartDefaultUIViewDelegate, GlanceUIViewDelegate>_Nullable)  delegate;


/**
 * Start a screensharing session with a random numeric key.  Note that for Visitor sessions, the key identifies the session but is not sufficient to join the session.  Agents joining sessions must be logged-in or otherwise authenticated to Glance and must be members of the group specified in the call to init.  After calling startSession, the delegate will receive either EventSessionStarted or EventStartSessionFailed.
 *
 * **init: must be called first.**
 */
+(void)startSession;

/**
 * Start a screensharing session with a key you provide.  The key may only contain characters from the Base64 URL set.  Maximum length is 63.
 *
 * **init: should be called first.**
 *
 * @param sessionKey     Session key (4 digit string), pass @"GLANCE_KEYTYPE_RANDOM" to generate a random numeric key _required_
 */
+(void)startSession:(NSString* _Nonnull)sessionKey;

/**
 * Start a screensharing session with additional parameters. See GlanceStartParams.
 *
 * **init: must be called first.**
 *
 * @param sp     Start parameters _required_
 */
+(void)startSession:(GlanceStartParams * _Nonnull)sp delegate:(id<SessionUIDelegate>_Nullable)delegate;

/**
  * Deprecated way to start video
 */
+(void)startVideoCall:(NSString*)sessionkey camera:(NSString*)camera;

/**
  * Add video to screensharing session
 */
+(void)addVideo:(GlanceVideoMode)video;
+(void)addVideo:(GlanceVideoMode)video showDialog:(BOOL)showDialog;

/**
 * Pause an active screensharing session
 *
 * @param pause     Pause session if true or unpause if false _required_
 */
+(void)pause:(BOOL)pause;

/**
 * Freeze an active screensharing session
 *
 * @param freeze     Freeze session if true or unfreeze if false _required_
 */
+(void)freeze:(BOOL)freeze;

/**
 * End the screensharing session.  Calling endSession initiates the process of ending the session. The session is not actually ended until the EventSessionEnded event is fired.
 */
+(void)endSession;

/**
 * Find out if screensharing session is active
 */
+(BOOL)isSessionActive;

/**
 * Adds a delegate for the Glance visitor session to receive events.
 *
 * @param delegate     GlanceVisitorDelegate instance _required_
 */
+(void)addDelegate:(id<GlanceVisitorDelegate>_Nullable) delegate;

/**
 * Removes a delegate for the Glance visitor session to receive events.
 *
 * @param delegate     GlanceVisitorDelegate instance _required_
 */
+(void)removeDelegate:(id<GlanceVisitorDelegate>_Nullable) delegate;

/**
 * Sets the custom viewer delegate to provide a custom implementation of the agent video viewer.
 *
 * @param customViewerDelegate     GlanceCustomViewerDelegate instance _required_
 */
+(void)setCustomViewerDelegate:(id<GlanceCustomViewerDelegate>) customViewerDelegate;


/**
 * Gets the custom viewer delegate to provide a custom implementation of the agent video viewer.
 */
+(id<GlanceCustomViewerDelegate>)customViewerDelegate;

/**
 * Sets the video content mode for the agent video viewer.
 *
 * @param contentMode              UIViewContentMode contentMode _required_
 */
+(void)setContentMode:(UIViewContentMode)contentMode;

/**
 * Adds a UIView to mask during the Glance visitor session.
 *
 * @param view  UIView instance to mask _required_
 */
+(void)addMaskedView:(UIView*)view;

/**
 * Adds a UIVIew to mask during the Glance visitor session with a label on top of the mask to identify it
 *
 * @param view UIView instance to mask _required_
 * @param label NSString to add on top of the mask _required_
 */
+(void)addMaskedView:(UIView*)view withLabel:(NSString*)label;

/**
 * Remove a UIView from being masked during a Glance visitor session.
 *
 * @param view  UIView instance to mask _required_
 */
+(void)removeMaskedView:(UIView*)view;

/**
 * Masks the keyboard from being shown
 *
 * @param maskKeyboard YES or NO to mask keyboard
 */
+(void)maskKeyboard:(BOOL)maskKeyboard;

/**
 * Sets the label that will be used on top of the keyboard mask to identify it
 * NOTE: This will only be visible if maskKeyboard method is set to YES
 * @param label NSString to add on top of the mask _required_
 */
+(void)setMaskedKeyboardLabel:(NSString*)label;

/**
 * Get Glance tag for UIWindows
 *
 * @return NSInteger of the tag used on Glance UIWindows
 */
+(NSInteger)glanceWindowTag;

/**
 * Set the tag to be used for Glance UIWindows.
 * This should be set before using GlanceVisitor or starting a session.
 *
 * @param tag - NSInteger of the tag to be used on Glance UIWindows
 */
+(void)setGlanceWindowTag:(NSInteger)tag;

/**
 * Is ReplayKit enabled
 *
 * @return BOOL true if ReplayKit is enabled
 */
+(BOOL)isReplayKitEnabled;

/**
 * Is ReplayKit enabled and available
 *
 * @return BOOL true if ReplayKit is enabled and available
 */
+(BOOL)isReplayKitEnabledAndAvailable;

/**
 * Set ReplayKit enabled
 * Setting this value to YES will try to use ReplayKit on iOS11+
 *
 * @param enabled - BOOL boolean to enable ro disable ReplayKit
 */
+(void)setReplayKitEnabled:(BOOL)enabled;

/**
 * Is ReplayKit enabled, available and currently running
 *
 * @return BOOL true if ReplayKit is enabled, available and running
 */
+(BOOL)isReplayKitRunning;


/**
 setDefaultUIDelegateAndTermsUrl  -  Assign an object that conforms to GlanceDefaultUIDelegate
 and a terms URL.
 
 @param delegate Allows the client application to pass in an object that conforms to the GlanceDefaultUIDelegate which allows for finer control of the terms and conditions presentation.
 Pass a nil delegate to just set the termsUrl;
 * @param termsUrl - Terms and Conditions URL to display when starting a session
 */
+(void)setDefaultUIDelegateAndTermsUrl:(id<GlanceDefaultUIDelegate>)delegate termsUrl:(NSString*)termsUrl;

/**
 * Start Glance 2-Way Video Call
 *
 * @param sessionkey
 * @param delegate
 * @param camera - "front" or "back"
 * @param termsUrl - Terms of Service URL to send users to
 */
+(void)startVideoCall:(NSString*)sessionkey delegate:(id<GlanceVideoDefaultUIDelegate>)delegate camera:(NSString*)camera termsUrl:(NSString *)termsUrl;

/**
 * Start Glance 2-Way Video Call (w/ option to toggle dialog off)
 *
 * @param sessionkey
 * @param delegate
 * @param camera - "front" or "back"
 * @param termsUrl - Terms of Service URL to send users to (can be nil or empty if showDialog is set to NO)
 * @param showDialog - if set to NO, skips default dialog that shows up before starting a video session. Video session will start automatically if this is set to NO
 */
+(void)startVideoCall:(NSString*)sessionkey delegate:(id<GlanceVideoDefaultUIDelegate>)delegate camera:(NSString*)camera termsUrl:(NSString *)termsUrl showDialog:(BOOL)showDialog;

/**
 * Start Glance 2-Way Video Call with customizable video parameters
 *
 * @param sessionkey
 * @param delegate
 * @param camera - "front" or "back"
 * @param preferredSize - default is CGSizeMake(0, 0)
 * @param framerate - default is 0
 * @param timeout - default is 0
 * @param termsUrl - Terms of Service URL to send users to
*/
+(void)startVideoCall:(NSString *)sessionKey delegate:(id<GlanceVideoDefaultUIDelegate>)delegate camera:(NSString *)camera preferredSize:(CGSize)preferredSize framerate:(int)framerate timeout:(int)timeout termsUrl:(NSString *)termsUrl;

/**
 * Start Glance 2-Way Video Call with customizable video parameters (& w/ option to toggle dialog off)
 *
 * @param sessionkey
 * @param delegate
 * @param camera - "front" or "back"
 * @param preferredSize - default is CGSizeMake(0, 0)
 * @param framerate - default is 0
 * @param timeout - default is 0
 * @param termsUrl - Terms of Service URL to send users to
 * @param showDialog - if set to NO, skips default dialog that shows up before starting a video session. Video session will start automatically if this is set to NO
*/
+(void)startVideoCall:(NSString *)sessionKey delegate:(id<GlanceVideoDefaultUIDelegate>)delegate camera:(NSString *)camera preferredSize:(CGSize)preferredSize framerate:(int)framerate timeout:(int)timeout termsUrl:(NSString *)termsUrl showDialog:(BOOL)showDialog;


/**
  End Glance Video Call
 * @param endVoiceCall end accompanying voice call along with video
 */
+(void)endVideoCall:(BOOL)endVoiceCall;

+(void)sendUserMessage:(NSString*)messageName properties:(NSString*)messageProperties;

+(void)setUserState:(NSString*)messageName properties:(NSString*)messageProperties;

@end

// Presence ---------------------------------------------------------------------------------------

/**
 * GlancePresenceVisitor
 *
 * A utility class to interact with the Glance Presence SDK.
 * There can only be one Presence session at a time with this SDK.
 */
@interface GlancePresenceVisitor : NSObject

+(void)connect:(id<SessionUIDelegate>)delegate maxAttempts:(int)maxAttempts;

+(void)connect:(id<SessionUIDelegate>)delegate maxAttempts:(int)maxAttempts registerForNotifications:(BOOL)registerForNotifications;

+(void)presence:(NSDictionary*)map;

+(NSString*)getPresenceServer;

+(void)handleMessage:(NSDictionary*)map;

+(void)signalVisibility:(BOOL)visible;

+(void)signalAgent:(NSString*)event map:(NSDictionary*)map;

+(void)disconnect;

+(void)setDefaultUI:(BOOL)defaultUI;

+(void)setDelegate:(id<GlancePresenceDialogDelegate>_Nonnull)delegate;

+(void)setTermsUrl:(NSString*_Nonnull) updatedTermsUrl;
@end
