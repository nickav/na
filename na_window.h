/*
    na_window.h - v0.1
    Nick Aversano's C/C++ cross platform window wrapper

    This is a single header file with OS-independant wrapper functions for:
    windows, menus, trays and file dialogs.

    Depends on na.h
===========================================================================

USAGE
    Define this in your source file:

    #define impl
    #include "na_window.h"

LICENSE
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

CREDITS
    Written by Nick Aversano
    Credits are much appreciated but not required.

VERSION HISTORY
    0.01  - Initial version
*/

#ifndef NA_WINDOW_H
#define NA_WINDOW_H

#ifndef OS_WINDOW_H
#define OS_WINDOW_H

typedef u32 Key_State;
enum {
    KeyState_Null,
    KeyState_Pressed,
    KeyState_Released,
    KeyState_Repeat,
    KeyState_COUNT,
};

typedef u32 Mouse_Button;
enum {
    Mouse_None,
    Mouse_Left,
    Mouse_Right,
    Mouse_Middle,
    Mouse_X1,
    Mouse_X2,
    Mouse_COUNT,
};

typedef u32 Keyboard_Key;
enum {
    Key_None = 0,

    Key_Space = 32,
    Key_Quote = 39,
    Key_Comma = 44,
    Key_Minus = 45,
    Key_Period = 46,
    Key_Slash = 47,

    Key_0 = 48,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,

    Key_Semicolon = 59,
    Key_Equal = 61,

    Key_A = 65,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    Key_LeftBracket = 91,
    Key_Backslash,
    Key_RightBracket,
    Key_Grave,

    Key_Escape = 256,
    Key_Enter,
    Key_Tab,
    Key_Backspace,
    Key_Insert,
    Key_Delete,
    Key_Right,
    Key_Left,
    Key_Down,
    Key_Up,
    Key_PageUp,
    Key_PageDown,
    Key_Home,
    Key_End,

    Key_CapsLock = 280,
    Key_ScrollLock,
    Key_NumLock,
    Key_PrintScreen,
    Key_Pause,

    Key_F1 = 290,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_F13,
    Key_F14,
    Key_F15,
    Key_F16,
    Key_F17,
    Key_F18,
    Key_F19,
    Key_F20,
    Key_F21,
    Key_F22,
    Key_F23,
    Key_F24,

    Key_Numpad0 = 320,
    Key_Numpad1,
    Key_Numpad2,
    Key_Numpad3,
    Key_Numpad4,
    Key_Numpad5,
    Key_Numpad6,
    Key_Numpad7,
    Key_Numpad8,
    Key_Numpad9,
    Key_NumpadDot,
    Key_NumpadDivide,
    Key_NumpadMultiply,
    Key_NumpadSubtract,
    Key_NumpadAdd,
    Key_NumpadEnter,
    Key_NumpadEquals,

    Key_LeftShift = 340,
    Key_LeftCtrl,
    Key_LeftAlt,
    Key_LeftSuper,
    Key_RightShift,
    Key_RightCtrl,
    Key_RightAlt,
    Key_RightSuper,

    Key_Menu = 348,

    Key_MediaNext = 349,
    Key_MediaPrev,
    Key_MediaPlay,
    Key_MediaStop,

    Key_COUNT,
};

typedef u32 Keyboard_Mod;
enum {
    Mod_Shift = 0x01,
    Mod_Ctrl  = 0x02,
    Mod_Alt   = 0x04,
    Mod_Super = 0x08,
};

const Keyboard_Mod Mod_None = (Keyboard_Mod)0;
const Keyboard_Mod Mod_Any = (Keyboard_Mod)-1;

typedef u32 Cursor_Style;
enum {
    Cursor_Null,
    Cursor_Hidden,
    Cursor_Pointer,
    Cursor_Hand,
    Cursor_Text,
    Cursor_Loading,
    Cursor_Blocked,
    Cursor_Pan,
    Cursor_SizeH,
    Cursor_SizeV,
    Cursor_SizeNESW,
    Cursor_SizeNWSE,
    Cursor_COUNT,
};

typedef u32 Event_Type;
enum {
    Event_Null,
    Event_WindowClose,
    Event_WindowFocus,
    Event_WindowBlur,
    Event_KeyPress,
    Event_KeyRelease,
    Event_Scroll,
    Event_Text,
    Event_DropFile,
    Event_COUNT,
};

typedef struct Window Window;

typedef struct Event Event;
struct Event {
    Event *next;
    Event *prev;

    Event_Type type;
    Window *window;

    Keyboard_Key key;
    Keyboard_Mod modifiers;
    b32 is_repeat;
    u32 character;
    Vector2 position;
    Vector2 scroll;
    String path;
};

typedef struct Event_List Event_List;
struct Event_List {
    Event *first;
    Event *last;
    u64 count;
};

typedef struct Window Window;
struct Window
{
    f64 then;
    f64 dt;
    f64 uncapped_dt;
    f64 target_dt;
    Timing_f64 frame_info;

    Event_List events;
    
    // @Memory: this could be packed even smaller
    b8 key_states[Key_COUNT];
};

typedef struct Window_Create_Params Window_Create_Params;
struct Window_Create_Params {
    bool software_renderer;
    bool center;
};

typedef struct Software_Framebuffer Software_Framebuffer;
struct Software_Framebuffer {
    i32 width;
    i32 height;
    i32 bytes_per_pixel; // hopefully this is always 4
    u32 *pixels;
};

#define WINDOW_RESIZE_PROC(name) void (name)(Window *window);
typedef WINDOW_RESIZE_PROC(Window_Resize_Proc);

//
// API Functions
//

// Helpers
function String mouse_to_string(Mouse_Button button);
function String key_to_string(Keyboard_Key key);

// Window
function bool window_init();
function Event_List window_get_events(Arena *arena);
function void window_set_resize_callback(Window *window, Window_Resize_Proc *callback);

function Window *window_create(Vector2i size, String title, Window_Create_Params *params);
function void window_destroy(Window *window);
function bool window_should_close(Window *window);

function void window_show(Window *window);
function void window_hide(Window *window);
function void window_maximize(Window *window);
function void window_minimize(Window *window);
function void window_set_fullscreen(Window *window, bool fullscreen);

function void window_set_title(Window *window, String title);
function void window_set_cursor(Window *window, Cursor_Style cursor);
function void window_set_icon(Window *window, Vector2i size, String rgba_data);
function void window_set_size(Window *window, Vector2i size);
function void window_set_position(Window *window, Vector2i p);
function void window_set_minimum_size(Window *window, Vector2i minimum_size);
function void window_set_mouse_trap(Window *window, b32 trap);
function void window_set_mouse_position(Window *window, Vector2i position);

function bool window_is_focused(Window *window);
function bool window_is_maximized(Window *window);
function bool window_is_fullscreen(Window *window);
function Rectangle2 window_get_client_rect(Window *window);
function Rectangle2 window_get_frame_rect(Window *window);
function f32 window_get_refresh_rate(Window *window);
function f32 window_get_pixel_scale(Window *window);
function Vector2 window_get_mouse_position(Window *window);

function void window_swap_buffers(Window *window);
function Software_Framebuffer window_get_framebuffer(Window *window);

// Window functions
function void window_set_target_dt(Window *window, f64 target_dt);
function f64 window_begin_frame(Arena *arena, Window *window);
function void window_end_frame(Window *window);

// Helpers (derrived from above APIs)
function Vector2 window_get_client_size(Window *window);
function Vector2 window_get_frame_size(Window *window);
function Vector2 window_get_position(Window *window);
function Vector2 window_get_mouse_scroll(Window *window);

typedef struct Menu_Item Menu_Item;

typedef struct Menu_Item_Array Menu_Item_Array;
struct Menu_Item_Array
{
    _ArrayHeader_;
    Menu_Item *data;
};

struct Menu_Item
{
    u32 id;

    String title;
    String shortcut;

    b32 disabled;
    b32 checked;
    b32 radio;
    b32 hidden;
    b32 separator;

    Menu_Item_Array subitems;
};

function u32 window_show_menu(Window *window, Vector2 pos, Menu_Item_Array menu);
// function void window_show_file_picker();

//
// Controllers
//

typedef u32 Button_State;
enum {
    ButtonState_Down     = 1 << 0,
    ButtonState_Pressed  = 1 << 1,
    ButtonState_Released = 1 << 2,
};

typedef u32 Controller_Button;
enum {
    Button_A,
    Button_B,
    Button_X,
    Button_Y,

    Button_Start,
    Button_Back,

    Button_LeftStick,
    Button_RightStick,
    Button_LeftBumper,
    Button_RightBumper,
    Button_LeftTrigger,
    Button_RightTrigger,

    Button_PadUp,
    Button_PadDown,
    Button_PadLeft,
    Button_PadRight,

    Button_COUNT,
};

typedef u32 Controller_Axis;
enum {
    Axis_LeftX = 0,
    Axis_LeftY,
    Axis_RightX,
    Axis_RightY,
    Axis_LeftTrigger,
    Axis_RightTrigger,
};

typedef u32 Controller_Stick;
enum {
    Stick_Left = 0,
    Stick_Right,
    Stick_Trigger,
};

typedef u32 Controller_Hat;
enum {
    Hat_Up    = 0x1,
    Hat_Right = 0x2,
    Hat_Down  = 0x4,
    Hat_Left  = 0x8,
};

#define CONTROLLER_COUNT 16
#define CONTROLLER_MAX_BUTTONS 16
#define CONTROLLER_MAX_AXIS 16
#define CONTROLLER_MAX_HATS 8

typedef struct Controller Controller;
struct Controller {
    u16 index;
    u16 flags;

    String guid;
    void *handle;

    bool is_connected;

    u16 button_count;
    Button_State buttons[CONTROLLER_MAX_BUTTONS];

    u16 axis_count;
    f32 axis[CONTROLLER_MAX_AXIS];

    u16 hat_count;
    u32 hats[CONTROLLER_MAX_HATS];
};

//
// API
//

// Events
function void eat_event(Event_List *events, Event *event);

function b32 key_press_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods);
function b32 key_repeat_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods);
function b32 key_release_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods);
function b32 key_press_or_repeat_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods);

function b32 key_press(Window *window, Keyboard_Key key);
function b32 key_repeat(Window *window, Keyboard_Key key);
function b32 key_release(Window *window, Keyboard_Key key);
function b32 key_press_or_repeat(Window *window, Keyboard_Key key);

function b32 key_down(Window *window, Keyboard_Key key);

// Controllers
function void button_set_state(Button_State *state, b32 is_down);
function b32 controller_down(Controller *ctrl, Controller_Button button);
function b32 controller_pressed(Controller *ctrl, Controller_Button button);
function b32 controller_released(Controller *ctrl, Controller_Button button);
function Vector2 controller_get_axis(Controller *ctrl, u32 axis_index);
function Controller *controller_from_index(u32 controller_index);
function b32 controller_set_vibration(Controller *ctrl, f32 left_motor, f32 right_motor);

// Strings
function String button_to_string(Controller_Button button);
function String axis_to_string(Controller_Axis axis);
function String hat_to_string(Controller_Hat hat);

// Easy Mode
function void window_set_current(Window *window);
function b32 keyboard_pressed(Keyboard_Key key);
function b32 keyboard_down(Keyboard_Key key);
function b32 keyboard_released(Keyboard_Key key);
function b32 keyboard_repeat(Keyboard_Key key);
function b32 keyboard_pressed_or_repeat(Keyboard_Key key);

function Vector2 mouse_get_position();
function Vector2 mouse_get_scroll();
function b32 mouse_pressed(Keyboard_Key key);
function b32 mouse_released(Keyboard_Key key);
function b32 mouse_down(Keyboard_Key key);


#endif // OS_WINDOW_H


#endif // NA_H

//
// impl:
//

#ifdef impl
#ifndef NA_WINDOW_H_IMPL
#define NA_WINDOW_H_IMPL

