# MEGA Plates

**Plates** is a lightweight C++ GUI library built on top of SDL2, SDL2_ttf, SDL2_image, SDL2_gfx, Vulkan, STB, shaderc and miniaudio.  
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
|shaderc|Vulkan shaders compilation|
|glm|Vulkan's mathematical functions|
|STB true type|rasterization & parse fonts|
|STB image|load images|
|ffmpeg|Videos|

The dependencies have already been installed.

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

    createBox(wnd, {50, 50, 200, 50}, {70, 130, 255, 255}, "btn1");
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

### Box

```cpp
Box& createBox(Window* win, Rect rect, Color clr, std::string name);
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
Line& createLine(Window* win, Rect cords, Color color, std::string name);
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

### Video

```cpp
void playVideo              (Video& v);
void stopSound              (Video& v);
void setVideoLooped         (Video& v);
void setVideoUnlooped       (Video& v);
void setVideoVolume         (Video& v, float vol);
void setVideoPosition       (Video& v, float pos);

Image& uploadFrameFromVideo (Window* win, const char* path, Rect rect, double seekSeconds, std::string name);
```

### Manual Element Drawing

Every UI element normally draws itself automatically each frame via `UpdateAll()`. These functions expose that per-element drawing manually — useful for custom render order, drawing into an off-screen target, or drawing a single element outside the normal update cycle.

```cpp
void drawBox(Box& box, bool ownDraw = true, int offX = 0, int offY = 0);
void drawCircle(Circle& cir, bool ownDraw = true, int offX = 0, int offY = 0);
void drawText(Text& txt, bool ownDraw = true, int offX = 0, int offY = 0);
void drawImage(Image& img, bool ownDraw = true, int offX = 0, int offY = 0);
void drawLine(Line& ln, bool ownDraw = true, int offX = 0, int offY = 0);
void drawInputBox(InputBox& box, bool ownDraw = true, int offX = 0, int offY = 0);
void drawSlider(Slider& s, bool ownDraw = true, int offX = 0, int offY = 0);
void drawRangeSlider(RangeSlider& s, int offX = 0, int offY = 0);

void UpdateCheckBoxButton(CheckBox& box, SDL_Renderer* renderer);
void UpdateRadioButton(RadioButton& radio, bool ownDraw = true);

void drawElement(UIElement& el, bool ownDraw = true, int offX = 0, int offY = 0);
```

Each `draw*` function renders one instance of its corresponding element type (`Box`, `Circle`, `Text`, `Image`, `Line`, `InputBox`, `Slider`, `RangeSlider`).

- `ownDraw` — when `true`, the function performs a full, self-contained draw (flushing its own vertices/geometry as needed). When `false`, it assumes it's being called as part of a larger batched render pass managed externally (e.g. from `drawElement` or a custom render loop), and skips the parts of the draw that only make sense standalone.
- `offX` / `offY` — pixel offset applied to the element's position for this draw call, without moving the element's actual `rect`/`uRect`. Useful for scrolling containers, shake effects, or drawing a "ghost" copy elsewhere.

`UpdateCheckBoxButton` refreshes a checkbox's rendered visual state (including its animation) to match `isOn`, using the given `SDL_Renderer` directly rather than the `offX`/`offY`/`ownDraw` pattern the others use.

`UpdateRadioButton` does the equivalent for a `RadioButton`, including enforcing mutual exclusivity within its `group`.

`drawElement` is the generic dispatcher: given a `UIElement` variant, it inspects the held type and calls the matching `draw*` function above. This is what `UpdateAll()` calls internally for every object in `mainSpace.uiElements` each frame — reach for it directly only if you're iterating `uiElements` yourself.

**Example — manually redraw a single box outside the normal frame:**

```cpp
Box* b = findElement<Box>("btn1");
if (b) {
    drawBox(*b, true, 10, 0); // draw 10px to the right of its normal position
}
```

### Canvas & Pixel Access

```cpp
void    createCanvas(Window* win, const char* name, int w, int h);
void    renderCanvas(const char* name, Rect rect);

void    setPixel(Window* win, const char* name, int x, int y, Color color);
Color   getPixel(Window* win, const char* canvasName, int x, int y);
```

`createCanvas` allocates a named off-screen pixel surface of size `w`×`h` inside the window.  
`setPixel` / `getPixel` read and write individual pixels on that canvas by name — useful for procedural textures, software effects, or pixel-art tools.  
`renderCanvas` draws the named canvas into the given destination rectangle.

**Example — plasma-style pixel canvas:**

```cpp
Window wnd;

