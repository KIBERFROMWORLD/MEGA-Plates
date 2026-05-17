#define MINIAUDIO_IMPLEMENTATION

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "miniaudio.h"

#include <variant>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <set>
#include <filesystem>
#include <algorithm>
#include <csignal>
#include <cmath>
#include <functional>
#include <bitset>
#include <sstream>

#define Main \
    void entry()

#define EventLoop \
    void loop()

enum WindowFlags {
    WINDOW_FULLSCREEN          = SDL_WINDOW_FULLSCREEN,
    WINDOW_FULLSCREEN_DESKTOP  = SDL_WINDOW_FULLSCREEN_DESKTOP,
    WINDOW_OPENGL              = SDL_WINDOW_OPENGL,
    WINDOW_SHOWN               = SDL_WINDOW_SHOWN,
    WINDOW_HIDDEN              = SDL_WINDOW_HIDDEN,
    WINDOW_BORDERLESS          = SDL_WINDOW_BORDERLESS,
    WINDOW_RESIZABLE           = SDL_WINDOW_RESIZABLE,
    WINDOW_MINIMIZED           = SDL_WINDOW_MINIMIZED,
    WINDOW_MAXIMIZED           = SDL_WINDOW_MAXIMIZED,
    WINDOW_INPUT_GRABBED       = SDL_WINDOW_INPUT_GRABBED,
    WINDOW_INPUT_FOCUS         = SDL_WINDOW_INPUT_FOCUS,
    WINDOW_MOUSE_FOCUS         = SDL_WINDOW_MOUSE_FOCUS,
    WINDOW_FOREIGN             = SDL_WINDOW_FOREIGN,
    WINDOW_ALLOW_HIGHDPI       = SDL_WINDOW_ALLOW_HIGHDPI,
    WINDOW_MOUSE_CAPTURE       = SDL_WINDOW_MOUSE_CAPTURE,
    WINDOW_ALWAYS_ON_TOP       = SDL_WINDOW_ALWAYS_ON_TOP,
    WINDOW_SKIP_TASKBAR        = SDL_WINDOW_SKIP_TASKBAR,
    WINDOW_UTILITY             = SDL_WINDOW_UTILITY,
    WINDOW_TOOLTIP             = SDL_WINDOW_TOOLTIP,
    WINDOW_POPUP_MENU          = SDL_WINDOW_POPUP_MENU,
    WINDOW_VULKAN              = SDL_WINDOW_VULKAN
};

enum RendererFlags {
    RENDERER_SOFTWARE      = SDL_RENDERER_SOFTWARE,
    RENDERER_ACCELERATED   = SDL_RENDERER_ACCELERATED,
    RENDERER_PRESENTVSYNC  = SDL_RENDERER_PRESENTVSYNC,
    RENDERER_TARGETTEXTURE = SDL_RENDERER_TARGETTEXTURE
};

enum SoundFlags {
    SOUND_FLAG_DECODE              = MA_SOUND_FLAG_DECODE,
    SOUND_FLAG_STREAM              = MA_SOUND_FLAG_STREAM,
    SOUND_FLAG_ASYNC               = MA_SOUND_FLAG_ASYNC,
    SOUND_FLAG_NO_DEFAULT_ATTACHMENT = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT
};

enum EventTypes : uint64_t {
    EVENT_NONE                       = 0,
    EVENT_KEY_DOWN                   = 1ULL << 0,
    EVENT_KEY_UP                     = 1ULL << 1,
    EVENT_MOUSE_MOTION               = 1ULL << 2,
    EVENT_MOUSE_BUTTON_DOWN          = 1ULL << 3,
    EVENT_MOUSE_BUTTON_UP            = 1ULL << 4,
    EVENT_WINDOW_RESIZED             = 1ULL << 5,
    EVENT_WINDOW_MOVED               = 1ULL << 6,
    EVENT_QUIT                       = 1ULL << 7,
    EVENT_FINGER_DOWN                = 1ULL << 8,
    EVENT_FINGER_UP                  = 1ULL << 9,
    EVENT_FINGER_MOTION              = 1ULL << 10,
    EVENT_CLIPBOARD_UPDATE           = 1ULL << 11,
    EVENT_DROP_FILE                  = 1ULL << 12,
    EVENT_DROP_TEXT                  = 1ULL << 13,
    EVENT_DROP_BEGIN                 = 1ULL << 14,
    EVENT_DROP_COMPLETE              = 1ULL << 15,
    EVENT_AUDIO_DEVICE_ADDED         = 1ULL << 16,
    EVENT_AUDIO_DEVICE_REMOVED       = 1ULL << 17,
    EVENT_TERMINATING                = 1ULL << 18,
    EVENT_LOW_MEMORY                 = 1ULL << 19,
    EVENT_WILL_ENTER_BACKGROUND      = 1ULL << 20,
    EVENT_DID_ENTER_BACKGROUND       = 1ULL << 21,
    EVENT_WILL_ENTER_FOREGROUND      = 1ULL << 22,
    EVENT_DID_ENTER_FOREGROUND       = 1ULL << 23,
    EVENT_JOYSTICK_AXIS_MOTION       = 1ULL << 24,
    EVENT_JOYSTICK_BALL_MOTION       = 1ULL << 25,
    EVENT_JOYSTICK_HAT_MOTION        = 1ULL << 26,
    EVENT_WINDOWEVENT_MAXIMIZED      = 1ULL << 27,
    EVENT_WINDOWEVENT_MINIMIZED      = 1ULL << 28,
    EVENT_WINDOWEVENT_ENTER          = 1ULL << 29,
    EVENT_WINDOWEVENT_EXPOSED        = 1ULL << 30,
    EVENT_WINDOWEVENT_DISPLAY_CHANGED= 1ULL << 31,
    EVENT_WINDOWEVENT_FOCUS_LOST     = 1ULL << 32,
    EVENT_WINDOWEVENT_LEAVE          = 1ULL << 33,
    EVENT_WINDOWEVENT_SHOWN          = 1ULL << 34,
    EVENT_WINDOWEVENT_TAKE_FOCUS     = 1ULL << 35,
    EVENT_WINDOWEVENT_SIZE_CHANGED   = 1ULL << 36,
    EVENT_JOYSTICK_BATTERY_UPDATED   = 1ULL << 37,
    EVENT_JOYSTICK_BUTTON_DOWN       = 1ULL << 38,
    EVENT_JOYSTICK_BUTTON_UP         = 1ULL << 39,
    EVENT_JOYSTICK_DEVICE_ADDED      = 1ULL << 40,
    EVENT_JOYSTICK_DEVICE_REMOVED    = 1ULL << 41,
    EVENT_JOYSTICK_POWER_EMPTY       = 1ULL << 42,
    EVENT_JOYSTICK_POWER_LOW         = 1ULL << 43,
    EVENT_JOYSTICK_POWER_MIN         = 1ULL << 44,
    EVENT_JOYSTICK_POWER_MAX         = 1ULL << 45,
    EVENT_JOYSTICK_POWER_FULL        = 1ULL << 46,
    EVENT_JOYSTICK_POWER_MEDIUM      = 1ULL << 47,
    EVENT_JOYSTICK_POWER_UNKNOWN     = 1ULL << 48,
    EVENT_JOYSTICK_POWER_WIRED       = 1ULL << 49,
};

enum Keys {
    SCANCODE_UNKNOWN = SDL_SCANCODE_UNKNOWN,
    SCANCODE_A = SDL_SCANCODE_A, SCANCODE_B = SDL_SCANCODE_B,
    SCANCODE_C = SDL_SCANCODE_C, SCANCODE_D = SDL_SCANCODE_D,
    SCANCODE_E = SDL_SCANCODE_E, SCANCODE_F = SDL_SCANCODE_F,
    SCANCODE_G = SDL_SCANCODE_G, SCANCODE_H = SDL_SCANCODE_H,
    SCANCODE_I = SDL_SCANCODE_I, SCANCODE_J = SDL_SCANCODE_J,
    SCANCODE_K = SDL_SCANCODE_K, SCANCODE_L = SDL_SCANCODE_L,
    SCANCODE_M = SDL_SCANCODE_M, SCANCODE_N = SDL_SCANCODE_N,
    SCANCODE_O = SDL_SCANCODE_O, SCANCODE_P = SDL_SCANCODE_P,
    SCANCODE_Q = SDL_SCANCODE_Q, SCANCODE_R = SDL_SCANCODE_R,
    SCANCODE_S = SDL_SCANCODE_S, SCANCODE_T = SDL_SCANCODE_T,
    SCANCODE_U = SDL_SCANCODE_U, SCANCODE_V = SDL_SCANCODE_V,
    SCANCODE_W = SDL_SCANCODE_W, SCANCODE_X = SDL_SCANCODE_X,
    SCANCODE_Y = SDL_SCANCODE_Y, SCANCODE_Z = SDL_SCANCODE_Z,
    SCANCODE_1 = SDL_SCANCODE_1, SCANCODE_2 = SDL_SCANCODE_2,
    SCANCODE_3 = SDL_SCANCODE_3, SCANCODE_4 = SDL_SCANCODE_4,
    SCANCODE_5 = SDL_SCANCODE_5, SCANCODE_6 = SDL_SCANCODE_6,
    SCANCODE_7 = SDL_SCANCODE_7, SCANCODE_8 = SDL_SCANCODE_8,
    SCANCODE_9 = SDL_SCANCODE_9, SCANCODE_0 = SDL_SCANCODE_0,
    SCANCODE_RETURN     = SDL_SCANCODE_RETURN,
    SCANCODE_ESCAPE     = SDL_SCANCODE_ESCAPE,
    SCANCODE_BACKSPACE  = SDL_SCANCODE_BACKSPACE,
    SCANCODE_TAB        = SDL_SCANCODE_TAB,
    SCANCODE_SPACE      = SDL_SCANCODE_SPACE,
    SCANCODE_MINUS      = SDL_SCANCODE_MINUS,
    SCANCODE_EQUALS     = SDL_SCANCODE_EQUALS,
    SCANCODE_LEFTBRACKET  = SDL_SCANCODE_LEFTBRACKET,
    SCANCODE_RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
    SCANCODE_BACKSLASH  = SDL_SCANCODE_BACKSLASH,
    SCANCODE_NONUSHASH  = SDL_SCANCODE_NONUSHASH,
    SCANCODE_SEMICOLON  = SDL_SCANCODE_SEMICOLON,
    SCANCODE_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
    SCANCODE_GRAVE      = SDL_SCANCODE_GRAVE,
    SCANCODE_COMMA      = SDL_SCANCODE_COMMA,
    SCANCODE_PERIOD     = SDL_SCANCODE_PERIOD,
    SCANCODE_SLASH      = SDL_SCANCODE_SLASH,
    SCANCODE_CAPSLOCK   = SDL_SCANCODE_CAPSLOCK,
    SCANCODE_F1  = SDL_SCANCODE_F1,  SCANCODE_F2  = SDL_SCANCODE_F2,
    SCANCODE_F3  = SDL_SCANCODE_F3,  SCANCODE_F4  = SDL_SCANCODE_F4,
    SCANCODE_F5  = SDL_SCANCODE_F5,  SCANCODE_F6  = SDL_SCANCODE_F6,
    SCANCODE_F7  = SDL_SCANCODE_F7,  SCANCODE_F8  = SDL_SCANCODE_F8,
    SCANCODE_F9  = SDL_SCANCODE_F9,  SCANCODE_F10 = SDL_SCANCODE_F10,
    SCANCODE_F11 = SDL_SCANCODE_F11, SCANCODE_F12 = SDL_SCANCODE_F12,
    SCANCODE_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
    SCANCODE_SCROLLLOCK  = SDL_SCANCODE_SCROLLLOCK,
    SCANCODE_PAUSE       = SDL_SCANCODE_PAUSE,
    SCANCODE_INSERT      = SDL_SCANCODE_INSERT,
    SCANCODE_HOME        = SDL_SCANCODE_HOME,
    SCANCODE_PAGEUP      = SDL_SCANCODE_PAGEUP,
    SCANCODE_DELETE      = SDL_SCANCODE_DELETE,
    SCANCODE_END         = SDL_SCANCODE_END,
    SCANCODE_PAGEDOWN    = SDL_SCANCODE_PAGEDOWN,
    SCANCODE_RIGHT       = SDL_SCANCODE_RIGHT,
    SCANCODE_LEFT        = SDL_SCANCODE_LEFT,
    SCANCODE_DOWN        = SDL_SCANCODE_DOWN,
    SCANCODE_UP          = SDL_SCANCODE_UP,
    SCANCODE_KP_DIVIDE   = SDL_SCANCODE_KP_DIVIDE,
    SCANCODE_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
    SCANCODE_KP_MINUS    = SDL_SCANCODE_KP_MINUS,
    SCANCODE_KP_PLUS     = SDL_SCANCODE_KP_PLUS,
    SCANCODE_KP_ENTER    = SDL_SCANCODE_KP_ENTER,
    SCANCODE_KP_1 = SDL_SCANCODE_KP_1, SCANCODE_KP_2 = SDL_SCANCODE_KP_2,
    SCANCODE_KP_3 = SDL_SCANCODE_KP_3, SCANCODE_KP_4 = SDL_SCANCODE_KP_4,
    SCANCODE_KP_5 = SDL_SCANCODE_KP_5, SCANCODE_KP_6 = SDL_SCANCODE_KP_6,
    SCANCODE_KP_7 = SDL_SCANCODE_KP_7, SCANCODE_KP_8 = SDL_SCANCODE_KP_8,
    SCANCODE_KP_9 = SDL_SCANCODE_KP_9, SCANCODE_KP_0 = SDL_SCANCODE_KP_0,
    SCANCODE_KP_PERIOD = SDL_SCANCODE_KP_PERIOD,
};