//
// Platform-Specific:
//

#if OS_WINDOWS
    #include <shellapi.h>
#include <xinput.h>

#pragma comment(lib, "gdi32")
#pragma comment(lib, "shell32")
#pragma comment(lib, "advapi32")


#ifndef WINDOW_CLASS_NAME
#define WINDOW_CLASS_NAME L"Cyan Engine Window Class"
#endif

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

typedef struct Win32_Software_Renderer Win32_Software_Renderer;
struct Win32_Software_Renderer
{
    i32 width;
    i32 height;
    i32 bytes_per_pixel;
    void *pixels;
    
    BITMAPINFO bitmap_info;
    HBITMAP bitmap;
};

typedef struct Win32_Window Win32_Window;
struct Win32_Window
{
    Window data;

    Win32_Window *next;
    Win32_Window *prev;

    HWND hwnd;
    HDC hdc;

    bool is_fullscreen;
    bool should_close;

    Vector2i minimum_size;
    WINDOWPLACEMENT placement;
    WCHAR high_surrogate;

    Cursor_Style cursor;
    Win32_Software_Renderer *software_renderer;
};

static Win32_Window *win32_free_window = NULL;
static Win32_Window *win32_first_window = NULL;
static Win32_Window *win32_last_window = NULL;

static Window_Resize_Proc *win32_resize_callback = NULL;

static bool win32_platform_initted = false;

static Arena *win32_events_arena = 0;
static Event_List *win32_events_list = 0;

static HCURSOR win32_cursor_table[Cursor_COUNT];

typedef HRESULT Win32_DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
static Win32_DwmSetWindowAttribute *DwmSetWindowAttribute = NULL;

//
// Win32 Helpers
//

function LRESULT CALLBACK win32_window_callback(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

static void win32_xinput_poll_controllers();

function Win32_Window *win32_window_from_handle(Window *window)
{
    return (Win32_Window *)window;
}

function bool win32_resize_software_buffer(Win32_Window *window, int width, int height)
{
    Win32_Software_Renderer *it = window->software_renderer;
    assert(it);
    assert(width > 0 && height > 0);

    if (it->width == width && it->height == height)
    {
        return false;
    }

    BITMAPINFO *info = &it->bitmap_info;
    info->bmiHeader.biSize        = sizeof(info->bmiHeader);
    info->bmiHeader.biWidth       = width;
    info->bmiHeader.biHeight      = -height; // NOTE(bill): -ve is top-down, +ve is bottom-up
    info->bmiHeader.biPlanes      = 1;
    info->bmiHeader.biBitCount    = 32;
    info->bmiHeader.biCompression = BI_RGB;

    if (it->pixels)
    {
        VirtualFree(it->pixels, 0, MEM_RELEASE);
    }

    it->width = width;
    it->height = height;
    it->bytes_per_pixel = 4;

    i64 size = (it->width * it->height) * it->bytes_per_pixel;
    it->pixels = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);

    return true;
}

//
// Platform
//

function bool win32_init_xinput();

