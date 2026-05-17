#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <miniaudio.h>

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
#include <deque>

/**
 * @brief Macro to mark a function that runs automatically at program startup
 */
#define Main \
    void entry()

/**
 * @brief Macro to mark a function that runs automatically at program running (main loop)
 */
#define EventLoop \
    void loop()

/**
 * @enum WindowFlags
 * @brief Flags for creating an SDL2 window
 */
enum WindowFlags {
    WINDOW_FULLSCREEN           = SDL_WINDOW_FULLSCREEN,
    WINDOW_FULLSCREEN_DESKTOP   = SDL_WINDOW_FULLSCREEN_DESKTOP,
    WINDOW_OPENGL               = SDL_WINDOW_OPENGL,
    WINDOW_SHOWN                = SDL_WINDOW_SHOWN,
    WINDOW_HIDDEN               = SDL_WINDOW_HIDDEN,
    WINDOW_BORDERLESS           = SDL_WINDOW_BORDERLESS,
    WINDOW_RESIZABLE            = SDL_WINDOW_RESIZABLE,
    WINDOW_MINIMIZED            = SDL_WINDOW_MINIMIZED,
    WINDOW_MAXIMIZED            = SDL_WINDOW_MAXIMIZED,
    WINDOW_INPUT_GRABBED        = SDL_WINDOW_INPUT_GRABBED,
    WINDOW_INPUT_FOCUS          = SDL_WINDOW_INPUT_FOCUS,
    WINDOW_MOUSE_FOCUS          = SDL_WINDOW_MOUSE_FOCUS,
    WINDOW_FOREIGN              = SDL_WINDOW_FOREIGN,
    WINDOW_ALLOW_HIGHDPI        = SDL_WINDOW_ALLOW_HIGHDPI,
    WINDOW_MOUSE_CAPTURE        = SDL_WINDOW_MOUSE_CAPTURE,
    WINDOW_ALWAYS_ON_TOP        = SDL_WINDOW_ALWAYS_ON_TOP,
    WINDOW_SKIP_TASKBAR         = SDL_WINDOW_SKIP_TASKBAR,
    WINDOW_UTILITY              = SDL_WINDOW_UTILITY,
    WINDOW_TOOLTIP              = SDL_WINDOW_TOOLTIP,
    WINDOW_POPUP_MENU           = SDL_WINDOW_POPUP_MENU,
    WINDOW_VULKAN               = SDL_WINDOW_VULKAN
};

/**
 * @enum RendererFlags
 * @brief Flags for creating an SDL2 renderer
 */
enum RendererFlags {
    RENDERER_SOFTWARE       = SDL_RENDERER_SOFTWARE,
    RENDERER_ACCELERATED    = SDL_RENDERER_ACCELERATED,
    RENDERER_PRESENTVSYNC   = SDL_RENDERER_PRESENTVSYNC,
    RENDERER_TARGETTEXTURE  = SDL_RENDERER_TARGETTEXTURE
};

/**
 * @enum SoundFlags
 * @brief Flags for creating sounds with miniaudio
 */
enum SoundFlags {
    SOUND_FLAG_DECODE                  = MA_SOUND_FLAG_DECODE,
    SOUND_FLAG_STREAM                  = MA_SOUND_FLAG_STREAM,
    SOUND_FLAG_ASYNC                   = MA_SOUND_FLAG_ASYNC,
    SOUND_FLAG_NO_DEFAULT_ATTACHMENT   = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT
};

/**
 * @enum EventTypes
 * @brief Bitmask flags for custom event handling
 */
