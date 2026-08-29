/*

Copyright (c) 2026 MEGA Unit

This software is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#pragma once

#include "incl/vulkan/vulkan.h"

#if defined(__linux__)
    #define VK_USE_PLATFORM_WAYLAND_KHR
    #include "incl/vulkan/vulkan_wayland.h"
#elif defined(_WIN32)
    #define VK_USE_PLATFORM_WIN32_KHR
    #define VK_USE_PLATFORM_WIN32_GDI_NV
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include "incl/vulkan/vulkan_win32.h"
#endif

#define BUTTON(X)       (1 << ((X)-1))
#define BUTTON_LEFT     1
#define BUTTON_MIDDLE   2
#define BUTTON_RIGHT    3
#define BUTTON_X1       4
#define BUTTON_X2       5
#define BUTTON_LMASK    BUTTON(BUTTON_LEFT)
#define BUTTON_MMASK    BUTTON(BUTTON_MIDDLE)
#define BUTTON_RMASK    BUTTON(BUTTON_RIGHT)
#define BUTTON_X1MASK   BUTTON(BUTTON_X1)
#define BUTTON_X2MASK   BUTTON(BUTTON_X2)

#include "incl/SDL2/SDL.h"
#include "incl/SDL2/SDL_ttf.h"
#include "incl/SDL2/SDL_image.h"
#include "incl/SDL2/SDL2_gfxPrimitives.h"
#include "incl/shaderc/shaderc.hpp"
#include "incl/SDL2/SDL_vulkan.h"
#include "incl/SDL2/SDL_opengl.h"
#include "incl/glm/glm.hpp"
#include "incl/glm/gtc/matrix_transform.hpp"
#include "incl/miniaudio.h"
#include "incl/stb/stb_truetype.h"
#include "incl/stb/stb_image.h"

extern "C" {
    #include "incl/ffmpeg/libavformat/avformat.h"
    #include "incl/ffmpeg/libavcodec/avcodec.h"
    #include "incl/ffmpeg/libswscale/swscale.h"
    #include "incl/ffmpeg/libavutil/imgutils.h"
    #include "incl/ffmpeg/libavutil/time.h"
    #include "incl/ffmpeg/libswresample/swresample.h"
}

#include <fstream>
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
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <stdarg.h>

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

using namespace glm;

using Keymod         = SDL_Keymod;
using Cursor         = SDL_Cursor;
using TimerCallback  = SDL_TimerCallback;
using TimerID        = SDL_TimerID;

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
    RENDERER_VULKAN            = 0x00000010,
    RENDERER_OPTIMAL           = 0x00000012,
    RENDERER_SDL_SOFTWARE      = SDL_RENDERER_SOFTWARE,
    RENDERER_SDL_ACCELERATED   = SDL_RENDERER_ACCELERATED,
    RENDERER_SDL_PRESENTVSYNC  = SDL_RENDERER_PRESENTVSYNC,
    RENDERER_SDL_TARGETTEXTURE = SDL_RENDERER_TARGETTEXTURE
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
    EVENT_NONE                        = 0x0000000000000000ULL,
    EVENT_KEY_DOWN                    = 0x0000000000000001ULL, 
    EVENT_KEY_UP                      = 0x0000000000000002ULL, 
    EVENT_MOUSE_MOTION                = 0x0000000000000004ULL, 
    EVENT_MOUSE_BUTTON_DOWN           = 0x0000000000000008ULL, 
    EVENT_MOUSE_BUTTON_UP             = 0x0000000000000010ULL, 
    EVENT_WINDOW_RESIZED              = 0x0000000000000020ULL,
    EVENT_WINDOW_MOVED                = 0x0000000000000040ULL, 
    EVENT_QUIT                        = 0x0000000000000080ULL, 
    EVENT_FINGER_DOWN                 = 0x0000000000000100ULL, 
    EVENT_FINGER_UP                   = 0x00000000000000200ULL,
    EVENT_FINGER_MOTION               = 0x0000000000000400ULL, 
    EVENT_CLIPBOARD_UPDATE            = 0x0000000000000800ULL, 
    EVENT_DROP_FILE                   = 0x0000000000001000ULL, 
    EVENT_DROP_TEXT                   = 0x0000000000002000ULL, 
    EVENT_DROP_BEGIN                  = 0x0000000000004000ULL,
    EVENT_DROP_COMPLETE               = 0x0000000000008000ULL, 
    EVENT_AUDIO_DEVICE_ADDED          = 0x0000000000010000ULL,
    EVENT_AUDIO_DEVICE_REMOVED        = 0x0000000000020000ULL,
    EVENT_TERMINATING                 = 0x0000000000040000ULL, 
    EVENT_LOW_MEMORY                  = 0x0000000000080000ULL,
    EVENT_WILL_ENTER_BACKGROUND       = 0x0000000000100000ULL,
    EVENT_DID_ENTER_BACKGROUND        = 0x0000000000200000ULL,
    EVENT_WILL_ENTER_FOREGROUND       = 0x0000000000400000ULL,
    EVENT_DID_ENTER_FOREGROUND        = 0x0000000000800000ULL,
    EVENT_JOYSTICK_AXIS_MOTION        = 0x0000000001000000ULL,
    EVENT_JOYSTICK_BALL_MOTION        = 0x0000000002000000ULL, 
    EVENT_JOYSTICK_HAT_MOTION         = 0x0000000004000000ULL, 
    EVENT_WINDOWEVENT_MAXIMIZED       = 0x0000000008000000ULL, 
    EVENT_WINDOWEVENT_MINIMIZED       = 0x0000000010000000ULL, 
    EVENT_WINDOWEVENT_ENTER           = 0x0000000020000000ULL, 
    EVENT_WINDOWEVENT_EXPOSED         = 0x0000000040000000ULL, 
    EVENT_WINDOWEVENT_DISPLAY_CHANGED = 0x0000000080000000ULL, 
    EVENT_WINDOWEVENT_FOCUS_LOST      = 0x0000000100000000ULL, 
    EVENT_WINDOWEVENT_LEAVE           = 0x0000000200000000ULL, 
    EVENT_WINDOWEVENT_SHOWN           = 0x0000000400000000ULL, 
    EVENT_WINDOWEVENT_TAKE_FOCUS      = 0x0000000800000000ULL, 
    EVENT_WINDOWEVENT_SIZE_CHANGED    = 0x0000001000000000ULL, 
    EVENT_JOYSTICK_BATTERY_UPDATED    = 0x0000002000000000ULL, 
    EVENT_JOYSTICK_BUTTON_DOWN        = 0x0000004000000000ULL, 
    EVENT_JOYSTICK_BUTTON_UP          = 0x0000008000000000ULL, 
    EVENT_JOYSTICK_DEVICE_ADDED       = 0x0000010000000000ULL, 
    EVENT_JOYSTICK_DEVICE_REMOVED     = 0x0000020000000000ULL, 
    EVENT_JOYSTICK_POWER_EMPTY        = 0x0000040000000000ULL, 
    EVENT_JOYSTICK_POWER_LOW          = 0x0000080000000000ULL, 
    EVENT_JOYSTICK_POWER_MIN          = 0x0000100000000000ULL,
    EVENT_JOYSTICK_POWER_MAX          = 0x0000200000000000ULL, 
    EVENT_JOYSTICK_POWER_FULL         = 0x0000400000000000ULL, 
    EVENT_JOYSTICK_POWER_MEDIUM       = 0x0000800000000000ULL, 
    EVENT_JOYSTICK_POWER_UNKNOWN      = 0x0001000000000000ULL,
    EVENT_JOYSTICK_POWER_WIRED        = 0x0002000000000000ULL,
};

/**
 * @enum Keycode
 * @brief Bitmask flags for keyboard scancodes
 */
enum Keycode {
    KEYCODE_UNKNOWN = 0,
    KEYCODE_RETURN = '\r',
    KEYCODE_ESCAPE = '\x1B',
    KEYCODE_BACKSPACE = '\b',
    KEYCODE_TAB = '\t',
    KEYCODE_SPACE = ' ',
    KEYCODE_EXCLAIM = '!',
    KEYCODE_QUOTEDBL = '"',
    KEYCODE_HASH = '#',
    KEYCODE_PERCENT = '%',
    KEYCODE_DOLLAR = '$',
    KEYCODE_AMPERSAND = '&',
    KEYCODE_QUOTE = '\'',
    KEYCODE_LEFTPAREN = '(',
    KEYCODE_RIGHTPAREN = ')',
    KEYCODE_ASTERISK = '*',
    KEYCODE_PLUS = '+',
    KEYCODE_COMMA = ',',
    KEYCODE_MINUS = '-',
    KEYCODE_PERIOD = '.',
    KEYCODE_SLASH = '/',
    KEYCODE_0 = '0',
    KEYCODE_1 = '1',
    KEYCODE_2 = '2',
    KEYCODE_3 = '3',
    KEYCODE_4 = '4',
    KEYCODE_5 = '5',
    KEYCODE_6 = '6',
    KEYCODE_7 = '7',
    KEYCODE_8 = '8',
    KEYCODE_9 = '9',
    KEYCODE_COLON = ':',
    KEYCODE_SEMICOLON = ';',
    KEYCODE_LESS = '<',
    KEYCODE_EQUALS = '=',
    KEYCODE_GREATER = '>',
    KEYCODE_QUESTION = '?',
    KEYCODE_AT = '@',
    KEYCODE_LEFTBRACKET = '[',
    KEYCODE_BACKSLASH = '\\',
    KEYCODE_RIGHTBRACKET = ']',
    KEYCODE_CARET = '^',
    KEYCODE_UNDERSCORE = '_',
    KEYCODE_BACKQUOTE = '`',
    KEYCODE_a = 'a',
    KEYCODE_b = 'b',
    KEYCODE_c = 'c',
    KEYCODE_d = 'd',
    KEYCODE_e = 'e',
    KEYCODE_f = 'f',
    KEYCODE_g = 'g',
    KEYCODE_h = 'h',
    KEYCODE_i = 'i',
    KEYCODE_j = 'j',
    KEYCODE_k = 'k',
    KEYCODE_l = 'l',
    KEYCODE_m = 'm',
    KEYCODE_n = 'n',
    KEYCODE_o = 'o',
    KEYCODE_p = 'p',
    KEYCODE_q = 'q',
    KEYCODE_r = 'r',
    KEYCODE_s = 's',
    KEYCODE_t = 't',
    KEYCODE_u = 'u',
    KEYCODE_v = 'v',
    KEYCODE_w = 'w',
    KEYCODE_x = 'x',
    KEYCODE_y = 'y',
    KEYCODE_z = 'z',
    KEYCODE_CAPSLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),
    KEYCODE_F1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
    KEYCODE_F2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
    KEYCODE_F3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
    KEYCODE_F4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
    KEYCODE_F5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
    KEYCODE_F6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
    KEYCODE_F7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
    KEYCODE_F8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
    KEYCODE_F9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
    KEYCODE_F10 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
    KEYCODE_F11 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
    KEYCODE_F12 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),
    KEYCODE_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
    KEYCODE_SCROLLLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
    KEYCODE_PAUSE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
    KEYCODE_INSERT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
    KEYCODE_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
    KEYCODE_PAGEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
    KEYCODE_DELETE = '\x7F',
    KEYCODE_END = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
    KEYCODE_PAGEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
    KEYCODE_RIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
    KEYCODE_LEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
    KEYCODE_DOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
    KEYCODE_UP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),
    KEYCODE_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
    KEYCODE_KP_DIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
    KEYCODE_KP_MULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
    KEYCODE_KP_MINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
    KEYCODE_KP_PLUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
    KEYCODE_KP_ENTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
    KEYCODE_KP_1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
    KEYCODE_KP_2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
    KEYCODE_KP_3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
    KEYCODE_KP_4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
    KEYCODE_KP_5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
    KEYCODE_KP_6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
    KEYCODE_KP_7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
    KEYCODE_KP_8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
    KEYCODE_KP_9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
    KEYCODE_KP_0 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
    KEYCODE_KP_PERIOD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),
    KEYCODE_APPLICATION = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
    KEYCODE_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
    KEYCODE_KP_EQUALS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
    KEYCODE_F13 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
    KEYCODE_F14 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
    KEYCODE_F15 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
    KEYCODE_F16 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
    KEYCODE_F17 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
    KEYCODE_F18 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
    KEYCODE_F19 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
    KEYCODE_F20 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
    KEYCODE_F21 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
    KEYCODE_F22 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
    KEYCODE_F23 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
    KEYCODE_F24 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
    KEYCODE_EXECUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
    KEYCODE_HELP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
    KEYCODE_MENU = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
    KEYCODE_SELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
    KEYCODE_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
    KEYCODE_AGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
    KEYCODE_UNDO = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
    KEYCODE_CUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
    KEYCODE_COPY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
    KEYCODE_PASTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
    KEYCODE_FIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
    KEYCODE_MUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
    KEYCODE_VOLUMEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
    KEYCODE_VOLUMEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
    KEYCODE_KP_COMMA = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
    KEYCODE_KP_EQUALSAS400 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),
    KEYCODE_ALTERASE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
    KEYCODE_SYSREQ = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
    KEYCODE_CANCEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
    KEYCODE_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
    KEYCODE_PRIOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
    KEYCODE_RETURN2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
    KEYCODE_SEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
    KEYCODE_OUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
    KEYCODE_OPER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
    KEYCODE_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
    KEYCODE_CRSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
    KEYCODE_EXSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),
    KEYCODE_KP_00 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
    KEYCODE_KP_000 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
    KEYCODE_THOUSANDSSEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
    KEYCODE_DECIMALSEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
    KEYCODE_CURRENCYUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
    KEYCODE_CURRENCYSUBUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
    KEYCODE_KP_LEFTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
    KEYCODE_KP_RIGHTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
    KEYCODE_KP_LEFTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
    KEYCODE_KP_RIGHTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
    KEYCODE_KP_TAB = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
    KEYCODE_KP_BACKSPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
    KEYCODE_KP_A = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
    KEYCODE_KP_B = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
    KEYCODE_KP_C = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
    KEYCODE_KP_D = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
    KEYCODE_KP_E = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
    KEYCODE_KP_F = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
    KEYCODE_KP_XOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
    KEYCODE_KP_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
    KEYCODE_KP_PERCENT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
    KEYCODE_KP_LESS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
    KEYCODE_KP_GREATER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
    KEYCODE_KP_AMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
    KEYCODE_KP_DBLAMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
    KEYCODE_KP_VERTICALBAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
    KEYCODE_KP_DBLVERTICALBAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
    KEYCODE_KP_COLON = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
    KEYCODE_KP_HASH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
    KEYCODE_KP_SPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
    KEYCODE_KP_AT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
    KEYCODE_KP_EXCLAM = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
    KEYCODE_KP_MEMSTORE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
    KEYCODE_KP_MEMRECALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
    KEYCODE_KP_MEMCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
    KEYCODE_KP_MEMADD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
    KEYCODE_KP_MEMSUBTRACT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
    KEYCODE_KP_MEMMULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
    KEYCODE_KP_MEMDIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
    KEYCODE_KP_PLUSMINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
    KEYCODE_KP_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
    KEYCODE_KP_CLEARENTRY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
    KEYCODE_KP_BINARY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
    KEYCODE_KP_OCTAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
    KEYCODE_KP_DECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
    KEYCODE_KP_HEXADECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),
    KEYCODE_LCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
    KEYCODE_LSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
    KEYCODE_LALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
    KEYCODE_LGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
    KEYCODE_RCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
    KEYCODE_RSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
    KEYCODE_RALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
    KEYCODE_RGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),
    KEYCODE_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),
    KEYCODE_AUDIONEXT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
    KEYCODE_AUDIOPREV = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
    KEYCODE_AUDIOSTOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
    KEYCODE_AUDIOPLAY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
    KEYCODE_AUDIOMUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
    KEYCODE_MEDIASELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
    KEYCODE_WWW = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
    KEYCODE_MAIL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
    KEYCODE_CALCULATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
    KEYCODE_COMPUTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
    KEYCODE_AC_SEARCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
    KEYCODE_AC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
    KEYCODE_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
    KEYCODE_AC_FORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
    KEYCODE_AC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
    KEYCODE_AC_REFRESH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
    KEYCODE_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),
    KEYCODE_BRIGHTNESSDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
    KEYCODE_BRIGHTNESSUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
    KEYCODE_DISPLAYSWITCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
    KEYCODE_KBDILLUMTOGGLE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
    KEYCODE_KBDILLUMDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
    KEYCODE_KBDILLUMUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
    KEYCODE_EJECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
    KEYCODE_SLEEP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP),
    KEYCODE_APP1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP1),
    KEYCODE_APP2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP2),
    KEYCODE_AUDIOREWIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOREWIND),
    KEYCODE_AUDIOFASTFORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOFASTFORWARD),
    KEYCODE_SOFTLEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT),
    KEYCODE_SOFTRIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT),
    KEYCODE_CALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL),
    KEYCODE_ENDCALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL)
};

enum Scancode {
    SCANCODE_UNKNOWN = 0,

    SCANCODE_A = 4,
    SCANCODE_B = 5,
    SCANCODE_C = 6,
    SCANCODE_D = 7,
    SCANCODE_E = 8,
    SCANCODE_F = 9,
    SCANCODE_G = 10,
    SCANCODE_H = 11,
    SCANCODE_I = 12,
    SCANCODE_J = 13,
    SCANCODE_K = 14,
    SCANCODE_L = 15,
    SCANCODE_M = 16,
    SCANCODE_N = 17,
    SCANCODE_O = 18,
    SCANCODE_P = 19,
    SCANCODE_Q = 20,
    SCANCODE_R = 21,
    SCANCODE_S = 22,
    SCANCODE_T = 23,
    SCANCODE_U = 24,
    SCANCODE_V = 25,
    SCANCODE_W = 26,
    SCANCODE_X = 27,
    SCANCODE_Y = 28,
    SCANCODE_Z = 29,

    SCANCODE_1 = 30,
    SCANCODE_2 = 31,
    SCANCODE_3 = 32,
    SCANCODE_4 = 33,
    SCANCODE_5 = 34,
    SCANCODE_6 = 35,
    SCANCODE_7 = 36,
    SCANCODE_8 = 37,
    SCANCODE_9 = 38,
    SCANCODE_0 = 39,

    SCANCODE_RETURN = 40,
    SCANCODE_ESCAPE = 41,
    SCANCODE_BACKSPACE = 42,
    SCANCODE_TAB = 43,
    SCANCODE_SPACE = 44,

    SCANCODE_MINUS = 45,
    SCANCODE_EQUALS = 46,
    SCANCODE_LEFTBRACKET = 47,
    SCANCODE_RIGHTBRACKET = 48,
    SCANCODE_BACKSLASH = 49, 
    SCANCODE_NONUSHASH = 50, 
    SCANCODE_SEMICOLON = 51,
    SCANCODE_APOSTROPHE = 52,
    SCANCODE_GRAVE = 53, 
    SCANCODE_COMMA = 54,
    SCANCODE_PERIOD = 55,
    SCANCODE_SLASH = 56,

    SCANCODE_CAPSLOCK = 57,

    SCANCODE_F1 = 58,
    SCANCODE_F2 = 59,
    SCANCODE_F3 = 60,
    SCANCODE_F4 = 61,
    SCANCODE_F5 = 62,
    SCANCODE_F6 = 63,
    SCANCODE_F7 = 64,
    SCANCODE_F8 = 65,
    SCANCODE_F9 = 66,
    SCANCODE_F10 = 67,
    SCANCODE_F11 = 68,
    SCANCODE_F12 = 69,

    SCANCODE_PRINTSCREEN = 70,
    SCANCODE_SCROLLLOCK = 71,
    SCANCODE_PAUSE = 72,
    SCANCODE_INSERT = 73, 
    SCANCODE_HOME = 74,
    SCANCODE_PAGEUP = 75,
    SCANCODE_DELETE = 76,
    SCANCODE_END = 77,
    SCANCODE_PAGEDOWN = 78,
    SCANCODE_RIGHT = 79,
    SCANCODE_LEFT = 80,
    SCANCODE_DOWN = 81,
    SCANCODE_UP = 82,

    SCANCODE_NUMLOCKCLEAR = 83, 
    SCANCODE_KP_DIVIDE = 84,
    SCANCODE_KP_MULTIPLY = 85,
    SCANCODE_KP_MINUS = 86,
    SCANCODE_KP_PLUS = 87,
    SCANCODE_KP_ENTER = 88,
    SCANCODE_KP_1 = 89,
    SCANCODE_KP_2 = 90,
    SCANCODE_KP_3 = 91,
    SCANCODE_KP_4 = 92,
    SCANCODE_KP_5 = 93,
    SCANCODE_KP_6 = 94,
    SCANCODE_KP_7 = 95,
    SCANCODE_KP_8 = 96,
    SCANCODE_KP_9 = 97,
    SCANCODE_KP_0 = 98,
    SCANCODE_KP_PERIOD = 99,

    SCANCODE_NONUSBACKSLASH = 100, 
    SCANCODE_APPLICATION = 101, 
    SCANCODE_POWER = 102, 
    SCANCODE_KP_EQUALS = 103,
    SCANCODE_F13 = 104,
    SCANCODE_F14 = 105,
    SCANCODE_F15 = 106,
    SCANCODE_F16 = 107,
    SCANCODE_F17 = 108,
    SCANCODE_F18 = 109,
    SCANCODE_F19 = 110,
    SCANCODE_F20 = 111,
    SCANCODE_F21 = 112,
    SCANCODE_F22 = 113,
    SCANCODE_F23 = 114,
    SCANCODE_F24 = 115,
    SCANCODE_EXECUTE = 116,
    SCANCODE_HELP = 117,    
    SCANCODE_MENU = 118,    
    SCANCODE_SELECT = 119,
    SCANCODE_STOP = 120,   
    SCANCODE_AGAIN = 121,   
    SCANCODE_UNDO = 122,  
    SCANCODE_CUT = 123, 
    SCANCODE_COPY = 124,
    SCANCODE_PASTE = 125,  
    SCANCODE_FIND = 126,   
    SCANCODE_MUTE = 127,
    SCANCODE_VOLUMEUP = 128,
    SCANCODE_VOLUMEDOWN = 129,
    SCANCODE_KP_COMMA = 133,
    SCANCODE_KP_EQUALSAS400 = 134,