function bool window_init()
{
    if (win32_platform_initted)
    {
        return false;
    }
    win32_platform_initted = true;

    // NOTE(nick): Set DPI Awareness
    HMODULE user32 = LoadLibraryA("user32.dll");

    typedef BOOL Win32_SetProcessDpiAwarenessContext(HANDLE);
    typedef BOOL Win32_SetProcessDpiAwareness(int);

    Win32_SetProcessDpiAwarenessContext *SetProcessDpiAwarenessContext =
        (Win32_SetProcessDpiAwarenessContext *) GetProcAddress(user32, "SetProcessDpiAwarenessContext");
    Win32_SetProcessDpiAwareness *SetProcessDpiAwareness =
        (Win32_SetProcessDpiAwareness *) GetProcAddress(user32, "SetProcessDpiAwareness");

    if (SetProcessDpiAwarenessContext) {
        SetProcessDpiAwarenessContext(((HANDLE) -4) /* DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 */);
    } else if (SetProcessDpiAwareness) {
        SetProcessDpiAwareness(1 /* PROCESS_SYSTEM_DPI_AWARE */);
    } else {
        SetProcessDPIAware();
    }

    // NOTE(nick): not freeing user32.dll here because we use it later

    // NOTE(nick): set up global window class
    HINSTANCE instance = GetModuleHandleW(0);

    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = (WNDPROC)win32_window_callback;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = instance;
    wc.hIcon         = LoadIconA(instance, "APPICON");
    wc.hCursor       = LoadCursorA(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm       = NULL;

    if (!RegisterClassExW(&wc))
    {
        print("[window] Failed to register window class.\n");
        return false;
    }

    // NOTE(nick): load function for dark theme
    HMODULE dwmapi = LoadLibraryA("dwmapi.dll");
    if (dwmapi)
    {
        DwmSetWindowAttribute = (Win32_DwmSetWindowAttribute *)GetProcAddress(dwmapi, "DwmSetWindowAttribute");
    }

    // NOTE(nick): init controllers
    win32_init_xinput();

    return true;
}

function LRESULT CALLBACK
win32_window_callback(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;

    Win32_Window *window = (Win32_Window *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    Window *window_handle = (Window *)window;

    Event *event = NULL;
    i32 scroll_axis = 1;
    bool is_release = false;

    switch (message)
    {
        default:
        {
            result = DefWindowProcW(hwnd, message, w_param, l_param);
        } break;

        case WM_CREATE:
        {
            if (DwmSetWindowAttribute)
            {
                DWORD use_light_theme;
                DWORD dataSize = sizeof(use_light_theme);
                LSTATUS status = RegGetValueA(HKEY_CURRENT_USER,
                    "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                    "AppsUseLightTheme", RRF_RT_ANY, NULL, &use_light_theme, &dataSize);

                if (status == ERROR_SUCCESS)
                {
                    if (!use_light_theme)
                    {
                        BOOL value = TRUE;
                        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
                    }
                }
            }

            result = DefWindowProcW(hwnd, message, w_param, l_param);
        } break;

        case WM_CLOSE:
        {
            if (win32_events_arena)
            {
                event = PushArrayZero(win32_events_arena, Event, 1);
                event->type = Event_WindowClose;
                event->window = window_handle;
            }

            if (window)
            {
                window->should_close = true;
            }
        } break;

        case WM_SETFOCUS:
        {
            if (win32_events_arena)
            {
                event = PushArrayZero(win32_events_arena, Event, 1);
                event->type = Event_WindowFocus;
                event->window = window_handle;
            }

            result = DefWindowProcW(hwnd, message, w_param, l_param);
        } break;

        case WM_KILLFOCUS:
        {
            if (win32_events_arena)
            {
                event = PushArrayZero(win32_events_arena, Event, 1);
                event->type = Event_WindowBlur;
                event->window = window_handle;
            }

            result = DefWindowProcW(hwnd, message, w_param, l_param);
        } break;

        case WM_PAINT:
        {
            if (window->software_renderer)
            {
                RECT wr;
                GetClientRect(window->hwnd, &wr);
                int width = (int)(wr.right - wr.left);
                int height = (int)(wr.bottom - wr.top);
                win32_resize_software_buffer(window, width, height);
            }

            if (win32_resize_callback)
            {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);
                win32_resize_callback(window_handle);
                EndPaint(hwnd, &ps);
            }
            else
            {
                result = DefWindowProcW(hwnd, message, w_param, l_param);
            }
        } break;

        #if 0
        case WM_SIZING:
        {
            RECT rect = *(RECT *)l_param;
            int width = (int)(rect.right - rect.left);
            int height = (int)(rect.bottom - rect.top);
            Dump2(width, height);
        } break;
        #endif

        case WM_SETCURSOR:
        {
            if (LOWORD(l_param) == HTCLIENT && window && window->cursor != Cursor_Null)
            {
                SetCursor(win32_cursor_table[window->cursor]);
            }
            else
            {
                result = DefWindowProcW(hwnd, message, w_param, l_param);
            }
        } break;

        case WM_DPICHANGED:
        {
            // Resize windowed mode windows that either permit rescaling or that
            // need it to compensate for non-client area scaling
            RECT *suggested = cast(RECT *)l_param;
            SetWindowPos(window->hwnd, HWND_TOP,
                                     suggested->left,
                                     suggested->top,
                                     suggested->right - suggested->left,
                                     suggested->bottom - suggested->top,
                                     SWP_NOACTIVATE | SWP_NOZORDER);
        } break;

        case WM_WINDOWPOSCHANGED:
        {
            WINDOWPOS *pos = (WINDOWPOS *)l_param;

            MONITORINFO monitor_info = {sizeof(monitor_info)};
            if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monitor_info))
            {
                i32 monitor_width = (monitor_info.rcMonitor.right - monitor_info.rcMonitor.left);
                i32 monitor_height = (monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top);

                bool becoming_fullscreen = (
                    (monitor_info.rcMonitor.left == pos->x) && (monitor_info.rcMonitor.top == pos->y) &&
                    (monitor_width == pos->cx) && (monitor_height == pos->cy)
                );

                if (window)
                {
                    window->is_fullscreen = becoming_fullscreen;
                }
            }
        } break;

        case WM_GETMINMAXINFO:
        {
            if (window)
            {
                DWORD style = WS_OVERLAPPEDWINDOW;
                RECT wr = {0, 0, (LONG)window->minimum_size.x, (LONG)window->minimum_size.y};
                AdjustWindowRect(&wr, style, FALSE);
                int width = (int)(wr.right - wr.left);
                int height = (int)(wr.bottom - wr.top);

                MINMAXINFO *info = (MINMAXINFO *)l_param;
                info->ptMinTrackSize.x = width;
                info->ptMinTrackSize.y = height;
            }
            else
            {
                result = DefWindowProcW(hwnd, message, l_param, w_param);
            }
        } break;

        case WM_SYSCOMMAND:
        {
            switch (w_param)
            {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER: {
                    // NOTE(nick): this is where we could disallow screen saver and screen blanking
                    // by not calling the default window proc
                    result = DefWindowProcW(hwnd, message, w_param, l_param);
                } break;

                // User trying to access application menu using ALT
                case SC_KEYMENU: {
                    // NOTE(nick): prevent beep sound when pressing alt key combo (e.g. alt + enter)
                    result = 0;
                } break;
                
                default:
                {
                    result = DefWindowProcW(hwnd, message, w_param, l_param);
                } break;
            }
        } break;

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            is_release = true;
        } fallthrough;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        {
            Mouse_Button button = Mouse_Left;
            switch (message)
            {
                case WM_MBUTTONUP: case WM_MBUTTONDOWN: button = Mouse_Middle; break;
                case WM_RBUTTONUP: case WM_RBUTTONDOWN: button = Mouse_Right; break;
            }

            // NOTE(nick): this makes it so that we still get the mouse up events if the user
            // moves the mouse outside of the window during the press
            if (!is_release)
            {
                SetCapture(hwnd);
            }
            else
            {
                ReleaseCapture();
            }
            
            if (win32_events_arena)
            {
                event = PushArrayZero(win32_events_arena, Event, 1);
                event->type = is_release ? Event_KeyRelease : Event_KeyPress;
                event->window = window_handle;
                event->key = button;
                event->position = window_get_mouse_position(window_handle);
            }
        } break;

        case WM_MOUSEHWHEEL: { scroll_axis = 0; } fallthrough;
        case WM_MOUSEWHEEL:
        {
            f32 delta = (SHORT)HIWORD(w_param) / (f32)WHEEL_DELTA;
            if (win32_events_arena)
            {
                // NOTE(nick): invert the X-axis for consistency with MacOS and X11
                if (scroll_axis == 0) { delta *= -1; }

                event = PushArrayZero(win32_events_arena, Event, 1);
                event->type = Event_Scroll;
                event->window = window_handle;
                event->scroll.e[scroll_axis] = delta;
                event->position = window_get_mouse_position(window_handle);
            }
        } break;

        case WM_SYSKEYDOWN: case WM_SYSKEYUP:
        {
            DefWindowProcW(hwnd, message, w_param, l_param);
        } fallthrough;
        case WM_KEYDOWN: case WM_KEYUP:
        {
            b32 was_down = !!(l_param & (1 << 30));
            b32 is_down  =  !(l_param & (1 << 31));
            
            local_persist Keyboard_Key key_table[Key_COUNT] = {0};
            local_persist b32 key_table_initialized = false;
            if (!key_table_initialized)
            {
                key_table_initialized = true;
                
                for (u32 i = 'A', j = Key_A; i <= 'Z'; i += 1, j += 1)
                {
                    key_table[i] = (Keyboard_Key)j;
                }
                for (u32 i = '0', j = Key_0; i <= '9'; i += 1, j += 1)
                {
                    key_table[i] = (Keyboard_Key)j;
                }
                for (u32 i = VK_F1, j = Key_F1; i <= VK_F24; i += 1, j += 1)
                {
                    key_table[i] = (Keyboard_Key)j;
                }

                key_table[VK_SPACE]         = Key_Space;
                key_table[VK_OEM_7]         = Key_Quote;
                key_table[VK_OEM_COMMA]     = Key_Comma;
                key_table[VK_OEM_MINUS]     = Key_Minus;
                key_table[VK_OEM_PERIOD]    = Key_Period;
                key_table[VK_OEM_2]         = Key_Slash;

                key_table[VK_OEM_1]         = Key_Semicolon;
                key_table[VK_OEM_PLUS]      = Key_Equal;

                key_table[VK_OEM_4]         = Key_LeftBracket;
                key_table[VK_OEM_5]         = Key_Backslash;
                key_table[VK_OEM_6]         = Key_RightBracket;
                key_table[VK_OEM_3]         = Key_Grave;
                
                key_table[VK_ESCAPE]        = Key_Escape;
                key_table[VK_RETURN]        = Key_Enter;
                key_table[VK_TAB]           = Key_Tab;
                key_table[VK_BACK]          = Key_Backspace;
                key_table[VK_INSERT]        = Key_Insert;
                key_table[VK_DELETE]        = Key_Delete;
                key_table[VK_RIGHT]         = Key_Right;
                key_table[VK_LEFT]          = Key_Left;
                key_table[VK_DOWN]          = Key_Down;
                key_table[VK_UP]            = Key_Up;
                key_table[VK_PRIOR]         = Key_PageUp;
                key_table[VK_NEXT]          = Key_PageDown;
                key_table[VK_HOME]          = Key_Home;
                key_table[VK_END]           = Key_End;

                key_table[VK_CAPITAL]       = Key_CapsLock;
                key_table[VK_SCROLL]        = Key_ScrollLock;
                key_table[VK_NUMLOCK]       = Key_NumLock;
                key_table[VK_SNAPSHOT]      = Key_PrintScreen;
                key_table[VK_PAUSE]         = Key_Pause;

                key_table[VK_NUMPAD0]       = Key_Numpad0;
                key_table[VK_NUMPAD1]       = Key_Numpad1;
                key_table[VK_NUMPAD2]       = Key_Numpad2;
                key_table[VK_NUMPAD3]       = Key_Numpad3;
                key_table[VK_NUMPAD4]       = Key_Numpad4;
                key_table[VK_NUMPAD5]       = Key_Numpad5;
                key_table[VK_NUMPAD6]       = Key_Numpad6;
                key_table[VK_NUMPAD7]       = Key_Numpad7;
                key_table[VK_NUMPAD8]       = Key_Numpad8;
                key_table[VK_NUMPAD9]       = Key_Numpad9;

                key_table[VK_CONTROL]       = Key_LeftCtrl;
                key_table[VK_SHIFT]         = Key_LeftShift;
                key_table[VK_MENU]          = Key_LeftAlt;
                key_table[VK_SHIFT]         = Key_LeftShift;
                key_table[VK_LWIN]          = Key_LeftSuper;
                key_table[VK_RWIN]          = Key_RightSuper;

                key_table[VK_MEDIA_NEXT_TRACK] = Key_MediaNext;
                key_table[VK_MEDIA_PREV_TRACK] = Key_MediaPrev;
                key_table[VK_MEDIA_PLAY_PAUSE] = Key_MediaPlay;
                key_table[VK_MEDIA_STOP]       = Key_MediaStop;
            }
            
            Keyboard_Key key = Key_None;
            if (w_param < count_of(key_table))
            {
                key = key_table[w_param];
            }

            i32 scancode = (HIWORD(l_param) & (KF_EXTENDED | 0xff));
            if (!scancode) {
                // NOTE: Some synthetic key messages have a scancode of zero
                // HACK: Map the virtual key back to a usable scancode
                scancode = MapVirtualKeyW((UINT)w_param, MAPVK_VK_TO_VSC);
            }

            // disambiguate left and right modifier keys
            if (key == Key_LeftShift || key == Key_LeftCtrl || key == Key_LeftAlt || key == Key_LeftSuper)
            {
                switch (scancode)
                {
                    case 0x036: { key = Key_RightShift; } break;
                    case 0x11D: { key = Key_RightCtrl; } break;
                    case 0x138: { key = Key_RightAlt; } break;
                    case 0x15C: { key = Key_RightSuper; } break;
                    default: break;
                }
            }

            if (scancode == 0x15D)
            {
                key = Key_Menu;
            }

            if (GetKeyState(VK_NUMLOCK) & 1)
            {
                // NOTE(nick): scancodes map to the physical location of the key on the keyboard
                // maybe the user just wants the default numpad behavior
                // in which case, we shouldn't remap these keys
                // that will give you the defaults which are the shortcuts labeled on the number pad
                // BUT, if the numlock is on, it seems like we don't get any events for these keys
                // in which case it seems fine to remap them
                switch (scancode)
                {
                    case 0x04E: { key = Key_NumpadAdd; } break;
                    case 0x053: { key = Key_NumpadDot; } break;
                    case 0x135: { key = Key_NumpadDivide; } break;
                    case 0x11C: { key = Key_NumpadEnter; } break;
                    case 0x059: { key = Key_NumpadEquals; } break;
                    case 0x037: { key = Key_NumpadMultiply; } break;
                    case 0x04A: { key = Key_NumpadSubtract; } break;
                    default: break;
                }
            }
            
            if (key != Key_None)
            {
                if (win32_events_arena)
                {
                    event = PushArrayZero(win32_events_arena, Event, 1);
                    event->type = is_down ? Event_KeyPress : Event_KeyRelease;
                    event->window = window_handle;
                    event->is_repeat = is_down && was_down;
                    event->key = key;
                }
            }
        } break;

        case WM_CHAR:
        case WM_SYSCHAR:
        {
            u16 character = (u16)w_param;
            if (character >= 0xd800 && character <= 0xdbff)
            {
                window->high_surrogate = (WCHAR)w_param;
            }
            else
            {
                u32 codepoint = 0;
                if (character >= 0xdc00 && character <= 0xdfff)
                {
                    if (window->high_surrogate)
                    {
                        codepoint += (window->high_surrogate - 0xd800) << 10;
                        codepoint += (WCHAR) character - 0xdc00;
                        codepoint += 0x10000;
                    }
                }
                else
                {
                    codepoint = character;
                }

                window->high_surrogate = 0;

                if (codepoint == '\r')
                {
                    codepoint = '\n';
                }

                // NOTE(nick): filter out non display characters (e.g. backspace)
                if ((codepoint >= 32 && codepoint != 127) || (codepoint == '\t' || codepoint == '\n'))
                {
                    if (win32_events_arena)
                    {
                        event = PushArrayZero(win32_events_arena, Event, 1);
                        event->type = Event_Text;
                        event->window = window_handle;
                        event->character = codepoint;
                    }
                }
            }

            result = DefWindowProcW(hwnd, message, w_param, l_param);
        } break;

        case WM_UNICHAR:
        {
            if (w_param == UNICODE_NOCHAR)
            {
                // WM_UNICHAR is not sent by Windows, but is sent by some
                // third-party input method engine
                // Returning TRUE here announces support for this message
                result = true;
            }
            else
            {
                u32 codepoint = (u32)w_param;
                if (codepoint == '\r')
                {
                    codepoint = '\n';
                }

                // NOTE(nick): filter out non display characters (e.g. backspace)
                if ((codepoint >= 32 && codepoint != 127) || (codepoint == '\t' || codepoint == '\n'))
                {
                    if (win32_events_arena)
                    {
                        event = PushArrayZero(win32_events_arena, Event, 1);
                        event->type = Event_Text;
                        event->window = window_handle;
                        event->character = codepoint;
                    }
                }
            }

        } break;

        // @Incomplete:
        case WM_IME_REQUEST:
        {
            switch (w_param)
            {
                case IMR_QUERYCHARPOSITION:
                {
                    IMECHARPOSITION *char_pos = (IMECHARPOSITION *)l_param;
                    char_pos->dwSize = sizeof(IMECHARPOSITION);
                    char_pos->pt.x = 0;
                    char_pos->pt.y = 0;
                    // char_pos->cLineHeight = ;
                    // char_pos->rcDocument.left = ;
                    // char_pos->rcDocument.top = ;
                    // char_pos->rcDocument.right = ;
                    // char_pos->rcDocument.bottom = ;

                    result = true;
                } break;
            }

        } break;

        case WM_DROPFILES:
        {
            HDROP drop = (HDROP)w_param;

            POINT point;
            DragQueryPoint(drop, &point);

            if (win32_events_arena)
            {
                M_Temp scratch = GetScratch(&win32_events_arena, 1);

                int count = DragQueryFileW(drop, 0xffffffff, NULL, 0);
                for (int i = 0; i < count; i += 1)
                {
                    UINT length = DragQueryFileW(drop, i, NULL, 0);
                    u16 *buffer = PushArray(scratch.arena, u16, length + 1);
                    DragQueryFileW(drop, i, (WCHAR *)buffer, length + 1);

                    String path = string_from_string16(win32_events_arena, string16_make(buffer, length));

                    Event *event = PushArrayZero(win32_events_arena, Event, 1);
                    event->type = Event_DropFile;
                    event->window = window_handle;
                    event->position = v2((f32)point.x, (f32)point.y);
                    event->path = path;

                    DLLPushBack(win32_events_list->first, win32_events_list->last, event);
                    win32_events_list->count += 1;
                }

                ReleaseScratch(scratch);
            }

            DragFinish(drop);
        } break;
    }

    if (event)
    {
        Keyboard_Mod mods = 0;
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            mods |= Mod_Ctrl;
        }
        if (GetKeyState(VK_SHIFT) & 0x8000)
        {
            mods |= Mod_Shift;
        }
        if (GetKeyState(VK_MENU) & 0x8000)
        {
            mods |= Mod_Alt;
        }
        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
        {
            mods |= Mod_Super;
        }

        event->modifiers = mods;
        DLLPushBack(win32_events_list->first, win32_events_list->last, event);
        win32_events_list->count += 1;
    }

    return result;
}

