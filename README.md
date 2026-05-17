![Thumbnail](Thumbnail.png)

# MEGA Plates

**Plates** is a lightweight C++ GUI library built on top of SDL2, SDL2_ttf, SDL2_image, SDL2_gfx and miniaudio.  
Single-file — just `#include "plates.cpp"` and you're good to go.

---

## Dependencies

|Library|Purpose|
|---|---|
|SDL2|Windows, rendering, events|
|SDL2_ttf|Text and fonts|
|SDL2_image|PNG / JPG loading|
|SDL2_gfx|Rounded rectangles, circles|
|miniaudio|Audio playback|

---

## Installation

 Add `#include "plates.h"` to your source file and don't forget install libplates.so /.a

---

## Program Structure

```cpp
#include "plates.h"

Main {
    // Setup: create windows, buttons, texts, etc.
}

EventLoop {
    // Called every frame (~60 FPS)
    // Handle events and game logic here
}
```

`Main` and `EventLoop` are macros that expand into `entry()` and `loop()` functions.

---

## Quick Start

```cpp
#include "plates.cpp"

Window wnd;

Main {
    wnd = createWindow("My App", {100, 100, 800, 600},
                       WINDOW_SHOWN,
                       RENDERER_ACCELERATED | RENDERER_TARGETTEXTURE,
                       "win");

    setBackground({30, 30, 30, 255}, wnd);

    createButton(wnd, {50, 50, 200, 50}, {70, 130, 255, 255}, "btn1");
    createText(wnd, "NotoSans", "Hello, Plates!", {50, 20, 300, 40},
               {255, 255, 255, 255}, 24, "title");
}

EventLoop {
    if (mainSpace.Event & EVENT_MOUSE_BUTTON_DOWN) {
        // handle click
    }
}
```

---

## API Reference

### Window

```cpp
Window& createWindow(const char* title, Rect rect, int windowFlags, int rendererFlags, std::string name);
void   setBackground(Color clr, Window* wnd);
```

### Button

```cpp
Button& createButton(Window* win, Rect rect, Color clr, std::string name);
void   addRoundCorners(std::string objName, int radius);
```

### Text

```cpp
Text& createText(Window* win, const char* fontPath, const char* text,
                Rect rect, Color clr, int fontSize, std::string name);
```

Fonts are resolved from the `Fonts/` folder. The `.ttf` / `.otf` extension is optional.

### Image

```cpp
Image& createImage(Window* win, const char* path, Rect rect, std::string name);
```

### Circle

```cpp
Circle& createCircle(Window* win, UniformRect rect, Color clr, std::string name);
// UniformRect: {x, y, radius}
```

### Line

```cpp
Line& drawLine(Window* win, Rect cords, Color color, std::string name);
// cords: {x1, y1, x2, y2}
```

### CheckBox

```cpp
CheckBox& createCheckBox(Window* win, UniformRect rect, Color clr, bool isOn, std::string name);
```

### Radio Button

```cpp
auto* createRadioButton(Window* win, UniformRect uRect,
                        Color clrOff, Color clrOn,
                        bool status, int group, std::string name);
```

All radio buttons sharing the same `group` value are mutually exclusive.

### Slider

```cpp
Slider&      createSlider(Window* win, Rect rect, Color clrOff, Color clrOn,
                         float progress, std::string name);
RangeSlider& createRangeSlider(Window* win, Rect rect, Color clrOff, Color clrOn,
                               float progress, float progress2, std::string name);
```

`progress` is a value between `0.0` and `1.0`.

### Input Box

```cpp
InputBox& createInputBox(Window* win, const char* hint, Rect rect,
                        const char* text, int fontSize, Color clr, std::string name);
```

Supports Unicode, text selection (Shift+arrows, Ctrl+A), caret navigation, and multiline input.  
Font is fixed to `Fonts/NotoSans.ttf`.

### Scroll Bar

```cpp
ScrollBar& createScrollBar(Window* win, Rect rect, Color clr, std::string name);
void      addScrollBarElement(Window* win, std::string objName, std::string scrollBarName);
```

Child elements are registered by name via `addScrollBarElement`.

### Sound

```cpp
Sound createSound(const char* path, std::string name, int soundFlags);
void  playSound(Sound sound);
void  stopSound(Sound sound);
void  setSoundLooping(Sound sound);
void  setSoundVolume(Sound sound, float volume);
void  setSoundPosition(Sound sound, float x, float y);
```

### Object Management

```cpp
void        DeleteObject(std::string name);
void        changeObjectProperties(std::string prop, std::string value, std::string name);
std::string getObjectProperty(std::string prop, std::string name);
```

### Display Info

```cpp
int                getDisplayWidth(int displayIndex);
int                getDisplayHeight(int displayIndex);
std::pair<int,int> getDisplaySize(int displayIndex);
```

---

## Data Types

```cpp
struct Rect        { uint16_t x, y, w, h; };
struct UniformRect { int x, y, size; };
struct Color       { uint8_t r, g, b, a; };
```