namespace TweenType {
    float Linear(float t)   { return t; }
    float EaseIn(float t)   { return t * t; }
    float EaseOut(float t)  { return t * (2 - t); }
    float EaseInOut(float t){ return t < 0.5f ? 2*t*t : -1+(4-2*t)*t; }
    float Cubic(float t)    { return t*t*t; }
    float Elastic(float t)  { float p=0.3f; return pow(2,-10*t)*sin((t-p/4)*(2*M_PI)/p)+1; }
    float Bounce(float t) {
        if (t < 1/2.75f) return 7.5625f*t*t;
        if (t < 2/2.75f) { t-=1.5f/2.75f; return 7.5625f*t*t+.75f; }
        if (t < 2.5f/2.75f) { t-=2.25f/2.75f; return 7.5625f*t*t+.9375f; }
        t-=2.625f/2.75f; return 7.5625f*t*t+.984375f;
    }
    float Back(float t)     { float s=1.70158f; return t*t*((s+1)*t-s); }
    const float PI = 3.14159265358979323846f;
    float SineIn(float t)   { return 1 - cos((t*PI)/2); }
    float SineOut(float t)  { return sin((t*PI)/2); }
    float SineInOut(float t){ return -(cos(PI*t)-1)/2; }
    float QuadIn(float t)   { return t*t; }
    float QuadOut(float t)  { return t*(2-t); }
    float QuadInOut(float t){ return t<0.5f ? 2*t*t : 1-pow(-2*t+2,2)/2; }
    float QuartIn(float t)  { return t*t*t*t; }
    float QuartOut(float t) { return 1-pow(1-t,4); }
    float QuartInOut(float t){ return t<0.5f ? 8*t*t*t*t : 1-pow(-2*t+2,4)/2; }
    float QuintIn(float t)  { return t*t*t*t*t; }
    float QuintOut(float t) { return 1-pow(1-t,5); }
    float QuintInOut(float t){ return t<0.5f ? 16*t*t*t*t*t : 1-pow(-2*t+2,5)/2; }
    float ExpoIn(float t)   { return t==0 ? 0 : pow(2,10*t-10); }
    float ExpoOut(float t)  { return t==1 ? 1 : 1-pow(2,-10*t); }
    float ExpoInOut(float t){
        if(t==0) return 0; if(t==1) return 1;
        return t<0.5f ? pow(2,20*t-10)/2 : (2-pow(2,-20*t+10))/2;
    }
    float CircIn(float t)   { return 1-sqrt(1-pow(t,2)); }
    float CircOut(float t)  { return sqrt(1-pow(t-1,2)); }
    float CircInOut(float t){
        return t<0.5f ? (1-sqrt(1-pow(2*t,2)))/2 : (sqrt(1-pow(-2*t+2,2))+1)/2;
    }
    float ElasticIn(float t){
        if(t==0) return 0; if(t==1) return 1;
        return -pow(2,10*t-10)*sin((t*10-10.75f)*((2*PI)/3));
    }
    float ElasticOut(float t){
        if(t==0) return 0; if(t==1) return 1;
        return pow(2,-10*t)*sin((t*10-0.75f)*((2*PI)/3))+1;
    }
}

class Tween {
    float elapsed = 0; bool done = false;
public:
    float* target; float from, to, duration;
    std::function<float(float)> anim;
    void update(float dt) {
        elapsed += dt;
        float t = std::min(elapsed/duration, 1.0f);
        *target = from + (to-from)*anim(t);
        if (t >= 1.0f) done = true;
    }
};

struct Rect  { uint16_t x, y, w, h; };
struct Color { uint8_t  r, g, b, a; };

struct Window {
    SDL_Window*   wnd;
    SDL_Renderer* renderer;
    std::string   name;
    SDL_Rect      rect;
};

struct Renderer {
    SDL_Renderer* renderer;
};

struct Button {
    Rect rect; Color color; std::string name; Window* win; int rad = 0;
};

struct Text {
    Rect rect; Color color;
    std::string pathToFont; int fontSize;
    std::string text; std::string name; Window* win;
    TTF_Font*    font = nullptr;
    SDL_Surface* surf = nullptr;
    SDL_Texture* tex  = nullptr;

    Text() = default;
    Text(Rect r, Color c, std::string pf, int fs, std::string t,
         std::string n, Window* w, TTF_Font* f, SDL_Surface* s, SDL_Texture* tx)
        : rect(r), color(c), pathToFont(std::move(pf)), fontSize(fs),
          text(std::move(t)), name(std::move(n)), win(w),
          font(f), surf(s), tex(tx) {}
    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&& o) noexcept
        : rect(o.rect), color(o.color), pathToFont(std::move(o.pathToFont)),
          fontSize(o.fontSize), text(std::move(o.text)), name(std::move(o.name)),
          win(o.win), font(o.font), surf(o.surf), tex(o.tex) {
        o.font = nullptr; o.surf = nullptr; o.tex = nullptr; o.win = nullptr;
    }
    Text& operator=(Text&& o) noexcept {
        if (this != &o) {
            if (font) TTF_CloseFont(font);
            if (tex)  SDL_DestroyTexture(tex);
            if (surf) SDL_FreeSurface(surf);
            rect=o.rect; color=o.color; pathToFont=std::move(o.pathToFont);
            fontSize=o.fontSize; text=std::move(o.text); name=std::move(o.name);
            win=o.win; font=o.font; surf=o.surf; tex=o.tex;
            o.font=nullptr; o.surf=nullptr; o.tex=nullptr; o.win=nullptr;
        }
        return *this;
    }
    ~Text() {
        if (font) TTF_CloseFont(font);
        if (tex)  SDL_DestroyTexture(tex);
        if (surf) SDL_FreeSurface(surf);
    }
};

struct Image {
    Rect rect; std::string pathToImage;
    SDL_Texture* tex = nullptr;
    std::string name; Window* win;

    Image() = default;
    Image(Rect r, std::string p, SDL_Texture* t, std::string n, Window* w)
        : rect(r), pathToImage(std::move(p)), tex(t),
          name(std::move(n)), win(w) {}
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&& o) noexcept
        : rect(o.rect), pathToImage(std::move(o.pathToImage)),
          tex(o.tex), name(std::move(o.name)), win(o.win) {
        o.tex = nullptr; o.win = nullptr;
    }
    Image& operator=(Image&& o) noexcept {
        if (this != &o) {
            if (tex) SDL_DestroyTexture(tex);
            rect=o.rect; pathToImage=std::move(o.pathToImage);
            tex=o.tex; name=std::move(o.name); win=o.win;
            o.tex=nullptr; o.win=nullptr;
        }
        return *this;
    }
    ~Image() {
        if (tex) SDL_DestroyTexture(tex);
    }
};

struct UniformRect { int x, y, size; };

struct Circle {
    UniformRect uRect; Color clr; std::string name; Window* win;
};

struct InputBox {
    Rect rect; Color clr; std::string name;
    std::string hint, text; Window* win;
    int  carrot = 0; bool focus = false;
    TTF_Font*    font = nullptr; int fontSize;
    std::string  buf, pathToFont;
    std::vector<int> textWidth;
    bool changed = false; int animTime = 0; bool animBack = false;
    int selectStart = -1, selectEnd = -1;