function Event_List window_get_events(Arena *arena) {
    Event_List result = {0};
    win32_events_arena = arena;
    win32_events_list = &result;

    for (;;) {
        MSG msg = {0};

        if (!PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    win32_xinput_poll_controllers();

    win32_events_arena = 0;
    win32_events_list = 0;
    return result;
}

function void window_set_resize_callback(Window *window, Window_Resize_Proc *callback)
{
    // @Incomplete: this should be per-window
    // Also maybe consider providing a global thing?
    // :WindowResizeCallbackSituation
    win32_resize_callback = callback;
}

//
// Window
//

function Window *window_create(Vector2i size, String title, Window_Create_Params *optional_params)
{
    Window_Create_Params params = {0};
    if (optional_params)
    {
        MemoryCopyStruct(&params, optional_params);
    }

    if (!win32_platform_initted)
    {
        window_init();
        win32_platform_initted = true;
    }

    Win32_Window *window = win32_free_window;
    {
        if (window)
        {
            win32_free_window = win32_free_window->next;
            MemoryZeroStruct(window);
        }
        else
        {
            window = PushArrayZero(win32_global_arena, Win32_Window, 1);
        }
        DLLPushBack(win32_first_window, win32_last_window, window);
    }

    HWND hwnd = 0;
    HDC hdc = 0;
    {
        M_Temp scratch = GetScratch(0, 0);

        String16 title16 = string16_from_string(scratch.arena, title);

        HINSTANCE instance = GetModuleHandleW(0);

        hwnd = CreateWindowW(WINDOW_CLASS_NAME, (WCHAR *)title16.data, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y,
            0, 0, instance, 0
        );

        hdc = GetDC(hwnd);

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)window);

        f32 scale = (f32)GetDeviceCaps(hdc, LOGPIXELSX) / (f32)USER_DEFAULT_SCREEN_DPI;
        RECT wr = {0, 0, (LONG)(size.x * scale), (LONG)(size.y * scale)};
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
        int width = (i32)(wr.right - wr.left);
        int height = (i32)(wr.bottom - wr.top);

        SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);

        if (params.center)
        {
            HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);

            MONITORINFO info = {0};
            info.cbSize = sizeof(MONITORINFO);
            GetMonitorInfoW(monitor, &info);
            int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
            int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

            int center_x = (monitor_width - width) / 2;
            int center_y = (monitor_height - height) / 2;

            SetWindowPos(hwnd, HWND_TOP, center_x, center_y, width, height, SWP_NOOWNERZORDER);
        }

        DragAcceptFiles(hwnd, TRUE);
        ShowWindow(hwnd, SW_SHOW);

        ReleaseScratch(scratch);
    }

    {
        window->hwnd = hwnd;
        window->hdc  = hdc;

        if (params.software_renderer)
        {
            window->software_renderer = (Win32_Software_Renderer *)os_alloc(sizeof(Win32_Software_Renderer));
        }
    }

    Window *result = (Window *)window;
    return result;
}

function void window_destroy(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        DLLRemove(win32_first_window, win32_last_window, it);
        StackPush(win32_free_window, it);

        if (it->hdc)
        {
            ReleaseDC(it->hwnd, it->hdc);
            it->hdc = 0;
        }

        if (it->hwnd)
        {
            DestroyWindow(it->hwnd);
            it->hwnd = 0;
        }

        if (it->software_renderer)
        {
            os_free(it->software_renderer);
            it->software_renderer = NULL;
        }
    }
}

function bool window_should_close(Window *window) {
    Win32_Window *it = win32_window_from_handle(window);
    bool result = false;
    if (it)
    {
        result = it->should_close;
    }
    return result;
}

function void window_show(Window *window) {
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        ShowWindow(it->hwnd, SW_SHOW);
    }
}

function void window_hide(Window *window) {
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        ShowWindow(it->hwnd, SW_HIDE);
    }
}

function void window_maximize(Window *window) {
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        ShowWindow(it->hwnd, SW_MAXIMIZE);
    }
}

function void window_minimize(Window *window) {
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        ShowWindow(it->hwnd, SW_SHOWMINNOACTIVE);
    }
}

function void window_set_fullscreen(Window *window, bool fullscreen) {
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        HWND hwnd = it->hwnd;
        DWORD style = GetWindowLongW(hwnd, GWL_STYLE);

        if (fullscreen)
        {
            MONITORINFO monitor_info = {sizeof(monitor_info)};

            if (
                GetWindowPlacement(hwnd, &it->placement) &&
                GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monitor_info)
            ) {
                SetWindowLongW(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);

                SetWindowPos(
                    hwnd,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    monitor_info.rcMonitor.right  - monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED
                );
            }
        } else {
            SetWindowLongW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
            SetWindowPlacement(hwnd, &it->placement);
            DWORD flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;
            SetWindowPos(hwnd, 0, 0, 0, 0, 0, flags);
        }

        it->is_fullscreen = fullscreen;
    }
}

function void window_set_title(Window *window, String title) {
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        M_Temp scratch = GetScratch(0, 0);
        String16 title16 = string16_from_string(scratch.arena, title);
        SetWindowTextW(it->hwnd, (WCHAR *)title16.data);
        ReleaseScratch(scratch);
    }
}

function void window_set_cursor(Window *window, Cursor_Style cursor)
{
    // @Incomplete: should this only apply when the cursor is over the window?
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        if (it->cursor != cursor)
        {
            local_persist b32 table_initialized = false;
            if (!table_initialized)
            {
                win32_cursor_table[Cursor_Hidden]   = NULL;
                win32_cursor_table[Cursor_Pointer]  = LoadCursorA(0, IDC_ARROW);
                win32_cursor_table[Cursor_Hand]     = LoadCursorA(0, IDC_HAND);
                win32_cursor_table[Cursor_Text]     = LoadCursorA(0, IDC_IBEAM);
                win32_cursor_table[Cursor_Loading]  = LoadCursorA(0, IDC_WAIT);
                win32_cursor_table[Cursor_Blocked]  = LoadCursorA(0, IDC_NO);
                win32_cursor_table[Cursor_Pan]      = LoadCursorA(0, IDC_SIZEALL);
                win32_cursor_table[Cursor_SizeH]    = LoadCursorA(0, IDC_SIZEWE);
                win32_cursor_table[Cursor_SizeV]    = LoadCursorA(0, IDC_SIZENS);
                win32_cursor_table[Cursor_SizeNESW] = LoadCursorA(0, IDC_SIZENESW);
                win32_cursor_table[Cursor_SizeNWSE] = LoadCursorA(0, IDC_SIZENWSE);

                table_initialized = true;
            }

            it->cursor = cursor;
            SetCursor(win32_cursor_table[it->cursor]);
        }
    }
}

function void window_set_icon(Window *window, Vector2i size, String rgba_data)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        HICON icon = NULL;

        ICONINFO icon_info = {0};
        icon_info.fIcon = TRUE;

        M_Temp scratch = GetScratch(0, 0);
        u64 count = size.x * size.y;
        u8 *bgra_data = PushArray(scratch.arena, u8, 4 * count);

        u8 *rgba = rgba_data.data;
        u8 *bgra = bgra_data;
        for (i32 i = 0; i < count; i += 1)
        {
            bgra[0] = rgba[2];
            bgra[1] = rgba[1];
            bgra[2] = rgba[0];
            bgra[3] = rgba[3];

            rgba += 4;
            bgra += 4;
        }

        icon_info.hbmColor = CreateBitmap(size.x, size.y, 1, 32, bgra_data);
        if (icon_info.hbmColor)
        {
            icon_info.hbmMask = CreateCompatibleBitmap(it->hdc, size.x, size.y);
            if (icon_info.hbmMask)
            {
                icon = CreateIconIndirect(&icon_info);
                DeleteObject(icon_info.hbmMask);
            }
            DeleteObject(icon_info.hbmColor);
        }

        if (icon)
        {
            SendMessage(it->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
            SendMessage(it->hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
        }

        ReleaseScratch(scratch);
    }
}

function void window_set_size(Window *window, Vector2i size)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        SetWindowPos(it->hwnd, HWND_TOP, 0, 0, size.x, size.y, SWP_NOOWNERZORDER | SWP_NOMOVE);
    }
}

function void window_set_position(Window *window, Vector2i p)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        SetWindowPos(it->hwnd, HWND_TOP, p.x, p.y, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE);
    }
}

function void window_set_minimum_size(Window *window, Vector2i minimum_size)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        it->minimum_size = minimum_size;
    }
}

function void window_set_mouse_trap(Window *window, b32 trap)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        if (trap)
        {
            RECT rect;
            GetClientRect(it->hwnd, &rect);
            ClientToScreen(it->hwnd, (POINT *)&rect.left);
            ClientToScreen(it->hwnd, (POINT *)&rect.right);
            ClipCursor(&rect);
        }
        else
        {
            ClipCursor(NULL);
        }
    }
}

function void window_set_mouse_position(Window *window, Vector2i position)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        RECT rect;
        GetClientRect(it->hwnd, &rect);
        ClientToScreen(it->hwnd, (POINT *)&rect.left);
        ClientToScreen(it->hwnd, (POINT *)&rect.right);

        position.x = clamp_i32(position.x, 0, rect.right - rect.left);
        position.y = clamp_i32(position.y, 0, rect.bottom - rect.top);

        SetCursorPos(rect.left + position.x, rect.top + position.y);
    }
}

function bool window_is_focused(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    bool result = false;
    if (it)
    {
        result = GetForegroundWindow() == it->hwnd;
    }
    return result;
}

function bool window_is_maximized(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    bool result = false;
    if (it)
    {
        result = !!(IsZoomed(it->hwnd));
    }
    return result;
}

function bool window_is_fullscreen(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    bool result = false;
    if (it)
    {
        result = it->is_fullscreen;
    }
    return result;
}

function Rectangle2 window_get_client_rect(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    Rectangle2 result = {0};
    if (it)
    {
        RECT rect = {0};
        if (GetClientRect(it->hwnd, &rect))
        {
            result.x0 = (f32)rect.left;
            result.y0 = (f32)rect.top;
            result.x1 = (f32)rect.right;
            result.y1 = (f32)rect.bottom;
        }
    }
    return result;
}

function Rectangle2 window_get_frame_rect(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    Rectangle2 result = {0};
    if (it)
    {
        RECT rect = {0};
        if (GetWindowRect(it->hwnd, &rect))
        {
            DWORD style = WS_OVERLAPPEDWINDOW;
            result.x0 = (f32)rect.left;
            result.y0 = (f32)rect.top;
            result.x1 = (f32)rect.right;
            result.y1 = (f32)rect.bottom;
        }
    }
    return result;
}

function f32 window_get_refresh_rate(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    f32 result = 0;
    if (it)
    {
        HMONITOR monitor = MonitorFromWindow(it->hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFOEXW mi;
        ZeroMemory(&mi, sizeof(mi));
        mi.cbSize = sizeof(mi);
        GetMonitorInfoW(monitor, (MONITORINFO*)&mi);

        DEVMODEW dm;
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);
        if (EnumDisplaySettingsW(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm))
        {
            result = (f32)dm.dmDisplayFrequency;
        }

        if (result <= 1)
        {
            result = (f32)GetDeviceCaps(it->hdc, VREFRESH);
        }

        if (result <= 1)
        {
            if (EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &dm))
            {
                result = (f32)dm.dmDisplayFrequency;
            }
        }
    }
    return result;
}

function f32 window_get_pixel_scale(Window *window) {
    Win32_Window *it = win32_window_from_handle(window);
    f32 result = 1.0f;
    if (it)
    {
        typedef UINT GetDpiForWindowType(HWND hwnd);
        static GetDpiForWindowType *win32_GetDpiForWindow = 0;
        static bool did_load = false;
        if (!did_load)
        {
            HMODULE user32 = LoadLibraryA("user32.dll");
            win32_GetDpiForWindow = (GetDpiForWindowType *)GetProcAddress(user32, "GetDpiForWindow");
            did_load = true;
        }


        if (win32_GetDpiForWindow == 0)
        {
            result = (f32)GetDeviceCaps(it->hdc, LOGPIXELSX) / (f32)USER_DEFAULT_SCREEN_DPI;
        }
        else
        {
            result = win32_GetDpiForWindow(it->hwnd) / (f32)USER_DEFAULT_SCREEN_DPI;
        }
        return result;
    }
    return result;
}