---

## Events

The current frame's events are available via `mainSpace.Event` — a `uint64_t` bitmask:

```cpp
EventLoop {
    if (mainSpace.Event & EVENT_KEY_DOWN)          { /* key pressed  */ }
    if (mainSpace.Event & EVENT_MOUSE_BUTTON_DOWN) { /* mouse click  */ }
    if (mainSpace.Event & EVENT_QUIT)              { /* window close */ }
}
```

**Keyboard & Mouse**

|Event|Description|
|---|---|
|`EVENT_KEY_DOWN`|A key was pressed|
|`EVENT_KEY_UP`|A key was released|
|`EVENT_MOUSE_MOTION`|Mouse cursor moved|
|`EVENT_MOUSE_BUTTON_DOWN`|Mouse button pressed|
|`EVENT_MOUSE_BUTTON_UP`|Mouse button released|

**Touch**

|Event|Description|
|---|---|
|`EVENT_FINGER_DOWN`|Finger touched the screen|
|`EVENT_FINGER_UP`|Finger lifted from the screen|
|`EVENT_FINGER_MOTION`|Finger moved on the screen|

**Window**

|Event|Description|
|---|---|
|`EVENT_QUIT`|Window close button pressed|
|`EVENT_WINDOW_RESIZED`|Window was resized|
|`EVENT_WINDOW_MOVED`|Window was moved|
|`EVENT_WINDOWEVENT_SHOWN`|Window became visible|
|`EVENT_WINDOWEVENT_MINIMIZED`|Window was minimized|
|`EVENT_WINDOWEVENT_MAXIMIZED`|Window was maximized|
|`EVENT_WINDOWEVENT_SIZE_CHANGED`|Window size changed (any reason)|
|`EVENT_WINDOWEVENT_ENTER`|Mouse entered the window|
|`EVENT_WINDOWEVENT_LEAVE`|Mouse left the window|
|`EVENT_WINDOWEVENT_FOCUS_LOST`|Window lost keyboard focus|
|`EVENT_WINDOWEVENT_TAKE_FOCUS`|Window is being offered focus|
|`EVENT_WINDOWEVENT_EXPOSED`|Window needs to be redrawn|
|`EVENT_WINDOWEVENT_DISPLAY_CHANGED`|Window moved to a different display|

**Drag & Drop / Clipboard**

|Event|Description|
|---|---|
|`EVENT_DROP_FILE`|A file was dropped onto the window|
|`EVENT_DROP_TEXT`|Text was dropped onto the window|
|`EVENT_DROP_BEGIN`|A drag-and-drop sequence started|
|`EVENT_DROP_COMPLETE`|A drag-and-drop sequence finished|
|`EVENT_CLIPBOARD_UPDATE`|Clipboard contents changed|

**Audio Device**

| Event                        | Description                      |
| ---------------------------- | -------------------------------- |
| `EVENT_AUDIO_DEVICE_ADDED`   | An audio device was connected    |
| `EVENT_AUDIO_DEVICE_REMOVED` | An audio device was disconnected |

**Joystick**

|Event|Description|
|---|---|
|`EVENT_JOYSTICK_AXIS_MOTION`|Joystick axis moved|
|`EVENT_JOYSTICK_BALL_MOTION`|Joystick trackball moved|
|`EVENT_JOYSTICK_HAT_MOTION`|Joystick hat (D-pad) moved|
|`EVENT_JOYSTICK_BUTTON_DOWN`|Joystick button pressed|
|`EVENT_JOYSTICK_BUTTON_UP`|Joystick button released|
|`EVENT_JOYSTICK_DEVICE_ADDED`|A joystick was connected|
|`EVENT_JOYSTICK_DEVICE_REMOVED`|A joystick was disconnected|
|`EVENT_JOYSTICK_BATTERY_UPDATED`|Joystick battery level changed|
|`EVENT_JOYSTICK_POWER_EMPTY`|Battery critically low|
|`EVENT_JOYSTICK_POWER_LOW`|Battery low|
|`EVENT_JOYSTICK_POWER_MIN`|Battery at minimum|
|`EVENT_JOYSTICK_POWER_MEDIUM`|Battery at medium level|
|`EVENT_JOYSTICK_POWER_FULL`|Battery full|
|`EVENT_JOYSTICK_POWER_MAX`|Battery at maximum|
|`EVENT_JOYSTICK_POWER_WIRED`|Joystick is wired (no battery)|
|`EVENT_JOYSTICK_POWER_UNKNOWN`|Battery status unknown|

**App Lifecycle (mobile)**

|Event|Description|
|---|---|
|`EVENT_TERMINATING`|App is being terminated by the OS|
|`EVENT_LOW_MEMORY`|OS is low on memory|
|`EVENT_WILL_ENTER_BACKGROUND`|App is about to go to background|
|`EVENT_DID_ENTER_BACKGROUND`|App entered background|
|`EVENT_WILL_ENTER_FOREGROUND`|App is about to return to foreground|
|`EVENT_DID_ENTER_FOREGROUND`|App returned to foreground|