Main {
    wnd = createWindow("Pixels", {100, 100, 640, 480},
                       WINDOW_SHOWN, RENDERER_OPTIMAL, "win");

    // 256x256 off-screen pixel buffer
    createCanvas(wnd, "plasma", 256, 256);
}

EventLoop {
    static float t = 0.0f;
    t += mainSpace.deltaTime;

    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            uint8_t v = (uint8_t)(127 + 127 * sinf(x * 0.05f + t) * cosf(y * 0.05f + t));
            setPixel(wnd, "plasma", x, y, {v, (uint8_t)(255 - v), 128, 255});
        }
    }

    // sample a pixel back (e.g. for collision/color picking)
    Color center = getPixel(wnd, "plasma", 128, 128);

    // upscale the 256x256 buffer onto the window
    renderCanvas("plasma", {0, 0, 640, 480});
}
```

`setPixel` / `getPixel` operate on CPU-side pixel data, so touching a lot of pixels every frame (as above) is fine for small canvases but will get expensive on large ones — batch your changes per frame rather than calling them sporadically across the loop.

### Frame / Render Control

```cpp
void flush(Window* win);
void drawFrame(Window* win);
```

`flush` submits all buffered drawing operations for the window to the GPU (uploads vertex/index data).  
`drawFrame` composites everything that was flushed — including any canvases — and presents the final image to the screen. Both are normally called once per frame; `mainSpace` handles this automatically in the standard `Main` / `EventLoop` structure, but they're exposed for manual/advanced render control.

### Advanced Rendering (Engine-level)

```cpp
bool renderSceneAndCapture(int captureX, int captureY, int w, int h,
                            std::vector<Color>& outPixels, uint32_t& imageIndexOut);

void renderOverlayAndPresent(uint32_t imageIndex);
```

These two live on `Window::eng` (the underlying `Engine` instance) and split a normal `drawFrame` into two explicit steps — useful when you need the rendered pixels on the CPU *before* the frame is shown (screenshots, thumbnails, post-processing, mirrors/render-to-texture) while still drawing UI on top afterwards.

- **`renderSceneAndCapture`** submits the currently queued draw calls, then reads back a `w`×`h` region starting at `(captureX, captureY)` into `outPixels` (one `Color` per pixel, row-major). It does **not** present the frame — the swapchain image is left acquired, and its index is written to `imageIndexOut` so you can hand it off to the next call. Returns `false` if there was nothing to draw or the swapchain needed to be recreated (in which case just skip the frame).
- **`renderOverlayAndPresent`** takes the `imageIndexOut` from the call above, renders any vertices queued *after* the capture (e.g. UI drawn afterwards) on top of that same image, and presents it to the screen.

**Example — capture the scene to a CPU buffer, then draw a HUD on top:**

```cpp
Window* wnd;

Main {
    wnd = createWindow("Capture", {100, 100, 800, 600},
                       WINDOW_SHOWN, RENDERER_OPTIMAL, "win");

    createBox(wnd, {50, 50, 200, 50}, {70, 130, 255, 255}, "scene_box");
}

EventLoop {
    std::vector<Color> pixels;
    uint32_t imageIndex;

    // 1. flush the scene draw calls and read them back on the CPU
    bool ok = wnd->eng.renderSceneAndCapture(0, 0, 800, 600, pixels, imageIndex);

    if (ok) {
        // pixels[] now holds the rendered frame — e.g. save a screenshot,
        // run analysis, or feed it into another system
        Color topLeft = pixels[0];

        // 2. queue overlay content (drawn after the capture)
        createText(wnd, "NotoSans", "Captured!", {10, 10, 200, 30},
                   {255, 255, 255, 255}, 18, "hud_label");
        drawText(getObjectAs<Text>("hud_label")); // or your usual draw call for it

        // 3. composite the overlay on top of the captured frame and present
        wnd->eng.renderOverlayAndPresent(imageIndex);
    }
}
```

Because `renderSceneAndCapture` performs a GPU readback (`vkQueueWaitIdle` under the hood), it's noticeably slower than a plain `drawFrame` — reach for it only when you actually need the pixels on the CPU that frame, not as a replacement for the normal render path.

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

### Keyboard

```cpp
const uint8_t* GetKeyboardState(int* numKeys);
Keymod         GetModState();
void           SetModState(Keymod modstate);
void           ResetKeyboard();

Keycode        GetKeyFromScancode(Scancode scancode);
Scancode       GetScancodeFromKey(Keycode key);