function void window_swap_buffers(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        // NOTE(nick): present!
        if (it->software_renderer)
        {
            RECT wr = {0};
            GetClientRect(it->hwnd, &wr);
            int width = (int)(wr.right - wr.left);
            int height = (int)(wr.bottom - wr.top);

            Software_Framebuffer buffer = window_get_framebuffer(window);
            BITMAPINFO *bitmap_info = &it->software_renderer->bitmap_info;

            StretchDIBits(it->hdc,
                0, 0, width, height,
                0, 0, buffer.width, buffer.height, buffer.pixels,
                bitmap_info,
                DIB_RGB_COLORS, SRCCOPY);
        }
        else
        {
            SwapBuffers(it->hdc);
        }
    }
}

function Vector2 window_get_mouse_position(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    Vector2 result = v2(-1, -1);
    if (it)
    {
        POINT point;
        if (GetCursorPos(&point))
        {
            if (ScreenToClient(it->hwnd, &point))
            {
                result = v2((f32)point.x, (f32)point.y);
            }
        }
    }
    return result;
}

function Software_Framebuffer window_get_framebuffer(Window *window)
{
    Win32_Window *it = win32_window_from_handle(window);
    Software_Framebuffer result = {0};
    if (it)
    {
        if (it->software_renderer)
        {
            Win32_Software_Renderer *renderer = it->software_renderer;
            result.width = renderer->width;
            result.height = renderer->height;
            result.pixels = (u32 *)renderer->pixels;
            // this is fine.
            result.bytes_per_pixel = renderer->bytes_per_pixel;
            assert(result.bytes_per_pixel == 4);
        }
    }
    return result;
}

static HMENU win32__build_menu(Arena *arena, Menu_Item_Array items)
{
    HMENU hmenu = CreatePopupMenu();

    for (u64 i = 0; i < items.count; i++)
    {
        Menu_Item *it = &items.data[i];

        if (it->hidden) continue;

        if (it->separator)
        {
            AppendMenuW(hmenu, MF_SEPARATOR, 0, NULL);
            continue;
        }

        String label = it->title;
        if (it->shortcut.count > 0)
        {
            label = sprint("%.*s\t%.*s", LIT(it->title), LIT(it->shortcut));
        }

        String16 label16 = string16_from_string(arena, label);
        if (it->subitems.data && it->subitems.count > 0)
        {
            HMENU submenu = win32__build_menu(arena, it->subitems);

            UINT flags = MF_POPUP | MF_STRING;
            if (it->disabled) flags |= MF_GRAYED;
            AppendMenuW(hmenu, flags, (UINT_PTR)submenu, (LPCWSTR)label16.data);
        }
        else
        {
            MENUITEMINFOW info = {0};
            info.cbSize    = sizeof(info);
            info.fMask     = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_STRING;
            info.fType     = it->radio ? (MFT_RADIOCHECK | MFT_STRING) : MFT_STRING;
            info.fState    = MFS_ENABLED;
            info.wID       = (UINT)(it->id + 1);
            info.dwTypeData = (LPWSTR)label16.data;

            if (it->checked)  info.fState |= MFS_CHECKED;
            if (it->disabled) info.fState |= MFS_DISABLED;

            InsertMenuItemW(hmenu, GetMenuItemCount(hmenu), TRUE, &info);
        }
    }

    return hmenu;
}

function u32 window_show_menu(Window *window, Vector2 pos, Menu_Item_Array items)
{
    u32 result = 0;
    Win32_Window *it = win32_window_from_handle(window);
    if (it)
    {
        M_Temp scratch = GetScratch(0, 0);
        POINT point = {(LONG)pos.x, (LONG)pos.y};
        ClientToScreen(it->hwnd, &point);

        HMENU hmenu = win32__build_menu(scratch.arena, items);

        SetForegroundWindow(it->hwnd);

        UINT flags = TPM_RETURNCMD | TPM_LEFTALIGN | TPM_NONOTIFY;
        i64 tmp_result = (u32)TrackPopupMenu(hmenu, flags, point.x, point.y, 0, it->hwnd, NULL);
        tmp_result -= 1;
        result = tmp_result;

        DestroyMenu(hmenu);
        ReleaseScratch(scratch);
    }

    return result;
}

//
// Controller
//

#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef XINPUT_GET_STATE(XInput_Get_State_t);

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef XINPUT_SET_STATE(XInput_Set_State_t);

#define XINPUT_GET_CAPABILITIES(name) DWORD WINAPI name(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities)
typedef XINPUT_GET_CAPABILITIES(XInput_Get_Capabilities_t);

XINPUT_GET_STATE(XInputGetStateStub) { return ERROR_DEVICE_NOT_CONNECTED; }
XINPUT_SET_STATE(XInputSetStateStub) { return ERROR_DEVICE_NOT_CONNECTED; }
XINPUT_GET_CAPABILITIES(XInputGetCapabilitiesStub) { return ERROR_DEVICE_NOT_CONNECTED; }

static XInput_Get_State_t *_XInputGetState = XInputGetStateStub;
static XInput_Set_State_t  *_XInputSetState = XInputSetStateStub;
static XInput_Get_Capabilities_t  *_XInputGetCapabilities = XInputGetCapabilitiesStub;

#define XInputGetState _XInputGetState
#define XInputSetState _XInputSetState
#define XInputGetCapabilities _XInputGetCapabilities

static Controller win32_controllers[CONTROLLER_COUNT];

//
// Win32 Helpers
//

function bool win32_init_xinput()
{
    HMODULE libxinput         = LoadLibraryA("xinput1_4.dll");
    if (!libxinput) libxinput = LoadLibraryA("xinput9_1_0.dll");
    if (!libxinput) libxinput = LoadLibraryA("xinput1_3.dll");

    if (!libxinput)
    {
        print("[xinput] Failed to load an xinput dll!\n");
        return false;
    }

    XInputGetState = (XInput_Get_State_t *)GetProcAddress(libxinput, "XInputGetState");
    if (!XInputGetState) { XInputGetState = XInputGetStateStub; }

    XInputSetState = (XInput_Set_State_t *)GetProcAddress(libxinput, "XInputSetState");
    if (!XInputSetState) { XInputSetState = XInputSetStateStub; }

    XInputGetCapabilities = (XInput_Get_Capabilities_t *)GetProcAddress(libxinput, "XInputGetCapabilities");
    if (!XInputGetCapabilities) { XInputGetCapabilities = XInputGetCapabilitiesStub; }

    return true;
}

function f32 process_xinput_stick_value(SHORT value, SHORT deadzone_threshold)
{
    f32 result = 0;

    if (value < -deadzone_threshold) {
        result = (f32)((value + deadzone_threshold) / (32768.0f - deadzone_threshold));
    } else if (value > deadzone_threshold) {
        result = (f32)((value - deadzone_threshold) / (32767.0f - deadzone_threshold));
    }

    return result;
}

function bool process_xinput_button_value(DWORD button_state, DWORD button_bit)
{
    return (button_state & button_bit) == button_bit;
}

//
// API
//

function void win32_xinput_poll_controllers()
{
    //
    //  check for new controllers
    //
    Controller *controllers = win32_controllers;

    // Poll for new connected controllers every 2 seconds

    // According to Microsoft, calling XInputGetState for not connected controllers can cause performance problems
    //  > For performance reasons, don't call XInputGetState for an 'empty' user slot every frame.
    //  > We recommend that you space out checks for new controllers every few seconds instead.

    static i32 xinput_poll_counter = 0;

    if (xinput_poll_counter > 0) 
    {
        xinput_poll_counter --;
    }
    else
    {
        // NOTE(nick): we use the first 4 indices to map directly to any XInput controllers
        for (u32 index = 0; index < XUSER_MAX_COUNT; index++)
        {
            Controller *it = &controllers[index];

            XINPUT_STATE state;
            if (XInputGetState(index, &state) == ERROR_SUCCESS)
            {
                if (!it->is_connected)
                {
                    it->button_count = 14;
                    it->axis_count = 6;
                    it->hat_count = 1;

                    it->is_connected = true;
                }
            }
            else
            {
                if (it)
                {
                    it->is_connected = false;
                }
            }
        }

        // @Incomplete @Cleanup: rough estimate of 2 seconds...
        xinput_poll_counter = (i32)(60 * 2.0f);
    }

    //
    // read input from all active controllers
    //

    for (u32 index = 0; index < XUSER_MAX_COUNT; index++)
    {
        Controller *it = &controllers[index];

        if (!it->is_connected) continue;

        XINPUT_STATE state;
        if (XInputGetState(it->index, &state) == ERROR_SUCCESS)
        {

            //@Speed: check dwPacketNumber (indicates if state has changed from last call)
            XINPUT_GAMEPAD *pad = &state.Gamepad;

            // axis
            it->axis[Axis_LeftX] = process_xinput_stick_value(pad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            it->axis[Axis_LeftY] = process_xinput_stick_value(pad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

            it->axis[Axis_RightX] = process_xinput_stick_value(pad->sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            it->axis[Axis_RightY] = process_xinput_stick_value(pad->sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

            it->axis[Axis_LeftTrigger] = pad->bLeftTrigger / 255.0f;
            it->axis[Axis_RightTrigger] = pad->bRightTrigger / 255.0f;

            // buttons
            button_set_state(&it->buttons[Button_A], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_A));
            button_set_state(&it->buttons[Button_B], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_B));
            button_set_state(&it->buttons[Button_X], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_X));
            button_set_state(&it->buttons[Button_Y], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_Y));

            button_set_state(&it->buttons[Button_LeftBumper], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER));
            button_set_state(&it->buttons[Button_RightBumper], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER));
            button_set_state(&it->buttons[Button_Start], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_START));
            button_set_state(&it->buttons[Button_Back], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_BACK));

            button_set_state(&it->buttons[Button_LeftStick], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_LEFT_THUMB));
            button_set_state(&it->buttons[Button_RightStick], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_RIGHT_THUMB));

            button_set_state(&it->buttons[Button_LeftTrigger], pad->bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
            button_set_state(&it->buttons[Button_RightTrigger], pad->bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);


            // @Incomplete: Button pressed events seem jacked up for the DPad?
            button_set_state(&it->buttons[Button_PadUp], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_UP));
            button_set_state(&it->buttons[Button_PadDown], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_DOWN));
            button_set_state(&it->buttons[Button_PadLeft], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_LEFT));
            button_set_state(&it->buttons[Button_PadRight], process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_RIGHT));

            // hats
            bool hat_up = process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_UP);
            bool hat_down = process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
            bool hat_left = process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
            bool hat_right = process_xinput_button_value(pad->wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);

            u8 hat = (hat_up << 0) | (hat_right << 1) | (hat_down << 2) | (hat_left << 3);
            it->hats[0] = hat;
        }
        else
        {
            it->is_connected = false;
        }
    }
}

function Controller *controller_from_index(u32 controller_index)
{
    Controller *result = NULL;
    if (controller_index >= 0 && controller_index < count_of(win32_controllers))
    {
        result = &win32_controllers[controller_index];
    }
    return result;
}


// NOTE(nick): the right motor is the high-frequency motor, the left motor is the low-frequency motor.
// They do not always need to be set to the same amount, as they provide different effects.
// The values should be in the range [0, 1]
function b32 controller_set_vibration(Controller *ctrl, f32 left_motor, f32 right_motor)
{
    bool result = false;
    if (ctrl)
    {
        if (ctrl->is_connected)
        {
            left_motor = Clamp(left_motor, 0, 1);
            right_motor = Clamp(right_motor, 0, 1);

            XINPUT_VIBRATION vibration = {0};
            vibration.wLeftMotorSpeed  = (u16)(left_motor * U16_MAX);
            vibration.wRightMotorSpeed = (u16)(right_motor * U16_MAX);

            result = XInputSetState(ctrl->index, &vibration) == ERROR_SUCCESS;
        }
    }
    return result;
}

#elif OS_MACOS
    // #include "macos/macos_window.mm"
    #define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

struct GLFW_Window
{
    Window data;

    GLFW_Window *next;
    GLFW_Window *prev;

    GLFWwindow *handle;

    int position[2];
    int size[2];
};

static Arena *glfw_events_arena = 0;
static Event_List *glfw_events_list = 0;