    InputBox() = default;
    InputBox(Rect r, Color c, std::string n, std::string h,
             std::string t, Window* w, TTF_Font* f, int fs,
             std::string b, std::string pf)
        : rect(r), clr(c), name(std::move(n)), hint(std::move(h)),
          text(std::move(t)), win(w), font(f), fontSize(fs),
          buf(std::move(b)), pathToFont(std::move(pf)) {}
    InputBox(const InputBox&) = delete;
    InputBox& operator=(const InputBox&) = delete;
    InputBox(InputBox&& o) noexcept
        : rect(o.rect), clr(o.clr), name(std::move(o.name)),
          hint(std::move(o.hint)), text(std::move(o.text)), win(o.win),
          carrot(o.carrot), focus(o.focus), font(o.font), fontSize(o.fontSize),
          buf(std::move(o.buf)), pathToFont(std::move(o.pathToFont)),
          textWidth(std::move(o.textWidth)), changed(o.changed),
          animTime(o.animTime), animBack(o.animBack),
          selectStart(o.selectStart), selectEnd(o.selectEnd) {
        o.font = nullptr; o.win = nullptr;
    }
    InputBox& operator=(InputBox&& o) noexcept {
        if (this != &o) {
            if (font) TTF_CloseFont(font);
            rect=o.rect; clr=o.clr; name=std::move(o.name);
            hint=std::move(o.hint); text=std::move(o.text); win=o.win;
            carrot=o.carrot; focus=o.focus; font=o.font; fontSize=o.fontSize;
            buf=std::move(o.buf); pathToFont=std::move(o.pathToFont);
            textWidth=std::move(o.textWidth); changed=o.changed;
            animTime=o.animTime; animBack=o.animBack;
            selectStart=o.selectStart; selectEnd=o.selectEnd;
            o.font=nullptr; o.win=nullptr;
        }
        return *this;
    }
    ~InputBox() {
        if (font) TTF_CloseFont(font);
    }
};

struct ScrollBar {
    Rect rect; Color color; std::string name; Window* win;
    float scrollingSize = 0.8f, pos = 0.0f;
    std::vector<std::string> elements;
    int rad = 0;
};

struct Slider {
    Rect rect; Color clrOff, clrOn; float progress;
    std::string name; Window* win;
};

struct RangeSlider {
    Rect rect; Color clrOff, clrOn;
    float progress, progress2;
    std::string name; Window* win;
};

struct CheckBox {
    UniformRect uRect; Color clr; std::string name;
    bool isOn, isAnim; Window* win;
};

struct Line {
    Rect cords; Color color; std::string name; Window* win;
};

struct RadioButton {
    UniformRect uRect;
    bool status = false, animPending = false;
    Color clrOff, clrOn;
    int   group;
    SDL_Renderer* renderer = nullptr;
    std::string name;
};

struct Sound {
    ma_sound sound; std::string name;
};

using UIElement = std::variant<
    Button,
    Circle,
    Text,
    Image,
    InputBox,
    CheckBox,
    RadioButton,
    Line,
    Slider,
    RangeSlider,
    ScrollBar
>;

struct GetName {
    const std::string& operator()(const Button&      e) const { return e.name; }
    const std::string& operator()(const Circle&      e) const { return e.name; }
    const std::string& operator()(const Text&        e) const { return e.name; }
    const std::string& operator()(const Image&       e) const { return e.name; }
    const std::string& operator()(const InputBox&    e) const { return e.name; }
    const std::string& operator()(const CheckBox&    e) const { return e.name; }
    const std::string& operator()(const RadioButton& e) const { return e.name; }
    const std::string& operator()(const Line&        e) const { return e.name; }
    const std::string& operator()(const Slider&      e) const { return e.name; }
    const std::string& operator()(const RangeSlider& e) const { return e.name; }
    const std::string& operator()(const ScrollBar&   e) const { return e.name; }
};

struct Space {
    std::vector<UIElement>                       uiElements;
    std::vector<Window*>                         wnds;
    std::vector<Renderer*>                       renderers;
    std::vector<Sound>                           sounds;
    std::vector<std::tuple<Color, Window*>>      backgrounds;
    std::vector<std::string>                     names;

    std::string                                  RatioOnNow = "";
    bool                                         running    = true;
    ma_engine                                    engine;
    uint64_t                                     Event      = 0;
};

Space mainSpace;

template<typename T>
T* findElement(const std::string& name) {
    for (auto& el : mainSpace.uiElements)
        if (std::holds_alternative<T>(el) && std::get<T>(el).name == name)
            return &std::get<T>(el);
    return nullptr;
}

bool isInScrollBar(const std::string& name, ScrollBar** outSB = nullptr) {
    for (auto& el : mainSpace.uiElements) {
        if (!std::holds_alternative<ScrollBar>(el)) continue;
        ScrollBar& sb = std::get<ScrollBar>(el);
        for (auto& en : sb.elements)
            if (en == name) { if (outSB) *outSB = &sb; return true; }
    }
    return false;
}

bool parseNums4(std::string s, uint16_t& a, uint16_t& b, uint16_t& c, uint16_t& d) {
    for (char& ch : s) if (ch=='{'||ch=='}'||ch==',') ch=' ';
    std::istringstream ss(s);

    return !!(ss >> a >> b >> c >> d);
}

bool parseNums3(std::string s, uint16_t& a, uint16_t& b, uint16_t& c) {
    for (char& ch : s) if (ch=='{'||ch=='}'||ch==',') ch=' ';
    std::istringstream ss(s);

    return !!(ss >> a >> b >> c);
}

std::string resolveFontPath(const std::string& path) {
    if (path.find('/') != std::string::npos ||
        path.find('\\') != std::string::npos ||
        (path.size() >= 2 && path[1] == ':'))
        return path;

    std::string base = "Fonts/" + path;

    if (base.find('.') == std::string::npos) {
        if (std::filesystem::exists(base+".ttf")) return base+".ttf";
        if (std::filesystem::exists(base+".otf")) return base+".otf";
    }

    return base;
}

void UpdateCheckBoxButton(CheckBox& box, SDL_Renderer* renderer);
void UpdateRadioButton   (RadioButton& ratio, SDL_Renderer* renderer);

void drawButton(Button& btn, int offX=0, int offY=0) {
    roundedBoxRGBA(btn.win->renderer,
        btn.rect.x+offX, btn.rect.y+offY,
        btn.rect.x+offX+btn.rect.w, btn.rect.y+offY+btn.rect.h,
        btn.rad, btn.color.r, btn.color.g, btn.color.b, btn.color.a);
}

void drawCircle(Circle& cir, int offX=0, int offY=0) {
    filledCircleRGBA(cir.win->renderer, cir.uRect.x+offX, cir.uRect.y+offY, cir.uRect.size,
        cir.clr.r, cir.clr.g, cir.clr.b, cir.clr.a);
    aacircleRGBA(cir.win->renderer, cir.uRect.x+offX, cir.uRect.y+offY, cir.uRect.size,
        cir.clr.r, cir.clr.g, cir.clr.b, cir.clr.a);
}

void drawText(Text& txt, int offX=0, int offY=0) {
    if (!txt.tex) return;

    SDL_Rect dst = {txt.rect.x+offX, txt.rect.y+offY, txt.surf->w, txt.surf->h};
    SDL_RenderCopy(txt.win->renderer, txt.tex, nullptr, &dst);
}

void drawImage(Image& img, int offX=0, int offY=0) {
    SDL_Rect r = {img.rect.x+offX, img.rect.y+offY, img.rect.w, img.rect.h};
    SDL_RenderCopy(img.win->renderer, img.tex, nullptr, &r);
}

void drawInputBox(InputBox& box, int offX=0, int offY=0) {
    if (!box.font) { std::cerr<<"ERROR: InputBox '"<<box.name<<"' has no font!\n"; return; }

    SDL_Rect clip = {box.rect.x+offX, box.rect.y+offY, box.rect.w, box.rect.h};

    SDL_RenderSetClipRect(box.win->renderer, &clip);
    roundedBoxRGBA(box.win->renderer,
        box.rect.x+offX, box.rect.y+offY,
        box.rect.x+offX+box.rect.w, box.rect.y+offY+box.rect.h,
        0, 70, 70, 70, 255);

    auto renderText = [&](const std::string& s, SDL_Color c, int x, int y){
        SDL_Surface* sf = TTF_RenderUTF8_Blended_Wrapped(box.font, s.c_str(), c, 0);
        if (!sf) return;
        SDL_Texture* tx = SDL_CreateTextureFromSurface(box.win->renderer, sf);
        if (tx) {
            SDL_Rect d = {x, y, sf->w, sf->h};
            SDL_RenderCopy(box.win->renderer, tx, nullptr, &d);
            SDL_DestroyTexture(tx);
        }
        SDL_FreeSurface(sf);
    };

    if (!box.buf.empty())
        renderText(box.buf, {box.clr.r,box.clr.g,box.clr.b,box.clr.a},
            box.rect.x+offX, box.rect.y+offY);
    else if (!box.focus && !box.hint.empty()) {
        SDL_Surface* sf = TTF_RenderUTF8_Blended(box.font, box.hint.c_str(), {170,170,170,170});
        if (sf) {
            SDL_Texture* tx = SDL_CreateTextureFromSurface(box.win->renderer, sf);
            if (tx) {
                SDL_Rect d = {box.rect.x+offX, box.rect.y+offY, sf->w, sf->h};
                SDL_RenderCopy(box.win->renderer, tx, nullptr, &d);
                SDL_DestroyTexture(tx);
            }
            SDL_FreeSurface(sf);
        }
    }

    SDL_RenderSetClipRect(box.win->renderer, nullptr);
}

void drawLine(Line& ln, int offX=0, int offY=0) {
    SDL_SetRenderDrawColor(ln.win->renderer, ln.color.r, ln.color.g, ln.color.b, ln.color.a);
    SDL_RenderDrawLine(ln.win->renderer, ln.cords.x+offX, ln.cords.y+offY, ln.cords.w+offX, ln.cords.h+offY);
}