const char*    GetKeyName(Keycode key);
Keycode        GetKeyFromName(const char* name);
const char*    GetScancodeName(Scancode scancode);
Scancode       GetScancodeFromName(const char* name);
```

`GetKeyboardState` returns a live array of key states (1 = pressed, 0 = released), indexed by `Scancode`.  
`GetModState` / `SetModState` read or force the current modifier bitmask (see [Keyboard Modifiers](#keyboard-modifiers)).  
The `*FromName` / `*Name` helpers convert between human-readable key names and their `Keycode` / `Scancode` values.

### Text Input (IME)

```cpp
bool HasScreenKeyboardSupport();
bool IsScreenKeyboardShown(Window* window);

bool IsTextInputActive();
void StartTextInput();
void StopTextInput();

void SetTextInputRect(Rect* rect);
void ClearComposition();
bool IsTextInputShown();
```

`StartTextInput` / `StopTextInput` toggle SDL's text-input events (needed for `InputBox` and any custom Unicode/IME entry).  
`SetTextInputRect` tells the OS/IME where on screen to anchor its composition popup.  
`HasScreenKeyboardSupport` / `IsScreenKeyboardShown` are relevant on touch platforms with an on-screen keyboard.

### Mouse

```cpp
Window*  GetMouseFocus();

uint32_t GetMouseState(int* x, int* y);
uint32_t GetGlobalMouseState(int* x, int* y);
uint32_t GetRelativeMouseState(int* x, int* y);

void     WarpMouseInWindow(Window* window, int x, int y);
int      WarpMouseGlobal(int x, int y);

int      SetRelativeMouseMode(bool enabled);
bool     GetRelativeMouseMode();

int      CaptureMouse(bool enabled);
```

`GetMouseState` gives position relative to the focused window; `GetGlobalMouseState` gives absolute screen coordinates; `GetRelativeMouseState` gives movement delta since the last call. All three return a `BUTTON_*MASK` bitmask of currently pressed buttons.  
`WarpMouseInWindow` / `WarpMouseGlobal` teleport the cursor. `SetRelativeMouseMode` is typically used for FPS-style camera look. `CaptureMouse` keeps receiving mouse events even when the cursor leaves the window (e.g. during a drag).

### Cursors

```cpp
Cursor* CreateCursor(const uint8_t* data, const uint8_t* mask,
                     int w, int h, int hot_x, int hot_y);
Cursor* CreateColorCursor(SDL_Surface* surface, int hot_x, int hot_y);
Cursor* CreateSystemCursor(SystemCursor id);

void    SetCursor(Cursor* cursor);
Cursor* GetActiveCursor();
Cursor* GetDefaultCursor();
void    FreeCursor(Cursor* cursor);

int     SetCursorVisible(int toggle);
```

`CreateSystemCursor` is the easiest way to get a standard OS cursor shape (see [Cursors](#cursors) table below for `SYSTEM_CURSOR_*` values). `CreateCursor` / `CreateColorCursor` build custom cursors from raw bitmap data or an `SDL_Surface`. Cursors created manually must be released with `FreeCursor` when no longer needed.

### Timing

```cpp
uint64_t GetPerformanceCounter();
uint64_t GetPerformanceFrequency();

uint32_t GetTicks();
uint64_t GetTicks64();

void     Delay(uint32_t ms);
```

`GetPerformanceCounter` / `GetPerformanceFrequency` give a high-resolution timer (divide the counter delta by the frequency to get seconds) — useful for computing `deltaTime` manually. `GetTicks` / `GetTicks64` return milliseconds since library init. `Delay` blocks the calling thread.

### Timers

```cpp
TimerID AddTimer(uint32_t interval, TimerCallback callback, void* param);
bool    RemoveTimer(TimerID id);
```

Schedules `callback` to run every `interval` milliseconds on a separate thread. `RemoveTimer` cancels it.

---

## Data Types

```cpp
struct Rect        { uint16_t x, y, w, h; };
struct UniformRect { int x, y, size; };
struct Color       { uint8_t r, g, b, a; };
struct Cursor;   // SDL_Cursor
struct Surface;  // SDL_Surface
```

**Timer aliases**

```cpp
using TimerCallback = SDL_TimerCallback;
using TimerID        = SDL_TimerID;
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

## Keyboard Modifiers

Key modifier state is exposed as a bitmask (`KMOD_*`), typically checked alongside `EVENT_KEY_DOWN` / `EVENT_KEY_UP`:

