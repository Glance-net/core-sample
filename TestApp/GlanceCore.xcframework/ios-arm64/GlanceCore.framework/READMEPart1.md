<style>
    .grey {
        background-color: #D3D3D3;
    }
</style>

# ``GlanceSDK``


<!--@START_MENU_TOKEN@-->The best way to build an application with the GlanceSDK is to look at the sample programs and refer to the include file, published below.<!--@END_MENU_TOKEN@-->

[iOS quick star guide](https://help.glance.net/mobile-app-sharing/getting-started/ios/ios_quickstart/)

## Code 

<!--@START_MENU_TOKEN@-->The first step is to initialize the SDK.<!--@END_MENU_TOKEN@-->
<div class="grey">
```
GlanceVisitor.initVisitor(GLANCE_GROUP_ID, token: "", name: "", email: "", phone: "")
```
</div>
We recommend to call the init method as soon as your app starts. You can do this in your AppDelegate initialization. For example:

<div class="grey">
```
func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool
{
    ...
    // Configure Glance Visitor SDK
    GlanceVisitor.initVisitor(GLANCE_GROUP_ID, token: "", name: "", email: "", phone: "")
    return true
}
```
</div>

You can also define a Session Code when calling startSession
<div class="grey">
GlanceVisitor.startSession("22333")``
</div>


To receive events from the SDK, you need to implement the protocol GlanceVisitorDelegate like below:
<div class="grey">
```
class MyGlanceVisitorDelegate: GlanceVisitorDelegate {
    func glanceVisitorEvent(_ event: GlanceEvent!) {
        DispatchQueue.main.async {
            switch event.code {
            case EventSessionEnded:
                // Successfully connected to session
                // Get generated session key
                break

            case EventConnectedToSession:
                // Couldn't start session
                // Error message can be found in event.message
                break

            case EventStartSessionFailed:
                // Session ended
                break

            default:
                // It is recommended that you at log all other events to help debug during development
                // Best practice is to log all other events of type EventAssertFail, EventError, or EventWarning
                break
            }
        }
    }
}
```
</div>

### <!--@START_MENU_TOKEN@-->GlanceSDK include file<!--@END_MENU_TOKEN@-->
```