enum EventTypes : uint64_t {
    EVENT_NONE                      = 0,
    EVENT_KEY_DOWN                  = 1ULL << 0,
    EVENT_KEY_UP                    = 1ULL << 1,
    EVENT_MOUSE_MOTION              = 1ULL << 2,
    EVENT_MOUSE_BUTTON_DOWN         = 1ULL << 3,
    EVENT_MOUSE_BUTTON_UP           = 1ULL << 4,
    EVENT_WINDOW_RESIZED            = 1ULL << 5,
    EVENT_WINDOW_MOVED              = 1ULL << 6,
    EVENT_QUIT                      = 1ULL << 7,
    EVENT_FINGER_DOWN               = 1ULL << 8,
    EVENT_FINGER_UP                 = 1ULL << 9,
    EVENT_FINGER_MOTION             = 1ULL << 10,
    EVENT_CLIPBOARD_UPDATE          = 1ULL << 11,
    EVENT_DROP_FILE                 = 1ULL << 12,
    EVENT_DROP_TEXT                 = 1ULL << 13,
    EVENT_DROP_BEGIN                = 1ULL << 14,
    EVENT_DROP_COMPLETE             = 1ULL << 15,
    EVENT_AUDIO_DEVICE_ADDED        = 1ULL << 16,
    EVENT_AUDIO_DEVICE_REMOVED      = 1ULL << 17,
    EVENT_TERMINATING               = 1ULL << 18,
    EVENT_LOW_MEMORY                = 1ULL << 19,
    EVENT_WILL_ENTER_BACKGROUND     = 1ULL << 20,
    EVENT_DID_ENTER_BACKGROUND      = 1ULL << 21,
    EVENT_WILL_ENTER_FOREGROUND     = 1ULL << 22,
    EVENT_DID_ENTER_FOREGROUND      = 1ULL << 23,
    EVENT_JOYSTICK_AXIS_MOTION      = 1ULL << 24,
    EVENT_JOYSTICK_BALL_MOTION      = 1ULL << 25,
    EVENT_JOYSTICK_HAT_MOTION       = 1ULL << 26,
    EVENT_WINDOWEVENT_MAXIMIZED     = 1ULL << 27,
    EVENT_WINDOWEVENT_MINIMIZED     = 1ULL << 28,
    EVENT_WINDOWEVENT_ENTER         = 1ULL << 29,
    EVENT_WINDOWEVENT_EXPOSED       = 1ULL << 30,
    EVENT_WINDOWEVENT_DISPLAY_CHANGED = 1ULL << 31,
    EVENT_WINDOWEVENT_FOCUS_LOST    = 1ULL << 32,
    EVENT_WINDOWEVENT_LEAVE         = 1ULL << 33,
    EVENT_WINDOWEVENT_SHOWN         = 1ULL << 34,
    EVENT_WINDOWEVENT_TAKE_FOCUS    = 1ULL << 35,
    EVENT_WINDOWEVENT_SIZE_CHANGED  = 1ULL << 36,
    EVENT_JOYSTICK_BATTERY_UPDATED  = 1ULL << 37,
    EVENT_JOYSTICK_BUTTON_DOWN      = 1ULL << 38,
    EVENT_JOYSTICK_BUTTON_UP        = 1ULL << 39,
    EVENT_JOYSTICK_DEVICE_ADDED     = 1ULL << 40,
    EVENT_JOYSTICK_DEVICE_REMOVED   = 1ULL << 41,
    EVENT_JOYSTICK_POWER_EMPTY      = 1ULL << 42,
    EVENT_JOYSTICK_POWER_LOW        = 1ULL << 43,
    EVENT_JOYSTICK_POWER_MIN        = 1ULL << 44,
    EVENT_JOYSTICK_POWER_MAX        = 1ULL << 45,
    EVENT_JOYSTICK_POWER_FULL       = 1ULL << 46,
    EVENT_JOYSTICK_POWER_MEDIUM     = 1ULL << 47,
    EVENT_JOYSTICK_POWER_UNKNOWN    = 1ULL << 48,
    EVENT_JOYSTICK_POWER_WIRED      = 1ULL << 49
};

/**
 * @enum Keys
 * @brief Bitmask flags for keyboard scancodes
 */
enum Keys {
    SCANCODE_UNKNOWN = SDL_SCANCODE_UNKNOWN,

    SCANCODE_A = SDL_SCANCODE_A,
    SCANCODE_B = SDL_SCANCODE_B,
    SCANCODE_C = SDL_SCANCODE_C,
    SCANCODE_D = SDL_SCANCODE_D,
    SCANCODE_E = SDL_SCANCODE_E,
    SCANCODE_F = SDL_SCANCODE_F,
    SCANCODE_G = SDL_SCANCODE_G,
    SCANCODE_H = SDL_SCANCODE_H,
    SCANCODE_I = SDL_SCANCODE_I,
    SCANCODE_J = SDL_SCANCODE_J,
    SCANCODE_K = SDL_SCANCODE_K,
    SCANCODE_L = SDL_SCANCODE_L,
    SCANCODE_M = SDL_SCANCODE_M,
    SCANCODE_N = SDL_SCANCODE_N,
    SCANCODE_O = SDL_SCANCODE_O,
    SCANCODE_P = SDL_SCANCODE_P,
    SCANCODE_Q = SDL_SCANCODE_Q,
    SCANCODE_R = SDL_SCANCODE_R,
    SCANCODE_S = SDL_SCANCODE_S,
    SCANCODE_T = SDL_SCANCODE_T,
    SCANCODE_U = SDL_SCANCODE_U,
    SCANCODE_V = SDL_SCANCODE_V,
    SCANCODE_W = SDL_SCANCODE_W,
    SCANCODE_X = SDL_SCANCODE_X,
    SCANCODE_Y = SDL_SCANCODE_Y,
    SCANCODE_Z = SDL_SCANCODE_Z,

    SCANCODE_1 = SDL_SCANCODE_1,
    SCANCODE_2 = SDL_SCANCODE_2,
    SCANCODE_3 = SDL_SCANCODE_3,
    SCANCODE_4 = SDL_SCANCODE_4,
    SCANCODE_5 = SDL_SCANCODE_5,
    SCANCODE_6 = SDL_SCANCODE_6,
    SCANCODE_7 = SDL_SCANCODE_7,
    SCANCODE_8 = SDL_SCANCODE_8,
    SCANCODE_9 = SDL_SCANCODE_9,
    SCANCODE_0 = SDL_SCANCODE_0,