    SCANCODE_INTERNATIONAL1 = 135,
    SCANCODE_INTERNATIONAL2 = 136,
    SCANCODE_INTERNATIONAL3 = 137,
    SCANCODE_INTERNATIONAL4 = 138,
    SCANCODE_INTERNATIONAL5 = 139,
    SCANCODE_INTERNATIONAL6 = 140,
    SCANCODE_INTERNATIONAL7 = 141,
    SCANCODE_INTERNATIONAL8 = 142,
    SCANCODE_INTERNATIONAL9 = 143,

    SCANCODE_ALTERASE = 153,
    SCANCODE_SYSREQ = 154,
    SCANCODE_CANCEL = 155, 
    SCANCODE_CLEAR = 156,
    SCANCODE_PRIOR = 157,
    SCANCODE_RETURN2 = 158,
    SCANCODE_SEPARATOR = 159,
    SCANCODE_OUT = 160,
    SCANCODE_OPER = 161,
    SCANCODE_CLEARAGAIN = 162,
    SCANCODE_CRSEL = 163,
    SCANCODE_EXSEL = 164,

    SCANCODE_KP_00 = 176,
    SCANCODE_KP_000 = 177,
    SCANCODE_THOUSANDSSEPARATOR = 178,
    SCANCODE_DECIMALSEPARATOR = 179,
    SCANCODE_CURRENCYUNIT = 180,
    SCANCODE_CURRENCYSUBUNIT = 181,
    SCANCODE_KP_LEFTPAREN = 182,
    SCANCODE_KP_RIGHTPAREN = 183,
    SCANCODE_KP_LEFTBRACE = 184,
    SCANCODE_KP_RIGHTBRACE = 185,
    SCANCODE_KP_TAB = 186,
    SCANCODE_KP_BACKSPACE = 187,
    SCANCODE_KP_A = 188,
    SCANCODE_KP_B = 189,
    SCANCODE_KP_C = 190,
    SCANCODE_KP_D = 191,
    SCANCODE_KP_E = 192,
    SCANCODE_KP_F = 193,
    SCANCODE_KP_XOR = 194,
    SCANCODE_KP_POWER = 195,
    SCANCODE_KP_PERCENT = 196,
    SCANCODE_KP_LESS = 197,
    SCANCODE_KP_GREATER = 198,
    SCANCODE_KP_AMPERSAND = 199,
    SCANCODE_KP_DBLAMPERSAND = 200,
    SCANCODE_KP_VERTICALBAR = 201,
    SCANCODE_KP_DBLVERTICALBAR = 202,
    SCANCODE_KP_COLON = 203,
    SCANCODE_KP_HASH = 204,
    SCANCODE_KP_SPACE = 205,
    SCANCODE_KP_AT = 206,
    SCANCODE_KP_EXCLAM = 207,
    SCANCODE_KP_MEMSTORE = 208,
    SCANCODE_KP_MEMRECALL = 209,
    SCANCODE_KP_MEMCLEAR = 210,
    SCANCODE_KP_MEMADD = 211,
    SCANCODE_KP_MEMSUBTRACT = 212,
    SCANCODE_KP_MEMMULTIPLY = 213,
    SCANCODE_KP_MEMDIVIDE = 214,
    SCANCODE_KP_PLUSMINUS = 215,
    SCANCODE_KP_CLEAR = 216,
    SCANCODE_KP_CLEARENTRY = 217,
    SCANCODE_KP_BINARY = 218,
    SCANCODE_KP_OCTAL = 219,
    SCANCODE_KP_DECIMAL = 220,
    SCANCODE_KP_HEXADECIMAL = 221,

    SCANCODE_LCTRL = 224,
    SCANCODE_LSHIFT = 225,
    SCANCODE_LALT = 226, 
    SCANCODE_LGUI = 227, 
    SCANCODE_RCTRL = 228,
    SCANCODE_RSHIFT = 229,
    SCANCODE_RALT = 230,
    SCANCODE_RGUI = 231, 

    SCANCODE_MODE = 257,    
    SCANCODE_AUDIONEXT = 258,
    SCANCODE_AUDIOPREV = 259,
    SCANCODE_AUDIOSTOP = 260,
    SCANCODE_AUDIOPLAY = 261,
    SCANCODE_AUDIOMUTE = 262,
    SCANCODE_MEDIASELECT = 263,
    SCANCODE_WWW = 264,             
    SCANCODE_MAIL = 265,
    SCANCODE_CALCULATOR = 266,      
    SCANCODE_COMPUTER = 267,
    SCANCODE_AC_SEARCH = 268,    
    SCANCODE_AC_HOME = 269,        
    SCANCODE_AC_BACK = 270,      
    SCANCODE_AC_FORWARD = 271,    
    SCANCODE_AC_STOP = 272,   
    SCANCODE_AC_REFRESH = 273,    
    SCANCODE_AC_BOOKMARKS = 274,  
    SCANCODE_BRIGHTNESSDOWN = 275,
    SCANCODE_BRIGHTNESSUP = 276,
    SCANCODE_DISPLAYSWITCH = 277,
    SCANCODE_KBDILLUMTOGGLE = 278,
    SCANCODE_KBDILLUMDOWN = 279,
    SCANCODE_KBDILLUMUP = 280,
    SCANCODE_EJECT = 281,
    SCANCODE_SLEEP = 282,           

    SCANCODE_APP1 = 283,
    SCANCODE_APP2 = 284,

    SCANCODE_AUDIOREWIND = 285,
    SCANCODE_AUDIOFASTFORWARD = 286,

    SCANCODE_SOFTLEFT = 287,
    SCANCODE_SOFTRIGHT = 288, 
    SCANCODE_CALL = 289, 
    SCANCODE_ENDCALL = 290, 
    NUM_SCANCODES = 512 
};

typedef struct SDL_BlitMap SDL_BlitMap;  
struct Surface {
    Uint32 flags;    
    SDL_PixelFormat *format;  

    int w, h;               
    int pitch;               
    void *pixels;
    void *userdata;
    int locked;
    void *list_blitmap;

    SDL_Rect clip_rect;
    SDL_BlitMap *map; 

    int refcount; 
};

enum SystemCursor {
    SYSTEM_CURSOR_ARROW,    
    SYSTEM_CURSOR_IBEAM,    
    SYSTEM_CURSOR_WAIT,      
    SYSTEM_CURSOR_CROSSHAIR, 
    SYSTEM_CURSOR_WAITARROW, 
    SYSTEM_CURSOR_SIZENWSE,  
    SYSTEM_CURSOR_SIZENESW,  
    SYSTEM_CURSOR_SIZEWE,   
    SYSTEM_CURSOR_SIZENS,    
    SYSTEM_CURSOR_SIZEALL,   
    SYSTEM_CURSOR_NO,        
    SYSTEM_CURSOR_HAND,      
    NUM_SYSTEM_CURSORS
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
 * @struct Rect
 * @brief Represents a rectangle with origin at the top-left corner
 */
struct Rect {
    int16_t x, y, w, h;    ///< Position (x, y) and size (width, height)
};

/**
 * @struct Square
 * @brief Rectangle without separate width and height (used for squares/circles)
 */
struct Square {
    int x, y;       ///< Position
    int size;       ///< Width and height are the same
};

/**
 * @struct Color
 * @brief RGBA color representation
 */
struct Color {
    uint8_t r, g, b, a;     ///< Red, Green, Blue, Alpha components (0-255)
};

struct QueuedFrame {
    AVFrame* frame = nullptr;
    double pts_seconds = 0.0;
};
 
class FrameQueue {
public:
    explicit FrameQueue(size_t max_size) : max_size_(max_size) {}
 
    bool push(QueuedFrame f) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_not_full_.wait(lock, [&] { return queue_.size() < max_size_ || stop_; });
        if (stop_) return false;
        queue_.push_back(f);
        cv_not_empty_.notify_one();
        return true;
    }
 
    bool try_pop_ready(double now_seconds, QueuedFrame& out) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) return false;
        if (queue_.front().pts_seconds > now_seconds) return false;
        out = queue_.front();
        queue_.pop_front();
        cv_not_full_.notify_one();
        return true;
    }
 
    bool empty() {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }
 
    void stop() {
        std::lock_guard<std::mutex> lock(mtx_);
        stop_ = true;
        while (!queue_.empty()) {
            av_frame_free(&queue_.front().frame);
            queue_.pop_front();
        }
        cv_not_full_.notify_all();
        cv_not_empty_.notify_all();
    }
 
    void clear() {
        std::lock_guard<std::mutex> lock(mtx_);
        while (!queue_.empty()) {
            av_frame_free(&queue_.front().frame);
            queue_.pop_front();
        }
        cv_not_full_.notify_all();
    }
private:
    std::mutex mtx_;
    std::condition_variable cv_not_full_;
    std::condition_variable cv_not_empty_;
    std::deque<QueuedFrame> queue_;
    size_t max_size_;
    bool stop_ = false;
};

struct DecoderState {
    AVFormatContext* fmt_ctx = nullptr;
    AVCodecContext*  codec_ctx = nullptr;   
    SwsContext*      sws_ctx = nullptr;
    int video_stream_idx = -1;
    
    AVCodecContext*  audio_codec_ctx = nullptr;
    SwrContext*      swr_ctx = nullptr;
    int audio_stream_idx = -1;
    int out_sample_rate = 48000;
    int out_channels = 2;

    void cleanup() {
        if (sws_ctx) sws_freeContext(sws_ctx);
        if (codec_ctx) avcodec_free_context(&codec_ctx);
        if (swr_ctx) swr_free(&swr_ctx);
        if (audio_codec_ctx) avcodec_free_context(&audio_codec_ctx);
        if (fmt_ctx) avformat_close_input(&fmt_ctx);
    }
};

struct VideoDecoderContext {
    DecoderState decoderState;
    FrameQueue queue{8};
    SDL_AudioDeviceID audio_device = 0;
    std::atomic<bool> decoding{true};
    std::atomic<bool> loop_restart{false};
    std::atomic<bool> looped{false};
    std::atomic<float> volume{1.0f};
    std::atomic<bool> seek_requested{false};
    std::atomic<double> seek_target_seconds{0.0};
    
    bool isVulkan = false;
    
    ~VideoDecoderContext() {
        if (audio_device) SDL_CloseAudioDevice(audio_device);
        decoderState.cleanup();
    }
};

struct Window;

/**
 * @struct Video
 * @brief Video object managed with ffmpeg library
 */
struct Video {
    struct SDLTextureDeleter {
        void operator()(SDL_Texture* t) const { if (t) SDL_DestroyTexture(t); }
    };
    using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

    std::string name;
    uint64_t id = 0;

    Window* win = nullptr;

    SDL_Renderer* renderer = nullptr;
    SDL_Rect displayRect{};
    TexturePtr texture;
    
    int width = 0;
    int height = 0;

    int64_t playback_start = 0; 
    std::unique_ptr<VideoDecoderContext> ctx;
    std::thread decoderThread;
 
    Video() = default;
 
    Video(const Video&) = delete;
    Video& operator=(const Video&) = delete;
 
    Video(Video&&) = default;
    Video& operator=(Video&&) = default;
 
    ~Video() {
        if (ctx) {
            ctx->decoding.store(false);
            ctx->queue.stop();
        }
        if (decoderThread.joinable()) decoderThread.join();
    }
 
    bool finished() const {
        return ctx && !ctx->decoding.load() && ctx->queue.empty();
    }

    void seek(double seconds) {
        if (!ctx) return;
        ctx->seek_target_seconds.store(seconds);
        ctx->seek_requested.store(true);
    }

    void setVolume(float v) {
        if (ctx) ctx->volume.store(v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v));
    }
};

/**
 * @class Engine
 * @brief Vulkan Engine to deliver significantly higher frame rates, reduced power consumption, and smoother performance in modern resource-intensive applications.
 */
class Engine {
    private:
        struct Vertex {
            vec2 pos;
            vec3 color;
            vec2 uv;
            float alpha;
            float fontIndex;
            float type;
            float imgIndex;
        };

        struct UBO {
            mat4 proj;
        };

        struct FontAtlas {
            std::vector<stbtt_packedchar> glyphsAscii;  
            std::vector<stbtt_packedchar> glyphsCyr;     
            int atlasW = 1024, atlasH = 1024;
            std::vector<uint8_t> bitmap;
            float baselineOffset = 0.0f;

            VkImage fontImage;
            VkDeviceMemory fontMemory;
            VkImageView fontView;
            VkSampler fontSampler;
        };

        struct FontKey {
            std::string path;
            int size;
            bool operator==(const FontKey& o) const { return path == o.path && size == o.size; }
        };

        struct FontKeyHash {
            size_t operator()(const FontKey& k) const {
                return std::hash<std::string>()(k.path) ^ (std::hash<int>()(k.size) << 1);
            }
        };

        struct Image {
            std::string path;
            int w = 0;
            int h = 0;
            int ch = 0;

            bool dirty    = false;
            bool editable = false;
            bool captureTarget = false;
            
            int captureSrcX = 0, captureSrcY = 0;

            unsigned char* pixels = nullptr;

            VkDeviceSize getSize() const {
                return static_cast<VkDeviceSize>(w) * h * 4;
            }

            VkBuffer stagingBuf = VK_NULL_HANDLE;
            VkDeviceMemory stagingMem = VK_NULL_HANDLE;

            VkImage textureImage = VK_NULL_HANDLE;     
            VkDeviceMemory textureMem = VK_NULL_HANDLE;
            VkImageView imageView = VK_NULL_HANDLE;      
            VkSampler sampler = VK_NULL_HANDLE;
        };

        struct DrawCall {
            uint32_t indexOffset;
            uint32_t indexCount;
            VkRect2D scissor;
        };

        struct VideoTexture {
            VkImage image = VK_NULL_HANDLE;
            VkDeviceMemory memory = VK_NULL_HANDLE;
            VkImageView view = VK_NULL_HANDLE;
            VkSampler sampler = VK_NULL_HANDLE;
            VkBuffer stagingBuf = VK_NULL_HANDLE;
            VkDeviceMemory stagingMem = VK_NULL_HANDLE;
            void* stagingMapped = nullptr;
            int width = 0, height = 0;
            int imgIndex = -1; 
        };

        struct ColorFloat { 
            float    r, g, b, a; 
        };

        VkInstance                      instance                = VK_NULL_HANDLE;
        VkSurfaceKHR                    surface                 = VK_NULL_HANDLE;
        uint32_t                        graphicsFamily          = UINT32_MAX;
        uint32_t                        presentFamily           = UINT32_MAX;
        VkPhysicalDevice                physDevice              = VK_NULL_HANDLE;
        VkSwapchainKHR                  swapchain               = VK_NULL_HANDLE;
        VkSurfaceFormatKHR              chosenFormat            {};
        VkExtent2D                      extent                  {};
        VkImageView                     depthImageView          = VK_NULL_HANDLE;
        VkDevice                        device                  = VK_NULL_HANDLE;
        std::vector<VkImageView>        imageViews;
        std::vector<VkImage>            swapchainImages;
        VkRenderPass                    renderPass              = VK_NULL_HANDLE;
        VkDescriptorSetLayout           descriptorLayout        = VK_NULL_HANDLE;
        std::vector<VkFramebuffer>      framebuffers;
        VkBuffer                        uniformBuffer           = VK_NULL_HANDLE;
        VkDeviceMemory                  uniformMemory           = VK_NULL_HANDLE;
        void*                           uniformMapped           = nullptr;
        std::vector<VkSemaphore>        imageAvailableSems;
        std::vector<VkSemaphore>        renderFinishedSems;
        std::vector<VkFence>            inFlightFences;
        VkPipeline                      graphicsPipeline        = VK_NULL_HANDLE;
        VkQueue                         graphicsQueue           = VK_NULL_HANDLE;
        VkQueue                         presentQueue            = VK_NULL_HANDLE;
        VkCommandPool                   cmdPool                 = VK_NULL_HANDLE;
        uint32_t                        indexCount              = 0;
        const int                       MAX_FRAMES_IN_FLIGHT    = 2;
        std::vector<VkCommandBuffer>    cmdBuffers;
        VkPipelineLayout                pipelineLayout          = VK_NULL_HANDLE;
        VkDescriptorSet                 descriptorSet           = VK_NULL_HANDLE;
        std::array<VkBuffer, 2>         vertexBuffers{};
        std::array<VkBuffer, 2>         indexBuffers{};
        VkImage                         depthImage              = VK_NULL_HANDLE;
        VkDeviceMemory                  depthMemory             = VK_NULL_HANDLE;
        std::array<VkDeviceMemory, 2>   vertexBufferMemories{};
        std::array<VkDeviceMemory, 2>   indexBufferMemories{};
        VkDescriptorPool                descriptorPool          = VK_NULL_HANDLE;
        std::vector<FontAtlas>          fonts;
        std::vector<Image>              imgs;
        std::unordered_map
         <FontKey, int, FontKeyHash> fontAtlasCache;
        VkSampleCountFlagBits           msaaSamples             = VK_SAMPLE_COUNT_1_BIT;
        VkImage                         colorImageMSAA          = VK_NULL_HANDLE;
        VkDeviceMemory                  colorImageMemoryMSAA    = VK_NULL_HANDLE;
        VkImageView                     colorImageViewMSAA      = VK_NULL_HANDLE;
        VkImage                         dummyImage              = VK_NULL_HANDLE;
        VkDeviceMemory                  dummyMemory             = VK_NULL_HANDLE;
        VkImageView                     dummyImageView          = VK_NULL_HANDLE;
        VkSampler                       dummySampler            = VK_NULL_HANDLE;
        std::array<void*, 2> vertexMappeds{};
        std::array<void*, 2> indexMappeds{};
        std::array<VkDeviceSize, 2> vertexBufferCapacities{};
        std::array<VkDeviceSize, 2> indexBufferCapacities{};
        VkRect2D                        scissorRect             = {};
        bool                            useScissor              = false;
        uint32_t                        currentFrame            = 0;
        std::unordered_map
         <uint64_t, VideoTexture>       videoTextures;
        VkBuffer                        captureStagingBuf = VK_NULL_HANDLE;
        VkDeviceMemory                  captureStagingMem = VK_NULL_HANDLE;
        VkDeviceSize                    captureBufSize = 0;
        VkRenderPass                    renderPassOverlay       = VK_NULL_HANDLE;
        std::vector<VkFramebuffer>      framebuffersOverlay;
        VkPipeline                      overlayPipeline         = VK_NULL_HANDLE;
        VkPipelineLayout                overlayPipelineLayout   = VK_NULL_HANDLE; 
        std::unordered_map<std::string, std::string> resolvedFontPathCache;

        ColorFloat bg                                           = {10, 10, 10, 255};

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        std::vector<uint32_t> VERT;
        std::vector<uint32_t> FRAG;

        const char* vertSrc = R"(
            #version 450

            layout(location = 0) in vec2 inPos;
            layout(location = 1) in vec2 inUV;
            layout(location = 2) in vec3 inColor;
            layout(location = 3) in float inAlpha;
            layout(location = 4) in float inFontIndex;
            layout(location = 5) in float inType;
            layout(location = 6) in float inImgIndex;

            layout(location = 0) out vec3 fragColor;
            layout(location = 1) out vec2 fragUV;
            layout(location = 2) out float fragAlpha;
            layout(location = 3) out float fragFontIndex;
            layout(location = 4) out float fragType;
            layout(location = 5) out float fragImgIndex;

            layout(binding = 0) uniform UBO {
                mat4 proj;
            };