|Flag|Value|Description|
|---|---|---|
|`KMOD_NONE`|`0x0000`|No modifier keys held|
|`KMOD_LSHIFT`|`0x0001`|Left Shift held|
|`KMOD_RSHIFT`|`0x0002`|Right Shift held|
|`KMOD_LCTRL`|`0x0040`|Left Ctrl held|
|`KMOD_RCTRL`|`0x0080`|Right Ctrl held|
|`KMOD_LALT`|`0x0100`|Left Alt held|
|`KMOD_RALT`|`0x0200`|Right Alt held|
|`KMOD_LGUI`|`0x0400`|Left GUI/Super/Win key held|
|`KMOD_RGUI`|`0x0800`|Right GUI/Super/Win key held|
|`KMOD_NUM`|`0x1000`|Num Lock active|
|`KMOD_CAPS`|`0x2000`|Caps Lock active|
|`KMOD_MODE`|`0x4000`|AltGr/Mode key active|
|`KMOD_SCROLL`|`0x8000`|Scroll Lock active|

**Combined masks**

|Flag|Composition|Description|
|---|---|---|
|`KMOD_CTRL`|`KMOD_LCTRL \| KMOD_RCTRL`|Either Ctrl key|
|`KMOD_SHIFT`|`KMOD_LSHIFT \| KMOD_RSHIFT`|Either Shift key|
|`KMOD_ALT`|`KMOD_LALT \| KMOD_RALT`|Either Alt key|
|`KMOD_GUI`|`KMOD_LGUI \| KMOD_RGUI`|Either GUI key|
|`KMOD_RESERVED`|`KMOD_SCROLL`|Reserved, mirrors Scroll Lock|

---

## Mouse Buttons

Button bitmask values, as returned by `GetMouseState` / `GetGlobalMouseState` / `GetRelativeMouseState`:

|Flag|Description|
|---|---|
|`BUTTON_LEFT`|Left button index (1)|
|`BUTTON_MIDDLE`|Middle button index (2)|
|`BUTTON_RIGHT`|Right button index (3)|
|`BUTTON_X1`|Extra button 1 index (4)|
|`BUTTON_X2`|Extra button 2 index (5)|
|`BUTTON_LMASK`|Left button pressed bitmask|
|`BUTTON_MMASK`|Middle button pressed bitmask|
|`BUTTON_RMASK`|Right button pressed bitmask|
|`BUTTON_X1MASK`|X1 button pressed bitmask|
|`BUTTON_X2MASK`|X2 button pressed bitmask|

---

## Cursors

```cpp
struct Cursor;   // SDL_Cursor
struct Surface;  // SDL_Surface
```

System cursor types:

|Cursor|Description|
|---|---|
|`SYSTEM_CURSOR_ARROW`|Standard arrow pointer|
|`SYSTEM_CURSOR_IBEAM`|Text I-beam cursor|
|`SYSTEM_CURSOR_WAIT`|Wait/busy cursor|
|`SYSTEM_CURSOR_CROSSHAIR`|Crosshair cursor|
|`SYSTEM_CURSOR_WAITARROW`|Arrow with wait indicator|
|`SYSTEM_CURSOR_SIZENWSE`|Resize cursor, NW-SE diagonal|
|`SYSTEM_CURSOR_SIZENESW`|Resize cursor, NE-SW diagonal|
|`SYSTEM_CURSOR_SIZEWE`|Resize cursor, horizontal|
|`SYSTEM_CURSOR_SIZENS`|Resize cursor, vertical|
|`SYSTEM_CURSOR_SIZEALL`|Resize cursor, all directions|
|`SYSTEM_CURSOR_NO`|"Not allowed" cursor|
|`SYSTEM_CURSOR_HAND`|Hand/pointer cursor|
|`NUM_SYSTEM_CURSORS`|Count of available system cursors|

---

## Timers

```cpp
using TimerCallback = SDL_TimerCallback;
using TimerID        = SDL_TimerID;
```

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
|`RENDERER_SDL_VULKAN`|Use Vulkan like a renderer|
|`RENDERER_SDL_ACCELERATED`|Use hardware-accelerated rendering|
|`RENDERER_SDL_SOFTWARE`|Use software (CPU) rendering|
|`RENDERER_SDL_PRESENTVSYNC`|Sync to monitor refresh rate (V-Sync)|
|`RENDERER_SDL_TARGETTEXTURE`|Renderer supports rendering to a texture|

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
- `UpdateAll()` is called automatically every frame.
- Every object has a `Visible` property.