    SCANCODE_RETURN = SDL_SCANCODE_RETURN,
    SCANCODE_ESCAPE = SDL_SCANCODE_ESCAPE,
    SCANCODE_BACKSPACE = SDL_SCANCODE_BACKSPACE,
    SCANCODE_TAB = SDL_SCANCODE_TAB,
    SCANCODE_SPACE = SDL_SCANCODE_SPACE,

    SCANCODE_MINUS = SDL_SCANCODE_MINUS,
    SCANCODE_EQUALS = SDL_SCANCODE_EQUALS,
    SCANCODE_LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET,
    SCANCODE_RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
    SCANCODE_BACKSLASH = SDL_SCANCODE_BACKSLASH,
    SCANCODE_NONUSHASH = SDL_SCANCODE_NONUSHASH,      
    SCANCODE_SEMICOLON = SDL_SCANCODE_SEMICOLON,
    SCANCODE_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
    SCANCODE_GRAVE = SDL_SCANCODE_GRAVE,       
    SCANCODE_COMMA = SDL_SCANCODE_COMMA,
    SCANCODE_PERIOD = SDL_SCANCODE_PERIOD,
    SCANCODE_SLASH = SDL_SCANCODE_SLASH,

    SCANCODE_CAPSLOCK = SDL_SCANCODE_CAPSLOCK,

    SCANCODE_F1 = SDL_SCANCODE_F1,
    SCANCODE_F2 = SDL_SCANCODE_F2,
    SCANCODE_F3 = SDL_SCANCODE_F3,
    SCANCODE_F4 = SDL_SCANCODE_F4,
    SCANCODE_F5 = SDL_SCANCODE_F5,
    SCANCODE_F6 = SDL_SCANCODE_F6,
    SCANCODE_F7 = SDL_SCANCODE_F7,
    SCANCODE_F8 = SDL_SCANCODE_F8,
    SCANCODE_F9 = SDL_SCANCODE_F9,
    SCANCODE_F10 = SDL_SCANCODE_F10,
    SCANCODE_F11 = SDL_SCANCODE_F11,
    SCANCODE_F12 = SDL_SCANCODE_F12,

    SCANCODE_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
    SCANCODE_SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK,
    SCANCODE_PAUSE = SDL_SCANCODE_PAUSE,
    SCANCODE_INSERT = SDL_SCANCODE_INSERT,
    SCANCODE_HOME = SDL_SCANCODE_HOME,
    SCANCODE_PAGEUP = SDL_SCANCODE_PAGEUP,
    SCANCODE_DELETE = SDL_SCANCODE_DELETE,
    SCANCODE_END = SDL_SCANCODE_END,
    SCANCODE_PAGEDOWN = SDL_SCANCODE_PAGEDOWN,
    SCANCODE_RIGHT = SDL_SCANCODE_RIGHT,
    SCANCODE_LEFT = SDL_SCANCODE_LEFT,
    SCANCODE_DOWN = SDL_SCANCODE_DOWN,
    SCANCODE_UP = SDL_SCANCODE_UP,

    SCANCODE_NUMLOCKCLEAR = SDL_SCANCODE_NUMLOCKCLEAR,
    SCANCODE_KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
    SCANCODE_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
    SCANCODE_KP_MINUS = SDL_SCANCODE_KP_MINUS,
    SCANCODE_KP_PLUS = SDL_SCANCODE_KP_PLUS,
    SCANCODE_KP_ENTER = SDL_SCANCODE_KP_ENTER,
    SCANCODE_KP_1 = SDL_SCANCODE_KP_1,
    SCANCODE_KP_2 = SDL_SCANCODE_KP_2,
    SCANCODE_KP_3 = SDL_SCANCODE_KP_3,
    SCANCODE_KP_4 = SDL_SCANCODE_KP_4,
    SCANCODE_KP_5 = SDL_SCANCODE_KP_5,
    SCANCODE_KP_6 = SDL_SCANCODE_KP_6,
    SCANCODE_KP_7 = SDL_SCANCODE_KP_7,
    SCANCODE_KP_8 = SDL_SCANCODE_KP_8,
    SCANCODE_KP_9 = SDL_SCANCODE_KP_9,
    SCANCODE_KP_0 = SDL_SCANCODE_KP_0,
    SCANCODE_KP_PERIOD = SDL_SCANCODE_KP_PERIOD,

    SCANCODE_NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH,
    SCANCODE_APPLICATION = SDL_SCANCODE_APPLICATION,
    SCANCODE_POWER = SDL_SCANCODE_POWER,
    SCANCODE_KP_EQUALS = SDL_SCANCODE_KP_EQUALS,