void drawSlider(Slider& s, int offX=0, int offY=0) {
    roundedBoxRGBA(s.win->renderer,
        s.rect.x+offX, s.rect.y+offY,
        s.rect.x+offX+s.rect.w, s.rect.y+offY+s.rect.h,
        s.rect.h/2, s.clrOff.r, s.clrOff.g, s.clrOff.b, s.clrOff.a);
    roundedBoxRGBA(s.win->renderer,
        s.rect.x+offX, s.rect.y+offY,
        s.rect.x+offX+(int)(s.rect.w*s.progress), s.rect.y+offY+s.rect.h,
        s.rect.h/2, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);

    int cx = s.rect.x+offX+(int)(s.rect.w*s.progress);
    int cy = s.rect.y+offY+s.rect.h/2;

    filledCircleRGBA(s.win->renderer, cx, cy, (int)(s.rect.h*1.75f), s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
    aacircleRGBA    (s.win->renderer, cx, cy, (int)(s.rect.h*1.75f), s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
}

void drawRangeSlider(RangeSlider& s, int offX=0, int offY=0) {
    roundedBoxRGBA(s.win->renderer,
        s.rect.x+offX, s.rect.y+offY,
        s.rect.x+offX+s.rect.w, s.rect.y+offY+s.rect.h,
        s.rect.h/2, s.clrOff.r, s.clrOff.g, s.clrOff.b, s.clrOff.a);
    roundedBoxRGBA(s.win->renderer,
        s.rect.x+offX+(int)(s.rect.w*s.progress), s.rect.y+offY,
        s.rect.x+offX+(int)(s.rect.w*s.progress2), s.rect.y+offY+s.rect.h,
        s.rect.h/2, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);

    for (float p : {s.progress, s.progress2}) {
        int cx = s.rect.x+offX+(int)(s.rect.w*p);
        int cy = s.rect.y+offY+s.rect.h/2;
        filledCircleRGBA(s.win->renderer, cx, cy, (int)(s.rect.h*1.75f), s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
        aacircleRGBA    (s.win->renderer, cx, cy, (int)(s.rect.h*1.75f), s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
    }
}

void redrawAllOthObjs();

void UpdateCheckBoxButton(CheckBox& box, SDL_Renderer* renderer) {
    auto& u = box.uRect;

    if (box.isAnim) {
        int steps = 100;

        for (int i = 0; i < steps; i++) {
            redrawAllOthObjs();
            roundedBoxRGBA(renderer, u.x, u.y, u.x+u.size, u.y+u.size,
                u.size/6, box.clr.r, box.clr.g, box.clr.b, box.clr.a);
            int pad = (int)(u.size*0.07f);
            roundedBoxRGBA(renderer, u.x+pad, u.y+pad, u.x+u.size-pad, u.y+u.size-pad,
                u.size/6, 255,255,255,255);
            float t = box.isOn ? (float)i/steps : (float)(steps-i)/steps;
            float cs = u.size * t;
            float cx = u.x + (u.size-cs)/2.f;
            float cy = u.y + (u.size-cs)/2.f;
            roundedBoxRGBA(renderer, (int)cx, (int)cy, (int)(cx+cs), (int)(cy+cs),
                (int)(cs/6), box.clr.r, box.clr.g, box.clr.b, box.clr.a);
            SDL_RenderPresent(renderer);
            SDL_Delay(1);
        }
        box.isAnim = false;
    } else {
        roundedBoxRGBA(renderer, u.x, u.y, u.x+u.size, u.y+u.size,
            u.size/6, box.clr.r, box.clr.g, box.clr.b, box.clr.a);
        int pad = (int)(u.size*0.07f);
        roundedBoxRGBA(renderer, u.x+pad, u.y+pad, u.x+u.size-pad, u.y+u.size-pad,
            u.size/6, 255,255,255,255);
        if (box.isOn)
            roundedBoxRGBA(renderer, u.x, u.y, u.x+u.size, u.y+u.size,
                u.size/6, box.clr.r, box.clr.g, box.clr.b, box.clr.a);
    }
}

void UpdateRadioButton(RadioButton& ratio, SDL_Renderer* renderer) {
    auto& u   = ratio.uRect;
    auto& cOn = ratio.clrOn; auto& cOf = ratio.clrOff;
    bool  st  = ratio.status;

    if (ratio.animPending) {
        if (st) {
            for (auto& el : mainSpace.uiElements) {
                if (!std::holds_alternative<RadioButton>(el)) continue;

                RadioButton& r = std::get<RadioButton>(el);

                if (r.name != ratio.name && r.group == ratio.group)
                    r.status = false;
            }
            for (int i = 0; i < 100; i++) {
                redrawAllOthObjs();
                filledCircleRGBA(renderer, u.x, u.y, u.size, cOn.r,cOn.g,cOn.b,cOn.a);
                aacircleRGBA    (renderer, u.x, u.y, u.size, cOn.r,cOn.g,cOn.b,cOn.a);
                filledCircleRGBA(renderer, u.x, u.y, (int)(u.size-u.size*0.2f), 255,255,255,255);
                aacircleRGBA    (renderer, u.x, u.y, (int)(u.size-u.size*0.2f), 255,255,255,255);
                filledCircleRGBA(renderer, u.x, u.y, (int)(u.size/1.525f*i/100), cOn.r,cOn.g,cOn.b,cOn.a);
                aacircleRGBA    (renderer, u.x, u.y, (int)(u.size/1.525f*i/100), cOn.r,cOn.g,cOn.b,cOn.a);
                SDL_RenderPresent(renderer);
            }
        } else {
            RadioButton* prev = nullptr;

            if (!mainSpace.RatioOnNow.empty()) {
                for (auto& el2 : mainSpace.uiElements) {
                    if (!std::holds_alternative<RadioButton>(el2)) continue;
                    RadioButton& r2 = std::get<RadioButton>(el2);
                    if (r2.name == mainSpace.RatioOnNow) { prev = &r2; break; }
                }
            }
            for (int i = 0; i < 100; i++) {
                redrawAllOthObjs();

                if (prev) {
                    auto& pu = prev->uRect; auto& pc = prev->clrOn;
                    filledCircleRGBA(renderer, pu.x,pu.y,pu.size, prev->clrOff.r,prev->clrOff.g,prev->clrOff.b,prev->clrOff.a);
                    aacircleRGBA    (renderer, pu.x,pu.y,pu.size, prev->clrOff.r,prev->clrOff.g,prev->clrOff.b,prev->clrOff.a);
                    filledCircleRGBA(renderer, pu.x,pu.y,(int)(pu.size-pu.size*0.2f),255,255,255,255);
                    aacircleRGBA    (renderer, pu.x,pu.y,(int)(pu.size-pu.size*0.2f),255,255,255,255);
                    filledCircleRGBA(renderer, pu.x,pu.y,(int)(pu.size/1.525f*(100-i)/100),pc.r,pc.g,pc.b,pc.a);
                    aacircleRGBA    (renderer, pu.x,pu.y,(int)(pu.size/1.525f*(100-i)/100),pc.r,pc.g,pc.b,pc.a);
                }

                SDL_RenderPresent(renderer);
                SDL_Delay(1);
            }
        }

        ratio.animPending = false;
    } else {
        if (st) {
            filledCircleRGBA(renderer, u.x,u.y,u.size, cOn.r,cOn.g,cOn.b,cOn.a);
            aacircleRGBA    (renderer, u.x,u.y,u.size, cOn.r,cOn.g,cOn.b,cOn.a);
            filledCircleRGBA(renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
            aacircleRGBA    (renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
            filledCircleRGBA(renderer, u.x,u.y,(int)(u.size/1.525f),cOn.r,cOn.g,cOn.b,cOn.a);
            aacircleRGBA    (renderer, u.x,u.y,(int)(u.size/1.525f),cOn.r,cOn.g,cOn.b,cOn.a);
        } else {
            filledCircleRGBA(renderer, u.x,u.y,u.size, cOf.r,cOf.g,cOf.b,cOf.a);
            aacircleRGBA    (renderer, u.x,u.y,u.size, cOf.r,cOf.g,cOf.b,cOf.a);
            filledCircleRGBA(renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
            aacircleRGBA    (renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
        }
    }
}

void drawElement(UIElement& el, int offX=0, int offY=0) {
    std::visit([&](auto& e) {
        using T = std::decay_t<decltype(e)>;
        if      constexpr(std::is_same_v<T,Button>)      drawButton(e, offX, offY);
        else if constexpr(std::is_same_v<T,Circle>)      drawCircle(e, offX, offY);
        else if constexpr(std::is_same_v<T,Text>)        drawText(e, offX, offY);
        else if constexpr(std::is_same_v<T,Image>)       drawImage(e, offX, offY);
        else if constexpr(std::is_same_v<T,InputBox>)    drawInputBox(e, offX, offY);
        else if constexpr(std::is_same_v<T,CheckBox>)    UpdateCheckBoxButton(e, e.win->renderer);
        else if constexpr(std::is_same_v<T,RadioButton>) UpdateRadioButton(e, e.renderer);
        else if constexpr(std::is_same_v<T,Line>)        drawLine(e, offX, offY);
        else if constexpr(std::is_same_v<T,Slider>)      drawSlider(e, offX, offY);
        else if constexpr(std::is_same_v<T,RangeSlider>) drawRangeSlider(e, offX, offY);
    }, el);
}

void redrawAllOthObjs() {
    for (auto& wnd : mainSpace.wnds) {
        bool hasBg = false;
        for (auto& bg : mainSpace.backgrounds) {
            if (std::get<1>(bg)->renderer == wnd->renderer) {
                SDL_SetRenderDrawColor(wnd->renderer,
                    std::get<0>(bg).r, std::get<0>(bg).g, std::get<0>(bg).b, std::get<0>(bg).a);
                hasBg = true; break;
            }
        }
        if (!hasBg) SDL_SetRenderDrawColor(wnd->renderer, 100,100,100,255);
        SDL_RenderClear(wnd->renderer);
    }

    for (auto& el : mainSpace.uiElements) {
        bool skip = std::visit([](auto& e){
            using T = std::decay_t<decltype(e)>;
            if constexpr(std::is_same_v<T,CheckBox>)    return e.isAnim;
            if constexpr(std::is_same_v<T,RadioButton>) return e.animPending;
            if constexpr(std::is_same_v<T,ScrollBar>)   return true;
            return false;
        }, el);
        if (!skip) drawElement(el);
    }
}

void UpdateAll() {
    for (auto& el : mainSpace.uiElements) {
        if (std::holds_alternative<ScrollBar>(el)) continue;

        const std::string& ename = std::visit(GetName{}, el);

        if (isInScrollBar(ename)) continue;
        drawElement(el);
    }

    for (auto& el : mainSpace.uiElements) {
        if (!std::holds_alternative<ScrollBar>(el)) continue;
        ScrollBar& sb = std::get<ScrollBar>(el);

        SDL_Rect clip = {sb.rect.x, sb.rect.y, sb.rect.w, sb.rect.h};
        SDL_RenderSetClipRect(sb.win->renderer, &clip);

        roundedBoxRGBA(sb.win->renderer,
            sb.rect.x, sb.rect.y,
            sb.rect.x+sb.rect.w, sb.rect.y+sb.rect.h,
            sb.rad, sb.color.r, sb.color.g, sb.color.b, sb.color.a);

        float sliderH = sb.rect.h * sb.scrollingSize;
        float trackH  = sb.rect.h - sliderH;
        float thumbTop = sb.rect.y + sb.pos * trackH;

        roundedBoxRGBA(sb.win->renderer,
            sb.rect.x + (int)(sb.rect.w*0.9f), (int)thumbTop,
            sb.rect.x + sb.rect.w,             (int)(thumbTop+sliderH),
            sb.rect.w/21, 70,70,70,255);

        for (auto& childName : sb.elements) {
            for (auto& cel : mainSpace.uiElements) {
                if (std::visit(GetName{}, cel) != childName) continue;
                if (std::holds_alternative<ScrollBar>(cel)) continue;

                drawElement(cel, sb.rect.x, sb.rect.y);

                break;
            }
        }

        SDL_RenderSetClipRect(sb.win->renderer, nullptr);
    }
}

Window* createWindow(const char* title, Rect rect, int windowFlags, int rendererFlags, std::string name) {
    SDL_Window*   wnd = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, windowFlags);
    
    if (!wnd) { fprintf(stderr,"SDL_CreateWindow: %s\n",SDL_GetError()); return nullptr; }
    
    SDL_Renderer* rnd = SDL_CreateRenderer(wnd, -1, rendererFlags);
    
    if (!rnd) { fprintf(stderr,"SDL_CreateRenderer: %s\n",SDL_GetError()); SDL_DestroyWindow(wnd); return nullptr; }
    
    Window* win = new Window{wnd, rnd, name, {rect.x,rect.y,rect.w,rect.h}};
    mainSpace.wnds.push_back(win);
    mainSpace.names.push_back(name);
    return win;
}

Renderer* createRenderer(int rendererFlags, Window* wnd, std::string name) {
    SDL_Window* win = wnd->wnd;

    SDL_Renderer* rnd = SDL_CreateRenderer(win, -1, rendererFlags);
    
    if (!rnd) { 
        fprintf(stderr,"SDL_CreateRenderer: %s\n",SDL_GetError()); 
        SDL_DestroyRenderer(rnd); 

        return nullptr; 
    }
    
    Renderer* rnder;
    rnder->renderer = rnd;
    mainSpace.renderers.push_back(rnder);
    mainSpace.names.push_back(name);
    
    return rnder;
}

Button& createButton(Window* win, Rect rect, Color clr, std::string name) {
    mainSpace.uiElements.emplace_back(Button{rect, clr, name, win});
    mainSpace.names.push_back(name);
    return std::get<Button>(mainSpace.uiElements.back());
}

Circle& createCircle(Window* win, UniformRect rect, Color clr, std::string name) {
    mainSpace.uiElements.emplace_back(Circle{rect, clr, name, win});
    mainSpace.names.push_back(name);
    return std::get<Circle>(mainSpace.uiElements.back());
}

Text& createText(Window* win, std::string path, std::string text, Rect rect, Color clr, int fontSize, std::string name) {
    std::string fp = resolveFontPath(path);
    TTF_Font* font = TTF_OpenFont(fp.c_str(), fontSize);
    
    if (!font) { std::cerr<<"ERROR: Cannot find font\n"; mainSpace.running=false; static Text dummy; return dummy; }
    
    SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), {clr.r,clr.g,clr.b,clr.a}, 0);
    SDL_Texture* tex  = SDL_CreateTextureFromSurface(win->renderer, surf);
    mainSpace.uiElements.emplace_back(
        Text(rect,clr,path,fontSize,text,name,win,font,surf,tex));
    mainSpace.names.push_back(name);
    
    return std::get<Text>(mainSpace.uiElements.back());
}

Image& createImage(Window* win, std::string path, Rect rect, std::string name) {
    SDL_Texture* tex = IMG_LoadTexture(win->renderer, path.c_str());
    mainSpace.uiElements.emplace_back(Image(rect,path,tex,name,win));
    mainSpace.names.push_back(name);
    
    return std::get<Image>(mainSpace.uiElements.back());
}

InputBox& createInputBox(Window* win, std::string hint, Rect rect, std::string text, int fontSize, Color clr, std::string name) {
    std::string path = "NotoSans.ttf";
    std::string fp   = resolveFontPath(path);
    TTF_Font* font   = TTF_OpenFont(fp.c_str(), fontSize);
    
    if (!font) { std::cerr<<"ERROR: Cannot find font\n"; mainSpace.running=false; static InputBox dummy; return dummy; }
    
    InputBox box{};
    box.rect=rect; box.clr=clr; box.name=name; box.win=win;
    box.buf=text; box.hint=hint; box.text=text;
    box.pathToFont=path; box.font=font; box.fontSize=fontSize;
    mainSpace.uiElements.emplace_back(std::move(box));
    mainSpace.names.push_back(name);
    return std::get<InputBox>(mainSpace.uiElements.back());
}

CheckBox& createCheckBox(Window* win, UniformRect rect, Color clr, bool isOn, std::string name) {
    mainSpace.uiElements.emplace_back(CheckBox{rect,clr,name,isOn,false,win});
    mainSpace.names.push_back(name);
    
    return std::get<CheckBox>(mainSpace.uiElements.back());
}

RadioButton& createRadioButton(Window* win, UniformRect uRect, Color clrOff, Color clrOn, bool status, int group, std::string name) {
    if (status) {
        for (auto& el : mainSpace.uiElements) {
            if (!std::holds_alternative<RadioButton>(el)) continue;
            RadioButton& r = std::get<RadioButton>(el);
            if (r.group == group) r.status = false;
        }
    }

    mainSpace.uiElements.emplace_back(RadioButton{uRect,status,false,clrOff,clrOn,group,win->renderer,name});
    mainSpace.names.push_back(name);

    return std::get<RadioButton>(mainSpace.uiElements.back());
}

Line& drawLine(Window* win, Rect cords, Color color, std::string name) {
    mainSpace.uiElements.emplace_back(Line{cords,color,name,win});
    mainSpace.names.push_back(name);

    return std::get<Line>(mainSpace.uiElements.back());
}

Slider& createSlider(Window* win, Rect rect, Color clrOff, Color clrOn, float progress, std::string name) {
    mainSpace.uiElements.emplace_back(Slider{rect,clrOff,clrOn,progress,name,win});
    mainSpace.names.push_back(name);

    return std::get<Slider>(mainSpace.uiElements.back());
}

RangeSlider& createRangeSlider(Window* win, Rect rect, Color clrOff, Color clrOn, float progress, float progress2, std::string name) {
    mainSpace.uiElements.emplace_back(RangeSlider{rect,clrOff,clrOn,progress,progress2,name,win});
    mainSpace.names.push_back(name);

    return std::get<RangeSlider>(mainSpace.uiElements.back());
}

ScrollBar& createScrollBar(Window* win, Rect rect, Color clr, std::string name) {
    mainSpace.uiElements.emplace_back(ScrollBar{rect,clr,name,win});
    mainSpace.names.push_back(name);

    return std::get<ScrollBar>(mainSpace.uiElements.back());
}

void addScrollBarElement(Window*, std::string objName, std::string scrollBarName) {
    for (auto& el : mainSpace.uiElements) {
        if (!std::holds_alternative<ScrollBar>(el)) continue;
        ScrollBar& sb = std::get<ScrollBar>(el);
        if (sb.name == scrollBarName) { sb.elements.push_back(objName); return; }
    }

    SDL_Log("Scrollbar not found");
}

Sound createSound(const char* path, std::string name, int soundFlags) {
    Sound s{}; s.name = name;
    ma_sound_init_from_file(&mainSpace.engine, path, soundFlags, nullptr, nullptr, &s.sound);
    mainSpace.sounds.push_back(s);

    return mainSpace.sounds.back();
}

void setBackground(Color clr, Window* wnd) {
    mainSpace.backgrounds.emplace_back(clr, wnd);
}

void addRoundCorners(std::string objName, int radius) {
    for (auto& el : mainSpace.uiElements) {
        if (std::holds_alternative<Button>(el) && std::get<Button>(el).name == objName) {
            std::get<Button>(el).rad = radius; return;
        }
        if (std::holds_alternative<ScrollBar>(el) && std::get<ScrollBar>(el).name == objName) {
            std::get<ScrollBar>(el).rad = radius; return;
        }
    }

    SDL_Log("Object doesn't exist");
}

void DeleteObject(const std::string& name) {
    auto it = std::find_if(mainSpace.uiElements.begin(), mainSpace.uiElements.end(),
        [&](UIElement& el){ return std::visit(GetName{}, el) == name; });
    if (it == mainSpace.uiElements.end()) { SDL_Log("DeleteObject: '%s' not found", name.c_str()); return; }

    std::visit([](auto& e){
        using T = std::decay_t<decltype(e)>;
        if constexpr(std::is_same_v<T,Text>) {
            if (e.font) TTF_CloseFont(e.font);
            if (e.tex)  SDL_DestroyTexture(e.tex);
            if (e.surf) SDL_FreeSurface(e.surf);
        }
        if constexpr(std::is_same_v<T,Image>) {
            if (e.tex) SDL_DestroyTexture(e.tex);
        }
        if constexpr(std::is_same_v<T,InputBox>) {
            if (e.font) TTF_CloseFont(e.font);
        }
    }, *it);

    mainSpace.uiElements.erase(it);
    auto ni = std::find(mainSpace.names.begin(), mainSpace.names.end(), name);

    if (ni != mainSpace.names.end()) mainSpace.names.erase(ni);
}

std::string getObjectProperty(std::string propName, std::string objectName) {
    auto fmtR4 = [](uint16_t x,uint16_t y,uint16_t w,uint16_t h){
        return std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(w)+", "+std::to_string(h); };
    auto fmtR3 = [](int x,int y,int s){
        return std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(s); };
    auto fmtC  = [](uint8_t r,uint8_t g,uint8_t b,uint8_t a){
        return std::to_string(r)+", "+std::to_string(g)+", "+std::to_string(b)+", "+std::to_string(a); };

    for (auto& el : mainSpace.uiElements) {
        if (std::visit(GetName{}, el) != objectName) continue;

        return std::visit([&](auto& e) -> std::string {
            using T = std::decay_t<decltype(e)>;

            auto winName = [&]() -> std::string {
                if constexpr(requires{ e.win; })
                    return e.win ? e.win->name : "";
                return "";
            };

            if (propName=="Window"||propName=="window") return winName();

            if constexpr(std::is_same_v<T,Button>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
            }
            else if constexpr(std::is_same_v<T,Circle>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect")
                    return fmtR3(e.uRect.x,e.uRect.y,e.uRect.size);
                if (propName=="Color"||propName=="color") return fmtC(e.clr.r,e.clr.g,e.clr.b,e.clr.a);
            }
            else if constexpr(std::is_same_v<T,Text>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size")
                    return std::to_string(e.fontSize);
                if (propName=="Font"||propName=="font")   return e.pathToFont;
                if (propName=="Text"||propName=="text")   return e.text;
            }
            else if constexpr(std::is_same_v<T,Image>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Image"||propName=="image"||propName=="path"||propName=="Path")
                    return e.pathToImage;
            }
            else if constexpr(std::is_same_v<T,InputBox>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.clr.r,e.clr.g,e.clr.b,e.clr.a);
                if (propName=="Text"||propName=="text")   return e.buf;
                if (propName=="Placeholder"||propName=="placeholder") return e.hint;
                if (propName=="Caret"||propName=="caret") return std::to_string(e.carrot);
                if (propName=="Focus"||propName=="focus") return e.focus ? "true" : "false";
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size")
                    return std::to_string(e.fontSize);
                if (propName=="Font"||propName=="font")   return e.pathToFont;
            }
            else if constexpr(std::is_same_v<T,CheckBox>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect")
                    return fmtR3(e.uRect.x,e.uRect.y,e.uRect.size);
                if (propName=="isOn") return e.isOn ? "true" : "false";
                if (propName=="Color"||propName=="color") return fmtC(e.clr.r,e.clr.g,e.clr.b,e.clr.a);
            }
            else if constexpr(std::is_same_v<T,RadioButton>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect")
                    return fmtR3(e.uRect.x,e.uRect.y,e.uRect.size);
                if (propName=="isOn") return e.status ? "true" : "false";
                if (propName=="Group"||propName=="group") return std::to_string(e.group);
                if (propName=="ColorOff"||propName=="colorOff") return fmtC(e.clrOff.r,e.clrOff.g,e.clrOff.b,e.clrOff.a);
                if (propName=="ColorOn"||propName=="colorOn")   return fmtC(e.clrOn.r,e.clrOn.g,e.clrOn.b,e.clrOn.a);
            }
            else if constexpr(std::is_same_v<T,Line>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.cords.x,e.cords.y,e.cords.w,e.cords.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
            }
            else if constexpr(std::is_same_v<T,Slider>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Progress"||propName=="progress") return std::to_string(e.progress);
                if (propName=="ColorOn"||propName=="colorOn")   return fmtC(e.clrOn.r,e.clrOn.g,e.clrOn.b,e.clrOn.a);
                if (propName=="ColorOff"||propName=="colorOff") return fmtC(e.clrOff.r,e.clrOff.g,e.clrOff.b,e.clrOff.a);
            }
            else if constexpr(std::is_same_v<T,RangeSlider>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Progress"||propName=="progress")   return std::to_string(e.progress);
                if (propName=="Progress2"||propName=="progress2") return std::to_string(e.progress2);
                if (propName=="ColorOn"||propName=="colorOn")   return fmtC(e.clrOn.r,e.clrOn.g,e.clrOn.b,e.clrOn.a);
                if (propName=="ColorOff"||propName=="colorOff") return fmtC(e.clrOff.r,e.clrOff.g,e.clrOff.b,e.clrOff.a);
            }
            else if constexpr(std::is_same_v<T,ScrollBar>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
                if (propName=="ScrollingSize"||propName=="scrollingSize") return std::to_string(e.scrollingSize);
                if (propName=="Pos"||propName=="pos"||propName=="Position"||propName=="position")
                    return std::to_string(e.pos);
            }
            return "";
        }, el);
    }

    SDL_Log("getObjectProperty: object or property not found.");

    return "";
}

void changeObjectProperties(std::string propName, std::string newVal, std::string objectName) {
    auto parseC = [&](Color& c) {
        uint16_t r,g,b,a;
        if (parseNums4(newVal,r,g,b,a)) c={uint8_t(r),uint8_t(g),uint8_t(b),uint8_t(a)};
        else std::cerr<<"ERROR: Wrong property value.\n";
    };

    auto parseR4 = [&](Rect& r) {
        uint16_t x,y,w,h;
        if (parseNums4(newVal,x,y,w,h)) r={x,y,w,h};
        else std::cerr<<"ERROR: Wrong property value.\n";
    };

    auto parseR3 = [&](UniformRect& r) {
        uint16_t x,y,s;
        if (parseNums3(newVal,x,y,s)) r={(int)x,(int)y,(int)s};
        else std::cerr<<"ERROR: Wrong property value.\n";
    };

    auto reloadFont = [&](TTF_Font*& font, const std::string& path, int size) {
        if (font) TTF_CloseFont(font);
        font = TTF_OpenFont(resolveFontPath(path).c_str(), size);
    };

    for (auto& wnd : mainSpace.wnds) {
        if (wnd->name != objectName) continue;
        if (propName=="Name"||propName=="name") {
            for (auto& n : mainSpace.names) if (n==wnd->name) { n=newVal; break; }
            wnd->name = newVal;
        }
        return;
    }

    for (auto& el : mainSpace.uiElements) {
        if (std::visit(GetName{}, el) != objectName) continue;

        std::visit([&](auto& e){
            using T = std::decay_t<decltype(e)>;

            if (propName=="Name"||propName=="name") {
                for (auto& n : mainSpace.names) if (n==e.name) { n=newVal; break; }
                e.name = newVal;
                return;
            }

            if (propName=="Window"||propName=="window") {
                if constexpr(std::is_same_v<T,RadioButton>) {
                    for (auto& wnd : mainSpace.wnds)
                        if (wnd->name==newVal) { e.renderer=wnd->renderer; return; }
                } else {
                    for (auto& wnd : mainSpace.wnds)
                        if (wnd->name==newVal) { e.win=wnd; return; }
                }
                return;
            }

            if constexpr(std::is_same_v<T,Button>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect);  return; }
                if (propName=="Color"||propName=="color") { parseC(e.color);  return; }
            }
            else if constexpr(std::is_same_v<T,Circle>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect") { parseR3(e.uRect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.clr); return; }
            }
            else if constexpr(std::is_same_v<T,Text>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.color); return; }
                if (propName=="Text"||propName=="text") {
                    e.text = newVal;
                    if (e.surf) SDL_FreeSurface(e.surf);
                    if (e.tex)  SDL_DestroyTexture(e.tex);
                    e.surf = TTF_RenderUTF8_Blended_Wrapped(e.font, e.text.c_str(),
                        {e.color.r,e.color.g,e.color.b,e.color.a}, 0);
                    e.tex  = e.surf ? SDL_CreateTextureFromSurface(e.win->renderer, e.surf) : nullptr;
                    return;
                }
                if (propName=="Font"||propName=="font") {
                    e.pathToFont = newVal; reloadFont(e.font, e.pathToFont, e.fontSize); return;
                }
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size") {
                    e.fontSize = std::stoi(newVal); reloadFont(e.font, e.pathToFont, e.fontSize); return;
                }
            }
            else if constexpr(std::is_same_v<T,Image>) {
                if (propName=="Rect"||propName=="rect") { parseR4(e.rect); return; }
                if (propName=="Image"||propName=="image"||propName=="path"||propName=="Path") {
                    e.pathToImage = newVal;
                    if (e.tex) SDL_DestroyTexture(e.tex);
                    e.tex = IMG_LoadTexture(e.win->renderer, e.pathToImage.c_str());
                    return;
                }
            }
            else if constexpr(std::is_same_v<T,InputBox>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.clr);   return; }
                if (propName=="Text"||propName=="text")   { e.buf = newVal;  return; }
                if (propName=="Placeholder"||propName=="placeholder") { e.hint = newVal; return; }
                if (propName=="Caret"||propName=="caret") { e.carrot = std::stoi(newVal); return; }
                if (propName=="Font"||propName=="font") {
                    e.pathToFont = newVal; reloadFont(e.font, e.pathToFont, e.fontSize); return;
                }
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size") {
                    e.fontSize = std::stoi(newVal); reloadFont(e.font, e.pathToFont, e.fontSize); return;
                }
            }
            else if constexpr(std::is_same_v<T,CheckBox>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect") { parseR3(e.uRect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.clr); return; }
                if (propName=="isOn") { e.isOn = (newVal=="true"||newVal=="1"); return; }
            }
            else if constexpr(std::is_same_v<T,RadioButton>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect") { parseR3(e.uRect); return; }
                if (propName=="isOn") { e.status = (newVal=="true"||newVal=="1"); return; }
                if (propName=="Group"||propName=="group") { e.group = std::stoi(newVal); return; }
                if (propName=="ColorOff"||propName=="colorOff") { parseC(e.clrOff); return; }
                if (propName=="ColorOn"||propName=="colorOn")   { parseC(e.clrOn);  return; }
            }
            else if constexpr(std::is_same_v<T,Line>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.cords); return; }
                if (propName=="Color"||propName=="color") { parseC(e.color);  return; }
            }
            else if constexpr(std::is_same_v<T,Slider>) {
                if (propName=="Rect"||propName=="rect") { parseR4(e.rect); return; }
                if (propName=="Progress"||propName=="progress") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.progress=v; else std::cerr<<"Wrong Progress.\n";
                    return;
                }
                if (propName=="ColorOff"||propName=="colorOff") { parseC(e.clrOff); return; }
                if (propName=="ColorOn"||propName=="colorOn")   { parseC(e.clrOn);  return; }
            }
            else if constexpr(std::is_same_v<T,RangeSlider>) {
                if (propName=="Rect"||propName=="rect") { parseR4(e.rect); return; }
                if (propName=="Progress"||propName=="progress") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.progress=v; else std::cerr<<"Wrong Progress.\n";
                    return;
                }
                if (propName=="Progress2"||propName=="progress2") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.progress2=v; else std::cerr<<"Wrong Progress2.\n";
                    return;
                }
                if (propName=="ColorOff"||propName=="colorOff") { parseC(e.clrOff); return; }
                if (propName=="ColorOn"||propName=="colorOn")   { parseC(e.clrOn);  return; }
            }
            else if constexpr(std::is_same_v<T,ScrollBar>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect);  return; }
                if (propName=="Color"||propName=="color") { parseC(e.color);  return; }
                if (propName=="ScrollingSize"||propName=="scrollingSize") {
                    float v = std::stof(newVal);
                    if (v>0.f&&v<1.f) e.scrollingSize=v; else std::cerr<<"Wrong ScrollingSize.\n";
                    return;
                }
                if (propName=="Pos"||propName=="pos"||propName=="Position"||propName=="position") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.pos=v; else std::cerr<<"Wrong Pos.\n";
                    return;
                }
            }
        }, el);
        return;
    }

    SDL_Log("changeObjectProperties: object '%s' not found.", objectName.c_str());
}

