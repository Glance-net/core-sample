let elementsToMask = [];
let glanceSdkMasking_webViewScale = 1.0;

function glanceSdkMasking_log(msg) {
    window.webkit.messageHandlers.log.postMessage('webview: ' + msg);
}

function glanceSdkMasking_pause() {
    window.webkit.messageHandlers.pause.postMessage('pause');
}

function glanceSdkMasking_detectedViews(elementsToMask) {
    window.webkit.messageHandlers.detectedViews.postMessage(JSON.stringify(elementsToMask));
}

function glanceSdkMasking_hidden() {
    window.webkit.messageHandlers.hidden.postMessage('hidden');
}

function glanceSdkMasking_isPositionVisible(element){
    /* make sure element itself is not explicitly hidden */
    if (element.style.display && (element.style.display === 'none')) {
        /* not visible */
        return false;
    }
    if (element.style.visibility && (element.style.visibility === 'hidden')) {
        /* not visible */
        return false;
    }
    if ( !(element.offsetWidth || element.offsetHeight || element.getClientRects().length) ){
        /* not visible */
        return false;
    }
    if (element.style.position && element.style.overflow){
        if (element.style.position === 'absolute' || element.style.position === 'relative'){
            var elementBounds = element.getBoundingClientRect();
            if ((elementBounds.width === 0 || elementBounds.height === 0)
                && (element.style.overflow === 'clip' || element.style.overflow === 'hidden')){
                return false;
            }
        }
    }
    var parent = element.parentElement;
    if (!parent){
        return true;
    }
    return glanceSdkMasking_isPositionVisible(parent);
}

function glanceSdkMasking_isVisible(element) {
    /* make sure element itself is not explicitly hidden */
    if (element.style.display && (element.style.display === 'none')) {
        /* not visible */
        return false;
    }
    if (element.style.visibility && (element.style.visibility === 'hidden')) {
        /* not visible */
        return false;
    }
    if ( !(element.offsetWidth || element.offsetHeight || element.getClientRects().length) ){
        /* not visible */
        return false;
    }
    if (element.style.position){
        if (element.style.position === 'fixed'){
            return true;
        }
        if (element.style.position === 'absolute' || element.style.position === 'relative'){
            return glanceSdkMasking_isPositionVisible(element);
        }
    }

    var elementBounds = element.getBoundingClientRect();
    if (elementBounds.width === 0 || elementBounds.height === 0) {
        if (element.style.overflow
            && (element.style.overflow === 'clip' || element.style.overflow === 'hidden')){
            return false;
        }
    }

    var parent = element.parentElement;
    if (parent){
        /* make sure element is visible within parent */
        if (parent.style.overflow
            && (parent.style.overflow === 'scroll'
                || parent.style.overflow === 'clip'
                || parent.style.overflow === 'hidden')
            ){
            var parentBounds = parent.getBoundingClientRect();
            if (parentBounds.width === 0 || parentBounds.height === 0) {
                return false;
            }else if ((elementBounds.y < (parentBounds.y - elementBounds.height)) || (elementBounds.y > (parentBounds.y + parentBounds.height))) {
            /* not visible */
            return false;
            } else if (((elementBounds.x + elementBounds.width) < 0) || (elementBounds.x > parentBounds.width)) {
            /* not visible */
            return false;
            }
        }

        return glanceSdkMasking_isVisible(parent);
    }

  return true;
}

function glanceSdkMasking_isFrameVisible(frame) {
    /* make sure frame is being displayed

       - checks for frame not being displayed:
       display="none"
       visibiility="hidden"
       height OR width = 0
     */
    if (frame.style.display && (frame.style.display === 'none')) {
      /* not visible */
      return false;
    }
    if (frame.style.visibility && (frame.style.visibility === 'hidden')) {
      /* not visible */
      return false;
    }
    var frameBounds = frame.getBoundingClientRect();
    if (frameBounds.width === 0 || frameBounds.height === 0) {
      /* not visible */
      return false;
    }
/*    var parent = frame.parentElement;
    if (parent){
      *//* make sure frame is within visible parent *//*
      return glanceSdkMasking_isFrameVisible(parent);
    }*/
    return true;
}

function glanceSdkMasking_maskingCheck(node, toMaskItem, parentBounds, _contentWindow) {
    var rect = node.getBoundingClientRect();

    var viewport = window.visualViewport;
    var scale = 1.0;
    if (viewport && viewport.scale){
      scale = viewport.scale;
    }else {
      scale = glanceSdkMasking_webViewScale;
    }

    if (glanceSdkMasking_isVisible(node) && glanceSdkMasking_isWithinViewport(_contentWindow, rect)) {
      var top = rect.top;
      var bottom = rect.bottom;
      var left = rect.left;
      var right = rect.right;
      var height = rect.height;
      var width = rect.width;

      var padding = 5;
      top -= padding;
      bottom += padding;
      left -= padding;
      right += padding;
      height += 2*padding;
      width += 2*padding;

      var hiddenByParentFrame = false;
      if (parentBounds) {
        if ((top+height < 0) || (left+width < 0)){
          hiddenByParentFrame = true;
        }else {
          top += parentBounds.top;
          bottom = top + height;
          left += parentBounds.left;
          right = left + width;

          if (right > parentBounds.right){
            right = parentBounds.right;
            width = right - left;
          }
        }
      }

      if (!hiddenByParentFrame){
        top *= scale;
        bottom *= scale;
        left *= scale;
        right *= scale;
        width *= scale;
        height *= scale;

        var obj = {bottom: bottom, height: height, left: left, right: right, top: top, width: width};
        var elementData = {
          qs: toMaskItem.qs, label: toMaskItem.label, coordinates: obj
        };
        elementsToMask.push(elementData);
      }
    }
}