    SCANCODE_F13 = SDL_SCANCODE_F13,
    SCANCODE_F14 = SDL_SCANCODE_F14,
    SCANCODE_F15 = SDL_SCANCODE_F15,
    SCANCODE_F16 = SDL_SCANCODE_F16,
    SCANCODE_F17 = SDL_SCANCODE_F17,
    SCANCODE_F18 = SDL_SCANCODE_F18,
    SCANCODE_F19 = SDL_SCANCODE_F19,
    SCANCODE_F20 = SDL_SCANCODE_F20,
    SCANCODE_F21 = SDL_SCANCODE_F21,
    SCANCODE_F22 = SDL_SCANCODE_F22,
    SCANCODE_F23 = SDL_SCANCODE_F23,
    SCANCODE_F24 = SDL_SCANCODE_F24,

    SCANCODE_EXECUTE = SDL_SCANCODE_EXECUTE,
    SCANCODE_HELP = SDL_SCANCODE_HELP,
    SCANCODE_MENU = SDL_SCANCODE_MENU,
    SCANCODE_SELECT = SDL_SCANCODE_SELECT,
    SCANCODE_STOP = SDL_SCANCODE_STOP,
    SCANCODE_AGAIN = SDL_SCANCODE_AGAIN,
    SCANCODE_UNDO = SDL_SCANCODE_UNDO,
    SCANCODE_CUT = SDL_SCANCODE_CUT,
    SCANCODE_COPY = SDL_SCANCODE_COPY,
    SCANCODE_PASTE = SDL_SCANCODE_PASTE,
    SCANCODE_FIND = SDL_SCANCODE_FIND,
    SCANCODE_MUTE = SDL_SCANCODE_MUTE,
    SCANCODE_VOLUMEUP = SDL_SCANCODE_VOLUMEUP,
    SCANCODE_VOLUMEDOWN = SDL_SCANCODE_VOLUMEDOWN,

    SCANCODE_KP_COMMA = SDL_SCANCODE_KP_COMMA,
    SCANCODE_KP_EQUALSAS400 = SDL_SCANCODE_KP_EQUALSAS400,

    SCANCODE_INTERNATIONAL1 = SDL_SCANCODE_INTERNATIONAL1,   
    SCANCODE_INTERNATIONAL2 = SDL_SCANCODE_INTERNATIONAL2,
    SCANCODE_INTERNATIONAL3 = SDL_SCANCODE_INTERNATIONAL3,
    SCANCODE_INTERNATIONAL4 = SDL_SCANCODE_INTERNATIONAL4,
    SCANCODE_INTERNATIONAL5 = SDL_SCANCODE_INTERNATIONAL5,
    SCANCODE_INTERNATIONAL6 = SDL_SCANCODE_INTERNATIONAL6,
    SCANCODE_INTERNATIONAL7 = SDL_SCANCODE_INTERNATIONAL7,
    SCANCODE_INTERNATIONAL8 = SDL_SCANCODE_INTERNATIONAL8,
    SCANCODE_INTERNATIONAL9 = SDL_SCANCODE_INTERNATIONAL9,

    SCANCODE_LANG1 = SDL_SCANCODE_LANG1,        
    SCANCODE_LANG2 = SDL_SCANCODE_LANG2,
    SCANCODE_LANG3 = SDL_SCANCODE_LANG3,
    SCANCODE_LANG4 = SDL_SCANCODE_LANG4,
    SCANCODE_LANG5 = SDL_SCANCODE_LANG5,
    SCANCODE_LANG6 = SDL_SCANCODE_LANG6,
    SCANCODE_LANG7 = SDL_SCANCODE_LANG7,
    SCANCODE_LANG8 = SDL_SCANCODE_LANG8,
    SCANCODE_LANG9 = SDL_SCANCODE_LANG9,

    SCANCODE_ALTERASE = SDL_SCANCODE_ALTERASE,        
    SCANCODE_SYSREQ = SDL_SCANCODE_SYSREQ,
    SCANCODE_CANCEL = SDL_SCANCODE_CANCEL,
    SCANCODE_CLEAR = SDL_SCANCODE_CLEAR,
    SCANCODE_PRIOR = SDL_SCANCODE_PRIOR,
    SCANCODE_RETURN2 = SDL_SCANCODE_RETURN2,         
    SCANCODE_SEPARATOR = SDL_SCANCODE_SEPARATOR,
    SCANCODE_OUT = SDL_SCANCODE_OUT,
    SCANCODE_OPER = SDL_SCANCODE_OPER,
    SCANCODE_CLEARAGAIN = SDL_SCANCODE_CLEARAGAIN,
    SCANCODE_CRSEL = SDL_SCANCODE_CRSEL,
    SCANCODE_EXSEL = SDL_SCANCODE_EXSEL,