            void main() {
                fragColor = inColor;
                fragUV = inUV;
                fragAlpha = inAlpha;
                fragFontIndex = inFontIndex;
                fragType = inType;
                fragImgIndex = inImgIndex;
                gl_Position = proj * vec4(inPos, 0.0, 1.0);
            }
        )";

        const char* fragSrc = R"(
            #version 450
            layout(binding = 1) uniform sampler2D fontAtlases[FONT_COUNT];
            layout(binding = 2) uniform sampler2D imgSamplers[IMG_COUNT];

            layout(location = 0) in vec3 fragColor;
            layout(location = 1) in vec2 fragUV;
            layout(location = 2) in float fragAlpha;
            layout(location = 3) in float fragFontIndex;
            layout(location = 4) in float fragType;
            layout(location = 5) in float fragImgIndex;

            layout(location = 0) out vec4 outColor;

            void main() {
                int type = int(fragType);
                if (type == 0) {
                    int idx = int(fragFontIndex);
                    float texAlpha = texture(fontAtlases[idx], fragUV).r;
                    outColor = vec4(fragColor, fragAlpha * texAlpha);
                } else if (type == 1) {
                    int idx = int(fragImgIndex);
                    outColor = texture(imgSamplers[idx], fragUV);
                } else {
                    outColor = vec4(fragColor, fragAlpha);
                }
            }
        )";

        std::vector<char> readFile(const std::string& path) {
            std::ifstream file(path, std::ios::binary | std::ios::ate);

            if (!file.is_open()) {
                std::cerr << "ERROR: Cannot open file: " << path << "\n";
                        
                return std::vector<char>{};
            }

            size_t size = file.tellg();
            std::vector<char> buf(size);
            file.seekg(0);
            file.read(buf.data(), size);
            return buf;
        };

        void compileShaders() {
            uint32_t fontCount = (uint32_t)fonts.size();
            if (fontCount == 0) fontCount = 1;
            shaderc::SpvCompilationResult vertResult =
                compiler.CompileGlslToSpv(vertSrc, shaderc_vertex_shader, "vert", options);
            if (vertResult.GetCompilationStatus() != shaderc_compilation_status_success)
                std::cerr << "Vert ERROR: " << vertResult.GetErrorMessage() << "\n";
            VERT.assign(vertResult.cbegin(), vertResult.cend());

            shaderc::CompileOptions fragOpts;
            uint32_t imgCount = (uint32_t)imgs.size();
            if (imgCount == 0) imgCount = 1;
            fragOpts.AddMacroDefinition("FONT_COUNT", std::to_string(fontCount));
            fragOpts.AddMacroDefinition("IMG_COUNT", std::to_string(imgCount));
            
            shaderc::SpvCompilationResult fragResult =
                compiler.CompileGlslToSpv(fragSrc, shaderc_fragment_shader, "frag", fragOpts);
            if (fragResult.GetCompilationStatus() != shaderc_compilation_status_success)
                std::cerr << "Frag ERROR: " << fragResult.GetErrorMessage() << "\n";
            FRAG.assign(fragResult.cbegin(), fragResult.cend());
        }

        uint32_t findMemoryType(uint32_t filter, VkMemoryPropertyFlags props, VkPhysicalDevice physDevice) {
            VkPhysicalDeviceMemoryProperties mp;
            vkGetPhysicalDeviceMemoryProperties(physDevice, &mp);
            for (uint32_t i = 0; i < mp.memoryTypeCount; i++) {
                if ((filter & (1 << i)) && (mp.memoryTypes[i].propertyFlags & props) == props) {
                    return i;
                } else { 
                    std::runtime_error("no suitable memory type");
                    return 0;
                }
            }

            return 0;
        }

        void createBuffer(VkDevice device, VkPhysicalDevice physDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory) {
            VkBufferCreateInfo bi{};
            bi.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bi.size        = size;
            bi.usage       = usage;
            bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            vkCreateBuffer(device, &bi, nullptr, &buffer);

            VkMemoryRequirements memReqs;
            vkGetBufferMemoryRequirements(device, buffer, &memReqs);

            VkPhysicalDeviceMemoryProperties memProps;
            vkGetPhysicalDeviceMemoryProperties(physDevice, &memProps);

            uint32_t memTypeIndex = UINT32_MAX;
            for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
                if ((memReqs.memoryTypeBits & (1 << i)) &&
                    (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
                    memTypeIndex = i;
                    break;
                }
            }

            VkMemoryAllocateInfo ai{};
            ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            ai.allocationSize  = memReqs.size;
            ai.memoryTypeIndex = memTypeIndex;
            vkAllocateMemory(device, &ai, nullptr, &memory);
            vkBindBufferMemory(device, buffer, memory, 0);
        }

        void uploadBuffer(VkDevice device, VkPhysicalDevice physDevice, VkCommandPool cmdPool, VkQueue queue, void* data, VkDeviceSize size, VkBuffer& outBuffer, VkDeviceMemory& outMemory, VkBufferUsageFlags usage) {
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingMemory;

            createBuffer(device, physDevice, size,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingMemory);

            void* mapped;
            vkMapMemory(device, stagingMemory, 0, size, 0, &mapped);
            memcpy(mapped, data, size);
            vkUnmapMemory(device, stagingMemory);

            createBuffer(device, physDevice, size,
                usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                outBuffer, outMemory);

            VkCommandBufferAllocateInfo ai{};
            ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            ai.commandPool        = cmdPool;
            ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            ai.commandBufferCount = 1;

            VkCommandBuffer cmd;
            vkAllocateCommandBuffers(device, &ai, &cmd);

            VkCommandBufferBeginInfo bi{};
            bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vkBeginCommandBuffer(cmd, &bi);

            VkBufferCopy copy{ 0, 0, size };
            vkCmdCopyBuffer(cmd, stagingBuffer, outBuffer, 1, &copy);

            vkEndCommandBuffer(cmd);

            VkSubmitInfo si{};
            si.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            si.commandBufferCount = 1;
            si.pCommandBuffers    = &cmd;
            vkQueueSubmit(queue, 1, &si, VK_NULL_HANDLE);
            vkQueueWaitIdle(queue);

            vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
            vkDestroyBuffer(device, stagingBuffer, nullptr);
            vkFreeMemory(device, stagingMemory, nullptr);
        }

        void destroyBuffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory) {
            if (buffer != VK_NULL_HANDLE)
                vkDestroyBuffer(device, buffer, nullptr);
            if (memory != VK_NULL_HANDLE)
                vkFreeMemory(device, memory, nullptr);
        }
    
        void uploadFontAtlas(FontAtlas& atlas) {
            auto allocateImageMemory = [&](VkDevice device, VkPhysicalDevice physDevice,
                          VkImage image, VkMemoryPropertyFlags properties,
                          VkDeviceMemory& memory) {
                VkMemoryRequirements memReqs;
                vkGetImageMemoryRequirements(device, image, &memReqs);

                VkPhysicalDeviceMemoryProperties memProps;
                vkGetPhysicalDeviceMemoryProperties(physDevice, &memProps);

                uint32_t memTypeIndex = UINT32_MAX;
                for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
                    if ((memReqs.memoryTypeBits & (1 << i)) &&
                        (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
                        memTypeIndex = i;
                        break;
                    }
                }

                VkMemoryAllocateInfo ai{};
                ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                ai.allocationSize  = memReqs.size;
                ai.memoryTypeIndex = memTypeIndex;
                vkAllocateMemory(device, &ai, nullptr, &memory);
            };

            auto transitionImageLayout = [&](VkDevice device, VkCommandPool cmdPool, VkQueue queue,
                            VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
                VkCommandBufferAllocateInfo ai{};
                ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                ai.commandPool        = cmdPool;
                ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                ai.commandBufferCount = 1;
                VkCommandBuffer cmd;
                vkAllocateCommandBuffers(device, &ai, &cmd);

                VkCommandBufferBeginInfo bi{};
                bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                vkBeginCommandBuffer(cmd, &bi);

                VkImageMemoryBarrier barrier{};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = oldLayout;
                barrier.newLayout = newLayout;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = image;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                barrier.subresourceRange.baseMipLevel = 0;
                barrier.subresourceRange.levelCount = 1;
                barrier.subresourceRange.baseArrayLayer = 0;
                barrier.subresourceRange.layerCount = 1;

                VkPipelineStageFlags srcStage, dstStage;

                if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
                    newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                        newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                }
                else {
                    std::cerr << "ERROR: Unsupported layout transition\n";
                    srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                }

                vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0,
                    0, nullptr, 0, nullptr, 1, &barrier);

                vkEndCommandBuffer(cmd);

                VkSubmitInfo si{};
                si.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                si.commandBufferCount = 1;
                si.pCommandBuffers    = &cmd;
                vkQueueSubmit(queue, 1, &si, VK_NULL_HANDLE);
                vkQueueWaitIdle(queue);

                vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
            };

            auto copyBufferToImage = [&](VkDevice device, VkCommandPool cmdPool, VkQueue queue,
                        VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
                VkCommandBufferAllocateInfo ai{};
                ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                ai.commandPool        = cmdPool;
                ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                ai.commandBufferCount = 1;
                VkCommandBuffer cmd;
                vkAllocateCommandBuffers(device, &ai, &cmd);

                VkCommandBufferBeginInfo bi{};
                bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                vkBeginCommandBuffer(cmd, &bi);

                VkBufferImageCopy region{};
                region.bufferOffset = 0;
                region.bufferRowLength = 0;
                region.bufferImageHeight = 0;
                region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                region.imageSubresource.mipLevel = 0;
                region.imageSubresource.baseArrayLayer = 0;
                region.imageSubresource.layerCount = 1;
                region.imageOffset = {0, 0, 0};
                region.imageExtent = {width, height, 1};

                vkCmdCopyBufferToImage(cmd, buffer, image,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

                vkEndCommandBuffer(cmd);

                VkSubmitInfo si{};
                si.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                si.commandBufferCount = 1;
                si.pCommandBuffers    = &cmd;
                vkQueueSubmit(queue, 1, &si, VK_NULL_HANDLE);
                vkQueueWaitIdle(queue);

                vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
            };

            VkDeviceSize imageSize = atlas.atlasW * atlas.atlasH;

            VkBuffer stagingBuf;
            VkDeviceMemory stagingMem;
            createBuffer(device, physDevice, imageSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuf, stagingMem);

            void* data;
            vkMapMemory(device, stagingMem, 0, imageSize, 0, &data);
            memcpy(data, atlas.bitmap.data(), imageSize);
            vkUnmapMemory(device, stagingMem);

            VkImageCreateInfo imgInfo{};
            imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imgInfo.imageType = VK_IMAGE_TYPE_2D;
            imgInfo.format = VK_FORMAT_R8_UNORM;
            imgInfo.extent = {(uint32_t)atlas.atlasW, (uint32_t)atlas.atlasH, 1};
            imgInfo.mipLevels = 1;
            imgInfo.arrayLayers = 1;
            imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imgInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkCreateImage(device, &imgInfo, nullptr, &atlas.fontImage);

            allocateImageMemory(device, physDevice, atlas.fontImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, atlas.fontMemory);
            vkBindImageMemory(device, atlas.fontImage, atlas.fontMemory, 0);

            transitionImageLayout(device, cmdPool, graphicsQueue, atlas.fontImage,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            copyBufferToImage(device, cmdPool, graphicsQueue, stagingBuf, atlas.fontImage, atlas.atlasW, atlas.atlasH);

            transitionImageLayout(device, cmdPool, graphicsQueue, atlas.fontImage,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            vkDestroyBuffer(device, stagingBuf, nullptr);
            vkFreeMemory(device, stagingMem, nullptr);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = atlas.fontImage;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = VK_FORMAT_R8_UNORM;
            viewInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            vkCreateImageView(device, &viewInfo, nullptr, &atlas.fontView);

            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            vkCreateSampler(device, &samplerInfo, nullptr, &atlas.fontSampler);    
        }
    
        void updateImagePixels(Image& img, bool isFirstUpload) {
            VkDeviceSize imageSize = img.getSize();

            createBuffer(device, physDevice, imageSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                img.stagingBuf, img.stagingMem);

            void* data;
            vkMapMemory(device, img.stagingMem, 0, imageSize, 0, &data);
            memcpy(data, img.pixels, imageSize);
            vkUnmapMemory(device, img.stagingMem);

            if (!img.editable) {
                if (img.path.rfind("__avframe_", 0) == 0)
                    free(img.pixels);
                else
                    stbi_image_free(img.pixels);
                img.pixels = nullptr;
            }

            auto oneTimeCmd = [&]() {
                VkCommandBufferAllocateInfo cbai{};
                cbai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                cbai.commandPool        = cmdPool;
                cbai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                cbai.commandBufferCount = 1;
                VkCommandBuffer cmd;
                vkAllocateCommandBuffers(device, &cbai, &cmd);
                VkCommandBufferBeginInfo bi{};
                bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                vkBeginCommandBuffer(cmd, &bi);
                    
                return cmd;
            };

            auto submitCmd = [&](VkCommandBuffer cmd) {
                vkEndCommandBuffer(cmd);
                VkSubmitInfo si{};
                si.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                si.commandBufferCount = 1;
                si.pCommandBuffers    = &cmd;
                vkQueueSubmit(graphicsQueue, 1, &si, VK_NULL_HANDLE);
                vkQueueWaitIdle(graphicsQueue);
                vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
            };

            auto transition = [&](VkImageLayout oldL, VkImageLayout newL) {
                VkCommandBuffer cmd = oneTimeCmd();
                VkImageMemoryBarrier barrier{};
                barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout           = oldL;
                barrier.newLayout           = newL;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image               = img.textureImage;
                barrier.subresourceRange    = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

                VkPipelineStageFlags src, dst;
                if (oldL == VK_IMAGE_LAYOUT_UNDEFINED && newL == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    src = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    dst = VK_PIPELINE_STAGE_TRANSFER_BIT;
                } else if (oldL == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newL == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                    barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    src = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    dst = VK_PIPELINE_STAGE_TRANSFER_BIT;
                } else {
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    src = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    dst = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                }
                vkCmdPipelineBarrier(cmd, src, dst, 0, 0, nullptr, 0, nullptr, 1, &barrier);
                submitCmd(cmd);
            };

            VkCommandBuffer cmd = oneTimeCmd();

            VkImageMemoryBarrier toDst{};
            toDst.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toDst.oldLayout = isFirstUpload ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            toDst.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            toDst.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toDst.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toDst.image = img.textureImage;
            toDst.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            toDst.srcAccessMask = isFirstUpload ? 0 : VK_ACCESS_SHADER_READ_BIT;
            toDst.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            vkCmdPipelineBarrier(cmd,
                isFirstUpload ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT : VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toDst);

            VkBufferImageCopy region{};
            region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
            region.imageExtent = {(uint32_t)img.w, (uint32_t)img.h, 1};
            vkCmdCopyBufferToImage(cmd, img.stagingBuf, img.textureImage,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VkImageMemoryBarrier toRead{};
            toRead.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toRead.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            toRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            toRead.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toRead.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toRead.image = img.textureImage;
            toRead.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            toRead.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            toRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toRead);

            submitCmd(cmd);

            vkDestroyBuffer(device, img.stagingBuf, nullptr);
            vkFreeMemory(device, img.stagingMem, nullptr);
            img.stagingBuf = VK_NULL_HANDLE;
            img.stagingMem = VK_NULL_HANDLE;
        };

        void uploadImage(Image& img) {
            auto createImageResources = [&](Image& img) {
                VkImageCreateInfo imgInfo{};
                imgInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
                imgInfo.imageType     = VK_IMAGE_TYPE_2D;
                imgInfo.format        = VK_FORMAT_R8G8B8A8_UNORM;
                imgInfo.extent        = {(uint32_t)img.w, (uint32_t)img.h, 1};
                imgInfo.mipLevels     = 1;
                imgInfo.arrayLayers   = 1;
                imgInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
                imgInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
                imgInfo.usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
                imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                vkCreateImage(device, &imgInfo, nullptr, &img.textureImage);

                VkMemoryRequirements memReqs;
                vkGetImageMemoryRequirements(device, img.textureImage, &memReqs);
                VkPhysicalDeviceMemoryProperties memProps;
                vkGetPhysicalDeviceMemoryProperties(physDevice, &memProps);
                uint32_t memTypeIndex = UINT32_MAX;
                for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
                    if ((memReqs.memoryTypeBits & (1 << i)) &&
                        (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) ==
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
                        memTypeIndex = i; break;
                    }
                }

                VkMemoryAllocateInfo ai{};
                ai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                ai.allocationSize  = memReqs.size;
                ai.memoryTypeIndex = memTypeIndex;
                vkAllocateMemory(device, &ai, nullptr, &img.textureMem);
                vkBindImageMemory(device, img.textureImage, img.textureMem, 0);

                VkImageViewCreateInfo viewInfo{};
                viewInfo.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                viewInfo.image            = img.textureImage;
                viewInfo.viewType         = VK_IMAGE_VIEW_TYPE_2D;
                viewInfo.format           = VK_FORMAT_R8G8B8A8_UNORM;
                viewInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
                vkCreateImageView(device, &viewInfo, nullptr, &img.imageView);

                VkSamplerCreateInfo samplerInfo{};
                samplerInfo.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
                samplerInfo.magFilter    = VK_FILTER_LINEAR;
                samplerInfo.minFilter    = VK_FILTER_LINEAR;
                samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
                samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
                vkCreateSampler(device, &samplerInfo, nullptr, &img.sampler);
            };

            createImageResources(img);
            updateImagePixels(img, true);
        }

        void updateFontDescriptor() {
            if (fonts.empty()) return;

            std::vector<VkDescriptorImageInfo> fontInfos(fonts.size());
            for (size_t i = 0; i < fonts.size(); i++) {
                fontInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                fontInfos[i].imageView   = fonts[i].fontView;
                fontInfos[i].sampler     = fonts[i].fontSampler;
            }

            VkWriteDescriptorSet fontWrite{};
            fontWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            fontWrite.dstSet          = descriptorSet;
            fontWrite.dstBinding      = 1;
            fontWrite.dstArrayElement = 0;
            fontWrite.descriptorCount = (uint32_t)fontInfos.size();
            fontWrite.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            fontWrite.pImageInfo      = fontInfos.data();

            vkUpdateDescriptorSets(device, 1, &fontWrite, 0, nullptr);
        }
    
        void rebuildFontDependentResources() {
            if (graphicsPipeline != VK_NULL_HANDLE) {
                vkDeviceWaitIdle(device);
                vkDestroyPipeline(device, graphicsPipeline, nullptr);
                vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
                vkDestroyDescriptorPool(device, descriptorPool, nullptr);
                vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);
            }

            if (overlayPipeline != VK_NULL_HANDLE) {                      
                vkDestroyPipeline(device, overlayPipeline, nullptr);
                vkDestroyPipelineLayout(device, overlayPipelineLayout, nullptr); 
                overlayPipeline = VK_NULL_HANDLE;                          
                overlayPipelineLayout = VK_NULL_HANDLE;                  
            }

            if (uniformBuffer == VK_NULL_HANDLE) {
                createBuffer(device, physDevice,
                    sizeof(UBO),
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    uniformBuffer, uniformMemory);
                vkMapMemory(device, uniformMemory, 0, sizeof(UBO), 0, &uniformMapped);
            }

            compileShaders(); 

            VkDescriptorSetLayoutBinding uboBinding{};
            uboBinding.binding            = 0;
            uboBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboBinding.descriptorCount    = 1;
            uboBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutBinding samplerBinding{};
            samplerBinding.binding            = 1;
            samplerBinding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerBinding.descriptorCount    = (uint32_t)(fonts.empty() ? 1 : fonts.size());
            samplerBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

            VkDescriptorSetLayoutBinding samplerBindingImg{};
            samplerBindingImg.binding            = 2;
            samplerBindingImg.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerBindingImg.descriptorCount    = (uint32_t)(imgs.empty() ? 1 : imgs.size());
            samplerBindingImg.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

            std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboBinding, samplerBinding, samplerBindingImg };

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = (uint32_t)bindings.size();
            layoutInfo.pBindings    = bindings.data();
            vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorLayout);

            auto createGraphPipeline = [&]() {
                auto createShaderModule = [&](const std::vector<uint32_t>& code) {
                    VkShaderModuleCreateInfo ci{};
                    ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    ci.codeSize = code.size() * sizeof(uint32_t);
                    ci.pCode    = (uint32_t*)code.data();
                    VkShaderModule mod;
                    vkCreateShaderModule(device, &ci, nullptr, &mod);
                    return mod;
                };

                VkShaderModule vertMod = createShaderModule(VERT);
                VkShaderModule fragMod = createShaderModule(FRAG);

                VkPipelineShaderStageCreateInfo vertStage{};
                vertStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertStage.stage  = VK_SHADER_STAGE_VERTEX_BIT;
                vertStage.module = vertMod;
                vertStage.pName  = "main";

                VkPipelineShaderStageCreateInfo fragStage{};
                fragStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragStage.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragStage.module = fragMod;
                fragStage.pName  = "main";

                VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

                VkVertexInputBindingDescription bindingDesc{};
                bindingDesc.binding   = 0;
                bindingDesc.stride    = sizeof(Vertex);
                bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                std::array<VkVertexInputAttributeDescription, 7> attrDescs{};
                attrDescs[0] = { 0, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, pos)       };
                attrDescs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv)        };
                attrDescs[2] = { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)     };
                attrDescs[3] = { 3, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, alpha)     };
                attrDescs[4] = { 4, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, fontIndex) };
                attrDescs[5] = { 5, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, type)      };
                attrDescs[6] = { 6, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, imgIndex)  };

                VkPipelineVertexInputStateCreateInfo vertexInput{};
                vertexInput.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInput.vertexBindingDescriptionCount   = 1;
                vertexInput.pVertexBindingDescriptions      = &bindingDesc;
                vertexInput.vertexAttributeDescriptionCount = (uint32_t)attrDescs.size();
                vertexInput.pVertexAttributeDescriptions    = attrDescs.data();

                VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
                inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssembly.primitiveRestartEnable = VK_FALSE;

                VkViewport viewport{};
                viewport.x        = 0.0f;
                viewport.y        = 0.0f;
                viewport.width    = (float)extent.width;
                viewport.height   = (float)extent.height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                VkRect2D scissor{};
                scissor.offset = { 0, 0 };
                scissor.extent = extent;

                VkPipelineViewportStateCreateInfo viewportState{};
                viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportState.viewportCount = 1;
                viewportState.pViewports    = &viewport;
                viewportState.scissorCount  = 1;
                viewportState.pScissors     = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizer{};
                rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.depthClampEnable        = VK_FALSE;
                rasterizer.rasterizerDiscardEnable = VK_FALSE;
                rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
                rasterizer.cullMode                = VK_CULL_MODE_NONE;
                rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                rasterizer.lineWidth               = 1.0f;

                VkPipelineMultisampleStateCreateInfo multisampling{};
                multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling.rasterizationSamples = msaaSamples;
                multisampling.sampleShadingEnable  = VK_FALSE;

                VkPipelineDepthStencilStateCreateInfo depthStencil{};
                depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencil.depthTestEnable       = VK_FALSE;
                depthStencil.depthWriteEnable      = VK_FALSE;

                VkPipelineColorBlendAttachmentState colorBlendAttachment{};
                colorBlendAttachment.blendEnable         = VK_TRUE;
                colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
                colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;
                colorBlendAttachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                
                VkPipelineColorBlendStateCreateInfo colorBlending{};
                colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlending.logicOpEnable     = VK_FALSE;
                colorBlending.attachmentCount   = 1;
                colorBlending.pAttachments      = &colorBlendAttachment;

                VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
                pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pipelineLayoutInfo.setLayoutCount = 1;
                pipelineLayoutInfo.pSetLayouts    = &descriptorLayout;

                vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);

                VkDynamicState dynStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
                VkPipelineDynamicStateCreateInfo dynamicState{};
                dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicState.dynamicStateCount = 2;
                dynamicState.pDynamicStates    = dynStates;

                VkGraphicsPipelineCreateInfo pipelineInfo{};
                pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.stageCount          = 2;
                pipelineInfo.pStages             = stages;
                pipelineInfo.pDynamicState       = &dynamicState;
                pipelineInfo.pVertexInputState   = &vertexInput;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState      = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState   = &multisampling;
                pipelineInfo.pDepthStencilState  = &depthStencil;
                pipelineInfo.pColorBlendState    = &colorBlending;
                pipelineInfo.layout              = pipelineLayout;
                pipelineInfo.renderPass          = renderPass;
                pipelineInfo.subpass             = 0;

                vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
 
                vkDestroyShaderModule(device, vertMod, nullptr);
                vkDestroyShaderModule(device, fragMod, nullptr);
            };

            auto createDescriptorPool = [&]() {
                std::array<VkDescriptorPoolSize, 3> poolSizes{};
                poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                poolSizes[0].descriptorCount = 1;
                poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                poolSizes[1].descriptorCount = (uint32_t)(fonts.empty() ? 1 : fonts.size());
                poolSizes[2].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                poolSizes[2].descriptorCount = (uint32_t)(imgs.empty() ? 1 : imgs.size());

                VkDescriptorPoolCreateInfo dpInfo{};
                dpInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                dpInfo.poolSizeCount = (uint32_t)poolSizes.size();
                dpInfo.pPoolSizes    = poolSizes.data();
                dpInfo.maxSets       = 1;
                vkCreateDescriptorPool(device, &dpInfo, nullptr, &descriptorPool);

                VkDescriptorSetAllocateInfo dsAllocInfo{};
                dsAllocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                dsAllocInfo.descriptorPool     = descriptorPool;
                dsAllocInfo.descriptorSetCount = 1;
                dsAllocInfo.pSetLayouts        = &descriptorLayout;
                vkAllocateDescriptorSets(device, &dsAllocInfo, &descriptorSet);

                VkDescriptorBufferInfo bufInfo{};
                bufInfo.buffer = uniformBuffer;
                bufInfo.offset = 0;
                bufInfo.range  = sizeof(UBO);

                VkDescriptorImageInfo dummyInfo{};
                dummyInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                dummyInfo.imageView   = dummyImageView;
                dummyInfo.sampler     = dummySampler;

                std::vector<VkDescriptorImageInfo> fontInfos;
                if (fonts.empty()) {
                    fontInfos.push_back(dummyInfo);
                } else {
                    fontInfos.resize(fonts.size());
                    for (size_t i = 0; i < fonts.size(); i++) {
                        fontInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        fontInfos[i].imageView   = fonts[i].fontView;
                        fontInfos[i].sampler     = fonts[i].fontSampler;
                    }
                }

                std::vector<VkDescriptorImageInfo> imgInfos;
                if (imgs.empty()) {
                    imgInfos.push_back(dummyInfo);
                } else {
                    imgInfos.resize(imgs.size());
                    for (size_t i = 0; i < imgs.size(); i++) {
                        imgInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        imgInfos[i].imageView   = imgs[i].imageView;
                        imgInfos[i].sampler     = imgs[i].sampler;
                    }
                }

                std::vector<VkWriteDescriptorSet> writes;

                VkWriteDescriptorSet uboWrite{};
                uboWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                uboWrite.dstSet          = descriptorSet;
                uboWrite.dstBinding      = 0;
                uboWrite.descriptorCount = 1;
                uboWrite.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                uboWrite.pBufferInfo     = &bufInfo;
                writes.push_back(uboWrite);

                VkWriteDescriptorSet fontWrite{};
                fontWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                fontWrite.dstSet          = descriptorSet;
                fontWrite.dstBinding      = 1;
                fontWrite.dstArrayElement = 0;
                fontWrite.descriptorCount = (uint32_t)fontInfos.size();
                fontWrite.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                fontWrite.pImageInfo      = fontInfos.data();
                writes.push_back(fontWrite);

                VkWriteDescriptorSet imgWrite{};
                imgWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                imgWrite.dstSet          = descriptorSet;
                imgWrite.dstBinding      = 2;
                imgWrite.dstArrayElement = 0;
                imgWrite.descriptorCount = (uint32_t)imgInfos.size();
                imgWrite.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                imgWrite.pImageInfo      = imgInfos.data();
                writes.push_back(imgWrite);

                vkUpdateDescriptorSets(device, (uint32_t)writes.size(), writes.data(), 0, nullptr);
            };
            
            auto createOverlayPipeline = [&]() {
                auto createShaderModule = [&](const std::vector<uint32_t>& code) {
                    VkShaderModuleCreateInfo ci{};
                    ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    ci.codeSize = code.size() * sizeof(uint32_t);
                    ci.pCode    = (uint32_t*)code.data();
                    VkShaderModule mod;
                    vkCreateShaderModule(device, &ci, nullptr, &mod);
                    return mod;
                };

                if (VERT.empty() || FRAG.empty()) {
                    compileShaders();
                }

                VkShaderModule vertMod = createShaderModule(VERT);
                VkShaderModule fragMod = createShaderModule(FRAG);

                VkPipelineShaderStageCreateInfo vertStage{};
                vertStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertStage.stage  = VK_SHADER_STAGE_VERTEX_BIT;
                vertStage.module = vertMod; vertStage.pName = "main";

                VkPipelineShaderStageCreateInfo fragStage{};
                fragStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragStage.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragStage.module = fragMod; fragStage.pName = "main";

                VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

                VkVertexInputBindingDescription bindingDesc{};
                bindingDesc.binding = 0; bindingDesc.stride = sizeof(Vertex);
                bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                std::array<VkVertexInputAttributeDescription, 7> attrDescs{};
                attrDescs[0] = { 0, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, pos)       };
                attrDescs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv)        };
                attrDescs[2] = { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)     };
                attrDescs[3] = { 3, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, alpha)     };
                attrDescs[4] = { 4, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, fontIndex) };
                attrDescs[5] = { 5, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, type)      };
                attrDescs[6] = { 6, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, imgIndex)  };

                VkPipelineVertexInputStateCreateInfo vertexInput{};
                vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInput.vertexBindingDescriptionCount = 1;
                vertexInput.pVertexBindingDescriptions = &bindingDesc;
                vertexInput.vertexAttributeDescriptionCount = (uint32_t)attrDescs.size();
                vertexInput.pVertexAttributeDescriptions = attrDescs.data();

                VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
                inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

                VkViewport viewport{ 0,0,(float)extent.width,(float)extent.height,0.0f,1.0f };
                VkRect2D scissor{ {0,0}, extent };
                VkPipelineViewportStateCreateInfo viewportState{};
                viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportState.viewportCount = 1; viewportState.pViewports = &viewport;
                viewportState.scissorCount = 1; viewportState.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizer{};
                rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizer.cullMode = VK_CULL_MODE_NONE;
                rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                rasterizer.lineWidth = 1.0f;

                VkPipelineMultisampleStateCreateInfo multisampling{};
                multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

                VkPipelineColorBlendAttachmentState blendAtt{};
                blendAtt.blendEnable = VK_TRUE;
                blendAtt.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                blendAtt.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                blendAtt.colorBlendOp = VK_BLEND_OP_ADD;
                blendAtt.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                blendAtt.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                blendAtt.alphaBlendOp = VK_BLEND_OP_ADD;
                blendAtt.colorWriteMask = VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;

                VkPipelineColorBlendStateCreateInfo colorBlending{};
                colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlending.attachmentCount = 1; colorBlending.pAttachments = &blendAtt;

                VkPipelineLayoutCreateInfo pli{};
                pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pli.setLayoutCount = 1; pli.pSetLayouts = &descriptorLayout;
                vkCreatePipelineLayout(device, &pli, nullptr, &overlayPipelineLayout);

                VkDynamicState dynStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
                VkPipelineDynamicStateCreateInfo dynamicState{};
                dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicState.dynamicStateCount = 2; dynamicState.pDynamicStates = dynStates;

                VkGraphicsPipelineCreateInfo pipelineInfo{};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.stageCount = 2; pipelineInfo.pStages = stages;
                pipelineInfo.pDynamicState = &dynamicState;
                pipelineInfo.pVertexInputState = &vertexInput;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState = &multisampling;
                pipelineInfo.pColorBlendState = &colorBlending;
                pipelineInfo.layout = overlayPipelineLayout;
                pipelineInfo.renderPass = renderPassOverlay;
                pipelineInfo.subpass = 0;

                vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &overlayPipeline);

                vkDestroyShaderModule(device, vertMod, nullptr);
                vkDestroyShaderModule(device, fragMod, nullptr);
            };

            createGraphPipeline();
            createDescriptorPool();
            if (renderPassOverlay != VK_NULL_HANDLE) {
                createOverlayPipeline();
            }
        }
    
        VkSampleCountFlagBits getMaxUsableSampleCount() {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physDevice, &props);

            VkSampleCountFlags counts = props.limits.framebufferColorSampleCounts;
            if (counts & VK_SAMPLE_COUNT_8_BIT)  return VK_SAMPLE_COUNT_8_BIT;
            if (counts & VK_SAMPLE_COUNT_4_BIT)  return VK_SAMPLE_COUNT_4_BIT;
            if (counts & VK_SAMPLE_COUNT_2_BIT)  return VK_SAMPLE_COUNT_2_BIT;
            return VK_SAMPLE_COUNT_1_BIT;
        }
    
        void createDummyTexture(VkImage& image, VkImageView& imageView,
                         VkDeviceMemory& memory, VkSampler& sampler) {
            auto beginSingleTimeCommands = [&]() {
                VkCommandBufferAllocateInfo allocInfo{};
                allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandPool        = cmdPool;
                allocInfo.commandBufferCount = 1;

                VkCommandBuffer cmd;
                vkAllocateCommandBuffers(device, &allocInfo, &cmd);

                VkCommandBufferBeginInfo beginInfo{};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

                vkBeginCommandBuffer(cmd, &beginInfo);
                return cmd;
            };

            auto endSingleTimeCommands = [&](VkCommandBuffer cmd) {
                vkEndCommandBuffer(cmd);

                VkSubmitInfo submitInfo{};
                submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers    = &cmd;

                vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
                vkQueueWaitIdle(graphicsQueue);

                vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
            };

            uint8_t pixel[4] = {255, 255, 255, 255};

            VkDeviceSize imageSize = sizeof(pixel);

            VkBuffer stagingBuffer;
            VkDeviceMemory stagingMemory;
            createBuffer(device, physDevice, imageSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingMemory);

            void* data;
            vkMapMemory(device, stagingMemory, 0, imageSize, 0, &data);
            memcpy(data, pixel, imageSize);
            vkUnmapMemory(device, stagingMemory);

            VkImageCreateInfo imageInfo{};
            imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType     = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width  = 1;
            imageInfo.extent.height = 1;
            imageInfo.extent.depth  = 1;
            imageInfo.mipLevels     = 1;
            imageInfo.arrayLayers   = 1;
            imageInfo.format        = VK_FORMAT_R8G8B8A8_UNORM;
            imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

            vkCreateImage(device, &imageInfo, nullptr, &image);

            VkMemoryRequirements memReq;
            vkGetImageMemoryRequirements(device, image, &memReq);

            VkPhysicalDeviceMemoryProperties memProps;
            vkGetPhysicalDeviceMemoryProperties(physDevice, &memProps);

            uint32_t memTypeIndex = UINT32_MAX;

            for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
                if ((memReq.memoryTypeBits & (1 << i)) &&
                    (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
                    memTypeIndex = i;
                    break;
                }
            }

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize  = memReq.size;
            allocInfo.memoryTypeIndex = memTypeIndex;

            vkAllocateMemory(device, &allocInfo, nullptr, &memory);
            vkBindImageMemory(device, image, memory, 0);

            VkCommandBuffer cmd = beginSingleTimeCommands();

            VkImageMemoryBarrier barrier{};
            barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image               = image;
            barrier.subresourceRange    = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            barrier.srcAccessMask       = 0;
            barrier.dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            VkBufferImageCopy region{};
            region.bufferOffset      = 0;
            region.bufferRowLength   = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource  = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
            region.imageOffset       = {0, 0, 0};
            region.imageExtent       = {1, 1, 1};

            vkCmdCopyBufferToImage(cmd, stagingBuffer, image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            endSingleTimeCommands(cmd);

            vkDestroyBuffer(device, stagingBuffer, nullptr);
            vkFreeMemory(device, stagingMemory, nullptr);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image                           = image;
            viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format                          = VK_FORMAT_R8G8B8A8_UNORM;
            viewInfo.subresourceRange                = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

            vkCreateImageView(device, &viewInfo, nullptr, &imageView);

            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType         = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter     = VK_FILTER_NEAREST;
            samplerInfo.minFilter     = VK_FILTER_NEAREST;
            samplerInfo.addressModeU  = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.addressModeV  = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.addressModeW  = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.maxAnisotropy = 1.0f;
            samplerInfo.borderColor   = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

            vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
        }
    
        void createVideoTexture(Video& v, int w, int h) {
            VideoTexture tex;
            tex.width = w; tex.height = h;

            VkDeviceSize size = (VkDeviceSize)w * h * 4;

            createBuffer(device, physDevice, size,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                tex.stagingBuf, tex.stagingMem);
            vkMapMemory(device, tex.stagingMem, 0, size, 0, &tex.stagingMapped);

            VkImageCreateInfo imgInfo{};
            imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imgInfo.imageType = VK_IMAGE_TYPE_2D;
            imgInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            imgInfo.extent = {(uint32_t)w, (uint32_t)h, 1};
            imgInfo.mipLevels = 1;
            imgInfo.arrayLayers = 1;
            imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imgInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkCreateImage(device, &imgInfo, nullptr, &tex.image);

            VkMemoryRequirements memReq;
            vkGetImageMemoryRequirements(device, tex.image, &memReq);
            VkMemoryAllocateInfo ai{};
            ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            ai.allocationSize = memReq.size;
            ai.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physDevice);
            vkAllocateMemory(device, &ai, nullptr, &tex.memory);
            vkBindImageMemory(device, tex.image, tex.memory, 0);

            {
                VkCommandBufferAllocateInfo cbai{};
                cbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                cbai.commandPool = cmdPool;
                cbai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                cbai.commandBufferCount = 1;
                VkCommandBuffer cmd;
                vkAllocateCommandBuffers(device, &cbai, &cmd);
                VkCommandBufferBeginInfo bi{};
                bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                vkBeginCommandBuffer(cmd, &bi);

                VkImageMemoryBarrier barrier{};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = tex.image;
                barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

                vkEndCommandBuffer(cmd);
                VkSubmitInfo si{};
                si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                si.commandBufferCount = 1;
                si.pCommandBuffers = &cmd;
                vkQueueSubmit(graphicsQueue, 1, &si, VK_NULL_HANDLE);
                vkQueueWaitIdle(graphicsQueue);
                vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
            }

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = tex.image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            viewInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            vkCreateImageView(device, &viewInfo, nullptr, &tex.view);

            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            vkCreateSampler(device, &samplerInfo, nullptr, &tex.sampler);

            Image img;
            img.path = "__video_" + v.name; 
            img.w = w; img.h = h;
            img.textureImage = tex.image;
            img.textureMem = tex.memory;
            img.imageView = tex.view;
            img.sampler = tex.sampler;
            imgs.push_back(img);
            tex.imgIndex = (int)imgs.size() - 1;

            rebuildFontDependentResources(); 

            videoTextures[v.id] = tex;
        }
    
        void updateVideoTexture(VideoTexture& tex, const uint8_t* rgba_data, size_t size) {
            memcpy(tex.stagingMapped, rgba_data, size);

            VkCommandBufferAllocateInfo cbai{};
            cbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            cbai.commandPool = cmdPool;
            cbai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            cbai.commandBufferCount = 1;
            VkCommandBuffer cmd;
            vkAllocateCommandBuffers(device, &cbai, &cmd);
            VkCommandBufferBeginInfo bi{};
            bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vkBeginCommandBuffer(cmd, &bi);

            VkImageMemoryBarrier toDst{};
            toDst.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toDst.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            toDst.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            toDst.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toDst.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toDst.image = tex.image;
            toDst.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            toDst.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            toDst.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toDst);

            VkBufferImageCopy region{};
            region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
            region.imageExtent = {(uint32_t)tex.width, (uint32_t)tex.height, 1};
            vkCmdCopyBufferToImage(cmd, tex.stagingBuf, tex.image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VkImageMemoryBarrier toRead{};
            toRead.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toRead.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            toRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            toRead.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toRead.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toRead.image = tex.image;
            toRead.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            toRead.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            toRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toRead);

            vkEndCommandBuffer(cmd);
            VkSubmitInfo si{};
            si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            si.commandBufferCount = 1;
            si.pCommandBuffers = &cmd;
            vkQueueSubmit(graphicsQueue, 1, &si, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphicsQueue);
            vkFreeCommandBuffers(device, cmdPool, 1, &cmd);
        }
   
        void captureRegionToCanvas(const char* canvasName, VkImage srcImage, int sx, int sy, int w, int h) {
            auto it = std::find_if(imgs.begin(), imgs.end(),
                [&](Image& im){ return im.path == canvasName; });
            if (it == imgs.end()) return;

            VkDeviceSize size = (VkDeviceSize)w * h * 4;
            if (captureBufSize < size) {
                if (captureStagingBuf != VK_NULL_HANDLE) {
                    vkDestroyBuffer(device, captureStagingBuf, nullptr);
                    vkFreeMemory(device, captureStagingMem, nullptr);
                }
                createBuffer(device, physDevice, size,
                    VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    captureStagingBuf, captureStagingMem);
                captureBufSize = size;
            }

            VkCommandBufferAllocateInfo ai{};
            ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            ai.commandPool = cmdPool;
            ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            ai.commandBufferCount = 1;
            VkCommandBuffer cmd;
            vkAllocateCommandBuffers(device, &ai, &cmd);

            VkCommandBufferBeginInfo bi{};
            bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vkBeginCommandBuffer(cmd, &bi);

            VkImageMemoryBarrier toSrc{};
            toSrc.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toSrc.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            toSrc.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            toSrc.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toSrc.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toSrc.image = srcImage;
            toSrc.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            toSrc.srcAccessMask = 0;
            toSrc.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toSrc);

            VkBufferImageCopy region{};
            region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
            region.imageOffset = {sx, sy, 0};
            region.imageExtent = {(uint32_t)w, (uint32_t)h, 1};
            vkCmdCopyImageToBuffer(cmd, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                captureStagingBuf, 1, &region);

            VkImageMemoryBarrier toPresent = toSrc;
            toPresent.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            toPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            toPresent.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            toPresent.dstAccessMask = 0;
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &toPresent);

            vkEndCommandBuffer(cmd);
            VkSubmitInfo si{};
            si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            si.commandBufferCount = 1;
            si.pCommandBuffers = &cmd;
            vkQueueSubmit(graphicsQueue, 1, &si, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphicsQueue); 
            vkFreeCommandBuffers(device, cmdPool, 1, &cmd);

            void* mapped;
            vkMapMemory(device, captureStagingMem, 0, size, 0, &mapped);
            uint8_t* src = (uint8_t*)mapped;
            
            for (int i = 0; i < w * h; i++) {
                uint8_t b = src[i*4+0], g = src[i*4+1], r = src[i*4+2], a = src[i*4+3];
                it->pixels[i*4+0] = r;
                it->pixels[i*4+1] = g;
                it->pixels[i*4+2] = b;
                it->pixels[i*4+3] = 255;
            }
            vkUnmapMemory(device, captureStagingMem);
            it->dirty = true;
        }
    
        static uint32_t utf8Decode(const char*& p) {
            unsigned char c = (unsigned char)*p;
            if (c < 0x80) { p += 1; return c; }
            else if ((c & 0xE0) == 0xC0) {
                uint32_t cp = (c & 0x1F) << 6;
                cp |= (unsigned char)p[1] & 0x3F;
                p += 2; return cp;
            }
            else if ((c & 0xF0) == 0xE0) {
                uint32_t cp = (c & 0x0F) << 12;
                cp |= ((unsigned char)p[1] & 0x3F) << 6;
                cp |= (unsigned char)p[2] & 0x3F;
                p += 3; return cp;
            }
            else if ((c & 0xF8) == 0xF0) {
                uint32_t cp = (c & 0x07) << 18;
                cp |= ((unsigned char)p[1] & 0x3F) << 12;
                cp |= ((unsigned char)p[2] & 0x3F) << 6;
                cp |= (unsigned char)p[3] & 0x3F;
                p += 4; return cp;
            }
            p += 1; return c; 
        }
    public:
        SDL_Window*                     startWin = nullptr;

        bool                            started  = false;
        bool                            cleaned  = false;

        bool                            captureRequested = false;
        bool                            captureDone = false;
        bool                            presentedThisFrame = false;

        std::vector<Vertex>             vertices;
        std::vector<uint32_t>           indices;

        std::vector<DrawCall> drawCalls;
        std::vector<Vertex>   allVertices;
        std::vector<uint32_t> allIndices;

        std::unordered_map<FontKey, int, FontKeyHash> fontRefCount;
        std::unordered_map<std::string, int> imgRefCount;

        void acquireFontRef(const std::string& path, int size) {
            fontRefCount[FontKey{path, size}]++;
        }
        
        void acquireImgRef(const std::string& path) {
            imgRefCount[path]++;
        }

        void releaseFontAtlas(const std::string& path, int size) {
            FontKey key{path, size};
            auto refIt = fontRefCount.find(key);
            if (refIt == fontRefCount.end()) return;
            if (--refIt->second > 0) return;          
            fontRefCount.erase(refIt);

            auto it = fontAtlasCache.find(key);
            if (it == fontAtlasCache.end()) return;
            int idx = it->second;

            vkDeviceWaitIdle(device);

            FontAtlas& atlas = fonts[idx];
            vkDestroySampler(device, atlas.fontSampler, nullptr);
            vkDestroyImageView(device, atlas.fontView, nullptr);
            vkDestroyImage(device, atlas.fontImage, nullptr);
            vkFreeMemory(device, atlas.fontMemory, nullptr);

            fonts.erase(fonts.begin() + idx);
            fontAtlasCache.erase(it);

            for (auto& kv : fontAtlasCache)
                if (kv.second > idx) kv.second--;

            rebuildFontDependentResources();            
        }

        void releaseImage(const std::string& path) {
            auto refIt = imgRefCount.find(path);
            if (refIt == imgRefCount.end()) return;
            if (--refIt->second > 0) return;
            imgRefCount.erase(refIt);

            auto imgIt = std::find_if(imgs.begin(), imgs.end(),
                [&](Image& im){ return im.path == path; });
            if (imgIt == imgs.end()) return;

            vkDeviceWaitIdle(device);

            vkDestroySampler(device, imgIt->sampler, nullptr);
            vkDestroyImageView(device, imgIt->imageView, nullptr);
            vkDestroyImage(device, imgIt->textureImage, nullptr);
            vkFreeMemory(device, imgIt->textureMem, nullptr);

            imgs.erase(imgIt);
            rebuildFontDependentResources();
        }

        void recreateSwapchain() {
            if (device == VK_NULL_HANDLE) return;
            vkDeviceWaitIdle(device);

            auto createSwapchain = [&]() {
                int w, h;
                SDL_GetWindowSize(startWin, &w, &h);

                VkSurfaceCapabilitiesKHR surfCaps;
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &surfCaps);

                uint32_t formatCount = 0;
                vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);
                std::vector<VkSurfaceFormatKHR> surfFormats(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, surfFormats.data());

                chosenFormat = surfFormats[0];
                for (auto& f : surfFormats)
                    if (f.format == VK_FORMAT_B8G8R8A8_UNORM &&
                        f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                        chosenFormat = f;

                uint32_t presModeCount = 0;
                vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presModeCount, nullptr);
                std::vector<VkPresentModeKHR> surfPresModes(presModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presModeCount, surfPresModes.data());

                VkPresentModeKHR chosenMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
                for (auto& m : surfPresModes)
                    if (m == VK_PRESENT_MODE_FIFO_KHR)
                        chosenMode = m;

                if (surfCaps.currentExtent.width != UINT32_MAX) {
                    extent = surfCaps.currentExtent;
                } else {
                    extent.width  = std::clamp((uint32_t)w, surfCaps.minImageExtent.width,  surfCaps.maxImageExtent.width);
                    extent.height = std::clamp((uint32_t)h, surfCaps.minImageExtent.height, surfCaps.maxImageExtent.height);
                }

                VkSwapchainCreateInfoKHR sci{};
                sci.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                sci.surface          = surface;
                sci.minImageCount    = surfCaps.minImageCount + 1;
                sci.imageFormat      = chosenFormat.format;
                sci.imageColorSpace  = chosenFormat.colorSpace;
                sci.imageExtent      = extent;
                sci.imageArrayLayers = 1;
                sci.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
                sci.presentMode      = chosenMode;
                sci.preTransform     = surfCaps.currentTransform;
                sci.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                sci.clipped          = VK_TRUE;

                vkCreateSwapchainKHR(device, &sci, nullptr, &swapchain);
            };

            auto createImageViews = [&]() {
                uint32_t imgCount = 0;
                vkGetSwapchainImagesKHR(device, swapchain, &imgCount, nullptr);
                std::vector<VkImage> images(imgCount);
                vkGetSwapchainImagesKHR(device, swapchain, &imgCount, images.data());

                swapchainImages = images;
                imageViews.resize(imgCount);

                for (int i = 0; i < imgCount; i++) {
                    VkImageViewCreateInfo vi{};
                    vi.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    vi.image                           = images[i];
                    vi.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
                    vi.format                          = chosenFormat.format;
                    vi.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
                    vi.subresourceRange.levelCount     = 1;
                    vi.subresourceRange.layerCount     = 1;

                    vkCreateImageView(device, &vi, nullptr, &imageViews[i]);
                }
            };

            auto createColorResources = [&]() {
                VkImageCreateInfo imageInfo{};
                imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
                imageInfo.imageType = VK_IMAGE_TYPE_2D;
                imageInfo.extent = { extent.width, extent.height, 1 };
                imageInfo.mipLevels = 1;
                imageInfo.arrayLayers = 1;
                imageInfo.format = chosenFormat.format;
                imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
                imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                imageInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                imageInfo.samples = msaaSamples;
                imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
                vkCreateImage(device, &imageInfo, nullptr, &colorImageMSAA);

                VkMemoryRequirements memReqs;
                vkGetImageMemoryRequirements(device, colorImageMSAA, &memReqs);
                VkMemoryAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                allocInfo.allocationSize = memReqs.size;
                allocInfo.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physDevice);
                vkAllocateMemory(device, &allocInfo, nullptr, &colorImageMemoryMSAA);
                vkBindImageMemory(device, colorImageMSAA, colorImageMemoryMSAA, 0);

                VkImageViewCreateInfo viewInfo{};
                viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                viewInfo.image = colorImageMSAA;
                viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                viewInfo.format = chosenFormat.format;
                viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
                vkCreateImageView(device, &viewInfo, nullptr, &colorImageViewMSAA);
            };

            auto createFrameBuffer = [&]() {
                framebuffers.resize(imageViews.size());

                for (size_t i = 0; i < imageViews.size(); i++) {
                    std::array<VkImageView, 2> attachments = {
                        colorImageViewMSAA,
                        imageViews[i]       
                    };

                    VkFramebufferCreateInfo fi{};
                    fi.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    fi.renderPass      = renderPass;
                    fi.attachmentCount = (uint32_t)attachments.size();
                    fi.pAttachments    = attachments.data();
                    fi.width           = extent.width;
                    fi.height          = extent.height;
                    fi.layers          = 1;

                    vkCreateFramebuffer(device, &fi, nullptr, &framebuffers[i]);
                }
            };

            if (colorImageViewMSAA != VK_NULL_HANDLE) vkDestroyImageView(device, colorImageViewMSAA, nullptr);
            if (colorImageMSAA != VK_NULL_HANDLE)     vkDestroyImage(device, colorImageMSAA, nullptr);
            if (colorImageMemoryMSAA != VK_NULL_HANDLE) vkFreeMemory(device, colorImageMemoryMSAA, nullptr);

            for (auto fb : framebuffers)
                if (fb != VK_NULL_HANDLE) vkDestroyFramebuffer(device, fb, nullptr);
            for (auto iv : imageViews)
                if (iv != VK_NULL_HANDLE) vkDestroyImageView(device, iv, nullptr);
            vkDestroySwapchainKHR(device, swapchain, nullptr);

            createSwapchain();
            createImageViews();
            createColorResources();
            createFrameBuffer();
        }
    
        void start() {
            auto createInstance = [&]() {
                uint32_t sdlExtCount = 0;
                SDL_Vulkan_GetInstanceExtensions(startWin, &sdlExtCount, nullptr);
                std::vector<const char*> sdlExts(sdlExtCount);
                SDL_Vulkan_GetInstanceExtensions(startWin, &sdlExtCount, sdlExts.data());

                VkApplicationInfo appInfo{};
                appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                appInfo.pEngineName ="Plates Engine";
                appInfo.pApplicationName = "Plates";
                appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.apiVersion = VK_API_VERSION_1_4;

                const char* layers[] = { "VK_LAYER_KHRONOS_validation" };

                const char* extensions[] = {
                    "VK_KHR_surface",
                    "VK_KHR_wayland_surface",
                    "VK_EXT_debug_utils"
                };

                VkInstanceCreateInfo insInfo{};
                insInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                insInfo.pApplicationInfo        = &appInfo;
                insInfo.enabledExtensionCount   = (uint32_t)sdlExts.size();
                insInfo.ppEnabledExtensionNames = sdlExts.data();
                insInfo.enabledLayerCount       = 0;
                insInfo.ppEnabledLayerNames     = nullptr;
                
                vkCreateInstance(&insInfo, nullptr, &instance);
            };

            auto createSurface = [&]() {
                SDL_bool ok = SDL_Vulkan_CreateSurface(startWin, instance, &surface);
            };

            auto createPhysDevice = [&]() {
                uint32_t count = 0;
                vkEnumeratePhysicalDevices(instance, &count, nullptr);
                std::vector<VkPhysicalDevice> devices(count);
                vkEnumeratePhysicalDevices(instance, &count, devices.data());

                vkEnumeratePhysicalDevices(instance, &count, devices.data());
                
                for (auto& dev : devices) {
                    VkPhysicalDeviceProperties props;
                    vkGetPhysicalDeviceProperties(dev, &props);
                    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                        physDevice = dev;
                        break;
                    }
                }

                if (physDevice == VK_NULL_HANDLE) physDevice = devices[0];
                
                uint32_t qCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &qCount, nullptr);
                std::vector<VkQueueFamilyProperties> props(qCount);
                vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &qCount, props.data());

                for (uint32_t i = 0; i < qCount; i++) {
                    if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                        graphicsFamily = i;

                    VkBool32 present = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, surface, &present);
                    if (present) presentFamily = i;
                }

                msaaSamples = getMaxUsableSampleCount();
            };

            auto createDevice = [&]() {
                float priority = 1.0f;
                std::set<uint32_t> uniqueFamilies = { graphicsFamily, presentFamily };
                std::vector<VkDeviceQueueCreateInfo> queueInfos;

                for (uint32_t family : uniqueFamilies) {
                    VkDeviceQueueCreateInfo qi{};
                    qi.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    qi.queueFamilyIndex = family;
                    qi.queueCount       = 1;
                    qi.pQueuePriorities = &priority;
                    queueInfos.push_back(qi);
                }

                const char* devExts[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

                VkDeviceCreateInfo deviceInfo{};
                deviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                deviceInfo.queueCreateInfoCount    = (uint32_t)queueInfos.size();
                deviceInfo.pQueueCreateInfos       = queueInfos.data();
                deviceInfo.enabledExtensionCount   = 1;
                deviceInfo.ppEnabledExtensionNames = devExts;

                vkCreateDevice(physDevice, &deviceInfo, nullptr, &device);

                vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
                vkGetDeviceQueue(device, presentFamily,  0, &presentQueue);
            };

            auto createSwapchain = [&]() {
                int w, h;
                SDL_GetWindowSize(startWin, &w, &h);

                VkSurfaceCapabilitiesKHR surfCaps;
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &surfCaps);

                uint32_t formatCount = 0;
                vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);
                std::vector<VkSurfaceFormatKHR> surfFormats(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, surfFormats.data());

                chosenFormat = surfFormats[0];
                for (auto& f : surfFormats)
                    if (f.format == VK_FORMAT_B8G8R8A8_UNORM &&
                        f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                        chosenFormat = f;

                uint32_t presModeCount = 0;
                vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presModeCount, nullptr);
                std::vector<VkPresentModeKHR> surfPresModes(presModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presModeCount, surfPresModes.data());

                VkPresentModeKHR chosenMode = VK_PRESENT_MODE_FIFO_KHR;
                for (auto& m : surfPresModes)
                    if (m == VK_PRESENT_MODE_MAILBOX_KHR)
                        chosenMode = m;

                if (surfCaps.currentExtent.width != UINT32_MAX) {
                    extent = surfCaps.currentExtent;
                } else {
                    extent.width  = std::clamp((uint32_t)w, surfCaps.minImageExtent.width,  surfCaps.maxImageExtent.width);
                    extent.height = std::clamp((uint32_t)h, surfCaps.minImageExtent.height, surfCaps.maxImageExtent.height);
                }

                VkSwapchainCreateInfoKHR sci{};
                sci.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                sci.surface          = surface;
                sci.minImageCount    = surfCaps.minImageCount + 1;
                sci.imageFormat      = chosenFormat.format;
                sci.imageColorSpace  = chosenFormat.colorSpace;
                sci.imageExtent      = extent;
                sci.imageArrayLayers = 1;
                sci.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
                sci.presentMode      = chosenMode;
                sci.preTransform     = surfCaps.currentTransform;
                sci.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                sci.clipped          = VK_TRUE;

                vkCreateSwapchainKHR(device, &sci, nullptr, &swapchain);
            };

            auto createImageViews = [&]() {
                uint32_t imgCount = 0;
                vkGetSwapchainImagesKHR(device, swapchain, &imgCount, nullptr);
                std::vector<VkImage> images(imgCount);
                vkGetSwapchainImagesKHR(device, swapchain, &imgCount, images.data());

                swapchainImages = images;
                imageViews.resize(imgCount);

                for (int i = 0; i < imgCount; i++) {
                    VkImageViewCreateInfo vi{};
                    vi.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    vi.image                           = images[i];
                    vi.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
                    vi.format                          = chosenFormat.format;
                    vi.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
                    vi.subresourceRange.levelCount     = 1;
                    vi.subresourceRange.layerCount     = 1;

                    vkCreateImageView(device, &vi, nullptr, &imageViews[i]);
                }
            };

            auto createColorResources = [&]() {
                VkImageCreateInfo imageInfo{};
                imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
                imageInfo.imageType = VK_IMAGE_TYPE_2D;
                imageInfo.extent = { extent.width, extent.height, 1 };
                imageInfo.mipLevels = 1;
                imageInfo.arrayLayers = 1;
                imageInfo.format = chosenFormat.format;
                imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
                imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                imageInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                imageInfo.samples = msaaSamples;
                imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
                vkCreateImage(device, &imageInfo, nullptr, &colorImageMSAA);

                VkMemoryRequirements memReqs;
                vkGetImageMemoryRequirements(device, colorImageMSAA, &memReqs);
                VkMemoryAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                allocInfo.allocationSize = memReqs.size;
                allocInfo.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physDevice);
                vkAllocateMemory(device, &allocInfo, nullptr, &colorImageMemoryMSAA);
                vkBindImageMemory(device, colorImageMSAA, colorImageMemoryMSAA, 0);

                VkImageViewCreateInfo viewInfo{};
                viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                viewInfo.image = colorImageMSAA;
                viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                viewInfo.format = chosenFormat.format;
                viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
                vkCreateImageView(device, &viewInfo, nullptr, &colorImageViewMSAA);
            };

            auto createRenderPass = [&]() {
                VkAttachmentDescription colorAtt{};
                colorAtt.format        = chosenFormat.format;
                colorAtt.samples       = msaaSamples;                       
                colorAtt.loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR;
                colorAtt.storeOp       = VK_ATTACHMENT_STORE_OP_STORE;
                colorAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                colorAtt.finalLayout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 

                VkAttachmentDescription resolveAtt{};
                resolveAtt.format        = chosenFormat.format;
                resolveAtt.samples       = VK_SAMPLE_COUNT_1_BIT;
                resolveAtt.loadOp        = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                resolveAtt.storeOp       = VK_ATTACHMENT_STORE_OP_STORE;
                resolveAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                resolveAtt.finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  

                VkAttachmentReference colorRef{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
                VkAttachmentReference resolveRef{ 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

                VkSubpassDescription subpass{};
                subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass.colorAttachmentCount    = 1;
                subpass.pColorAttachments       = &colorRef;
                subpass.pResolveAttachments     = &resolveRef;  

                VkSubpassDependency dep{};
                dep.srcSubpass    = VK_SUBPASS_EXTERNAL;
                dep.dstSubpass    = 0;
                dep.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dep.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dep.srcAccessMask = 0;
                dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

                std::array<VkAttachmentDescription, 2> attachments = { colorAtt, resolveAtt };

                VkRenderPassCreateInfo rpInfo{};
                rpInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                rpInfo.attachmentCount = (uint32_t)attachments.size();
                rpInfo.pAttachments    = attachments.data();
                rpInfo.subpassCount    = 1;
                rpInfo.pSubpasses      = &subpass;
                rpInfo.dependencyCount = 1;
                rpInfo.pDependencies   = &dep;

                vkCreateRenderPass(device, &rpInfo, nullptr, &renderPass);
            };

            auto createFrameBuffer = [&]() {
                framebuffers.resize(imageViews.size());

                for (size_t i = 0; i < imageViews.size(); i++) {
                    std::array<VkImageView, 2> attachments = {
                        colorImageViewMSAA,
                        imageViews[i]       
                    };

                    VkFramebufferCreateInfo fi{};
                    fi.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    fi.renderPass      = renderPass;
                    fi.attachmentCount = (uint32_t)attachments.size();
                    fi.pAttachments    = attachments.data();
                    fi.width           = extent.width;
                    fi.height          = extent.height;
                    fi.layers          = 1;

                    vkCreateFramebuffer(device, &fi, nullptr, &framebuffers[i]);
                }
            };

            auto createGraphPipeline = [&]() {
                auto createShaderModule = [&](const std::vector<uint32_t>& code) {
                    VkShaderModuleCreateInfo ci{};
                    ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    ci.codeSize = code.size() * sizeof(uint32_t);
                    ci.pCode    = (uint32_t*)code.data();
                    VkShaderModule mod;
                    vkCreateShaderModule(device, &ci, nullptr, &mod);
                    return mod;
                };

                VkShaderModule vertMod = createShaderModule(VERT);
                VkShaderModule fragMod = createShaderModule(FRAG);

                VkPipelineShaderStageCreateInfo vertStage{};
                vertStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertStage.stage  = VK_SHADER_STAGE_VERTEX_BIT;
                vertStage.module = vertMod;
                vertStage.pName  = "main";

                VkPipelineShaderStageCreateInfo fragStage{};
                fragStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragStage.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragStage.module = fragMod;
                fragStage.pName  = "main";

                VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

                VkVertexInputBindingDescription bindingDesc{};
                bindingDesc.binding   = 0;
                bindingDesc.stride    = sizeof(Vertex);
                bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                std::array<VkVertexInputAttributeDescription, 7> attrDescs{};
                attrDescs[0] = { 0, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, pos)       };
                attrDescs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv)        };
                attrDescs[2] = { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)     };
                attrDescs[3] = { 3, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, alpha)     };
                attrDescs[4] = { 4, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, fontIndex) };
                attrDescs[5] = { 5, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, type)      };
                attrDescs[6] = { 6, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, imgIndex)  };

                VkPipelineVertexInputStateCreateInfo vertexInput{};
                vertexInput.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInput.vertexBindingDescriptionCount   = 1;
                vertexInput.pVertexBindingDescriptions      = &bindingDesc;
                vertexInput.vertexAttributeDescriptionCount = (uint32_t)attrDescs.size();
                vertexInput.pVertexAttributeDescriptions    = attrDescs.data();

                VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
                inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssembly.primitiveRestartEnable = VK_FALSE;

                VkViewport viewport{};
                viewport.x        = 0.0f;
                viewport.y        = 0.0f;
                viewport.width    = (float)extent.width;
                viewport.height   = (float)extent.height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                VkRect2D scissor{};
                scissor.offset = { 0, 0 };
                scissor.extent = extent;

                VkPipelineViewportStateCreateInfo viewportState{};
                viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportState.viewportCount = 1;
                viewportState.pViewports    = &viewport;
                viewportState.scissorCount  = 1;
                viewportState.pScissors     = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizer{};
                rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.depthClampEnable        = VK_FALSE;
                rasterizer.rasterizerDiscardEnable = VK_FALSE;
                rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
                rasterizer.cullMode                = VK_CULL_MODE_NONE;
                rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                rasterizer.lineWidth               = 1.0f;

                VkPipelineMultisampleStateCreateInfo multisampling{};
                multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling.rasterizationSamples = msaaSamples;
                multisampling.sampleShadingEnable  = VK_FALSE;

                VkPipelineDepthStencilStateCreateInfo depthStencil{};
                depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencil.depthTestEnable       = VK_FALSE;
                depthStencil.depthWriteEnable      = VK_FALSE;

                VkPipelineColorBlendAttachmentState colorBlendAttachment{};
                colorBlendAttachment.blendEnable         = VK_TRUE;
                colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
                colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;
                colorBlendAttachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                
                VkPipelineColorBlendStateCreateInfo colorBlending{};
                colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlending.logicOpEnable     = VK_FALSE;
                colorBlending.attachmentCount   = 1;
                colorBlending.pAttachments      = &colorBlendAttachment;

                VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
                pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pipelineLayoutInfo.setLayoutCount = 1;
                pipelineLayoutInfo.pSetLayouts    = &descriptorLayout;

                vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);

                VkDynamicState dynStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
                VkPipelineDynamicStateCreateInfo dynamicState{};
                dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicState.dynamicStateCount = 2;
                dynamicState.pDynamicStates    = dynStates;

                VkGraphicsPipelineCreateInfo pipelineInfo{};
                pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.stageCount          = 2;
                pipelineInfo.pStages             = stages;
                pipelineInfo.pDynamicState       = &dynamicState;
                pipelineInfo.pVertexInputState   = &vertexInput;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState      = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState   = &multisampling;
                pipelineInfo.pDepthStencilState  = &depthStencil;
                pipelineInfo.pColorBlendState    = &colorBlending;
                pipelineInfo.layout              = pipelineLayout;
                pipelineInfo.renderPass          = renderPass;
                pipelineInfo.subpass             = 0;

                vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
 
                vkDestroyShaderModule(device, vertMod, nullptr);
                vkDestroyShaderModule(device, fragMod, nullptr);
            };

            auto createCmdPool = [&]() {
                VkCommandPoolCreateInfo poolInfo{};
                poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                poolInfo.queueFamilyIndex = graphicsFamily;
                poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
                vkCreateCommandPool(device, &poolInfo, nullptr, &cmdPool);

                cmdBuffers.resize(framebuffers.size());
                VkCommandBufferAllocateInfo allocInfo{};
                allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.commandPool        = cmdPool;
                allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandBufferCount = (uint32_t)cmdBuffers.size();
                vkAllocateCommandBuffers(device, &allocInfo, cmdBuffers.data());
            };

            auto createVertexBuffer = [&]() {
                if (vertices.empty() || indices.empty()) return;

                vkDeviceWaitIdle(device);

                uploadBuffer(device, physDevice, cmdPool, graphicsQueue,
                    vertices.data(), vertices.size() * sizeof(Vertex),
                    vertexBuffers[0], vertexBufferMemories[0],
                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

                uploadBuffer(device, physDevice, cmdPool, graphicsQueue,
                    indices.data(), indices.size() * sizeof(uint32_t),
                    indexBuffers[0], indexBufferMemories[0],
                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

                indexCount = (uint32_t)indices.size();
            };

            auto createSyncObjects = [&]() {
                imageAvailableSems.resize(MAX_FRAMES_IN_FLIGHT);
                renderFinishedSems.resize(MAX_FRAMES_IN_FLIGHT);
                inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

                VkSemaphoreCreateInfo semInfo{};
                semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

                VkFenceCreateInfo fenceInfo{};
                fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
                fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

                for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                    vkCreateSemaphore(device, &semInfo, nullptr, &imageAvailableSems[i]);
                    vkCreateSemaphore(device, &semInfo, nullptr, &renderFinishedSems[i]);
                    vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]);
                }
            };

            auto createOverlayRenderPass = [&]() {
                VkAttachmentDescription att{};
                att.format         = chosenFormat.format;
                att.samples        = VK_SAMPLE_COUNT_1_BIT;
                att.loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;  
                att.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
                att.initialLayout  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                att.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                VkAttachmentReference ref{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

                VkSubpassDescription subpass{};
                subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass.colorAttachmentCount = 1;
                subpass.pColorAttachments    = &ref;

                VkSubpassDependency dep{};
                dep.srcSubpass    = VK_SUBPASS_EXTERNAL;
                dep.dstSubpass    = 0;
                dep.srcStageMask  = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                dep.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dep.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

                VkRenderPassCreateInfo rpInfo{};
                rpInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                rpInfo.attachmentCount = 1;
                rpInfo.pAttachments    = &att;
                rpInfo.subpassCount    = 1;
                rpInfo.pSubpasses      = &subpass;
                rpInfo.dependencyCount = 1;
                rpInfo.pDependencies   = &dep;

                vkCreateRenderPass(device, &rpInfo, nullptr, &renderPassOverlay);

                framebuffersOverlay.resize(imageViews.size());
                for (size_t i = 0; i < imageViews.size(); i++) {
                    VkFramebufferCreateInfo fi{};
                    fi.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    fi.renderPass      = renderPassOverlay;
                    fi.attachmentCount = 1;
                    fi.pAttachments    = &imageViews[i];
                    fi.width           = extent.width;
                    fi.height          = extent.height;
                    fi.layers          = 1;
                    vkCreateFramebuffer(device, &fi, nullptr, &framebuffersOverlay[i]);
                }
            };

            auto createOverlayPipeline = [&]() {
                auto createShaderModule = [&](const std::vector<uint32_t>& code) {
                    VkShaderModuleCreateInfo ci{};
                    ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    ci.codeSize = code.size() * sizeof(uint32_t);
                    ci.pCode    = (uint32_t*)code.data();
                    VkShaderModule mod;
                    vkCreateShaderModule(device, &ci, nullptr, &mod);
                    return mod;
                };

                if (VERT.empty() || FRAG.empty()) {
                    compileShaders();
                }

                VkShaderModule vertMod = createShaderModule(VERT);
                VkShaderModule fragMod = createShaderModule(FRAG);

                VkPipelineShaderStageCreateInfo vertStage{};
                vertStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertStage.stage  = VK_SHADER_STAGE_VERTEX_BIT;
                vertStage.module = vertMod; vertStage.pName = "main";

                VkPipelineShaderStageCreateInfo fragStage{};
                fragStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragStage.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragStage.module = fragMod; fragStage.pName = "main";

                VkPipelineShaderStageCreateInfo stages[] = { vertStage, fragStage };

                VkVertexInputBindingDescription bindingDesc{};
                bindingDesc.binding = 0; bindingDesc.stride = sizeof(Vertex);
                bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                std::array<VkVertexInputAttributeDescription, 7> attrDescs{};
                attrDescs[0] = { 0, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, pos)       };
                attrDescs[1] = { 1, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv)        };
                attrDescs[2] = { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)     };
                attrDescs[3] = { 3, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, alpha)     };
                attrDescs[4] = { 4, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, fontIndex) };
                attrDescs[5] = { 5, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, type)      };
                attrDescs[6] = { 6, 0, VK_FORMAT_R32_SFLOAT,       offsetof(Vertex, imgIndex)  };

                VkPipelineVertexInputStateCreateInfo vertexInput{};
                vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInput.vertexBindingDescriptionCount = 1;
                vertexInput.pVertexBindingDescriptions = &bindingDesc;
                vertexInput.vertexAttributeDescriptionCount = (uint32_t)attrDescs.size();
                vertexInput.pVertexAttributeDescriptions = attrDescs.data();

                VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
                inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

                VkViewport viewport{ 0,0,(float)extent.width,(float)extent.height,0.0f,1.0f };
                VkRect2D scissor{ {0,0}, extent };
                VkPipelineViewportStateCreateInfo viewportState{};
                viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportState.viewportCount = 1; viewportState.pViewports = &viewport;
                viewportState.scissorCount = 1; viewportState.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizer{};
                rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizer.cullMode = VK_CULL_MODE_NONE;
                rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                rasterizer.lineWidth = 1.0f;

                VkPipelineMultisampleStateCreateInfo multisampling{};
                multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

                VkPipelineColorBlendAttachmentState blendAtt{};
                blendAtt.blendEnable = VK_TRUE;
                blendAtt.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                blendAtt.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                blendAtt.colorBlendOp = VK_BLEND_OP_ADD;
                blendAtt.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                blendAtt.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                blendAtt.alphaBlendOp = VK_BLEND_OP_ADD;
                blendAtt.colorWriteMask = VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT|VK_COLOR_COMPONENT_A_BIT;

                VkPipelineColorBlendStateCreateInfo colorBlending{};
                colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlending.attachmentCount = 1; colorBlending.pAttachments = &blendAtt;

                VkPipelineLayoutCreateInfo pli{};
                pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pli.setLayoutCount = 1; pli.pSetLayouts = &descriptorLayout;
                vkCreatePipelineLayout(device, &pli, nullptr, &overlayPipelineLayout);

                VkDynamicState dynStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
                VkPipelineDynamicStateCreateInfo dynamicState{};
                dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicState.dynamicStateCount = 2; dynamicState.pDynamicStates = dynStates;

                VkGraphicsPipelineCreateInfo pipelineInfo{};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.stageCount = 2; pipelineInfo.pStages = stages;
                pipelineInfo.pDynamicState = &dynamicState;
                pipelineInfo.pVertexInputState = &vertexInput;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState = &multisampling;
                pipelineInfo.pColorBlendState = &colorBlending;
                pipelineInfo.layout = overlayPipelineLayout;
                pipelineInfo.renderPass = renderPassOverlay;
                pipelineInfo.subpass = 0;

                vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &overlayPipeline);

                vkDestroyShaderModule(device, vertMod, nullptr);
                vkDestroyShaderModule(device, fragMod, nullptr);
            };

            createInstance();
            createSurface();
            createPhysDevice();
            createDevice();
            createSwapchain();
            createImageViews();
            createColorResources();
            createRenderPass();
            createFrameBuffer();
            createCmdPool();

            createDummyTexture(dummyImage, dummyImageView, dummyMemory, dummySampler);
            createVertexBuffer();
            createSyncObjects();

            rebuildFontDependentResources();

            createOverlayRenderPass();
            createOverlayPipeline();

            started = true;
        }

        void cleanAll() {
            if (device == VK_NULL_HANDLE || cleaned) return;

            vkDeviceWaitIdle(device);

            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                vkDestroySemaphore(device, imageAvailableSems[i], nullptr);
                vkDestroySemaphore(device, renderFinishedSems[i], nullptr);
                vkDestroyFence(device, inFlightFences[i], nullptr);
            }

            vkDestroyCommandPool(device, cmdPool, nullptr);

            for (auto fb : framebuffers)
                vkDestroyFramebuffer(device, fb, nullptr);

            vkDestroyPipeline(device, graphicsPipeline, nullptr);
            vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

            vkDestroyRenderPass(device, renderPass, nullptr);

            vkDestroyImageView(device, depthImageView, nullptr);
            vkDestroyImage(device, depthImage, nullptr);
            vkFreeMemory(device, depthMemory, nullptr);

            for (auto iv : imageViews)
                vkDestroyImageView(device, iv, nullptr);

            vkDestroySwapchainKHR(device, swapchain, nullptr);

            for (int i = 0; i < 2; i++) {
                if (vertexBuffers[i] != VK_NULL_HANDLE) {
                    vkUnmapMemory(device, vertexBufferMemories[i]);
                    vkDestroyBuffer(device, vertexBuffers[i], nullptr);
                    vkFreeMemory(device, vertexBufferMemories[i], nullptr);
                }
                if (indexBuffers[i] != VK_NULL_HANDLE) {
                    vkUnmapMemory(device, indexBufferMemories[i]);
                    vkDestroyBuffer(device, indexBuffers[i], nullptr);
                    vkFreeMemory(device, indexBufferMemories[i], nullptr);
                }
            }

            vkDestroyBuffer(device, uniformBuffer, nullptr);
            vkUnmapMemory(device, uniformMemory);
            vkFreeMemory(device, uniformMemory, nullptr);

            vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);
            vkDestroyDescriptorPool(device, descriptorPool, nullptr);

            vkDestroyDevice(device, nullptr);
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);

            cleaned = true;
        }

        void drawFrame() {
            if (!started || cleaned || device == VK_NULL_HANDLE) return;
            if (drawCalls.empty()) return;

            bool presentedThisFrame = true;

            auto updateUBO = [&]() {
                UBO ubo{};
                ubo.proj = ortho(0.0f, (float)extent.width, 0.0f, (float)extent.height, -1.0f, 1.0f);
                memcpy(uniformMapped, &ubo, sizeof(ubo));

                for (auto& img : imgs) {
                    if (img.dirty) {
                        updateImagePixels(img, false);
                        img.dirty = false;
                    }
                }
            };

            vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

            uint32_t imageIndex;
            VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                imageAvailableSems[currentFrame], VK_NULL_HANDLE, &imageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR) { recreateSwapchain(); return; }
            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) return;

            vkResetFences(device, 1, &inFlightFences[currentFrame]);

            VkCommandBuffer cmd = cmdBuffers[currentFrame];
            vkResetCommandBuffer(cmd, 0);

            updateUBO();

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(cmd, &beginInfo);

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {{ bg.r, bg.g, bg.b, bg.a }};

            VkRenderPassBeginInfo rpBegin{};
            rpBegin.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            rpBegin.renderPass      = renderPass;
            rpBegin.framebuffer     = framebuffers[imageIndex];
            rpBegin.renderArea      = { {0, 0}, extent };
            rpBegin.clearValueCount = 2;
            rpBegin.pClearValues    = clearValues.data();

            vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            VkViewport viewport{};
            viewport.x = 0.0f; viewport.y = 0.0f;
            viewport.width = (float)extent.width;
            viewport.height = (float)extent.height;
            viewport.minDepth = 0.0f; viewport.maxDepth = 1.0f;
            vkCmdSetViewport(cmd, 0, 1, &viewport);

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffers[currentFrame], &offset);
            vkCmdBindIndexBuffer(cmd, indexBuffers[currentFrame], 0, VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

            for (auto& dc : drawCalls) {
                vkCmdSetScissor(cmd, 0, 1, &dc.scissor);
                vkCmdDrawIndexed(cmd, dc.indexCount, 1, dc.indexOffset, 0, 0);
            }

            vkCmdEndRenderPass(cmd);
            vkEndCommandBuffer(cmd);

            VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            VkSubmitInfo submitInfo{};
            submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount   = 1;
            submitInfo.pWaitSemaphores      = &imageAvailableSems[currentFrame];
            submitInfo.pWaitDstStageMask    = &waitStage;
            submitInfo.commandBufferCount   = 1;
            submitInfo.pCommandBuffers      = &cmd;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores    = &renderFinishedSems[currentFrame];

            result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
            if (result != VK_SUCCESS) return;

            if (captureRequested) {
                vkQueueWaitIdle(graphicsQueue); 
                
                for (auto& img : imgs) {
                    if (img.editable && img.captureTarget) {
                        captureRegionToCanvas(img.path.c_str(), swapchainImages[imageIndex],
                                            img.captureSrcX, img.captureSrcY, img.w, img.h);
                    }
                }

                captureRequested = false;
                captureDone = true;
            }

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores    = &renderFinishedSems[currentFrame];
            presentInfo.swapchainCount     = 1;
            presentInfo.pSwapchains        = &swapchain;
            presentInfo.pImageIndices      = &imageIndex;

            result = vkQueuePresentKHR(presentQueue, &presentInfo);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
                recreateSwapchain();

            currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

            drawCalls.clear();
            allVertices.clear();
            allIndices.clear();
        }
                
        void drawTextV(const char* pathToFont, const char* text, int fontSize, float x, float y, Color color) {
            auto getExecutableDir = [&]() {
                #ifdef _WIN32
                    char buf[MAX_PATH];
                    GetModuleFileNameA(nullptr, buf, MAX_PATH);
                    return std::filesystem::path(buf).parent_path();
                #elif defined(__linux__)
                    char buf[PATH_MAX];
                    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
                    if (len != -1) {
                        buf[len] = '\0';
                        return std::filesystem::path(buf).parent_path();
                    }
                    return std::filesystem::current_path();
                #else
                    return std::filesystem::current_path();
                #endif
            };

            auto resolveFontPath = [&](const std::string& path) -> std::string {
                auto cacheIt = resolvedFontPathCache.find(path);
                if (cacheIt != resolvedFontPathCache.end()) {
                    return cacheIt->second;
                }

                auto tryResolve = [&](const std::string& base, std::string& outPath) {
                    if (base.find('.') == std::string::npos) {
                        if (std::filesystem::exists(base + ".ttf")) { outPath = base + ".ttf"; return true; }
                        if (std::filesystem::exists(base + ".otf")) { outPath = base + ".otf"; return true; }
                        return false;
                    }
                    if (std::filesystem::exists(base)) { outPath = base; return true; }
                    return false;
                };

                std::string finalPath;

                if (path.find('/') != std::string::npos ||
                    path.find('\\') != std::string::npos ||
                    (path.size() >= 2 && path[1] == ':')) {
                    finalPath = path;
                } else {
                    std::string result;
                    bool found = false;

                    std::filesystem::path exeDir = getExecutableDir();
                    std::string localBase = (exeDir / "Fonts" / path).string();
                    if (tryResolve(localBase, result)) { finalPath = result; found = true; }

                    if (!found) {
                        std::string fontFolder = "Fonts";
                        std::string cwdBase = fontFolder + "/" + path;
                        if (tryResolve(cwdBase, result)) { finalPath = result; found = true; }

                        if (!found) {
                            #ifdef _WIN32
                                const char* userProfile = std::getenv("USERPROFILE");
                                std::vector<std::string> systemDirs = {
                                    "C:\\Program Files (x86)\\Plates\\" + fontFolder + "\\",
                                    "C:\\Program Files\\Plates\\" + fontFolder + "\\",
                                };
                                if (userProfile) {
                                    systemDirs.push_back(std::string(userProfile) + "\\AppData\\Local\\Programs\\Plates\\" + fontFolder + "\\");
                                }
                            #elif defined(__linux__)
                                const char* home = std::getenv("HOME");
                                std::vector<std::string> systemDirs = {
                                    "/usr/include/Plates/" + fontFolder + "/",
                                    "/usr/lib/Plates/" + fontFolder + "/",
                                    "/usr/lib64/Plates/" + fontFolder + "/"
                                };
                                if (home) {
                                    systemDirs.push_back(std::string(home) + "/.local/include/Plates/" + fontFolder + "/");
                                    systemDirs.push_back(std::string(home) + "/.local/lib/Plates/" + fontFolder + "/");
                                }
                            #else
                                std::vector<std::string> systemDirs;
                            #endif
                            for (const auto& dir : systemDirs) {
                                std::string base = dir + path;
                                if (tryResolve(base, result)) { finalPath = result; found = true; break; }
                            }
                        }

                        if (!found) finalPath = cwdBase;
                    }
                }

                resolvedFontPathCache[path] = finalPath;
                return finalPath;
            };

            std::string resolvedPath = resolveFontPath(pathToFont);
            FontKey key{resolvedPath, fontSize};

            int atlasIndex;
            auto it = fontAtlasCache.find(key);

            if (it != fontAtlasCache.end()) {
                atlasIndex = it->second; 
            } else {
                std::ifstream f(resolvedPath, std::ios::binary | std::ios::ate);
                
                if (!f.is_open()) {
                    std::cerr << "ERROR: Cannot open font: " << resolvedPath << "\n";
                    return;
                }

                size_t size = f.tellg();
                f.seekg(0);
                std::vector<uint8_t> ttf(size);
                f.read((char*)ttf.data(), size);

                FontAtlas atlas;
                atlas.bitmap.resize(atlas.atlasW * atlas.atlasH);
                atlas.glyphsAscii.resize(95);
                atlas.glyphsCyr.resize(256);   

                stbtt_pack_context packCtx;
                stbtt_PackBegin(&packCtx, atlas.bitmap.data(), atlas.atlasW, atlas.atlasH, 0, 1, nullptr);
                stbtt_PackSetOversampling(&packCtx, 2, 2);

                stbtt_pack_range ranges[2] = {};
                ranges[0].font_size = (float)fontSize * 1.3f;
                ranges[0].first_unicode_codepoint_in_range = 32;
                ranges[0].num_chars = 95;
                ranges[0].chardata_for_range = atlas.glyphsAscii.data();

                ranges[1].font_size = (float)fontSize * 1.3f;
                ranges[1].first_unicode_codepoint_in_range = 0x0400;
                ranges[1].num_chars = 256;
                ranges[1].chardata_for_range = atlas.glyphsCyr.data();

                stbtt_PackFontRanges(&packCtx, ttf.data(), 0, ranges, 2);
                stbtt_PackEnd(&packCtx);

                stbtt_fontinfo fontInfo;
                stbtt_InitFont(&fontInfo, ttf.data(), stbtt_GetFontOffsetForIndex(ttf.data(), 0));
                int ascent, descent, lineGap;
                stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
                float scale = stbtt_ScaleForPixelHeight(&fontInfo, (float)fontSize);
                atlas.baselineOffset = ascent * scale;

                uploadFontAtlas(atlas);
                fonts.push_back(atlas);
                atlasIndex = (int)fonts.size() - 1;
                fontAtlasCache[key] = atlasIndex;

                rebuildFontDependentResources(); 
            }

            FontAtlas& atlas = fonts[atlasIndex];
            float curX = x, curY = y + atlas.baselineOffset + 6.f;
            float startX = x;
            float lineHeight = fontSize * 1.2f;

            const char* p = text;
            while (*p) {
                if (*p == '\n') {
                    curX = startX;
                    curY += lineHeight;
                    p += 1;
                    continue;
                }

                uint32_t cp = utf8Decode(p);

                stbtt_packedchar* charData = nullptr;
                int glyphIndex = 0;

                if (cp >= 32 && cp <= 126) {
                    charData = atlas.glyphsAscii.data();
                    glyphIndex = (int)(cp - 32);
                } else if (cp >= 0x0400 && cp <= 0x04FF) {
                    charData = atlas.glyphsCyr.data();
                    glyphIndex = (int)(cp - 0x0400);
                } else {
                    continue; 
                }

                stbtt_aligned_quad q;
                stbtt_GetPackedQuad(charData, atlas.atlasW, atlas.atlasH, glyphIndex, &curX, &curY, &q, 1);

                uint32_t base = (uint32_t)vertices.size();
                vertices.insert(vertices.end(), {
                    {{q.x0, q.y0}, {color.r/255.f, color.g/255.f, color.b/255.f}, {q.s0, q.t0}, color.a/255.f, (float)atlasIndex, 0.0f, 0.0f},
                    {{q.x1, q.y0}, {color.r/255.f, color.g/255.f, color.b/255.f}, {q.s1, q.t0}, color.a/255.f, (float)atlasIndex, 0.0f, 0.0f},
                    {{q.x1, q.y1}, {color.r/255.f, color.g/255.f, color.b/255.f}, {q.s1, q.t1}, color.a/255.f, (float)atlasIndex, 0.0f, 0.0f},
                    {{q.x0, q.y1}, {color.r/255.f, color.g/255.f, color.b/255.f}, {q.s0, q.t1}, color.a/255.f, (float)atlasIndex, 0.0f, 0.0f}
                });
                indices.insert(indices.end(), { base, base+1, base+2, base, base+2, base+3 });
            }
        }

        float measureTextV(const char* pathToFont, const char* text, int fontSize) {
            auto getExecutableDir = [&]() {
                #ifdef _WIN32
                    char buf[MAX_PATH];
                    GetModuleFileNameA(nullptr, buf, MAX_PATH);
                    return std::filesystem::path(buf).parent_path();
                #elif defined(__linux__)
                    char buf[PATH_MAX];
                    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
                    if (len != -1) {
                        buf[len] = '\0';
                        return std::filesystem::path(buf).parent_path();
                    }
                    return std::filesystem::current_path();
                #else
                    return std::filesystem::current_path();
                #endif
            };

            auto resolveFontPath = [&](const std::string& path) -> std::string {
                auto cacheIt = resolvedFontPathCache.find(path);
                if (cacheIt != resolvedFontPathCache.end()) {
                    return cacheIt->second;
                }

                auto tryResolve = [&](const std::string& base, std::string& outPath) {
                    if (base.find('.') == std::string::npos) {
                        if (std::filesystem::exists(base + ".ttf")) { outPath = base + ".ttf"; return true; }
                        if (std::filesystem::exists(base + ".otf")) { outPath = base + ".otf"; return true; }
                        return false;
                    }
                    if (std::filesystem::exists(base)) { outPath = base; return true; }
                    return false;
                };

                std::string finalPath;

                if (path.find('/') != std::string::npos ||
                    path.find('\\') != std::string::npos ||
                    (path.size() >= 2 && path[1] == ':')) {
                    finalPath = path;
                } else {
                    std::string result;
                    bool found = false;

                    std::filesystem::path exeDir = getExecutableDir();
                    std::string localBase = (exeDir / "Fonts" / path).string();
                    if (tryResolve(localBase, result)) { finalPath = result; found = true; }

                    if (!found) {
                        std::string fontFolder = "Fonts";
                        std::string cwdBase = fontFolder + "/" + path;
                        if (tryResolve(cwdBase, result)) { finalPath = result; found = true; }

                        if (!found) {
                            #ifdef _WIN32
                                const char* userProfile = std::getenv("USERPROFILE");
                                std::vector<std::string> systemDirs = {
                                    "C:\\Program Files (x86)\\Plates\\" + fontFolder + "\\",
                                    "C:\\Program Files\\Plates\\" + fontFolder + "\\",
                                };
                                if (userProfile) {
                                    systemDirs.push_back(std::string(userProfile) + "\\AppData\\Local\\Programs\\Plates\\" + fontFolder + "\\");
                                }
                            #elif defined(__linux__)
                                const char* home = std::getenv("HOME");
                                std::vector<std::string> systemDirs = {
                                    "/usr/include/Plates/" + fontFolder + "/",
                                    "/usr/lib/Plates/" + fontFolder + "/",
                                    "/usr/lib64/Plates/" + fontFolder + "/"
                                };
                                if (home) {
                                    systemDirs.push_back(std::string(home) + "/.local/include/Plates/" + fontFolder + "/");
                                    systemDirs.push_back(std::string(home) + "/.local/lib/Plates/" + fontFolder + "/");
                                }
                            #else
                                std::vector<std::string> systemDirs;
                            #endif
                            for (const auto& dir : systemDirs) {
                                std::string base = dir + path;
                                if (tryResolve(base, result)) { finalPath = result; found = true; break; }
                            }
                        }

                        if (!found) finalPath = cwdBase;
                    }
                }

                resolvedFontPathCache[path] = finalPath;
                return finalPath;
            };

            std::string resolvedPath = resolveFontPath(pathToFont);
            FontKey key{resolvedPath, fontSize};

            auto it = fontAtlasCache.find(key);
            if (it == fontAtlasCache.end()) return 0.0f;

            FontAtlas& atlas = fonts[it->second];
            float curX = 0.0f, curY = 0.0f;

            const char* p = text;
            while (*p) {
                if (*p == '\n') { p += 1; continue; }
                uint32_t cp = utf8Decode(p);

                stbtt_packedchar* charData = nullptr;
                int glyphIndex = 0;
                if (cp >= 32 && cp <= 126) {
                    charData = atlas.glyphsAscii.data();
                    glyphIndex = (int)(cp - 32);
                } else if (cp >= 0x0400 && cp <= 0x04FF) {
                    charData = atlas.glyphsCyr.data();
                    glyphIndex = (int)(cp - 0x0400);
                } else {
                    continue;
                }

                stbtt_aligned_quad q;
                stbtt_GetPackedQuad(charData, atlas.atlasW, atlas.atlasH, glyphIndex, &curX, &curY, &q, 1);
            }

            return curX;
        }

        void drawImageV(const char* pathToImage, float x, float y, int w, int h, AVFrame* frame = nullptr) {
            auto updateImageDescriptor = [&]() {
                if (imgs.empty()) return;
                std::vector<VkDescriptorImageInfo> imgInfos(imgs.size());
                for (size_t i = 0; i < imgs.size(); i++) {
                    imgInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imgInfos[i].imageView   = imgs[i].imageView;
                    imgInfos[i].sampler     = imgs[i].sampler;
                }
                VkWriteDescriptorSet imgWrite{};
                imgWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                imgWrite.dstSet          = descriptorSet;
                imgWrite.dstBinding      = 2;
                imgWrite.dstArrayElement = 0;
                imgWrite.descriptorCount = (uint32_t)imgInfos.size();
                imgWrite.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                imgWrite.pImageInfo      = imgInfos.data();
                vkUpdateDescriptorSets(device, 1, &imgWrite, 0, nullptr);
            };

            std::string key = frame ? (std::string("__avframe_") + pathToImage) : pathToImage;

            for (int i = 0; i < (int)imgs.size(); i++) {
                if (imgs[i].path == key) {
                    if (frame) {
                        if (imgs[i].pixels == nullptr) {
                            imgs[i].pixels = (unsigned char*)malloc((size_t)imgs[i].w * imgs[i].h * 4);
                        }
                        for (int row = 0; row < imgs[i].h; row++) {
                            memcpy(imgs[i].pixels + row * imgs[i].w * 4,
                                frame->data[0] + row * frame->linesize[0],
                                imgs[i].w * 4);
                        }

                        updateImagePixels(imgs[i], false);
                    } else if (imgs[i].dirty) {
                        updateImagePixels(imgs[i], false);
                        imgs[i].dirty = false;
                    }
                    float x1 = x + w, y1 = y + h;
                    uint32_t base = (uint32_t)vertices.size();
                    vertices.insert(vertices.end(), {
                        {{x,  y }, {1,1,1}, {0,0}, 1.0f, 0.0f, 1.0f, (float)i},
                        {{x1, y }, {1,1,1}, {1,0}, 1.0f, 0.0f, 1.0f, (float)i},
                        {{x1, y1}, {1,1,1}, {1,1}, 1.0f, 0.0f, 1.0f, (float)i},
                        {{x,  y1}, {1,1,1}, {0,1}, 1.0f, 0.0f, 1.0f, (float)i},
                    });
                    indices.insert(indices.end(), { base, base+1, base+2, base, base+2, base+3 });
                    return;
                }
            }

            Image img;
            img.path = key;

            if (frame) {
                img.w = frame->width;
                img.h = frame->height;
                img.ch = 4;
                img.pixels = (unsigned char*)malloc((size_t)img.w * img.h * 4);
                for (int row = 0; row < img.h; row++) {
                    memcpy(img.pixels + row * img.w * 4,
                        frame->data[0] + row * frame->linesize[0],
                        img.w * 4);
                }
            } else {
                img.pixels = stbi_load(pathToImage, &img.w, &img.h, &img.ch, 4);
                if (!img.pixels) {
                    std::cerr << "ERROR: Cannot open image: " << pathToImage << "\n";
                    return;
                }
            }

            uploadImage(img);
            imgs.push_back(img);
            int idx = (int)imgs.size() - 1; 
            rebuildFontDependentResources();

            float x0 = x, y0 = y, x1 = x + w, y1 = y + h;
            uint32_t base = (uint32_t)vertices.size();
            vertices.insert(vertices.end(), {
                {{x0, y0}, {1,1,1}, {0, 0}, 1.0f, 0.0f, 1.0f, (float)idx},
                {{x1, y0}, {1,1,1}, {1, 0}, 1.0f, 0.0f, 1.0f, (float)idx},
                {{x1, y1}, {1,1,1}, {1, 1}, 1.0f, 0.0f, 1.0f, (float)idx},
                {{x0, y1}, {1,1,1}, {0, 1}, 1.0f, 0.0f, 1.0f, (float)idx},
            });
            indices.insert(indices.end(), { base, base+1, base+2, base, base+2, base+3 });
        }
        
        void drawRectV(int x, int y, int w, int h, Color color) {
            float x_float = (float)(x / 255); 
            float y_float = (float)(y / 255); 
            float w_float = (float)(w / 255); 
            float h_float = (float)(h / 255); 

            uint32_t base = vertices.size();

            vertices.insert(vertices.end(), {
                {{x    ,   y    }, {(float)(color.r/255.0f),(float)(color.g/255.0f),(float)(color.b/255.0f)}, {}, (float)(color.a/255.0f), 0.0f, 2.0f, 0.0f},
                {{x + w,   y    }, {(float)(color.r/255.0f),(float)(color.g/255.0f),(float)(color.b/255.0f)}, {}, (float)(color.a/255.0f), 0.0f, 2.0f, 0.0f},
                {{x + w,   y + h}, {(float)(color.r/255.0f),(float)(color.g/255.0f),(float)(color.b/255.0f)}, {}, (float)(color.a/255.0f), 0.0f, 2.0f, 0.0f},
                {{x    ,   y + h}, {(float)(color.r/255.0f),(float)(color.g/255.0f),(float)(color.b/255.0f)}, {}, (float)(color.a/255.0f), 0.0f, 2.0f, 0.0f}
            });

            indices.insert(indices.end(), {
                base, base + 1, base + 2,
                base + 2, base + 3, base
            });
        }

        void drawRoundedRectV(int x, int y, int w, int h, float radius, Color color) {
            auto calcSegments = [&](float radius, float maxError = 0.5f) {
                if (radius <= 0.0f) return 1;
                float clampedError = std::min(maxError, radius);
                float theta = 2.0f * acosf(1.0f - clampedError / radius);
                int segments = (int)ceilf((3.14159265f / 2.0f) / theta);
                return std::clamp(segments, 2, 32);
            };
            
            float r = std::min(radius, std::min((float)w, (float)h) / 2.0f);

            int segments = calcSegments(r, 0.5f);
            float col_r = color.r / 255.0f;
            float col_g = color.g / 255.0f;
            float col_b = color.b / 255.0f;
            float col_a = color.a / 255.0f;

            auto vcol = [&](float px, float py) {
                return Vertex{ {px, py}, {col_r, col_g, col_b}, {}, col_a, 0.0f, 2.0f, 0.0f };
            };

            struct Corner { float cx, cy, startAngle; };
            
            Corner corners[4] = {
                { x + w - r, y + r,     -90.0f },
                { x + w - r, y + h - r,   0.0f }, 
                { x + r,     y + h - r,  90.0f }, 
                { x + r,     y + r,     180.0f } 
            };

            std::vector<Vertex> boundary;
            boundary.reserve(4 * (segments + 1));

            for (auto& c : corners) {
                for (int i = 0; i <= segments; i++) {
                    float angle = (c.startAngle + (90.0f * i / segments)) * (3.14159265f / 180.0f);
                    float px = c.cx + cosf(angle) * r;
                    float py = c.cy + sinf(angle) * r;
                    boundary.push_back(vcol(px, py));
                }
            }

            uint32_t base = (uint32_t)vertices.size();

            float centerX = x + w / 2.0f;
            float centerY = y + h / 2.0f;

            vertices.push_back(vcol(centerX, centerY));
            vertices.insert(vertices.end(), boundary.begin(), boundary.end());

            uint32_t centerIdx = base;
            uint32_t count = (uint32_t)boundary.size();

            for (uint32_t i = 0; i < count; i++) {
                uint32_t curr = base + 1 + i;
                uint32_t next = base + 1 + ((i + 1) % count);
                indices.insert(indices.end(), { centerIdx, curr, next });
            }
        }

        void drawLineV(float x1, float y1, float x2, float y2, Color color) {
            float dx = x2 - x1;
            float dy = y2 - y1;
            float len = std::sqrt(dx * dx + dy * dy);
            if (len == 0.0f) return;

            float nx = -dy / len * 0.5f;
            float ny =  dx / len * 0.5f;

            uint32_t base = vertices.size();

            float col_r = color.r / 255.0f;
            float col_g = color.g / 255.0f;
            float col_b = color.b / 255.0f;
            float col_a = color.a / 255.0f;

            vertices.insert(vertices.end(), {
                {{x1 + nx, y1 + ny}, {col_r, col_g, col_b}, {}, col_a, 0.0f, 2.0f, 0.0f},
                {{x1 - nx, y1 - ny}, {col_r, col_g, col_b}, {}, col_a, 0.0f, 2.0f, 0.0f},
                {{x2 - nx, y2 - ny}, {col_r, col_g, col_b}, {}, col_a, 0.0f, 2.0f, 0.0f},
                {{x2 + nx, y2 + ny}, {col_r, col_g, col_b}, {}, col_a, 0.0f, 2.0f, 0.0f},
            });

            indices.insert(indices.end(), {
                base,     base + 1, base + 2,
                base + 2, base + 3, base
            });
        }

        void drawCircleV(float cx, float cy, float r, Color color) {   
            auto calcSegmentsFull = [&](float radius, float maxError = 0.5f) {
                if (radius <= 0.0f) return 3;
                float clampedError = std::min(maxError, radius);
                float theta = 2.0f * acosf(1.0f - clampedError / radius);
                int segments = (int)ceilf((2.0f * 3.14159265f) / theta);

                return std::clamp(segments, 8, 128);
            };

            int segments = calcSegmentsFull(r, .1f);
            
            float cx_float = (float)(cx / 255); 
            float cy_float = (float)(cy / 255);  
            
            uint32_t base = vertices.size();

            vertices.push_back({{cx, cy}, {(float)(color.r / 255.0f), (float)(color.g / 255.0f), (float)(color.b / 255.0f)}, {}, (float)(color.a / 255.0f), 0.0f, 2.0f, 0.0f});

            for (int i = 0; i <= segments; i++) {
                float a = (float)i / segments * 2.0f * M_PI;

                float x = cx + cos(a) * r;
                float y = cy + sin(a) * r;

                vertices.push_back({{x, y}, {(float)(color.r / 255.0f), (float)(color.g / 255.0f), (float)(color.b / 255.0f)}, {}, (float)(color.a / 255.0f), 0.0f, 2.0f, 0.0f});
            }

            for (int i = 1; i <= segments; i++) {
                indices.push_back(base);
                indices.push_back(base + i);
                indices.push_back(base + i + 1);
            }
        }

        void drawVideoV(Video& v, float x, float y, int w, int h) {
            if (!v.ctx) return;

            if (videoTextures.find(v.id) == videoTextures.end()) {
                createVideoTexture(v, v.width, v.height);
            }

            double now_seconds = (av_gettime() - v.playback_start) / 1000000.0;
            QueuedFrame qf;
            if (v.ctx->queue.try_pop_ready(now_seconds, qf)) {
                size_t size = (size_t)v.width * v.height * 4;
                updateVideoTexture(videoTextures[v.id], qf.frame->data[0], size);
                av_frame_free(&qf.frame);
            }

            int idx = videoTextures[v.id].imgIndex;
            uint32_t base = (uint32_t)vertices.size();
            vertices.insert(vertices.end(), {
                {{x,     y    }, {1,1,1}, {0, 0}, 1.0f, 0.0f, 1.0f, (float)idx},
                {{x + w, y    }, {1,1,1}, {1, 0}, 1.0f, 0.0f, 1.0f, (float)idx},
                {{x + w, y + h}, {1,1,1}, {1, 1}, 1.0f, 0.0f, 1.0f, (float)idx},
                {{x,     y + h}, {1,1,1}, {0, 1}, 1.0f, 0.0f, 1.0f, (float)idx},
            });
            indices.insert(indices.end(), { base, base+1, base+2, base, base+2, base+3 });
        }

        void changeBackground(Color color) {             
            bg.r = (float)(color.r) / 255.0f;             
            bg.g = (float)(color.g) / 255.0f;             
            bg.b = (float)(color.b) / 255.0f;             
            bg.a = (float)(color.a) / 255.0f;    
        }

        void setScissorV(int x, int y, int w, int h) {
            scissorRect = { {x, y}, {(uint32_t)w, (uint32_t)h} };
            useScissor = true;
        }

        void resetScissorV() {
            useScissor = false;
            scissorRect = { {0, 0}, extent };
        }

        void flush(bool scissored = false, int sx = 0, int sy = 0, int sw = 0, int sh = 0) {
            if (vertices.empty() || indices.empty()) return;

            auto createMappedBuffer = [&](VkDeviceSize size, VkBufferUsageFlags usage,
                VkBuffer& buffer, VkDeviceMemory& memory, void*& mapped) {
                createBuffer(device, physDevice, size, usage,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    buffer, memory);
                vkMapMemory(device, memory, 0, size, 0, &mapped);
            };

            int f = currentFrame;

            vkWaitForFences(device, 1, &inFlightFences[f], VK_TRUE, UINT64_MAX);

            uint32_t vertexOffset = (uint32_t)allVertices.size();
            for (auto& idx : indices)
                allIndices.push_back(idx + vertexOffset);

            allVertices.insert(allVertices.end(), vertices.begin(), vertices.end());

            DrawCall dc;
            dc.indexOffset = (uint32_t)(allIndices.size() - indices.size());
            dc.indexCount  = (uint32_t)indices.size();
            dc.scissor     = scissored
                ? VkRect2D{ {sx, sy}, {(uint32_t)sw, (uint32_t)sh} }
                : VkRect2D{ {0, 0}, extent };
            drawCalls.push_back(dc);

            vertices.clear();
            indices.clear();

            VkDeviceSize vSize = allVertices.size() * sizeof(Vertex);
            VkDeviceSize iSize = allIndices.size()  * sizeof(uint32_t);

            if (vSize > vertexBufferCapacities[f]) {
                vkDeviceWaitIdle(device);
                if (vertexBuffers[f] != VK_NULL_HANDLE) {
                    vkUnmapMemory(device, vertexBufferMemories[f]);
                    destroyBuffer(device, vertexBuffers[f], vertexBufferMemories[f]);
                }
                vertexBufferCapacities[f] = vSize * 4;
                createMappedBuffer(vertexBufferCapacities[f], VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                    vertexBuffers[f], vertexBufferMemories[f], vertexMappeds[f]);
            }

            if (iSize > indexBufferCapacities[f]) {
                vkDeviceWaitIdle(device);
                if (indexBuffers[f] != VK_NULL_HANDLE) {
                    vkUnmapMemory(device, indexBufferMemories[f]);
                    destroyBuffer(device, indexBuffers[f], indexBufferMemories[f]);
                }
                indexBufferCapacities[f] = iSize * 4;
                createMappedBuffer(indexBufferCapacities[f], VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                    indexBuffers[f], indexBufferMemories[f], indexMappeds[f]);
            }

            memcpy(vertexMappeds[f], allVertices.data(), vSize);
            memcpy(indexMappeds[f],  allIndices.data(),  iSize);
        }

        void setPixelV(const char* path, int x, int y, Color color) {
            auto it = std::find_if(imgs.begin(), imgs.end(),
                [&](Image& im){ return im.path == path; });
            if (it == imgs.end()) return;
            if (x < 0 || y < 0 || x >= it->w || y >= it->h) return;

            size_t offset = (y * it->w + x) * 4;
            it->pixels[offset + 0] = color.r;
            it->pixels[offset + 1] = color.g;
            it->pixels[offset + 2] = color.b;
            it->pixels[offset + 3] = color.a;
            it->dirty = true;
        }

        Color getPixelV(const char* path, int x, int y) {
            auto it = std::find_if(imgs.begin(), imgs.end(),
                [&](Image& im){ return im.path == path; });
            if (it == imgs.end()) return {0, 0, 0, 0};
            if (x < 0 || y < 0 || x >= it->w || y >= it->h) return {0, 0, 0, 0};
            if (it->pixels == nullptr) return {0, 0, 0, 0};

            size_t offset = (y * it->w + x) * 4;
            return Color{
                it->pixels[offset + 0],
                it->pixels[offset + 1],
                it->pixels[offset + 2],
                it->pixels[offset + 3]
            };
        }

        void createEditableImageV(const char* path, int w, int h, Color fill = {0,0,0,0}) {
            for (auto& im : imgs)
                if (im.path == path) return;

            Image img;
            img.path = path;
            img.w = w;
            img.h = h;
            img.ch = 4;
            img.editable = true;
            img.pixels = (unsigned char*)malloc((size_t)w * h * 4);

            for (int i = 0; i < w * h; i++) {
                img.pixels[i*4 + 0] = fill.r;
                img.pixels[i*4 + 1] = fill.g;
                img.pixels[i*4 + 2] = fill.b;
                img.pixels[i*4 + 3] = fill.a;
            }

            uploadImage(img);
            imgs.push_back(img);

            rebuildFontDependentResources(); 

            if (!imgs.empty()) {
                std::vector<VkDescriptorImageInfo> imgInfos(imgs.size());
                
                for (size_t i = 0; i < imgs.size(); i++) {
                    imgInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imgInfos[i].imageView   = imgs[i].imageView;
                    imgInfos[i].sampler     = imgs[i].sampler;
                }

                VkWriteDescriptorSet imgWrite{};
                imgWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                imgWrite.dstSet          = descriptorSet;
                imgWrite.dstBinding      = 2;
                imgWrite.dstArrayElement = 0;
                imgWrite.descriptorCount = (uint32_t)imgInfos.size();
                imgWrite.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                imgWrite.pImageInfo      = imgInfos.data();
                vkUpdateDescriptorSets(device, 1, &imgWrite, 0, nullptr);
            }
        }

        bool renderSceneAndCapture(int captureX, int captureY, int w, int h,
                                std::vector<Color>& outPixels, uint32_t& imageIndexOut) {
            if (!started || cleaned || drawCalls.empty()) return false;

            UBO ubo{};
            ubo.proj = ortho(0.0f, (float)extent.width, 0.0f, (float)extent.height, -1.0f, 1.0f);
            memcpy(uniformMapped, &ubo, sizeof(ubo));
            for (auto& img : imgs) { if (img.dirty) { updateImagePixels(img, false); img.dirty = false; } }

            vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

            uint32_t imageIndex;
            VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                imageAvailableSems[currentFrame], VK_NULL_HANDLE, &imageIndex);
            if (result == VK_ERROR_OUT_OF_DATE_KHR) { recreateSwapchain(); return false; }
            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) return false;

            vkResetFences(device, 1, &inFlightFences[currentFrame]);

            VkCommandBuffer cmd = cmdBuffers[currentFrame];
            vkResetCommandBuffer(cmd, 0);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(cmd, &beginInfo);

            std::array<VkClearValue,2> clearValues{};
            clearValues[0].color = {{ bg.r, bg.g, bg.b, bg.a }};

            VkRenderPassBeginInfo rpBegin{};
            rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            rpBegin.renderPass = renderPass;
            rpBegin.framebuffer = framebuffers[imageIndex];
            rpBegin.renderArea = { {0,0}, extent };
            rpBegin.clearValueCount = 2;
            rpBegin.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            VkViewport viewport{ 0,0,(float)extent.width,(float)extent.height,0.0f,1.0f };
            vkCmdSetViewport(cmd, 0, 1, &viewport);

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffers[currentFrame], &offset);
            vkCmdBindIndexBuffer(cmd, indexBuffers[currentFrame], 0, VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

            for (auto& dc : drawCalls) {
                vkCmdSetScissor(cmd, 0, 1, &dc.scissor);
                vkCmdDrawIndexed(cmd, dc.indexCount, 1, dc.indexOffset, 0, 0);
            }
            vkCmdEndRenderPass(cmd);

            vkEndCommandBuffer(cmd);

            VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &imageAvailableSems[currentFrame];
            submitInfo.pWaitDstStageMask = &waitStage;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &cmd;

            vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
            vkQueueWaitIdle(graphicsQueue);

            VkDeviceSize size = (VkDeviceSize)w * h * 4;
            if (captureBufSize < size) {
                if (captureStagingBuf != VK_NULL_HANDLE) {
                    vkDestroyBuffer(device, captureStagingBuf, nullptr);
                    vkFreeMemory(device, captureStagingMem, nullptr);
                }
                createBuffer(device, physDevice, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    captureStagingBuf, captureStagingMem);
                captureBufSize = size;
            }

            VkCommandBufferAllocateInfo ai{};
            ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            ai.commandPool = cmdPool; ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; ai.commandBufferCount = 1;
            VkCommandBuffer readCmd;
            vkAllocateCommandBuffers(device, &ai, &readCmd);

            VkCommandBufferBeginInfo rbi{};
            rbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            rbi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vkBeginCommandBuffer(readCmd, &rbi);

            VkImageMemoryBarrier toSrc{};
            toSrc.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toSrc.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            toSrc.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            toSrc.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toSrc.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toSrc.image = swapchainImages[imageIndex];
            toSrc.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1};
            toSrc.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            vkCmdPipelineBarrier(readCmd, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,0,0,nullptr,0,nullptr,1,&toSrc);

            VkBufferImageCopy region{};
            region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT,0,0,1};
            region.imageOffset = {captureX, captureY, 0};
            region.imageExtent = {(uint32_t)w,(uint32_t)h,1};
            vkCmdCopyImageToBuffer(readCmd, swapchainImages[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                captureStagingBuf, 1, &region);

            VkImageMemoryBarrier backToPresent = toSrc;
            backToPresent.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            backToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            backToPresent.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            backToPresent.dstAccessMask = 0;
            vkCmdPipelineBarrier(readCmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,0,0,nullptr,0,nullptr,1,&backToPresent);

            vkEndCommandBuffer(readCmd);
            VkSubmitInfo si{};
            si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            si.commandBufferCount = 1; si.pCommandBuffers = &readCmd;
            vkQueueSubmit(graphicsQueue, 1, &si, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphicsQueue);
            vkFreeCommandBuffers(device, cmdPool, 1, &readCmd);

            void* mapped;
            vkMapMemory(device, captureStagingMem, 0, size, 0, &mapped);
            uint8_t* src = (uint8_t*)mapped;
            outPixels.resize(w * h);
            for (int i = 0; i < w*h; i++) {
                outPixels[i] = { src[i*4+2], src[i*4+1], src[i*4+0], 255 };
            }
            vkUnmapMemory(device, captureStagingMem);

            drawCalls.clear(); allVertices.clear(); allIndices.clear();
            vertices.clear(); indices.clear();

            imageIndexOut = imageIndex;
            return true;
        }

        void renderOverlayAndPresent(uint32_t imageIndex) {
            presentedThisFrame = true;
            if (vertices.empty()) {
                VkPresentInfoKHR presentInfo{};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = &swapchain;
                presentInfo.pImageIndices = &imageIndex;
                vkQueuePresentKHR(presentQueue, &presentInfo);
                currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
                return;
            }

            uint32_t vertexOffset = 0;
            std::vector<Vertex> ov = vertices;
            std::vector<uint32_t> oi = indices;
            for (auto& idx : oi) idx += vertexOffset;

            flush(); 

            VkCommandBuffer cmd = cmdBuffers[currentFrame];
            vkResetCommandBuffer(cmd, 0);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(cmd, &beginInfo);

            VkRenderPassBeginInfo rpBegin{};
            rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            rpBegin.renderPass = renderPassOverlay;
            rpBegin.framebuffer = framebuffersOverlay[imageIndex];
            rpBegin.renderArea = { {0,0}, extent };
            rpBegin.clearValueCount = 0;

            vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, overlayPipeline);

            VkViewport viewport{ 0,0,(float)extent.width,(float)extent.height,0.0f,1.0f };
            vkCmdSetViewport(cmd, 0, 1, &viewport);

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffers[currentFrame], &offset);
            vkCmdBindIndexBuffer(cmd, indexBuffers[currentFrame], 0, VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                overlayPipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

            for (auto& dc : drawCalls) {
                vkCmdSetScissor(cmd, 0, 1, &dc.scissor);
                vkCmdDrawIndexed(cmd, dc.indexCount, 1, dc.indexOffset, 0, 0);
            }
            vkCmdEndRenderPass(cmd);
            vkEndCommandBuffer(cmd);

            VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &cmd;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &renderFinishedSems[currentFrame];

            vkResetFences(device, 1, &inFlightFences[currentFrame]);
            vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &renderFinishedSems[currentFrame];
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain;
            presentInfo.pImageIndices = &imageIndex;
            vkQueuePresentKHR(presentQueue, &presentInfo);

            currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

            drawCalls.clear(); allVertices.clear(); allIndices.clear();
        }

        void markCanvasForCapture(const char* canvasName, int srcX, int srcY) {
            for (auto& im : imgs) {
                if (im.path == canvasName) {
                    im.captureTarget = true;
                    im.captureSrcX = srcX;
                    im.captureSrcY = srcY;
                    return;
                }
            }
        }

        void unmarkCanvasForCapture(const char* canvasName) {
            for (auto& im : imgs) {
                if (im.path == canvasName) {
                    im.captureTarget = false;
                    return;
                }
            }
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
 * @struct Window
 * @brief Holds SDL window and renderer along with basic properties
 */
struct Window {
    SDL_Window* win;        ///< SDL window handle
    SDL_Surface* surface;   ///< SDL Surface
    SDL_Renderer* renderer; ///< SDL renderer handle
    std::string name;       ///< Unique name of the window
    SDL_Rect rect;          ///< Current window rectangle
    bool isVulkan;          ///< Check Vulkan compability
    Engine eng;             ///< Vulkan Engine

    struct SDLCanvas {
        std::string name;
        int w, h;
        std::vector<uint8_t> pixels;
        SDL_Texture* tex = nullptr;
        bool dirty = false;
    };

    std::vector<SDLCanvas> sdlCanvases;
};

struct Renderer {
    SDL_Renderer* renderer;
};

/**
 * @struct Box
 * @brief Represents a rectangular UI element
 */
struct Box {
    Rect rect; Color color; std::string name; Window* win; int rad = 0;
    
    bool ownDraw = false;
    bool Visible = true;

    SDL_Texture* aaCache = nullptr;
    bool aaDirty = true;
};

/**
 * @struct Text
 * @brief Represents a text label rendered with SDL_ttf
 */
struct Text {
    Rect rect; Color color;
    std::string pathToFont; int fontSize;
    std::string text; std::string name; Window* win;
    TTF_Font*    font = nullptr;
    SDL_Surface* surf = nullptr;
    SDL_Texture* tex  = nullptr;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct Image
 * @brief Represents an image rendered with SDL_image
 */
struct Image {
    Rect rect; std::string pathToImage;
    SDL_Texture* tex = nullptr;
    std::string name; Window* win;
    AVFrame* frame = nullptr;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct Circle
 * @brief Represents a circle UI element
 */
struct Circle {
    Square uRect; Color clr; std::string name; Window* win;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct InputBox
 * @brief Text input field with cursor and selection support
 */
struct InputBox {
    Rect rect; Color clr; std::string name;
    std::string hint, text; Window* win;
    int  caret = 0; bool focus = false;
    TTF_Font*    font = nullptr; int fontSize;
    std::string  buf, pathToFont;
    std::vector<int> textWidth;
    bool changed = false; int animTime = 0; bool animBack = false;
    int selectStart = -1, selectEnd = -1;
    bool debuggingBox = false;
    bool dragging = false;
    int dragAnchor = 0;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct ScrollBar
 * @brief Scrollbar UI component
 */
struct ScrollBar {
    Rect rect; Color color; std::string name; Window* win;
    bool debuggingBox = false;
    float scrollingSize = 1.0f, pos = 0.0f;
    std::vector<std::string> elements;
    int rad = 0;
    float sliderSize = 1.0f;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct Slider
 * @brief Simple progress slider
 */
struct Slider {
    Rect rect; Color clrOff, clrOn; float progress;
    std::string name; Window* win;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct RangeSlider
 * @brief Slider with two handles (range selection)
 */
struct RangeSlider {
    Rect rect; Color clrOff, clrOn;
    float progress, progress2;
    std::string name; Window* win;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct CheckBox
 * @brief Checkbox UI element with animation support
 */
struct CheckBox {
    Square uRect; Color clr; std::string name;
    bool isOn, isAnim; Window* win;
    float animProgress = 0.0f;
    float animDuration = .05f;

    SDL_Texture* aaTex4x = nullptr;
    SDL_Texture* aaTex2x = nullptr;
    SDL_Texture* aaTex1x = nullptr;
    int aaTexSize = -1;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

struct RadioButton {
    Square uRect;
    bool status = false, animPending = false;
    Color clrOff, clrOn;
    int   group;
    Window* win = nullptr;
    std::string name;
    float animProgress = 0.0f;
    float animDuration = 0.05f;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

/**
 * @struct Line
 * @brief Straight line for drawing
 */
struct Line {
    Rect cords; Color color; std::string name; Window* win;

    bool ownDraw = false;
    bool Visible = true;

    uint32_t baseVertex;
    uint32_t baseIndex;
};

struct Canvas {
    int w, h; const char* name; Window* win;

    bool Visible = true;
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
    Box,
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
    std::vector<UIElement>      uiElements;
    std::vector<Window*>                         wins;
    std::vector<Renderer*>                       renderers;
    std::vector<Sound>                           sounds;
    std::vector<Video>                           videos;
    std::vector<Surface*>                        surfaces;
    std::vector<std::tuple<Color, Window*>>      backgrounds;
    std::vector<Canvas>                          canvases;
    std::vector<std::string>                     names;

    std::string                                  RatioOnNow = "";
    bool                                         running    = true;
    ma_engine                                    engine;
    uint64_t                                     Event      = 0;
    float                                        deltaTime  = 0.0f;
};

inline Space mainSpace;

// ==================== Function Declarations ====================

/**
 * @brief Locates and returns a pointer to a UI element of type T by its name.
 *
 * Performs a type-safe search among registered elements and returns a pointer
 * to the matching instance if found.
 *
 * @tparam T The expected element type.
 * @param name Unique identifier of the element to locate.
 * @return Pointer to the found element of type T, or nullptr if no match exists.
 */
template<typename T>
T* findElement(const std::string& name) {
    for (auto& el : mainSpace.uiElements) {
        if (std::holds_alternative<T>(el) && std::get<T>(el).name == name)
            return &std::get<T>(el);
    }
    return nullptr;
}

/**
 * @brief Renders a rectangular box element.
 *
 * @param box Reference to the Box instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawBox(Box& box, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders a circular element.
 *
 * @param cir Reference to the Circle instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawCircle(Circle& cir, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders a text element.
 *
 * @param txt Reference to the Text instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawText(Text& txt, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders an image element.
 *
 * @param img Reference to the Image instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawImage(Image& img, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders a line element.
 *
 * @param ln Reference to the Line instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawLine(Line& ln, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders an input box element.
 *
 * @param box Reference to the InputBox instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawInputBox(InputBox& box, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders a slider element.
 *
 * @param s Reference to the Slider instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawSlider(Slider& s, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Renders a range slider element.
 *
 * @param s Reference to the RangeSlider instance to draw.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawRangeSlider(RangeSlider& s, int offX = 0, int offY = 0);

/**
 * @brief Updates the visual state of a checkbox button.
 *
 * Synchronizes the checkbox appearance with its current logical state using the provided renderer.
 *
 * @param box Reference to the CheckBox instance to update.
 * @param renderer Pointer to the SDL_Renderer used for drawing operations.
 */
void UpdateCheckBoxButton(CheckBox& box, SDL_Renderer* renderer);

/**
 * @brief Updates the visual state of a radio button.
 *
 * @param radio Reference to the RadioButton instance to update.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 */
void UpdateRadioButton(RadioButton& radio, bool ownDraw = true);

/**
 * @brief Renders a generic UI element by dispatching to the appropriate specialized drawing function.
 *
 * @param el Reference to the UIElement instance to draw.
 * @param ownDraw If true, performs a full independent draw operation; otherwise, assumes an external rendering context.
 * @param offX Horizontal offset applied to the drawing position.
 * @param offY Vertical offset applied to the drawing position.
 */
void drawElement(UIElement& el, bool ownDraw = true, int offX = 0, int offY = 0);

/**
 * @brief Creates a new box and adds it to the space
 * @param Win Parent window
 * @param rect Position and size of the button
 * @param clr Button color
 * @param name Unique name of the button
 */
Box& createBox(Window* Win, Rect rect, Color clr, std::string name);

/**
 * @brief Creates a new checkbox
 */
CheckBox& createCheckBox(Window* Win, Square rect, Color clr, bool isOn, std::string name);

/**
 * @brief Creates a new circle
 */
Circle& createCircle(Window* Win, Square rect, Color clr, std::string name);

/**
 * @brief Creates a radio button (part of a group)
 */
RadioButton& createRadioButton(Window* win, Square uRect, Color clrOff, Color clrOn, bool status, int group, std::string name);

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
void setBackground(Color clr, Window* win);

/**
 * @brief Deletes an object by its unique name
 */
void DeleteObject(const std::string& name);

/**
 * @brief Draws a line
 */
Line& createLine(Window* Win, Rect cords, Color color, std::string name);

/**
 * @brief Creates a text input box
 */
InputBox& createInputBox(Window* Win, std::string hint, Rect rect, std::string text, int fontSize, Color clr, std::string name, bool debuggingBox = false);

/**
 * @brief Creates a scrollbar
 */
ScrollBar& createScrollBar(Window* Win, Rect rect, Color clr, std::string name, bool debuggingBox = false);

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
 * @brief Creates a video object
 */
Video& createVideo(Window* win, Rect rect, const char* path, const std::string& name);

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

void playVideo              (Video& v);
void stopSound              (Video& v);
void setVideoLooped         (Video& v);
void setVideoUnlooped       (Video& v);
void setVideoVolume         (Video& v, float vol);
void setVideoPosition       (Video& v, float pos);

Image& uploadFrameFromVideo (Window* win, const char* path, Rect rect, double seekSeconds, std::string name);

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

/**
 * @brief Creates and initializes a renderer associated with the specified window.
 *
 * @param rendererFlags Bitmask of flags controlling renderer creation behavior.
 * @param win Pointer to the target Window instance.
 * @param name Identifier string for the renderer.
 * @return Pointer to the newly created Renderer, or nullptr on failure.
 */
Renderer* createRenderer(int rendererFlags, Window* win, std::string name);

/**
 * @brief Retrieves the primary surface associated with the given window.
 *
 * @param win Pointer to the Window instance.
 * @return Pointer to the Surface object, or nullptr if none exists.
 */
Surface* getSurfaceFromWin(Window* win);

/**
 * @brief Sets the color of a single pixel on the named canvas within the window.
 *
 * @param win Pointer to the Window instance.
 * @param name Name of the target canvas.
 * @param x Horizontal pixel coordinate.
 * @param y Vertical pixel coordinate.
 * @param color Color value to assign.
 */
void setPixel(Window* win, const char* name, int x, int y, Color color);

/**
 * @brief Retrieves the color of a single pixel from the named canvas.
 *
 * @param win Pointer to the Window instance.
 * @param canvasName Name of the source canvas.
 * @param x Horizontal pixel coordinate.
 * @param y Vertical pixel coordinate.
 * @return Color value at the specified coordinates.
 */
Color getPixel(Window* win, const char* canvasName, int x, int y);

/**
 * @brief Creates a new canvas of the specified dimensions within the window.
 *
 * @param win Pointer to the Window instance.
 * @param name Unique identifier for the canvas.
 * @param w Width of the canvas in pixels.
 * @param h Height of the canvas in pixels.
 */
void createCanvas(Window* win, const char* name, int w, int h);

/**
 * @brief Renders the named canvas into the given rectangular region.
 *
 * @param name Name of the canvas to render.
 * @param rect Destination rectangle defining position and size.
 */
void renderCanvas(const char* name, Rect rect);

/**
 * @brief Flushes pending drawing operations for the specified window.
 *
 * Ensures all buffered rendering commands are executed and the display is updated.
 *
 * @param win Pointer to the Window instance.
 */
void flush(Window* win);

/**
 * @brief Draws the current frame for the specified window.
 *
 * Composites all active canvases and presents the final image.
 *
 * @param win Pointer to the Window instance.
 */
void drawFrame(Window* win);

/**
 * @brief Gets the current state of the keyboard
 * @param numKeys Pointer to store the number of keys in the returned array
 * @return Array of key states (1 for pressed, 0 for released)
*/
const uint8_t* GetKeyboardState(int *numKeys);

/**
 * @brief Gets the current state of the mouse
 * @param x Pointer to store the x-coordinate of the mouse position
 * @param y Pointer to store the y-coordinate of the mouse position
 * @return Bitmask of currently pressed mouse buttons
*/
uint32_t GetMouseState(int *x, int *y);

/**
 * @brief Returns a high-resolution performance counter value in ticks.
 * @return Current performance counter value.
 */
uint64_t GetPerformanceCounter();

/**
 * @brief Returns the frequency of the performance counter in Hz.
 * @return Performance counter frequency.
 */
uint64_t GetPerformanceFrequency();

/**
 * @brief Returns milliseconds since SDL library initialization.
 * @return Milliseconds since init.
 */
uint32_t GetTicks();

/**
 * @brief Returns 64-bit milliseconds since SDL library initialization.
 * @return Milliseconds since init (64-bit).
 */
uint64_t GetTicks64();

/**
 * @brief Waits a specified number of milliseconds.
 * @param ms Milliseconds to delay.
 */
void Delay(uint32_t ms);

/**
 * @brief Returns a pointer to the current keyboard state array.
 * @param n Optional pointer to store the number of keys.
 * @return Pointer to the keyboard state array.
 */
const uint8_t* GetKeyboardState(int* n);

/**
 * @brief Returns the current keyboard modifier state.
 * @return Current Keymod state.
 */
Keymod GetModState();

/**
 * @brief Manually sets the current keyboard modifier state.
 * @param modstate New modifier state.
 */
void SetModState(Keymod modstate);

/**
 * @brief Resets the keyboard state (clears all pressed keys).
 */
void ResetKeyboard();

/**
 * @brief Converts a scancode to its corresponding keycode.
 * @param scancode The scancode to convert.
 * @return Corresponding Keycode.
 */
Keycode GetKeyFromScancode(Scancode scancode);

/**
 * @brief Converts a keycode to its physical scancode.
 * @param key The keycode to convert.
 * @return Corresponding Scancode.
 */
Scancode GetScancodeFromKey(Keycode key);

/**
 * @brief Returns the human-readable name of a keycode.
 * @param key The keycode.
 * @return Name of the key.
 */
const char* GetKeyName(Keycode key);

/**
 * @brief Returns the keycode for a given key name.
 * @param name Name of the key.
 * @return Corresponding Keycode.
 */
Keycode GetKeyFromName(const char* name);

/**
 * @brief Returns the name of a scancode.
 * @param scancode The scancode.
 * @return Name of the scancode.
 */
const char* GetScancodeName(Scancode scancode);

/**
 * @brief Returns the scancode for a given name.
 * @param name Name of the scancode.
 * @return Corresponding Scancode.
 */
Scancode GetScancodeFromName(const char* name);

/**
 * @brief Checks if the platform has on-screen keyboard support.
 * @return true if on-screen keyboard is supported.
 */
bool HasScreenKeyboardSupport();

/**
 * @brief Returns whether the on-screen keyboard is currently shown.
 * @param window The window to check.
 * @return true if on-screen keyboard is visible.
 */
bool IsScreenKeyboardShown(Window* window);

/**
 * @brief Returns whether text input is currently active.
 * @return true if text input is active.
 */
bool IsTextInputActive();

/**
 * @brief Starts text input and enables text input events.
 */
void StartTextInput();

/**
 * @brief Stops text input and disables text input events.
 */
void StopTextInput();

/**
 * @brief Sets the rectangle where the IME/composition text should appear.
 * @param rect Rectangle for text input UI.
 */
void SetTextInputRect(Rect* rect);

/**
 * @brief Clears any ongoing text composition (IME).
 */
void ClearComposition();

/**
 * @brief Returns whether a text input UI (IME) is currently shown.
 * @return true if text input UI is shown.
 */
bool IsTextInputShown();

/**
 * @brief Returns the window that currently has mouse focus.
 * @return Pointer to the window with mouse focus.
 */
Window* GetMouseFocus();

/**
 * @brief Returns current mouse button state and position relative to the focused window.
 * @param x Pointer to store x coordinate.
 * @param y Pointer to store y coordinate.
 * @return Bitmask of current mouse button state.
 */
uint32_t GetMouseState(int* x, int* y);

/**
 * @brief Returns global mouse button state and absolute screen position.
 * @param x Pointer to store global x coordinate.
 * @param y Pointer to store global y coordinate.
 * @return Bitmask of current mouse button state.
 */
uint32_t GetGlobalMouseState(int* x, int* y);

/**
 * @brief Returns relative mouse movement and button state since last call.
 * @param x Pointer to store relative x movement.
 * @param y Pointer to store relative y movement.
 * @return Bitmask of current mouse button state.
 */
uint32_t GetRelativeMouseState(int* x, int* y);

/**
 * @brief Moves the mouse cursor to a position inside a window.
 * @param window Target window.
 * @param x X coordinate inside the window.
 * @param y Y coordinate inside the window.
 */
void WarpMouseInWindow(Window* window, int x, int y);

/**
 * @brief Moves the mouse cursor to absolute global screen coordinates.
 * @param x Global x coordinate.
 * @param y Global y coordinate.
 * @return 0 on success, negative on error.
 */
int WarpMouseGlobal(int x, int y);

/**
 * @brief Enables or disables relative mouse mode.
 * @param enabled true to enable relative mode.
 * @return 0 on success.
 */
int SetRelativeMouseMode(bool enabled);

/**
 * @brief Returns whether relative mouse mode is enabled.
 * @return true if relative mouse mode is active.
 */
bool GetRelativeMouseMode();

/**
 * @brief Captures or releases the mouse to the current window.
 * @param enabled true to capture mouse.
 * @return 0 on success.
 */
int CaptureMouse(bool enabled);

/**
 * @brief Creates a monochrome cursor from bitmap data.
 * @param data Cursor pixel data.
 * @param mask Cursor mask data.
 * @param w Width of the cursor.
 * @param h Height of the cursor.
 * @param hot_x Hotspot x coordinate.
 * @param hot_y Hotspot y coordinate.
 * @return New Cursor pointer.
 */
Cursor* CreateCursor(const uint8_t* data, const uint8_t* mask,
                     int w, int h, int hot_x, int hot_y);

/**
 * @brief Creates a color cursor from an SDL surface.
 * @param surface Surface containing cursor image.
 * @param hot_x Hotspot x coordinate.
 * @param hot_y Hotspot y coordinate.
 * @return New Cursor pointer.
 */
Cursor* CreateColorCursor(SDL_Surface* surface, int hot_x, int hot_y);

/**
 * @brief Creates a system-standard cursor.
 * @param id System cursor ID.
 * @return New Cursor pointer.
 */
Cursor* CreateSystemCursor(SystemCursor id);

/**
 * @brief Sets the active cursor.
 * @param cursor Cursor to set.
 */
void SetCursor(Cursor* cursor);

/**
 * @brief Returns the currently active cursor.
 * @return Pointer to active Cursor.
 */
Cursor* GetActiveCursor();

/**
 * @brief Returns the default system cursor.
 * @return Pointer to default Cursor.
 */
Cursor* GetDefaultCursor();

/**
 * @brief Frees a cursor created by SDL.
 * @param cursor Cursor to free.
 */
void FreeCursor(Cursor* cursor);

/**
 * @brief Shows or hides the mouse cursor.
 * @param toggle Non-zero to show cursor, zero to hide.
 * @return Previous visibility state.
 */
int SetCursorVisible(int toggle);

/**
 * @brief Adds a new timer callback.
 * @param interval Interval in milliseconds.
 * @param callback Timer callback function.
 * @param param User data passed to callback.
 * @return TimerID on success, 0 on failure.
 */
TimerID AddTimer(uint32_t interval, TimerCallback callback, void* param);

/**
 * @brief Removes a timer.
 * @param id TimerID to remove.
 * @return true if timer was removed.
 */
bool RemoveTimer(TimerID id);