static Window_Resize_Proc *glfw_resize_callback = NULL;
static b32 glfw_platform_initted = false;

static Arena *glfw_global_arena = NULL;

static GLFW_Window *glfw_free_window = NULL;
static GLFW_Window *glfw_first_window = NULL;
static GLFW_Window *glfw_last_window = NULL;

function GLFW_Window *glfw_window_from_handle(Window *window)
{
    return (GLFW_Window *)window;
}

function GLFW_Window *glfw_window_from_internal(GLFWwindow *win)
{
    return (GLFW_Window *)glfwGetWindowUserPointer(win);
}

function Window *glfw_window_to_user(GLFW_Window *window)
{
    return (Window *)window;
}

function void glfw_window_size_callback(GLFWwindow *glfwwin, int width, int height) {
    if (glfw_resize_callback != NULL)
    {
        GLFW_Window *window = glfw_window_from_internal(glfwwin);
        if (window)
        {
            glfw_resize_callback(&window->data);
        }
    }
}

static Controller glfw_controllers[CONTROLLER_COUNT];

function void glfw_poll_events()
{
    glfwPollEvents();

    // mouse
    #if 0
    {
        Mouse *mouse = &platform.mouse;

        double xpos, ypos;
        glfwGetCursorPos(handle, &xpos, &ypos);
        mouse->position = v2(xpos, ypos);

        #if OS_MACOS
        mouse->position *= glfw_get_pixel_scale(handle);
        #endif

        button_set_state(&mouse->button_left, glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT));
        button_set_state(&mouse->button_right, glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_RIGHT));
        button_set_state(&mouse->button_middle, glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_MIDDLE));
        button_set_state(&mouse->button_x1, glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_4));
        button_set_state(&mouse->button_x2, glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_5));
    }
    #endif

    // keyboard
    // NOTE(nick): handled in glfw_key_callback

    // controllers
    {
        for (i32 index = GLFW_JOYSTICK_1; index < Min(GLFW_JOYSTICK_16, CONTROLLER_COUNT); index ++) {
            int present = glfwJoystickPresent(index);

            auto it = &glfw_controllers[index];
            it->index        = index;
            it->is_connected = present;

            if (!present) continue;

            if (glfwJoystickIsGamepad(index)) {
                GLFWgamepadstate state;

                if (!glfwGetGamepadState(index, &state)) continue;

                // axis
                it->axis[Axis_LeftX] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                it->axis[Axis_LeftY] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

                it->axis[Axis_RightX] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                it->axis[Axis_RightY] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

                it->axis[Axis_LeftTrigger] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
                it->axis[Axis_RightTrigger] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

                // buttons
                button_set_state(&it->buttons[Button_A], state.buttons[GLFW_GAMEPAD_BUTTON_A]);
                button_set_state(&it->buttons[Button_B], state.buttons[GLFW_GAMEPAD_BUTTON_B]);
                button_set_state(&it->buttons[Button_X], state.buttons[GLFW_GAMEPAD_BUTTON_X]);
                button_set_state(&it->buttons[Button_Y], state.buttons[GLFW_GAMEPAD_BUTTON_Y]);

                button_set_state(&it->buttons[Button_LeftBumper], state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]);
                button_set_state(&it->buttons[Button_RightBumper], state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]);
                button_set_state(&it->buttons[Button_Start], state.buttons[GLFW_GAMEPAD_BUTTON_START]);
                button_set_state(&it->buttons[Button_Back], state.buttons[GLFW_GAMEPAD_BUTTON_BACK]);

                button_set_state(&it->buttons[Button_LeftStick], state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]);
                button_set_state(&it->buttons[Button_RightStick], state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]);

                button_set_state(&it->buttons[Button_PadUp], state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]);
                button_set_state(&it->buttons[Button_PadDown], state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]);
                button_set_state(&it->buttons[Button_PadLeft], state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]);
                button_set_state(&it->buttons[Button_PadRight], state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]);

                // hats
                bool hat_up = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
                bool hat_down = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
                bool hat_left = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
                bool hat_right = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];

                u8 hat = (hat_up << 0) | (hat_right << 1) | (hat_down << 2) | (hat_left << 3);
                it->hats[0] = hat;

                it->button_count = 14;
                it->axis_count = 6;
                it->hat_count = 1;

            } else {
                int button_count;
                const unsigned char *buttons = glfwGetJoystickButtons(index, &button_count);
                if (button_count > CONTROLLER_MAX_BUTTONS) button_count = CONTROLLER_MAX_BUTTONS;

                int axis_count;
                const float *axis = glfwGetJoystickAxes(index, &axis_count);
                if (axis_count > CONTROLLER_MAX_AXIS) axis_count = CONTROLLER_MAX_AXIS;

                int hat_count;
                const unsigned char *hats = glfwGetJoystickHats(index, &hat_count);
                if (hat_count > CONTROLLER_MAX_HATS) hat_count = CONTROLLER_MAX_HATS;

                for (i32 i = 0; i < button_count; i++) {
                    button_set_state(&it->buttons[i], buttons[i]);
                }

                for (i32 i = 0; i < axis_count; i++) {
                    it->axis[i] = axis[i];
                }

                for (i32 i = 0; i < hat_count; i++) {
                    it->hats[i] = hats[i];
                }

                it->button_count = button_count;
                it->axis_count = axis_count;
                it->hat_count = hat_count;
            }
        }
    }
}

function bool window_init()
{
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

    glfw_global_arena = arena_alloc(Gigabytes(4));

    return true;
}

function void glfw_window_scroll_callback(GLFWwindow *win, double dx, double dy) {
    GLFW_Window *window = glfw_window_from_internal(win);
    if (window)
    {
        if (glfw_events_arena)
        {
            Event *event = PushArrayZero(glfw_events_arena, Event, 1);
            event->type = Event_Scroll;
            event->window = glfw_window_to_user(window);
            event->scroll.x = dx;
            event->scroll.y = dy;
            DLLPushBack(glfw_events_list->first, glfw_events_list->last, event);
            glfw_events_list->count += 1;
        }
    }
}

function void glfw_character_callback(GLFWwindow *win, unsigned int codepoint)
{
    GLFW_Window *window = glfw_window_from_internal(win);
    if (window)
    {
        if (glfw_events_arena)
        {
            Event *event = PushArrayZero(glfw_events_arena, Event, 1);
            event->type = Event_Text;
            event->window = glfw_window_to_user(window);
            event->character = codepoint;

            DLLPushBack(glfw_events_list->first, glfw_events_list->last, event);
            glfw_events_list->count += 1;
        }
    }
}

function void glfw_key_callback(GLFWwindow *win, int keycode, int scancode, int action, int mods) {
    static Keyboard_Key key_table[Key_COUNT] = {0};
    static bool key_table_initted = false;
    if (!key_table_initted)
    {
        key_table_initted = true;

        // @Robustness: verify this is a direct mapping
        // https://www.glfw.org/docs/3.3/group__keys.html

        for (int i = (int)Key_Space; i < (int)Key_COUNT; i += 1)
        {
            key_table[i] = (Keyboard_Key)i;
        }

        key_table[GLFW_MOUSE_BUTTON_LEFT] = Mouse_Left;
        key_table[GLFW_MOUSE_BUTTON_RIGHT] = Mouse_Right;
        key_table[GLFW_MOUSE_BUTTON_MIDDLE] = Mouse_Middle;
        key_table[GLFW_MOUSE_BUTTON_4] = Mouse_X1;
        key_table[GLFW_MOUSE_BUTTON_5] = Mouse_X2;
    }

    GLFW_Window *window = glfw_window_from_internal(win);
    if (window)
    {
        Keyboard_Key key = keycode < count_of(key_table) ? key_table[keycode] : Key_None;
        b32 is_down = action == GLFW_PRESS || action == GLFW_REPEAT;
        b32 was_down = action == GLFW_REPEAT;

        if (key != Key_None)
        {
            if (glfw_events_arena)
            {
                Keyboard_Mod modifiers = 0;
                if ((mods & GLFW_MOD_CONTROL) != 0) modifiers |= Mod_Ctrl;
                if ((mods & GLFW_MOD_SHIFT) != 0) modifiers |= Mod_Shift;
                if ((mods & GLFW_MOD_ALT) != 0) modifiers |= Mod_Alt;
                if ((mods & GLFW_MOD_SUPER) != 0) modifiers |= Mod_Super;

                Event *event = PushArrayZero(glfw_events_arena, Event, 1);
                event->type = is_down ? Event_KeyPress : Event_KeyRelease;
                event->window = glfw_window_to_user(window);
                event->is_repeat = is_down && was_down;
                event->key = key;
                event->modifiers = modifiers;
                DLLPushBack(glfw_events_list->first, glfw_events_list->last, event);
                glfw_events_list->count += 1;
            }
        }
    }
}

function void glfw_mouse_button_callback(GLFWwindow *win, int button, int action, int mods)
{
    // @Incomplete: map mouse buttons
    glfw_key_callback(win, button, 0, action, mods);
}

function Window *window_create(Vector2i size, String title, Window_Create_Params *optional_params)
{
    Window_Create_Params params = {0};
    if (optional_params)
    {
        MemoryCopyStruct(&params, optional_params);
    }

    if (!glfw_platform_initted)
    {
        window_init();
        glfw_platform_initted = true;
    }

    GLFW_Window *window = glfw_free_window;
    {
        if (window)
        {
            glfw_free_window = glfw_free_window->next;
            MemoryZeroStruct(window);
        }
        else
        {
            window = PushArrayZero(glfw_global_arena, GLFW_Window, 1);
        }
        DLLPushBack(glfw_first_window, glfw_last_window, window);
    }

    M_Temp scratch = GetScratch(0, 0);

    char *title_str = string_to_cstr(scratch.arena, title);
    GLFWwindow *handle = glfwCreateWindow(size.width, size.height, title_str, NULL, NULL);
    if (!handle)
    {
        return NULL;
    }

    // NOTE(nick): center window in monitor
    if (params.center)
    {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        if (monitor) {
            GLFWvidmode *mode = (GLFWvidmode *)glfwGetVideoMode(monitor);

            if (mode) {
                int monitorX, monitorY;
                glfwGetMonitorPos(monitor, &monitorX, &monitorY);

                int windowWidth, windowHeight;
                glfwGetWindowSize(handle, &windowWidth, &windowHeight);

                glfwSetWindowPos(
                    handle,
                    monitorX + (mode->width - windowWidth) / 2,
                    monitorY + (mode->height - windowHeight) / 2);
            }
        }
    }

    ReleaseScratch(scratch);

    glfwSetWindowUserPointer(handle, window);
    glfwSetWindowSizeCallback(handle, glfw_window_size_callback);
    glfwSetScrollCallback(handle, glfw_window_scroll_callback);
    glfwSetCharCallback(handle, glfw_character_callback);
    glfwSetKeyCallback(handle, glfw_key_callback);
    glfwSetMouseButtonCallback(handle, glfw_mouse_button_callback);

    window->handle = handle;
    Window *result = (Window *)window;
    return result;
}

function void window_destroy(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        DLLRemove(glfw_first_window, glfw_last_window, it);
        StackPush(glfw_free_window, it);

        if (it->handle)
        {
            glfwDestroyWindow(it->handle);
        }
    }
}

function Event_List window_get_events(Arena *arena)
{
    Event_List result = {0};
    glfw_events_arena = arena;
    glfw_events_list = &result;

    glfw_poll_events();

    return result;
}


function void window_set_resize_callback(Window *window, Window_Resize_Proc *callback)
{
    // @Incomplete: this should be per-window
    // Also maybe consider providing a global thing?
    // :WindowResizeCallbackSituation
    glfw_resize_callback = callback;
}

function bool window_should_close(Window *window)
{
    bool result = false;
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        result = glfwWindowShouldClose(it->handle);
    }
    return result;
}