    SCANCODE_KP_00 = SDL_SCANCODE_KP_00,
    SCANCODE_KP_000 = SDL_SCANCODE_KP_000,
    SCANCODE_THOUSANDSSEPARATOR = SDL_SCANCODE_THOUSANDSSEPARATOR,
    SCANCODE_DECIMALSEPARATOR = SDL_SCANCODE_DECIMALSEPARATOR,
    SCANCODE_CURRENCYUNIT = SDL_SCANCODE_CURRENCYUNIT,
    SCANCODE_CURRENCYSUBUNIT = SDL_SCANCODE_CURRENCYSUBUNIT,
    SCANCODE_KP_LEFTPAREN = SDL_SCANCODE_KP_LEFTPAREN,
    SCANCODE_KP_RIGHTPAREN = SDL_SCANCODE_KP_RIGHTPAREN,
    SCANCODE_KP_LEFTBRACE = SDL_SCANCODE_KP_LEFTBRACE,
    SCANCODE_KP_RIGHTBRACE = SDL_SCANCODE_KP_RIGHTBRACE,
    SCANCODE_KP_TAB = SDL_SCANCODE_KP_TAB,
    SCANCODE_KP_BACKSPACE = SDL_SCANCODE_KP_BACKSPACE,
    SCANCODE_KP_A = SDL_SCANCODE_KP_A,
    SCANCODE_KP_B = SDL_SCANCODE_KP_B,
    SCANCODE_KP_C = SDL_SCANCODE_KP_C,
    SCANCODE_KP_D = SDL_SCANCODE_KP_D,
    SCANCODE_KP_E = SDL_SCANCODE_KP_E,
    SCANCODE_KP_F = SDL_SCANCODE_KP_F,
    SCANCODE_KP_XOR = SDL_SCANCODE_KP_XOR,
    SCANCODE_KP_POWER = SDL_SCANCODE_KP_POWER,
    SCANCODE_KP_PERCENT = SDL_SCANCODE_KP_PERCENT,
    SCANCODE_KP_LESS = SDL_SCANCODE_KP_LESS,
    SCANCODE_KP_GREATER = SDL_SCANCODE_KP_GREATER,
    SCANCODE_KP_AMPERSAND = SDL_SCANCODE_KP_AMPERSAND,
    SCANCODE_KP_DBLAMPERSAND = SDL_SCANCODE_KP_DBLAMPERSAND,
    SCANCODE_KP_VERTICALBAR = SDL_SCANCODE_KP_VERTICALBAR,
    SCANCODE_KP_DBLVERTICALBAR = SDL_SCANCODE_KP_DBLVERTICALBAR,
    SCANCODE_KP_COLON = SDL_SCANCODE_KP_COLON,
    SCANCODE_KP_HASH = SDL_SCANCODE_KP_HASH,
    SCANCODE_KP_SPACE = SDL_SCANCODE_KP_SPACE,
    SCANCODE_KP_AT = SDL_SCANCODE_KP_AT,
    SCANCODE_KP_EXCLAM = SDL_SCANCODE_KP_EXCLAM,
    SCANCODE_KP_MEMSTORE = SDL_SCANCODE_KP_MEMSTORE,
    SCANCODE_KP_MEMRECALL = SDL_SCANCODE_KP_MEMRECALL,
    SCANCODE_KP_MEMCLEAR = SDL_SCANCODE_KP_MEMCLEAR,
    SCANCODE_KP_MEMADD = SDL_SCANCODE_KP_MEMADD,
    SCANCODE_KP_MEMSUBTRACT = SDL_SCANCODE_KP_MEMSUBTRACT,
    SCANCODE_KP_MEMMULTIPLY = SDL_SCANCODE_KP_MEMMULTIPLY,
    SCANCODE_KP_MEMDIVIDE = SDL_SCANCODE_KP_MEMDIVIDE,
    SCANCODE_KP_PLUSMINUS = SDL_SCANCODE_KP_PLUSMINUS,
    SCANCODE_KP_CLEAR = SDL_SCANCODE_KP_CLEAR,
    SCANCODE_KP_CLEARENTRY = SDL_SCANCODE_KP_CLEARENTRY,
    SCANCODE_KP_BINARY = SDL_SCANCODE_KP_BINARY,
    SCANCODE_KP_OCTAL = SDL_SCANCODE_KP_OCTAL,
    SCANCODE_KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
    SCANCODE_KP_HEXADECIMAL = SDL_SCANCODE_KP_HEXADECIMAL,
};

/**
 * @namespace TweenType
 * @brief Contains various easing functions for animations (tweening)
*/
namespace TweenType {
    /**
     * @brief Linear interpolation
     */
    float Linear(float t) { return t; }

    /**
     * @brief Quadratic ease-in
     */
    float EaseIn(float t) { return t * t; }

    /**
     * @brief Quadratic ease-out
     */
    float EaseOut(float t) { return t * (2 - t); }

    /**
     * @brief Quadratic ease-in-out
     */
    float EaseInOut(float t) {
        return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
    }

    float Cubic(float t) { return t * t * t; }

    /**
     * @brief Elastic easing function
     */
    float Elastic(float t) {
        float p = 0.3f;
        return pow(2, -10 * t) * sin((t - p / 4) * (2 * M_PI) / p) + 1;
    }

    /**
     * @brief Bounce easing function
     */
    float Bounce(float t) {
        if (t < 1 / 2.75f) return 7.5625f * t * t;
        if (t < 2 / 2.75f) { t -= 1.5f / 2.75f; return 7.5625f * t * t + .75f; }
        if (t < 2.5f / 2.75f) { t -= 2.25f / 2.75f; return 7.5625f * t * t + .9375f; }
        t -= 2.625f / 2.75f; return 7.5625f * t * t + .984375f;
    }