---

## Tween Animations

```cpp
Tween tw;
tw.target   = &someFloat;
tw.from     = 0.0f;
tw.to       = 1.0f;
tw.duration = 1.0f; // seconds
tw.anim     = TweenType::EaseInOut;

// Inside EventLoop:
tw.update(0.016f); // deltaTime in seconds
```

|Function|Description|
|---|---|
|`Linear`|Constant speed, no easing|
|`EaseIn`|Starts slow, accelerates|
|`EaseOut`|Starts fast, decelerates|
|`EaseInOut`|Slow at both ends, fast in the middle|
|`Cubic`|Cubic acceleration (t³)|
|`Elastic`|Overshoots and snaps back like a spring|
|`Bounce`|Bounces at the end like a ball|
|`Back`|Slightly overshoots before settling|
|`SineIn`|Gentle acceleration using a sine curve|
|`SineOut`|Gentle deceleration using a sine curve|
|`SineInOut`|Smooth sine ease on both ends|
|`QuadIn`|Quadratic acceleration (t²)|
|`QuadOut`|Quadratic deceleration|
|`QuadInOut`|Quadratic ease on both ends|
|`QuartIn`|Quartic acceleration (t⁴)|
|`QuartOut`|Quartic deceleration|
|`QuartInOut`|Quartic ease on both ends|
|`QuintIn`|Quintic acceleration (t⁵)|
|`QuintOut`|Quintic deceleration|
|`QuintInOut`|Quintic ease on both ends|
|`ExpoIn`|Exponential acceleration, very sharp start|
|`ExpoOut`|Exponential deceleration, very sharp end|
|`ExpoInOut`|Exponential ease on both ends|
|`CircIn`|Circular acceleration (based on √)|
|`CircOut`|Circular deceleration|
|`CircInOut`|Circular ease on both ends|
|`ElasticIn`|Spring-like wobble at the start|
|`ElasticOut`|Spring-like wobble at the end|

---

## Window / Renderer / Sound Flags

**Window flags** — pass to `createWindow()` as `windowFlags`:

|Flag|Description|
|---|---|
|`WINDOW_SHOWN`|Window is visible on creation|
|`WINDOW_HIDDEN`|Window starts hidden|
|`WINDOW_FULLSCREEN`|True fullscreen mode|
|`WINDOW_FULLSCREEN_DESKTOP`|Fullscreen at desktop resolution|
|`WINDOW_BORDERLESS`|No title bar or frame|
|`WINDOW_RESIZABLE`|User can resize the window|
|`WINDOW_MINIMIZED`|Starts minimized|
|`WINDOW_MAXIMIZED`|Starts maximized|
|`WINDOW_ALWAYS_ON_TOP`|Window stays above all others|
|`WINDOW_OPENGL`|Window usable with OpenGL context|
|`WINDOW_VULKAN`|Window usable with Vulkan|
|`WINDOW_ALLOW_HIGHDPI`|Enable high-DPI mode if supported|
|`WINDOW_INPUT_GRABBED`|Grabs mouse input|
|`WINDOW_SKIP_TASKBAR`|Don't show in taskbar|
|`WINDOW_UTILITY`|Treat as a utility window|
|`WINDOW_TOOLTIP`|Treat as a tooltip window|
|`WINDOW_POPUP_MENU`|Treat as a popup menu|

**Renderer flags** — pass to `createWindow()` as `rendererFlags`:

|Flag|Description|
|---|---|
|`RENDERER_ACCELERATED`|Use hardware-accelerated rendering|
|`RENDERER_SOFTWARE`|Use software (CPU) rendering|
|`RENDERER_PRESENTVSYNC`|Sync to monitor refresh rate (V-Sync)|
|`RENDERER_TARGETTEXTURE`|Renderer supports rendering to a texture|

**Sound flags** — pass to `createSound()` as `soundFlags`:

|Flag|Description|
|---|---|
|`SOUND_FLAG_DECODE`|Decode the entire file into memory upfront|
|`SOUND_FLAG_STREAM`|Stream audio from disk (good for large files)|
|`SOUND_FLAG_ASYNC`|Load the sound asynchronously|
|`SOUND_FLAG_NO_DEFAULT_ATTACHMENT`|Don't attach to the default audio graph node|

---

## Global State

Everything lives in the global `mainSpace` object (type `Space`). You can access object lists directly:

```cpp
mainSpace.uiElements  // std::vector<uiElement>
mainSpace.running     // bool — set to false to quit
mainSpace.Event       // uint64_t — bitmask of events this frame
```

---

## Notes

- Object names must be **unique** and **non-empty** — the app will abort on startup if they aren't.
- Draw order follows creation order.
- `UpdateAll()` is called automatically every frame.
- Frame rate is capped at ~62 FPS (`SDL_Delay(16)`).