function void window_show(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwShowWindow(it->handle);
    }
}

function void window_hide(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwHideWindow(it->handle);
    }
}

function void window_maximize(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwMaximizeWindow(it->handle);
    }
}

function void window_minimize(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwIconifyWindow(it->handle);
    }
}

function NSWindow *window_get_native_handle(Window *window)
{
    NSWindow *result = NULL;
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        result = glfwGetCocoaWindow(it->handle);
    }
    return result;
}

function void window_set_fullscreen(Window *window, bool fullscreen)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        int *position = it->position;
        int *size     = it->size;

        if (fullscreen)
        {
            // backup window position and window size
            glfwGetWindowPos(it->handle, &position[0], &position[1]);
            glfwGetWindowSize(it->handle, &size[0], &size[1]);

            // get resolution of monitor
            auto monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);

            // switch to full screen
            glfwSetWindowMonitor(it->handle, monitor, 0, 0, mode->width, mode->height, 0);
        }
        else
        {
            // restore last window size and position
            glfwSetWindowMonitor(it->handle, NULL, position[0], position[1], size[0], size[1], 0);
        }
    }
}

function void window_set_title(Window *window, String title)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        M_Temp scratch = GetScratch(0, 0);
        glfwSetWindowTitle(it->handle, string_to_cstr(scratch.arena, title));
        ReleaseScratch(scratch);
    }
}

function void window_set_cursor(Window *window, Cursor_Style cursor)
{
    //
    // @Incomplete: GLFW doesn't provide a reasonable set of cursors.... we will have to just shill out to
    // the OS directly.
    // :GlfwCursorClipping
    //

    // @See: https://developer.apple.com/documentation/appkit/nscursor

    #if OS_MACOS

    #if 0
    - (vod)someSetup;
    {
        NSTrackingArea *const trackingArea = [[NSTrackingArea alloc] initWithRect:NSZeroRect options: (NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways | NSTrackingInVisibleRect) owner:self userInfo:nil];
        [self.view addTrackingArea:trackingArea];
    }

    - (void)mouseEntered:(NSEvent *)theEvent;
    {
        [[NSCursor IBeamCursor] push];
    }

    - (void)mouseExited:(NSEvent *)theEvent;
    {
        [[NSCursor IBeamCursor] pop];
    }
    #endif

    NSWindow *handle = window_get_native_handle(window);
    if (handle)
    {
        NSCursor *value = NULL;
        switch (cursor)
        {
            case Cursor_Hand: { value = [NSCursor pointingHandCursor]; } break;
        }
        
        Unused(value);

        // if (!value) value = [NSCursor arrow];
        // [handle setCursor:value];
    }

    #endif


    GLFW_Window *it = glfw_window_from_handle(window);
    if (cursor == Cursor_Hidden)
    {
        glfwSetInputMode(it->handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else
    {
        glfwSetInputMode(it->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

function void window_set_icon(Window *window, Vector2i size, String rgba_data)
{
    // @Incomplete
}

function void window_set_size(Window *window, Vector2i size)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwSetWindowSize(it->handle, size.x, size.y);
    }
}

function void window_set_position(Window *window, Vector2i p)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwSetWindowPos(it->handle, p.x, p.y);
    }
}

function void window_set_minimum_size(Window *window, Vector2i minimum_size)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwSetWindowSizeLimits(it->handle, minimum_size.x, minimum_size.y, GLFW_DONT_CARE, GLFW_DONT_CARE);
    }
}

function void window_set_mouse_trap(Window *window, b32 trap)
{
    assert(!"Not implemented");

    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
    }
}

function void glfw_window_mouse_constrain(Window *window, bool constrain)
{
    //
    // NOTE(nick): glfw doesn't support cursor clipping in the native windows
    // sense. So, this function and window_set_cursor are not mututally
    // exclusive!!!!!
    // :GlfwCursorClipping
    //
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        if (constrain) {
            glfwSetInputMode(it->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            // @Robustness: could save and restore the cursor visibility state here
            glfwSetInputMode(it->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

function void window_set_mouse_position(Window *window, Vector2i position)
{
    // @Incoomplte:
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwSetCursorPos(it->handle, position.x, position.y);
    }
}

function bool window_is_focused(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    bool result = false;
    if (it)
    {
        result = glfwGetWindowAttrib(it->handle, GLFW_FOCUSED);
    }
    return result;
}

function bool window_is_maximized(Window *window)
{
    bool result = false;
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        // @Incomplete
    }
    return result;
}

function bool window_is_fullscreen(Window *window)
{
    bool result = false;
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        result = glfwGetWindowMonitor(it->handle) != NULL;
    }
    return result;
}

function Rectangle2 window_get_client_rect(Window *window)
{
    Rectangle2 result = {0};
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        int width, height;
        glfwGetFramebufferSize(it->handle, &width, &height);

        // @Incomplete: y should be shifted by (fb height - w height)?

        result = {(f32)0, (f32)0, (f32)(width), (f32)(height)};
    }
    return result;
}

function Rectangle2 window_get_rect(Window *window)
{
    Rectangle2 result = {0};
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        int x, y;
        glfwGetWindowPos(it->handle, &x, &y);
        int width, height;
        glfwGetWindowSize(it->handle, &width, &height);

        result = {(f32)x, (f32)y, (f32)(x + width), (f32)(y + height)};
    }
    return result;
}

function f32 window_get_refresh_rate(Window *window)
{
    f32 result = 0;
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        // @Hack: missing `monitorFromWindow` or equivalant
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        result = 1.0/mode->refreshRate;
    }
    return result;
}

function f32 window_get_pixel_scale(Window *window)
{
    f32 result = 0;
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        float scalex, scaley;
        glfwGetWindowContentScale(it->handle, &scalex, &scaley);
        result = scalex;
    }
    return result;
}

function Vector2 window_get_mouse_position(Window *window)
{
    Vector2 result = {0};
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        double xpos, ypos;
        glfwGetCursorPos(it->handle, &xpos, &ypos);
        result = {(f32)xpos, (f32)ypos};
        f32 scale = window_get_pixel_scale(window);
        result.x *= scale;
        result.y *= scale;
    }
    return result;
}

function void window_swap_buffers(Window *window)
{
    GLFW_Window *it = glfw_window_from_handle(window);
    if (it)
    {
        glfwSwapBuffers(it->handle);
    }
}
#elif OS_LINUX
    #error Not implemented
#endif

//
// Shared:
//

function String mouse_to_string(Mouse_Button button)
{
    switch (button)
    {
        case Mouse_Left: return S("Mouse Left");
        case Mouse_Right: return S("Mouse Right");
        case Mouse_Middle: return S("Mouse Middle");
        case Mouse_X1: return S("Mouse X1");
        case Mouse_X2: return S("Mouse X2");
    }

    return S("<unknown>");
}

function String key_to_string(Keyboard_Key key)
{
    switch (key)
    {
        case Key_None: return S("None");
            
        case Mouse_Left: return S("Mouse Left");
        case Mouse_Right: return S("Mouse Right");
        case Mouse_Middle: return S("Mouse Middle");
        case Mouse_X1: return S("Mouse X1");
        case Mouse_X2: return S("Mouse X2");

        case Key_Space:  return S("Space");
        case Key_Quote:  return S("'");
        case Key_Comma:  return S(",");
        case Key_Minus:  return S("-");
        case Key_Period: return S(".");
        case Key_Slash:  return S("/");

        case Key_0: return S("0");
        case Key_1: return S("1");
        case Key_2: return S("2");
        case Key_3: return S("3");
        case Key_4: return S("4");
        case Key_5: return S("5");
        case Key_6: return S("6");
        case Key_7: return S("7");
        case Key_8: return S("8");
        case Key_9: return S("9");

        case Key_Semicolon: return S(";");
        case Key_Equal:     return S("=");

        case Key_A: return S("A");
        case Key_B: return S("B");
        case Key_C: return S("C");
        case Key_D: return S("D");
        case Key_E: return S("E");
        case Key_F: return S("F");
        case Key_G: return S("G");
        case Key_H: return S("H");
        case Key_I: return S("I");
        case Key_J: return S("J");
        case Key_K: return S("K");
        case Key_L: return S("L");
        case Key_M: return S("M");
        case Key_N: return S("N");
        case Key_O: return S("O");
        case Key_P: return S("P");
        case Key_Q: return S("Q");
        case Key_R: return S("R");
        case Key_S: return S("S");
        case Key_T: return S("T");
        case Key_U: return S("U");
        case Key_V: return S("V");
        case Key_W: return S("W");
        case Key_X: return S("X");
        case Key_Y: return S("Y");
        case Key_Z: return S("Z");

        case Key_LeftBracket:  return S("[");
        case Key_Backslash:    return S("\\");
        case Key_RightBracket: return S("]");
        case Key_Grave:        return S("`");

        case Key_Escape:    return S("Escape");
        case Key_Enter:     return S("Enter");
        case Key_Tab:       return S("Tab");
        case Key_Backspace: return S("Backspace");
        case Key_Insert:    return S("Insert");
        case Key_Delete:    return S("Delete");
        case Key_Right:     return S("Right");
        case Key_Left:      return S("Left");
        case Key_Down:      return S("Down");
        case Key_Up:        return S("Up");
        case Key_PageUp:    return S("Page Up");
        case Key_PageDown:  return S("Page Down");
        case Key_Home:      return S("Home");
        case Key_End:       return S("End");

        case Key_CapsLock:    return S("Caps Lock");
        case Key_ScrollLock:  return S("Scroll Lock");
        case Key_NumLock:     return S("Num Lock");
        case Key_PrintScreen: return S("Print Screen");
        case Key_Pause:       return S("Pause");

        case Key_F1:  return S("F1");
        case Key_F2:  return S("F2");
        case Key_F3:  return S("F3");
        case Key_F4:  return S("F4");
        case Key_F5:  return S("F5");
        case Key_F6:  return S("F6");
        case Key_F7:  return S("F7");
        case Key_F8:  return S("F8");
        case Key_F9:  return S("F9");
        case Key_F10: return S("F10");
        case Key_F11: return S("F11");
        case Key_F12: return S("F12");
        case Key_F13: return S("F13");
        case Key_F14: return S("F14");
        case Key_F15: return S("F15");
        case Key_F16: return S("F16");
        case Key_F17: return S("F17");
        case Key_F18: return S("F18");
        case Key_F19: return S("F19");
        case Key_F20: return S("F20");
        case Key_F21: return S("F21");
        case Key_F22: return S("F22");
        case Key_F23: return S("F23");
        case Key_F24: return S("F24");

        case Key_Numpad0: return S("Numpad 0");
        case Key_Numpad1: return S("Numpad 1");
        case Key_Numpad2: return S("Numpad 2");
        case Key_Numpad3: return S("Numpad 3");
        case Key_Numpad4: return S("Numpad 4");
        case Key_Numpad5: return S("Numpad 5");
        case Key_Numpad6: return S("Numpad 6");
        case Key_Numpad7: return S("Numpad 7");
        case Key_Numpad8: return S("Numpad 8");
        case Key_Numpad9: return S("Numpad 9");
        case Key_NumpadDot: return S("Numpad .");
        case Key_NumpadDivide: return S("Numpad /");
        case Key_NumpadMultiply: return S("Numpad *");
        case Key_NumpadSubtract: return S("Numpad -");
        case Key_NumpadAdd: return S("Numpad +");
        case Key_NumpadEnter: return S("Numpad Enter");
        case Key_NumpadEquals: return S("Numpad =");

        case Key_LeftShift: return S("Left Shift");
        case Key_LeftCtrl: return S("Left Ctrl");
        case Key_LeftAlt: return S("Left Alt");
        case Key_LeftSuper: return S("Left Super");
        case Key_RightShift: return S("Right Shift");
        case Key_RightCtrl: return S("Right Ctrl");
        case Key_RightAlt: return S("Right Alt");
        case Key_RightSuper: return S("Right Super");

        case Key_Menu: return S("Menu");

        case Key_MediaNext: return S("Media Next");
        case Key_MediaPrev: return S("Media Prev");
        case Key_MediaPlay: return S("Media Play");
        case Key_MediaStop: return S("Media Stop");
    }

    return S("<unknown>");
}