    float Back(float t) {
        float s = 1.70158f;
        return t * t * ((s + 1) * t - s);
    }

    const float PI = 3.14159265358979323846f;

    float SineIn(float t)     { return 1 - cos((t * PI) / 2); }
    float SineOut(float t)    { return sin((t * PI) / 2); }
    float SineInOut(float t)  { return -(cos(PI * t) - 1) / 2; }

    float QuadIn(float t)     { return t * t; }
    float QuadOut(float t)    { return t * (2 - t); }
    float QuadInOut(float t)  { return t < 0.5f ? 2 * t * t : 1 - pow(-2 * t + 2, 2) / 2; }

    float QuartIn(float t)    { return t * t * t * t; }
    float QuartOut(float t)   { return 1 - pow(1 - t, 4); }
    float QuartInOut(float t) { return t < 0.5f ? 8 * t * t * t * t : 1 - pow(-2 * t + 2, 4) / 2; }

    float QuintIn(float t)    { return t * t * t * t * t; }
    float QuintOut(float t)   { return 1 - pow(1 - t, 5); }
    float QuintInOut(float t) { return t < 0.5f ? 16 * t * t * t * t * t : 1 - pow(-2 * t + 2, 5) / 2; }

    float ExpoIn(float t)     { return t == 0 ? 0 : pow(2, 10 * t - 10); }
    float ExpoOut(float t)    { return t == 1 ? 1 : 1 - pow(2, -10 * t); }

    float ExpoInOut(float t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return t < 0.5f ? pow(2, 20 * t - 10) / 2 : (2 - pow(2, -20 * t + 10)) / 2;
    }

    float CircIn(float t)     { return 1 - sqrt(1 - pow(t, 2)); }
    float CircOut(float t)    { return sqrt(1 - pow(t - 1, 2)); }
    float CircInOut(float t) {
        return t < 0.5f ? (1 - sqrt(1 - pow(2 * t, 2))) / 2 : (sqrt(1 - pow(-2 * t + 2, 2)) + 1) / 2;
    }

    float ElasticIn(float t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return -pow(2, 10 * t - 10) * sin((t * 10 - 10.75f) * ((2 * PI) / 3));
    }

    float ElasticOut(float t) {
        if (t == 0) return 0;
        if (t == 1) return 1;
        return pow(2, -10 * t) * sin((t * 10 - 0.75f) * ((2 * PI) / 3)) + 1;
    }
};

/**
 * @class Tween
 * @brief Represents a tween (interpolation) animation that smoothly changes a value over time
 */
class Tween {
private:
    float elapsed = 0.0f;
    bool done = false;

public:
    float* target;                          ///< Pointer to the variable to animate
    float from;                             ///< Starting value
    float to;                               ///< Target (end) value
    float duration;                         ///< Duration of the animation in seconds
    std::function<float(float)> anim;       ///< Easing function from TweenType namespace

    /**
     * @brief Updates the tween animation
     * @param dt Delta time since last update (in seconds)
     */
    void update(float dt) {
        elapsed += dt;
        float t = std::min(elapsed / duration, 1.0f);
        *target = from + (to - from) * anim(t);
        if (t >= 1.0f) done = true;
    }
};

/**
 * @struct Rect
 * @brief Represents a rectangle with origin at the top-left corner
 */
struct Rect {
    uint16_t x, y, w, h;    ///< Position (x, y) and size (width, height)
};

/**
 * @struct Color
 * @brief RGBA color representation
 */
struct Color {
    uint8_t r, g, b, a;     ///< Red, Green, Blue, Alpha components (0-255)
};

/**
 * @struct Window
 * @brief Holds SDL window and renderer along with basic properties
 */
struct Window {
    SDL_Window* wnd;        ///< SDL window handle
    SDL_Renderer* renderer; ///< SDL renderer handle
    std::string name;       ///< Unique name of the window
    SDL_Rect rect;          ///< Current window rectangle
};

struct Renderer {
    SDL_Renderer* renderer;
};

/**
 * @struct Button
 * @brief Represents a clickable button UI element
 */
struct Button {
    Rect rect;              ///< Button position and size
    Color color;            ///< Button color
    std::string name;       ///< Unique name of the button
    Window* win;             ///< Parent window
    int rad = 0;            ///< Corner radius (for rounded buttons)
};

/**
 * @struct Text
 * @brief Represents a text label rendered with SDL_ttf
 */