function glanceSdkMasking_processNodes(_document, element, parentBounds, _contentWindow) {
  glanceSdkMasking_log("processNodes");
  glanceSdkMasking_pause();
  var frames = _document.getElementsByTagName('iframe');
  if (frames.length > 0) {
    for (var i = 0; i < frames.length; ++i)
    {
      glanceSdkMasking_log("processNodes - into iframe");
      var frame = frames[i];
      if (glanceSdkMasking_isFrameVisible(frame)) {
        var frameDocument = null;
        try {
          frameDocument = frame.contentWindow.document;
        } catch(e) {
          glanceSdkMasking_log(e);
          glanceSdkMasking_maskingCheck(frame, {"qs": "iframe", "label": ""}, parentBounds, _contentWindow);
        }
        if (frameDocument) {
          glanceSdkMasking_log("processNodes - found frameDocument");

          var frameRect = frame.getBoundingClientRect();
          if (parentBounds){
            var relativeRect = {
                width: frameRect.width,
                height: frameRect.height
            };
            relativeRect.top = frameRect.top + parentBounds.top;
            relativeRect.bottom = relativeRect.top + frameRect.height;
            relativeRect.left = frameRect.left + parentBounds.left;
            relativeRect.right = relativeRect.left + frameRect.width;
            frameRect = relativeRect;
          }

          glanceSdkMasking_processNodes(frameDocument, frame, frameRect, frame.contentWindow);
        }
      }
    }
  }

  for (var i = 0; i < toMask.length; i++){
    var toMaskItem = toMask[i];
    var nodes = _document.querySelectorAll(toMaskItem.qs);
    for (var j = 0; j < nodes.length; j++){
      var n = nodes[j];
      glanceSdkMasking_maskingCheck(n, toMaskItem, parentBounds, _contentWindow);
    }
  }
  if (parentBounds == null) {
      if (!window.glanceSdkLastScrolledAt
          || (((new Date()).getTime() - window.glanceSdkLastScrolledAt) >= 1000) ){
          glanceSdkMasking_log("glanceSdkMasking_detectedViews");
          glanceSdkMasking_detectedViews(elementsToMask);
      }
  }
}

function glanceSdkMasking_isWithinViewport(_contentWindow, rect){
    if(_contentWindow && _contentWindow.visualViewport){
        var viewport = _contentWindow.visualViewport;
        if ((rect.x + rect.width) < 0){
            return false;
        }
        if (rect.x > viewport.width){
            return false;
        }
        if ((rect.y + rect.height) < 0){
            return false;
        }
        if (rect.y > (viewport.height)){
            return false;
        }
    }
    return true;
}

var glanceMutationObserverConfig = { childList: true, subtree: true, attributes: true };
var glanceMutationObserverCallback = function(mutationsList, observer) {
    glanceSdkMasking_log("processNodes - via mutation");
    glanceSdkMasking_reprocess();
};

function setup(frameWindow) {
    glanceSdkMasking_log("setup");

    var observer = new MutationObserver(glanceMutationObserverCallback);
    observer.observe(frameWindow.document.body, glanceMutationObserverConfig);
    
    setupScrollListener(frameWindow);

  var frames = frameWindow.document.getElementsByTagName('iframe');
  if (frames.length > 0) {
    for (var i = 0; i < frames.length; ++i)
    {
      var frame = frames[i];
      if (glanceSdkMasking_isFrameVisible(frame)) {
        var _frameWindow = frame.contentWindow;
        setup(_frameWindow);
      }
    }
  }
}

function setupScrollListener(frameWindow) {
    frameWindow.addEventListener("scroll", function() {
        glanceSdkMasking_pause();
        let scrollTime = (new Date()).getTime();
        window.glanceSdkLastScrolledAt = scrollTime;
        frameWindow.glanceSdkLastScrolledAt = scrollTime;
        if (!frameWindow.glanceSdkScrollInterval) {
            frameWindow.glanceSdkScrollInterval = setInterval(function() {
                let now = (new Date()).getTime();
                if ((now - window.glanceSdkLastScrolledAt) >= 1000){
                    glanceSdkMasking_pause();
                    elementsToMask = [];
                    glanceSdkMasking_processNodes(document, document.body, null);
                    if (frameWindow.glanceSdkScrollInterval
                        && ((now - frameWindow.glanceSdkLastScrolledAt) >= 1000)
                        ) {
                        clearInterval(frameWindow.glanceSdkScrollInterval);
                        frameWindow.glanceSdkScrollInterval = undefined;
                    }
                }
            }, 1000);
        }
    })
}

function glanceSdkMasking_reprocess(){
    glanceSdkMasking_log("glanceSdkMasking_reprocess");
    glanceSdkMasking_pause();
    elementsToMask = [];
    glanceSdkMasking_processNodes(window.document, window.document.body, null);
}

glanceSdkMasking_pause();
window.addEventListener('load', function () {
    glanceSdkMasking_pause();
    setup(window);
    glanceSdkMasking_log("load");
    glanceSdkMasking_reprocess();
});
document.addEventListener("visibilitychange", function() {
    glanceSdkMasking_log("visibilitychange "+document.visibilityState);
    if (document.visibilityState === 'visible') {
        glanceSdkMasking_reprocess();
    } else {
        glanceSdkMasking_hidden();
    }
});
document.addEventListener("pagehide", function() {
    glanceSdkMasking_log("pagehide");
    glanceSdkMasking_hidden();
});
window.addEventListener("orientationchange", function() {
    glanceSdkMasking_log("orientationchange");
    glanceSdkMasking_reprocess();
});
window.addEventListener("resize", function() {
    glanceSdkMasking_log("resize");
    glanceSdkMasking_reprocess();
});
