
#ifdef ULTRALIGHT_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of the JUCE Ultralight cpp file"
#endif

#include "ultralight.h"
#include "core/ultralight_SplitComponent.cpp"
#include "core/ultralight_Browser.cpp"
#include "core/ultralight_UltralightComponent.cpp"