struct Text {
    Rect rect;                            ///< Position and size of the text area
    Color color;                          ///< Text color
    std::string pathToFont;               ///< Path to the font file
    int fontSize;                         ///< Font size in points
    std::string text;                     ///< Text content
    TTF_Font* font = nullptr;             ///< Loaded font
    std::string name;                     ///< Unique name of the text object
    Window* win;                          ///< Parent window
    SDL_Surface* surf = nullptr;          ///< SDL surface (internal)
    SDL_Texture* tex = nullptr;           ///< SDL texture for rendering

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

/**
 * @struct Image
 * @brief Represents an image rendered with SDL_image
 */
struct Image {
    Rect rect;                  ///< Position and size
    std::string pathToImage;    ///< Path to the image file
    SDL_Texture* tex;           ///< Loaded texture
    std::string name;           ///< Unique name of the image
    Window* win;                 ///< Parent window

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

/**
 * @struct UniformRect
 * @brief Rectangle without separate width and height (used for squares/circles)
 */
struct UniformRect {
    int x, y;       ///< Position
    int size;       ///< Width and height are the same
};

/**
 * @struct Circle
 * @brief Represents a circle UI element
 */
struct Circle {
    UniformRect uRect;  ///< Position and size
    Color clr;          ///< Circle color
    std::string name;   ///< Unique name
    Window* win;         ///< Parent window
};

/**
 * @struct InputBox
 * @brief Text input field with cursor and selection support
 */
struct InputBox {
    Rect rect;                  ///< Position and size
    Color clr;                  ///< Background color
    std::string name;           ///< Unique name
    std::string hint;           ///< Placeholder text
    std::string text;           ///< Current text
    Window* win;                 ///< Parent window
    int carrot = 0;             ///< Cursor position (typo: should be "caret")
    bool focus = false;         ///< Whether the input box is focused
    TTF_Font* font = nullptr;             ///< Font used
    int fontSize = 0;               ///< Font size
    std::string buf;            ///< Internal text buffer
    std::string pathToFont;     ///< Path to font file
    std::vector<int> textWidth; ///< Cached text widths (for cursor positioning)
    bool changed = false;       ///< Flag indicating text has changed
    int animTime = 0;           ///< Animation timer for cursor blink
    bool animBack = false;              ///< Animation direction
    int selectStart = -1;       ///< Selection start index
    int selectEnd = -1;         ///< Selection end index

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

/**
 * @struct ScrollBar
 * @brief Scrollbar UI component
 */
struct ScrollBar {
    Rect rect;                          ///< Position and size
    Color color;                        ///< Scrollbar color
    std::string name;                   ///< Unique name
    Window* win;                         ///< Parent window
    float scrollingSize = 0.8f;         ///< Visible portion ratio
    float pos = 0.0f;                   ///< Current scroll position (0.0 - 1.0)
    std::vector<std::string> elements;  ///< List of elements (for future use)
    int rad = 0;                        ///< Corner radius
};

/**
 * @struct Slider
 * @brief Simple progress slider
 */
struct Slider {
    Rect rect;              ///< Position and size
    Color clrOff;           ///< Background (off) color
    Color clrOn;            ///< Foreground (on) color
    float progress;         ///< Current progress (0.0 - 1.0)
    std::string name;       ///< Unique name
    Window* win;             ///< Parent window
};

/**
 * @struct RangeSlider
 * @brief Slider with two handles (range selection)
 */
struct RangeSlider {
    Rect rect;              ///< Position and size
    Color clrOff;           ///< Background color
    Color clrOn;            ///< Active range color
    float progress;         ///< First handle position (0.0 - 1.0)
    float progress2;        ///< Second handle position (0.0 - 1.0)
    std::string name;       ///< Unique name
    Window* win;             ///< Parent window
};

/**
 * @struct CheckBox
 * @brief Checkbox UI element with animation support
 */
struct CheckBox {
    UniformRect uRect;      ///< Position and size
    Color clr;              ///< Color
    std::string name;       ///< Unique name
    bool isOn;              ///< Checked state
    bool isAnim;            ///< Animation in progress
    Window* win;             ///< Parent window
};

struct RadioButton {
    UniformRect uRect;
    bool status = false, animPending = false;
    Color clrOff, clrOn;
    int   group;
    SDL_Renderer* renderer = nullptr;
    std::string name;
};

/**
 * @struct Line
 * @brief Straight line for drawing
 */
struct Line {
    Rect cords;             ///< Start and end coordinates (x,y,w,h → x1,y1,x2,y2)
    Color color;            ///< Line color
    std::string name;       ///< Unique name
    Window* win;             ///< Parent window
};

/**
 * @struct Sound
 * @brief Audio object managed with miniaudio library
 */
struct Sound {
    ma_sound sound;
    std::string name;
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

/**
 * @struct Space
 * @brief Main container holding all UI objects and resources
 */
struct Space {
    std::deque<UIElement>                        uiElements;
    std::vector<Window*>                         wnds;
    std::vector<Renderer*>                       renderers;
    std::vector<Sound>                           sounds;
    std::vector<std::tuple<Color, Window*>>      backgrounds;
    std::vector<std::string>                     names;