function String event_to_string(Event *it)
{
    String table[] = {
        S("Null"),
        S("WindowClose"),
        S("WindowFocus"),
        S("WindowBlur"),
        S("KeyPress"),
        S("KeyRelease"),
        S("Scroll"),
        S("Text"),
        S("DropFile"),
    };
    StaticAssert(count_of(table) == Event_COUNT, "Make the things match or you're fired!");

    String type = S("<Unknown>");
    if (it->type >= 0 && it->type <= count_of(table))
    {
        type = table[it->type];
    }

    String key = key_to_string(it->key);
    String is_repeat = b32_to_string(it->is_repeat);

    return sprint("Event{ type=%S, window=%p, key=%S, modifiers=%d, is_repeat=%S, character=%lu, position=Vector2{%f,%f}, scroll=Vector2{%f,%f}, path=\"%S\"}",
        type, it->window, key, it->modifiers, is_repeat, it->character, it->position.x, it->position.y, it->scroll.x, it->scroll.y, it->path );
}

function Vector2 window_get_client_size(Window *window)
{
    return r2_size(window_get_client_rect(window));
}

function Vector2 window_get_frame_size(Window *window)
{
    return r2_size(window_get_frame_rect(window));
}

function Vector2 window_get_position(Window *window)
{
    return window_get_frame_rect(window).p0;
}

//
// Controller
//

function void button_set_state(Button_State *state, b32 is_down) {
    b32 was_down = (*state & ButtonState_Down) != 0;
    is_down = is_down != 0; // NOTE(bill): Make sure it's a boolean

    SetFlagState(*state, ButtonState_Down,     is_down);
    SetFlagState(*state, ButtonState_Down,     is_down);
    SetFlagState(*state, ButtonState_Pressed,  !was_down && is_down);
    SetFlagState(*state, ButtonState_Released, was_down && !is_down);
}

function b32 controller_pressed(Controller *ctrl, Controller_Button button) {
    assert(button < CONTROLLER_MAX_BUTTONS);
    return (ctrl->buttons[button] & ButtonState_Down) != 0;
}

function b32 controller_released(Controller *ctrl, Controller_Button button) {
    assert(button < CONTROLLER_MAX_BUTTONS);
    return (ctrl->buttons[button] & ButtonState_Released) != 0;
}

function b32 controller_down(Controller *ctrl, Controller_Button button) {
    assert(button < CONTROLLER_MAX_BUTTONS);
    return (ctrl->buttons[button] & ButtonState_Down) != 0;
}

function Vector2 controller_get_axis(Controller *ctrl, u32 axis_index)
{
    assert(axis_index < (CONTROLLER_MAX_AXIS / 2));
    f32 x = ctrl->axis[axis_index * 2 + 0];
    f32 y = ctrl->axis[axis_index * 2 + 1];
    return v2(x, y);
}

function String button_to_string(Controller_Button button)
{
    String string_table[] = {
        S("A"),
        S("B"),
        S("X"),
        S("Y"),
        S("Start"),
        S("Back"),
        S("Left Stick"),
        S("Right Stick"),
        S("Left Bumper"),
        S("Right Bumper"),
        S("Left Trigger"),
        S("Right Trigger"),
        S("Pad Up"),
        S("Pad Down"),
        S("Pad Left"),
        S("Pad Right"),
    };
    if (button >= 0 && button < count_of(string_table))
    {
        return string_table[button];
    }
    return S("<unknown>");
}

function String axis_to_string(Controller_Axis axis)
{
    switch (axis)
    {
        case Axis_LeftX: return S("Left Thumb X");
        case Axis_LeftY: return S("Left Thumb Y");
        case Axis_RightX: return S("Right Thumb X");
        case Axis_RightY: return S("Right Thumb Y");
        case Axis_LeftTrigger: return S("Left Trigger");
        case Axis_RightTrigger: return S("Right Trigger");
    }

    return S("<unknown>");
}

function String hat_to_string(Controller_Hat hat)
{
    switch (hat)
    {
        case Hat_Up: return S("Hat Up");
        case Hat_Left: return S("Hat Left");
        case Hat_Down: return S("Hat Down");
        case Hat_Right: return S("Hat Right");
    }

    return S("<unknown>");
}

function Event_List event_list_copy(Arena *arena, Event_List events)
{
    Event_List result = {0};
    result.count = events.count;

    Event *events_copy = PushArrayZero(arena, Event, events.count);
    Event *copy = &events_copy[0];

    for (Event *it = events.first; it != NULL; it = it->next)
    {
        MemoryCopy(copy, it, sizeof(Event));
        DLLPushBack(result.first, result.last, copy);
        copy += 1;
    }

    return result;
}

function Event_List event_list_for_window(Arena *arena, Window *window)
{
    Event_List result = event_list_copy(arena, window->events);
    for (Event *it = window->events.first; it != NULL; it = it->next)
    {
        if (it->window != window)
        {
            DLLRemove(result.first, result.last, it);
        }
    }

    return result;
}

function void eat_event(Event_List *events, Event *event)
{
    Event *next = event->next;
    Event *prev = event->prev;
    DLLRemove(events->first, events->last, event);
    events->count -= 1;
    event->next = next;
    event->prev = prev;
}

function void event_push(Event_List *events, Event *event)
{
    DLLPushBack(events->first, events->last, event);
    events->count += 1;
}

function b32 key_press_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods)
{
    Event_List *events = &window->events;

    b32 result = false;
    for (Event *e = events->first; e != 0; e = e->next)
    {
        if (e->type == Event_KeyPress && !e->is_repeat && window == e->window &&
           (e->key == key) &&
           ((e->modifiers & mods) == mods || mods == Mod_Any || (mods == Mod_None && e->modifiers == 0)))
        {
            result = true;
            break;
        }
    }
    return result;
}

function b32 key_press(Window *window, Keyboard_Key key)
{
    return key_press_mods(window, key, Mod_Any);
}

function b32 key_repeat_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods)
{
    Event_List *events = &window->events;

    b32 result = false;
    for (Event *e = events->first; e != 0; e = e->next)
    {
        if (e->type == Event_KeyPress && e->is_repeat && window == e->window &&
           (e->key == key) &&
           ((e->modifiers & mods) == mods || mods == Mod_Any || (mods == Mod_None && e->modifiers == 0)))
        {
            result = true;
            break;
        }
    }
    return result;
}

function b32 key_repeat(Window *window, Keyboard_Key key)
{
    return key_repeat_mods(window, key, Mod_Any);
}

function b32 key_release_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods)
{
    Event_List *events = &window->events;

    b32 result = false;
    for (Event *e = events->first; e != 0; e = e->next)
    {
        if (e->type == Event_KeyRelease && window == e->window &&
           (e->key == key) &&
           ((e->modifiers & mods) == mods || mods == Mod_Any || (mods == Mod_None && e->modifiers == 0)))
        {
            result = true;
            break;
        }
    }
    return result;
}

function b32 key_release(Window *window, Keyboard_Key key)
{
    return key_release_mods(window, key, Mod_Any);
}

function b32 key_down(Window *window, Keyboard_Key key)
{
    return window->key_states[key];
}

function b32 key_press_or_repeat_mods(Window *window, Keyboard_Key key, Keyboard_Mod mods)
{
    return key_press_mods(window, key, mods) || key_repeat_mods(window, key, mods);
}

function b32 key_press_or_repeat(Window *window, Keyboard_Key key)
{
    return key_press_mods(window, key, Mod_Any) || key_repeat_mods(window, key, Mod_Any);
}

function Vector2 window_get_mouse_scroll(Window *window)
{
    Vector2 result = {0};
    Event_List *events = &window->events;
    for (Event *event = events->first; event != NULL; event = event->next)
    {
        if (event->type == Event_Scroll)
        {
            result = v2_add(result, event->scroll);
        }
    }
    return result;
}

function void window_set_target_dt(Window *window, f64 target_dt)
{
    if (target_dt < 0) target_dt = 0;
    window->target_dt = target_dt;
}

function f64 window_begin_frame(Arena *arena, Window *window)
{
    u64 current_fps = (u64)window_get_refresh_rate(window);
    if (current_fps <= 1) current_fps = 60;

    f64 target_dt = window->target_dt;
    if (!target_dt)
    {
        target_dt = 1.0 / current_fps;
    }

    if (!window->then)
    {
        window->then = os_time();
    }

    f64 now = os_time();
    f64 uncapped_dt = now - window->then;
    // NOTE(nick): limit large spikes in framerate
    f64 dt = uncapped_dt;
    if (dt > 0.25) dt = 0.25;

    timing_update(&window->frame_info, uncapped_dt, current_fps);

    window->then      = now;
    window->dt        = dt;
    window->uncapped_dt  = uncapped_dt;
    window->target_dt    = target_dt;

    Event_List events = window_get_events(arena);
    window->events = events;
    
    for (Event *e = events.first; e != 0; e = e->next)
    {
        if (e->type == Event_KeyPress) { window->key_states[e->key] = true; }
        if (e->type == Event_KeyRelease) { window->key_states[e->key] = false; }
    }

    return dt;
}

function void window_end_frame(Window *window)
{
    MemoryZeroStruct(&window->events);

    f64 now = os_time();
    // f64 remaining_seconds = target_dt - (now - then);

    // NOTE(nick): wait for next frame
    if (window_is_focused(window))
    {
        while ((now - window->then) < window->target_dt)
        {
            now = os_time();

            f64 wait_s = window->target_dt - (now - window->then);
            if (wait_s > 2.0 / 1000.0)
            {
                // NOTE(nick): sleep for 80% of the time to account for sleep inaccuracies
                os_sleep(wait_s * 0.8);
            }
        }
    }
    else
    {
        // NOTE(nick): we're in the background so we don't have to care too much about exact timing.
        now = os_time();
        if (now - window->then < window->target_dt) {
            os_sleep(window->target_dt);
        }
    }
}

static Window *g_current_window = NULL;

function void window_set_current(Window *window)
{
    g_current_window = window;
}

function b32 keyboard_pressed(Keyboard_Key key)
{
    assert(g_current_window);
    return key_press_mods(g_current_window, key, Mod_Any);
}

function b32 keyboard_repeat(Keyboard_Key key)
{
    assert(g_current_window);
    return key_repeat_mods(g_current_window, key, Mod_Any);
}

function b32 keyboard_pressed_or_repeat(Keyboard_Key key)
{
    assert(g_current_window);
    return key_press_or_repeat_mods(g_current_window, key, Mod_Any);
}

function b32 keyboard_released(Keyboard_Key key)
{
    assert(g_current_window);
    return key_release_mods(g_current_window, key, Mod_Any);
}

function b32 keyboard_down(Keyboard_Key key)
{
    assert(g_current_window);
    return key_down(g_current_window, key);
}

// function b32 keyboard_mods(Keyboard_Mod mods)
// {
// }

function Vector2 mouse_get_position()
{
    assert(g_current_window);
    return window_get_mouse_position(g_current_window);
}

function Vector2 mouse_get_scroll()
{
    assert(g_current_window);
    return window_get_mouse_scroll(g_current_window);
}

function b32 mouse_pressed(Keyboard_Key key)
{
    assert(g_current_window);
    return key_press_mods(g_current_window, key, Mod_Any);
}

function b32 mouse_released(Keyboard_Key key)
{
    assert(g_current_window);
    return key_release_mods(g_current_window, key, Mod_Any);
}

function b32 mouse_down(Keyboard_Key key)
{
    assert(g_current_window);
    return key_down(g_current_window, key);
}


#endif // NA_H_IMPL
#endif // impl