void playSound      (Sound& s) { ma_sound_start(&s.sound); }
void stopSound      (Sound& s) { ma_sound_stop (&s.sound); }
void setSoundLooping(Sound& s) { ma_sound_set_looping(&s.sound, MA_TRUE); }
void setSoundVolume (Sound& s, float v) { ma_sound_set_volume(&s.sound, v); }
void setSoundPosition(Sound& s, float x, float y) { ma_sound_set_position(&s.sound, x, y, 0); }

int getDisplayWidth (int idx) { SDL_DisplayMode dm{}; SDL_GetDesktopDisplayMode(idx,&dm); return dm.w; }
int getDisplayHeight(int idx) { SDL_DisplayMode dm{}; SDL_GetDesktopDisplayMode(idx,&dm); return dm.h; }
std::pair<int,int> getDisplaySize(int idx) {
    SDL_DisplayMode dm{}; SDL_GetDesktopDisplayMode(idx,&dm);
    return {dm.w, dm.h};
}

const uint8_t* GetKeyboardState(int* n) { return SDL_GetKeyboardState(n); }
uint32_t       GetMouseState(int* x, int* y) { return SDL_GetMouseState(x,y); }

void entry() {}
void loop()  {}

int main(int argc, char** argv) {
    signal(SIGINT, [](int){ mainSpace.running = false; });

    auto init = [&]{
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
            SDL_Log("Init error: %s", SDL_GetError()); mainSpace.running=false;
        }
        if (TTF_Init() != 0) {
            SDL_Log("TTF error: %s", TTF_GetError()); mainSpace.running=false;
        }
        if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) == -1) {
            SDL_Log("Image error: %s", IMG_GetError()); mainSpace.running=false;
        }
        
        ma_engine_config cfg = ma_engine_config_init();
        cfg.noDevice = MA_TRUE;
        cfg.channels = 2;
        cfg.sampleRate = 44100;
        ma_engine_init(&cfg, &mainSpace.engine);
    };

    auto warnings = [&]{
        for (int i = 0; i < (int)mainSpace.names.size(); i++) {
            if (mainSpace.names[i].empty()) {
                std::cerr<<"ERROR: Object names cannot be empty.\n";
                mainSpace.running=false;
            }
            if (i+1 < (int)mainSpace.names.size() && mainSpace.names[i]==mainSpace.names[i+1]) {
                std::cerr<<"ERROR: Object names cannot be repeated.\n";
                mainSpace.running=false;
            }
        }

        std::set<int> groups;

        for (auto& el : mainSpace.uiElements)
            if (std::holds_alternative<RadioButton>(el))
                groups.insert(std::get<RadioButton>(el).group);

        for (int grp : groups) {
            int total=0, on=0;

            for (auto& el : mainSpace.uiElements) {
                if (!std::holds_alternative<RadioButton>(el)) continue;
                auto& r = std::get<RadioButton>(el);
                if (r.group!=grp) continue;
                total++; if (r.status) on++;
            }

            if (on==0||on==total) {
                SDL_Log("RadioButton group %d: all on or all off — fixing.", grp);

                for (auto& el : mainSpace.uiElements) {
                    if (!std::holds_alternative<RadioButton>(el)) continue;
                    auto& r = std::get<RadioButton>(el);
                    if (r.group==grp) { r.status=true; break; }
                }

                for (auto& el : mainSpace.uiElements) {
                    if (!std::holds_alternative<RadioButton>(el)) continue;
                    auto& r = std::get<RadioButton>(el);
                    if (r.group==grp && r.status) continue;
                    r.status=false;
                }
            }
        }
    };

    auto quit = [&]{
        for (auto& s : mainSpace.sounds) ma_sound_uninit(&s.sound);

        ma_engine_uninit(&mainSpace.engine);

        for (auto& el : mainSpace.uiElements) {
            std::visit([](auto& e){
                using T = std::decay_t<decltype(e)>;
                if constexpr(std::is_same_v<T,Text>) {
                    if (e.font) TTF_CloseFont(e.font);
                    if (e.tex)  SDL_DestroyTexture(e.tex);
                    if (e.surf) SDL_FreeSurface(e.surf);
                }
                if constexpr(std::is_same_v<T,Image>) {
                    if (e.tex) SDL_DestroyTexture(e.tex);
                }
                if constexpr(std::is_same_v<T,InputBox>) {
                    if (e.font) TTF_CloseFont(e.font);
                }
            }, el);
        }

        for (auto* w : mainSpace.wnds) {
            if (!w) continue;
            if (w->renderer) { SDL_DestroyRenderer(w->renderer); w->renderer=nullptr; }
            if (w->wnd)      { SDL_DestroyWindow(w->wnd);        w->wnd=nullptr; }
            delete w;
        }

        TTF_Quit(); IMG_Quit(); SDL_Quit();

        return 0;
    };

    auto eventLoop = [&]{
        SDL_Event event;
        int   draggedSlider  = -1;
        int   draggedProgress = 1;
        int   SliderType     = 0;   
        bool  isScrollBar    = false;

        while (mainSpace.running) {
            mainSpace.Event = 0;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    mainSpace.Event |= EVENT_QUIT; mainSpace.running=false;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    mainSpace.Event |= EVENT_MOUSE_BUTTON_DOWN;

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;

                        InputBox& box = std::get<InputBox>(el);
                        
                        if (event.button.x>=box.rect.x && event.button.x<=(box.rect.x+box.rect.w) &&
                            event.button.y>=box.rect.y && event.button.y<=(box.rect.y+box.rect.h)) {
                            box.focus = true;

                            if (!box.font||box.buf.empty()) { box.carrot=0; continue; }

                            int fh=0; TTF_SizeText(box.font,"A",nullptr,&fh);
                            int clickLine = (event.button.y-box.rect.y)/fh;
                            std::vector<std::string> lines; std::string cur;
                            
                            for (char c : box.buf) {
                                if (c=='\n') { lines.push_back(cur); cur.clear(); } else cur+=c;
                            }
                            
                            lines.push_back(cur);
                            clickLine = std::clamp(clickLine,0,(int)lines.size()-1);
                            int lineStart=0;
                            
                            for (int l=0;l<clickLine;l++) lineStart+=(int)lines[l].size()+1;
                            
                            const std::string& line=lines[clickLine];
                            int bestPos=0,bestDist=INT_MAX;
                            
                            for (int j=0;j<=(int)line.size();j++) {
                                int w=0; TTF_SizeText(box.font,line.substr(0,j).c_str(),&w,nullptr);
                                int d=std::abs(event.button.x-(box.rect.x+w));
                                if (d<bestDist){bestDist=d;bestPos=j;}
                            }

                            SDL_Keymod mod=SDL_GetModState();

                            if (mod&KMOD_SHIFT) {
                                if (box.selectStart==-1) box.selectStart=box.carrot;
                                box.selectEnd=lineStart+bestPos; box.carrot=box.selectEnd;
                            } else {
                                box.selectStart=-1; box.selectEnd=-1;
                                box.carrot=lineStart+bestPos;
                            }
                        } else { box.focus=false; }
                    }

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<CheckBox>(el)) continue;
                        
                        CheckBox& box=std::get<CheckBox>(el);
                        auto& u=box.uRect;
                        
                        if (event.button.x>=u.x&&event.button.x<=(u.x+u.size)&&
                            event.button.y>=u.y&&event.button.y<=(u.y+u.size)) {
                            box.isOn=!box.isOn; box.isAnim=true;
                        }
                    }

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<RadioButton>(el)) continue;
                        
                        RadioButton& r=std::get<RadioButton>(el);
                        int dx=event.button.x-r.uRect.x, dy=event.button.y-r.uRect.y;
                        
                        if (dx*dx+dy*dy <= r.uRect.size*r.uRect.size && !r.status) {
                            mainSpace.RatioOnNow = r.name;
                            r.status=true; r.animPending=true;
                            for (auto& el2 : mainSpace.uiElements) {
                                if (!std::holds_alternative<RadioButton>(el2)) continue;
                                RadioButton& r2=std::get<RadioButton>(el2);
                                if (r2.name!=r.name&&r2.group==r.group&&r2.status) {
                                    r2.status=false; r2.animPending=true;
                                }
                            }
                        }
                    }

                    isScrollBar=false; draggedSlider=-1;
                    
                    for (int i=0;i<(int)mainSpace.uiElements.size();i++) {
                        if (!std::holds_alternative<ScrollBar>(mainSpace.uiElements[i])) continue;
                        ScrollBar& sb=std::get<ScrollBar>(mainSpace.uiElements[i]);
                        int tx1=sb.rect.x+(int)(sb.rect.w*0.9f);
                        int ty1=sb.rect.y+(int)(sb.pos*(sb.rect.h-sb.scrollingSize*sb.rect.h));
                        int tx2=sb.rect.x+sb.rect.w;
                        int ty2=ty1+(int)(sb.scrollingSize*sb.rect.h);
                        if (event.button.x>=tx1&&event.button.x<=tx2&&
                            event.button.y>=ty1&&event.button.y<=ty2) {
                            draggedSlider=i; isScrollBar=true; break;
                        }
                    }

                    if (draggedSlider<0) {
                        for (int i=0;i<(int)mainSpace.uiElements.size();i++) {
                            if (!std::holds_alternative<Slider>(mainSpace.uiElements[i])) continue;
                            Slider& s=std::get<Slider>(mainSpace.uiElements[i]);
                            int dx=event.button.x-(s.rect.x+(int)(s.rect.w*s.progress));
                            int dy=event.button.y-(s.rect.y+s.rect.h/2);
                            if (dx*dx+dy*dy<=s.rect.h*s.rect.h) {
                                draggedSlider=i; SliderType=0; break;
                            }
                        }
                    }
                    
                    if (draggedSlider<0) {
                        for (int i=0;i<(int)mainSpace.uiElements.size();i++) {
                            if (!std::holds_alternative<RangeSlider>(mainSpace.uiElements[i])) continue;
                            RangeSlider& s=std::get<RangeSlider>(mainSpace.uiElements[i]);
                            int dy=event.button.y-(s.rect.y+s.rect.h/2);
                            int dx =event.button.x-(s.rect.x+(int)(s.rect.w*s.progress));
                            int dx2=event.button.x-(s.rect.x+(int)(s.rect.w*s.progress2));
                            if (dx*dx+dy*dy<=s.rect.h*s.rect.h) {
                                draggedSlider=i; draggedProgress=0; SliderType=1; break;
                            }
                            if (dx2*dx2+dy*dy<=s.rect.h*s.rect.h) {
                                draggedSlider=i; draggedProgress=1; SliderType=1; break;
                            }
                        }
                    }
                }

                if (event.type == SDL_KEYDOWN) {
                    mainSpace.Event |= EVENT_KEY_DOWN;
                    
                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        InputBox& box = std::get<InputBox>(el);
                        if (!box.focus) continue;

                        SDL_Keymod mod = SDL_GetModState();
                        const char* name = SDL_GetKeyName(event.key.keysym.sym);
                        box.changed = true;

                        bool hasSel = (box.selectStart != -1 && box.selectEnd != -1 && box.selectStart != box.selectEnd);

                        if (hasSel) {
                            bool isNav = (strcmp(name,"Left")==0 || strcmp(name,"Right")==0 ||
                                        strcmp(name,"Up")==0   || strcmp(name,"Down")==0  ||
                                        strcmp(name,"Left Shift")==0  || strcmp(name,"Right Shift")==0 ||
                                        strcmp(name,"Left Ctrl")==0   || strcmp(name,"Right Ctrl")==0);
                            
                            if (!isNav) {
                                int selMin = std::min(box.selectStart, box.selectEnd);
                                int selMax = std::max(box.selectStart, box.selectEnd);
                                box.buf.erase(selMin, selMax - selMin);
                                box.carrot = selMin;
                                box.selectStart = box.selectEnd = -1;
                                hasSel = false;
                                if (strcmp(name,"Backspace")==0 || strcmp(name,"Delete")==0) continue;
                            }
                        }

                        auto isIgnored = [&] {
                            static const char* ign[] = {
                                "Home","End","ScrollLock","Left Shift","Delete",
                                "Left Ctrl","Right Shift","Right Ctrl","Menu","Right Alt","Left Alt",
                                "Space","CapsLock","Tab","Escape","Alt","Return","Backspace",
                                "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13",
                                "Insert","Numlock","Keypad /","Keypad *","Keypad -","Keypad Enter",
                                "Keypad +","Keypad 7","Keypad 8","Keypad 9","Keypad 4","Keypad 5",
                                "Keypad 6","Keypad 1","Keypad 2","Keypad 3","Keypad 0","Keypad .",
                                "Up","Down","Left","Right", nullptr
                            };
                            for (int i = 0; ign[i]; i++) if (strcmp(name, ign[i]) == 0) return true;
                            return false;
                        };

                        if (!isIgnored()) {
                            std::string ch;
                            if (mod & KMOD_CAPS || mod & KMOD_SHIFT)
                                ch = SDL_GetKeyName(event.key.keysym.sym);
                            else
                                ch = std::string(1, (char)tolower((unsigned char)name[0]));
                            box.buf.insert(box.carrot, ch);
                            box.carrot += (int)ch.size();
                        } else if (strcmp(name,"Backspace")==0) {
                            if (box.carrot > 0 && !box.buf.empty()) {
                                box.buf.erase(box.carrot - 1, 1);
                                box.carrot--;
                            }
                        } else if (strcmp(name,"Delete")==0) {
                            if (box.carrot < (int)box.buf.size()) {
                                box.buf.erase(box.carrot, 1);
                            }
                        } else if (strcmp(name,"Left")==0) {
                            if (mod & KMOD_SHIFT) {
                                if (box.selectStart == -1) box.selectStart = box.carrot;
                                if (box.carrot > 0) box.carrot--;
                                box.selectEnd = box.carrot;
                            } else {
                                box.selectStart = box.selectEnd = -1;
                                if (box.carrot > 0) box.carrot--;
                            }
                        } else if (strcmp(name,"Right")==0) {
                            if (mod & KMOD_SHIFT) {
                                if (box.selectStart == -1) box.selectStart = box.carrot;
                                if (box.carrot < (int)box.buf.size()) box.carrot++;
                                box.selectEnd = box.carrot;
                            } else {
                                box.selectStart = box.selectEnd = -1;
                                if (box.carrot < (int)box.buf.size()) box.carrot++;
                            }
                        } else if (event.key.keysym.sym == SDLK_a && (mod & KMOD_CTRL)) {
                            box.selectStart = 0;
                            box.selectEnd = box.carrot = (int)box.buf.size();
                        } else if (strcmp(name,"Return")==0 || strcmp(name,"Keypad Enter")==0) {
                            box.buf.insert(box.carrot, 1, '\n'); box.carrot++;
                        } else if (strcmp(name,"Home")==0)       { box.carrot = 0; }
                        else if (strcmp(name,"End")==0)         { box.carrot = (int)box.buf.size(); }
                        else if (strcmp(name,"Space")==0)       { box.buf.insert(box.carrot, 1, ' '); box.carrot++; }
                        else if (strcmp(name,"Escape")==0)      { box.focus = false; }
                        else if (strcmp(name,"Keypad /")==0)    { box.buf.insert(box.carrot, 1, '/'); box.carrot++; }
                        else if (strcmp(name,"Keypad *")==0)    { box.buf.insert(box.carrot, 1, '*'); box.carrot++; }
                        else if (strcmp(name,"Keypad -")==0)    { box.buf.insert(box.carrot, 1, '-'); box.carrot++; }
                        else if (strcmp(name,"Keypad +")==0)    { box.buf.insert(box.carrot, 1, '+'); box.carrot++; }
                        else if (strcmp(name,"Keypad .")==0)    { box.buf.insert(box.carrot, 1, '.'); box.carrot++; }
                        else {
                            for (auto& p : std::initializer_list<std::pair<const char*,char>>{
                                {"Keypad 0",'0'},{"Keypad 1",'1'},{"Keypad 2",'2'},{"Keypad 3",'3'},
                                {"Keypad 4",'4'},{"Keypad 5",'5'},{"Keypad 6",'6'},{"Keypad 7",'7'},
                                {"Keypad 8",'8'},{"Keypad 9",'9'}})
                                if (strcmp(name, p.first)==0) {
                                    box.buf.insert(box.carrot, 1, p.second);
                                    box.carrot++;
                                    break;
                                }
                        }
                    }
                }

                if (event.type==SDL_WINDOWEVENT) {
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_MAXIMIZED:       mainSpace.Event|=EVENT_WINDOWEVENT_MAXIMIZED; break;
                        case SDL_WINDOWEVENT_MINIMIZED:       mainSpace.Event|=EVENT_WINDOWEVENT_MINIMIZED; break;
                        case SDL_WINDOWEVENT_ENTER:           mainSpace.Event|=EVENT_WINDOWEVENT_ENTER; break;
                        case SDL_WINDOWEVENT_EXPOSED:         mainSpace.Event|=EVENT_WINDOWEVENT_EXPOSED; break;
                        case SDL_WINDOWEVENT_DISPLAY_CHANGED: mainSpace.Event|=EVENT_WINDOWEVENT_DISPLAY_CHANGED; break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:      mainSpace.Event|=EVENT_WINDOWEVENT_FOCUS_LOST; break;
                        case SDL_WINDOWEVENT_LEAVE:           mainSpace.Event|=EVENT_WINDOWEVENT_LEAVE; break;
                        case SDL_WINDOWEVENT_SHOWN:           mainSpace.Event|=EVENT_WINDOWEVENT_SHOWN; break;
                        case SDL_WINDOWEVENT_TAKE_FOCUS:      mainSpace.Event|=EVENT_WINDOWEVENT_TAKE_FOCUS; break;
                        case SDL_WINDOWEVENT_SIZE_CHANGED:    mainSpace.Event|=EVENT_WINDOWEVENT_SIZE_CHANGED; break;
                    }
                }

                if (event.type==SDL_FINGERDOWN)          mainSpace.Event|=EVENT_FINGER_DOWN;
                if (event.type==SDL_FINGERUP)            mainSpace.Event|=EVENT_FINGER_UP;
                if (event.type==SDL_FINGERMOTION)        mainSpace.Event|=EVENT_FINGER_MOTION;
                if (event.type==SDL_CLIPBOARDUPDATE)     mainSpace.Event|=EVENT_CLIPBOARD_UPDATE;
                if (event.type==SDL_DROPFILE)            mainSpace.Event|=EVENT_DROP_FILE;
                if (event.type==SDL_DROPTEXT)            mainSpace.Event|=EVENT_DROP_TEXT;
                if (event.type==SDL_DROPBEGIN)           mainSpace.Event|=EVENT_DROP_BEGIN;
                if (event.type==SDL_DROPCOMPLETE)        mainSpace.Event|=EVENT_DROP_COMPLETE;
                if (event.type==SDL_APP_TERMINATING)     mainSpace.Event|=EVENT_TERMINATING;
                if (event.type==SDL_APP_LOWMEMORY)       mainSpace.Event|=EVENT_LOW_MEMORY;
                if (event.type==SDL_APP_WILLENTERBACKGROUND) mainSpace.Event|=EVENT_WILL_ENTER_BACKGROUND;
                if (event.type==SDL_APP_WILLENTERFOREGROUND) mainSpace.Event|=EVENT_WILL_ENTER_FOREGROUND;
                if (event.type==SDL_APP_DIDENTERBACKGROUND)  mainSpace.Event|=EVENT_DID_ENTER_BACKGROUND;
                if (event.type==SDL_APP_DIDENTERFOREGROUND)  mainSpace.Event|=EVENT_DID_ENTER_FOREGROUND;
                if (event.type==SDL_JOYAXISMOTION)       mainSpace.Event|=EVENT_JOYSTICK_AXIS_MOTION;
                if (event.type==SDL_JOYBALLMOTION)       mainSpace.Event|=EVENT_JOYSTICK_BALL_MOTION;
                if (event.type==SDL_JOYBATTERYUPDATED)   mainSpace.Event|=EVENT_JOYSTICK_BATTERY_UPDATED;
                if (event.type==SDL_JOYBUTTONDOWN)       mainSpace.Event|=EVENT_JOYSTICK_BUTTON_DOWN;
                if (event.type==SDL_JOYBUTTONUP)         mainSpace.Event|=EVENT_JOYSTICK_BUTTON_UP;
                if (event.type==SDL_JOYDEVICEADDED)      mainSpace.Event|=EVENT_JOYSTICK_DEVICE_ADDED;
                if (event.type==SDL_JOYDEVICEREMOVED)    mainSpace.Event|=EVENT_JOYSTICK_DEVICE_REMOVED;
                if (event.type==SDL_JOYHATMOTION)        mainSpace.Event|=EVENT_JOYSTICK_HAT_MOTION;

                if (event.type==SDL_MOUSEBUTTONUP) {
                    mainSpace.Event|=EVENT_MOUSE_BUTTON_UP; draggedSlider=-1;
                }

                if (event.type==SDL_MOUSEMOTION) {
                    mainSpace.Event|=EVENT_MOUSE_MOTION;
                    if (draggedSlider>=0) {
                        if (isScrollBar) {
                            ScrollBar& sb=std::get<ScrollBar>(mainSpace.uiElements[draggedSlider]);
                            float slH=sb.rect.h*sb.scrollingSize;
                            float trH=sb.rect.h-slH;
                            sb.pos=std::clamp((float)(event.motion.y-sb.rect.y)/trH, 0.f,1.f);
                        } else if (SliderType==0) {
                            Slider& s=std::get<Slider>(mainSpace.uiElements[draggedSlider]);
                            s.progress=std::clamp((float)(event.motion.x-s.rect.x)/s.rect.w, 0.f,1.f);
                        } else {
                            RangeSlider& s=std::get<RangeSlider>(mainSpace.uiElements[draggedSlider]);
                            float np=std::clamp((float)(event.motion.x-s.rect.x)/s.rect.w, 0.f,1.f);
                            if (draggedProgress==0 && np<=s.progress2-0.1f) s.progress=np;
                            if (draggedProgress==1 && np>=s.progress +0.1f) s.progress2=np;
                        }
                    }
                }
            }

            for (auto& wnd : mainSpace.wnds) {
                bool hasBg=false;
                for (auto& bg : mainSpace.backgrounds) {
                    if (std::get<1>(bg)->renderer==wnd->renderer) {
                        SDL_SetRenderDrawColor(wnd->renderer,
                            std::get<0>(bg).r,std::get<0>(bg).g,std::get<0>(bg).b,std::get<0>(bg).a);
                        hasBg=true; break;
                    }
                }
                if (!hasBg) SDL_SetRenderDrawColor(wnd->renderer,100,100,100,255);
                SDL_RenderClear(wnd->renderer);
            }

            UpdateAll();

            for (auto& wnd : mainSpace.wnds)
                SDL_RenderPresent(wnd->renderer);

            loop();
            SDL_Delay(16);
        }
    };

    init();
    entry();
    warnings();
    eventLoop();
    quit();
}