    RadioButton*                                 RatioOnNow = nullptr;
    bool                                         running    = true;
    ma_engine                                    engine;
    uint64_t                                     Event      = 0;
};

extern Space mainSpace;

// ==================== Function Declarations with Doxygen ====================

/**
 * @brief Creates a new button and adds it to the space
 * @param Win Parent window
 * @param rect Position and size of the button
 * @param clr Button color
 * @param name Unique name of the button
 */
Button& createButton(Window* Win, Rect rect, Color clr, std::string name);

/**
 * @brief Creates a new checkbox
 */
CheckBox& createCheckBox(Window* Win, UniformRect rect, Color clr, bool isOn, std::string name);

/**
 * @brief Creates a new circle
 */
Circle& createCircle(Window* Win, UniformRect rect, Color clr, std::string name);

/**
 * @brief Creates a radio button (part of a group)
 */
RadioButton& createRadioButton(Window* win, UniformRect uRect, Color clrOff, Color clrOn, bool status, int group, std::string name);

/**
 * @brief Creates a text element
 * @param Win Parent window
 * @param path Path to the font file
 * @param text Text content
 * @param rect Position and size
 * @param clr Text color
 * @param fontSize Font size in points
 * @param name Unique name
 */
Text& createText(Window* Win, std::string path, std::string text, Rect rect, Color clr, int fontSize, std::string name);

/**
 * @brief Creates an image element
 */
Image& createImage(Window* Win, std::string path, Rect rect, std::string name);

/**
 * @brief Sets the background color of a window
 */
void setBackground(Color clr, Window* wnd);

/**
 * @brief Deletes an object by its unique name
 */
void DeleteObject(std::string name);

/**
 * @brief Draws a line
 */
Line& drawLine(Window* Win, Rect cords, Color color, std::string name);

/**
 * @brief Creates a text input box
 */
InputBox& createInputBox(Window* Win, std::string hint, Rect rect, std::string text, int fontSize, Color clr, std::string name);

/**
 * @brief Creates a scrollbar
 */
ScrollBar& createScrollBar(Window* Win, Rect rect, Color clr, std::string name);

/**
 * @brief Adds an element to a scrollbar
 */
void addScrollBarElement(Window* Win, std::string objName, std::string scrollBarName);

/**
 * @brief Returns the width of a display
 */
int getDisplayWidth(int displayIndex);

/**
 * @brief Returns the size of a display as a pair (width, height)
 */
std::pair<int, int> getDisplaySize(int displayIndex);

/**
 * @brief Returns the height of a display
 */
int getDisplayHeight(int displayIndex);

/**
 * @brief Creates a slider
 */
Slider& createSlider(Window* Win, Rect rect, Color clrOff, Color clrOn, float progress, std::string name);

/**
 * @brief Creates a range slider (two handles)
 */
RangeSlider& createRangeSlider(Window* Win, Rect rect, Color clrOff, Color clrOn, float progress, float progress2, std::string name);

/**
 * @brief Adds rounded corners to an object (button, etc.)
 * @param objName Name of the object
 * @param radius Corner radius in pixels
 */
void addRoundCorners(std::string objName, int radius);

/**
 * @brief Gets a property value from an object as string
 */
std::string getObjectProperty(std::string propName, std::string objectName);

/**
 * @brief Changes a property of an object
 */
void changeObjectProperties(std::string propName, std::string newVal, std::string objectName);

/**
 * @brief Creates a sound object
 * @param pathToSound Path to the audio file
 * @param name Unique name of the sound
 * @param soundFlags Miniaudio sound flags
 * @return Sound object
 */
Sound createSound(const char* pathToSound, std::string name, int soundFlags);

/**
 * @brief Plays a sound
 */
void playSound(Sound sound);

/**
 * @brief Stops a sound
 */
void stopSound(Sound sound);

/**
 * @brief Enables looping for a sound
 */
void setSoundLooping(Sound sound);

/**
 * @brief Sets 2D position of a sound (for spatial audio)
 */
void setSoundPosition(Sound sound, int x, int y);
void setSoundPosition(Sound sound, float x, float y);

/**
 * @brief Sets volume of a sound (0.0 - 1.0)
 */
void setSoundVolume(Sound sound, float volume);

/**
 * @brief Creates a new window with renderer
 * @param Title Window title
 * @param rect Initial position and size
 * @param windowFlags SDL window flags
 * @param rendererFlags SDL renderer flags
 * @param name Unique name for the window
 * @return Created Window struct
 */
Window* createWindow(const char* Title, Rect rect, int windowFlags, int rendererFlags, std::string name);

Renderer* createRenderer(int rendererFlags, Window* wnd, std::string name);

/**
 * @brief Gets the current state of the keyboard
 * @param numKeys Pointer to store the number of keys in the returned array
 * @return Array of key states (1 for pressed, 0 for released)
*/
const uint8_t* GetKeyboardState(int *numKeys) {
    return SDL_GetKeyboardState(numKeys);
}

/**
 * @brief Gets the current state of the mouse
 * @param x Pointer to store the x-coordinate of the mouse position
 * @param y Pointer to store the y-coordinate of the mouse position
 * @return Bitmask of currently pressed mouse buttons
*/
uint32_t GetMouseState(int *x, int *y) {
    return SDL_GetMouseState(x, y);
}