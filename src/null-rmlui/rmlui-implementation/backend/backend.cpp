#include <windows.h>

#include "backend.h"

namespace null::rml::backend {
    int wnd_proc(Rml::Context* context, HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        if(!context) return -1;

        static bool tracking_mouse_leave{ };

        switch(message) {
            case WM_SIZE: {
                context->SetDimensions(Rml::Vector2i(LOWORD(l_param), HIWORD(l_param)));
            } return 0;

            case WM_LBUTTONDOWN: { SetCapture(window_handle); if(!context->ProcessMouseButtonDown(0, get_key_modifier_state())) return 0; } break;
            case WM_LBUTTONUP: { ReleaseCapture(); if(!context->ProcessMouseButtonUp(0, get_key_modifier_state())) return 0; } break;
            case WM_RBUTTONDOWN: { if(!context->ProcessMouseButtonDown(1, get_key_modifier_state())) return 0; } break;
            case WM_RBUTTONUP: { if(!context->ProcessMouseButtonUp(1, get_key_modifier_state())) return 0; } break;
            case WM_MBUTTONDOWN: { if(!context->ProcessMouseButtonDown(2, get_key_modifier_state())) return 0; } break;
            case WM_MBUTTONUP: { if(!context->ProcessMouseButtonUp(2, get_key_modifier_state())) return 0; } break;
            case WM_XBUTTONDOWN: { if(!context->ProcessMouseButtonDown(2 + GET_XBUTTON_WPARAM(w_param), get_key_modifier_state())) return 0; } break;
            case WM_XBUTTONUP: { if(!context->ProcessMouseButtonUp(2 + GET_XBUTTON_WPARAM(w_param), get_key_modifier_state())) return 0; } break;

            case WM_MOUSEWHEEL: { if(!context->ProcessMouseWheel((float)((short)HIWORD(w_param)) / (float)(-WHEEL_DELTA), get_key_modifier_state())) return 0; } break;
            case WM_MOUSEMOVE: {
                if(!tracking_mouse_leave) {
                    TRACKMOUSEEVENT tme{
                        .cbSize{ sizeof(TRACKMOUSEEVENT) },
                        .dwFlags{ TME_LEAVE },
                        .hwndTrack{ window_handle },
                    };
                    tracking_mouse_leave = TrackMouseEvent(&tme);
                }

                if(!context->ProcessMouseMove((short)LOWORD(l_param), (short)HIWORD(l_param), get_key_modifier_state())) return 0;
            } break;

            case WM_MOUSELEAVE: { tracking_mouse_leave = false; if(!context->ProcessMouseLeave()) return 0; } break;

            case WM_KEYDOWN: { if(!context->ProcessKeyDown(convert_key((int)w_param), get_key_modifier_state())) return 0; } break;
            case WM_KEYUP: { if(!context->ProcessKeyUp(convert_key((int)w_param), get_key_modifier_state())) return 0; } break;

            case WM_CHAR: {
                static wchar_t first_u16_code_unit{ };

                wchar_t c = (wchar_t)w_param;
                Rml::Character character{ c };

                if(c >= 0xD800 && c < 0xDC00) {
                    first_u16_code_unit = c;
                } else {
                    if(c >= 0xDC00 && c < 0xE000 && first_u16_code_unit != 0) character = Rml::StringUtilities::ToCharacter(convert_to_utf8(std::wstring(first_u16_code_unit, c)).data());
                    else if(c == '\r') character = (Rml::Character)'\n';

                    first_u16_code_unit = 0;

                    if((((char32_t)character >= 32 || character == (Rml::Character)'\n') && character != (Rml::Character)127) && !context->ProcessTextInput(character)) return 0;
                }
            }
                        break;
        }

        return -1;
    }

    std::string convert_to_utf8(std::wstring_view wstr) {
        std::string str(WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), NULL, 0, NULL, NULL), 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, &str[0], str.size(), NULL, NULL);
        return str;
    }

    std::wstring convert_to_utf16(std::string_view str) {
        std::wstring wstr(MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0), 0);
        MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &wstr[0], wstr.size());
        return wstr;
    }

    int get_key_modifier_state() {
        int key_modifier_state{ };

        if(GetKeyState(VK_CAPITAL) & 1) key_modifier_state |= Rml::Input::KM_CAPSLOCK;
        if(GetKeyState(VK_NUMLOCK) & 1) key_modifier_state |= Rml::Input::KM_NUMLOCK;
        if(HIWORD(GetKeyState(VK_SHIFT)) & 1) key_modifier_state |= Rml::Input::KM_SHIFT;
        if(HIWORD(GetKeyState(VK_CONTROL)) & 1) key_modifier_state |= Rml::Input::KM_CTRL;
        if(HIWORD(GetKeyState(VK_MENU)) & 1) key_modifier_state |= Rml::Input::KM_ALT;

        return key_modifier_state;
    }

    Rml::Input::KeyIdentifier convert_key(int win32_key_code) {
#define make_case(from, to) case from: return to;
        switch(win32_key_code) {
            make_case('A', Rml::Input::KI_A)
                make_case('B', Rml::Input::KI_B)
                make_case('C', Rml::Input::KI_C)
                make_case('D', Rml::Input::KI_D)
                make_case('E', Rml::Input::KI_E)
                make_case('F', Rml::Input::KI_F)
                make_case('G', Rml::Input::KI_G)
                make_case('H', Rml::Input::KI_H)
                make_case('I', Rml::Input::KI_I)
                make_case('J', Rml::Input::KI_J)
                make_case('K', Rml::Input::KI_K)
                make_case('L', Rml::Input::KI_L)
                make_case('M', Rml::Input::KI_M)
                make_case('N', Rml::Input::KI_N)
                make_case('O', Rml::Input::KI_O)
                make_case('P', Rml::Input::KI_P)
                make_case('Q', Rml::Input::KI_Q)
                make_case('R', Rml::Input::KI_R)
                make_case('S', Rml::Input::KI_S)
                make_case('T', Rml::Input::KI_T)
                make_case('U', Rml::Input::KI_U)
                make_case('V', Rml::Input::KI_V)
                make_case('W', Rml::Input::KI_W)
                make_case('X', Rml::Input::KI_X)
                make_case('Y', Rml::Input::KI_Y)
                make_case('Z', Rml::Input::KI_Z)

                make_case('0', Rml::Input::KI_0)
                make_case('1', Rml::Input::KI_1)
                make_case('2', Rml::Input::KI_2)
                make_case('3', Rml::Input::KI_3)
                make_case('4', Rml::Input::KI_4)
                make_case('5', Rml::Input::KI_5)
                make_case('6', Rml::Input::KI_6)
                make_case('7', Rml::Input::KI_7)
                make_case('8', Rml::Input::KI_8)
                make_case('9', Rml::Input::KI_9)

                make_case(VK_BACK, Rml::Input::KI_BACK)
                make_case(VK_TAB, Rml::Input::KI_TAB)

                make_case(VK_CLEAR, Rml::Input::KI_CLEAR)
                make_case(VK_RETURN, Rml::Input::KI_RETURN)

                make_case(VK_PAUSE, Rml::Input::KI_PAUSE)
                make_case(VK_CAPITAL, Rml::Input::KI_CAPITAL)

                make_case(VK_KANA, Rml::Input::KI_KANA)
                make_case(VK_JUNJA, Rml::Input::KI_JUNJA)
                make_case(VK_FINAL, Rml::Input::KI_FINAL)
                make_case(VK_HANJA, Rml::Input::KI_HANJA)

                make_case(VK_ESCAPE, Rml::Input::KI_ESCAPE)

                make_case(VK_CONVERT, Rml::Input::KI_CONVERT)
                make_case(VK_NONCONVERT, Rml::Input::KI_NONCONVERT)
                make_case(VK_ACCEPT, Rml::Input::KI_ACCEPT)
                make_case(VK_MODECHANGE, Rml::Input::KI_MODECHANGE)

                make_case(VK_SPACE, Rml::Input::KI_SPACE)
                make_case(VK_PRIOR, Rml::Input::KI_PRIOR)
                make_case(VK_NEXT, Rml::Input::KI_NEXT)
                make_case(VK_END, Rml::Input::KI_END)
                make_case(VK_HOME, Rml::Input::KI_HOME)
                make_case(VK_LEFT, Rml::Input::KI_LEFT)
                make_case(VK_UP, Rml::Input::KI_UP)
                make_case(VK_RIGHT, Rml::Input::KI_RIGHT)
                make_case(VK_DOWN, Rml::Input::KI_DOWN)
                make_case(VK_SELECT, Rml::Input::KI_SELECT)
                make_case(VK_PRINT, Rml::Input::KI_PRINT)
                make_case(VK_EXECUTE, Rml::Input::KI_EXECUTE)
                make_case(VK_SNAPSHOT, Rml::Input::KI_SNAPSHOT)
                make_case(VK_INSERT, Rml::Input::KI_INSERT)
                make_case(VK_DELETE, Rml::Input::KI_DELETE)
                make_case(VK_HELP, Rml::Input::KI_HELP)

                make_case(VK_LWIN, Rml::Input::KI_LWIN)
                make_case(VK_RWIN, Rml::Input::KI_RWIN)
                make_case(VK_APPS, Rml::Input::KI_APPS)

                make_case(VK_SLEEP, Rml::Input::KI_SLEEP)

                make_case(VK_NUMPAD0, Rml::Input::KI_NUMPAD0)
                make_case(VK_NUMPAD1, Rml::Input::KI_NUMPAD1)
                make_case(VK_NUMPAD2, Rml::Input::KI_NUMPAD2)
                make_case(VK_NUMPAD3, Rml::Input::KI_NUMPAD3)
                make_case(VK_NUMPAD4, Rml::Input::KI_NUMPAD4)
                make_case(VK_NUMPAD5, Rml::Input::KI_NUMPAD5)
                make_case(VK_NUMPAD6, Rml::Input::KI_NUMPAD6)
                make_case(VK_NUMPAD7, Rml::Input::KI_NUMPAD7)
                make_case(VK_NUMPAD8, Rml::Input::KI_NUMPAD8)
                make_case(VK_NUMPAD9, Rml::Input::KI_NUMPAD9)
                make_case(VK_MULTIPLY, Rml::Input::KI_MULTIPLY)
                make_case(VK_ADD, Rml::Input::KI_ADD)
                make_case(VK_SEPARATOR, Rml::Input::KI_SEPARATOR)
                make_case(VK_SUBTRACT, Rml::Input::KI_SUBTRACT)
                make_case(VK_DECIMAL, Rml::Input::KI_DECIMAL)
                make_case(VK_DIVIDE, Rml::Input::KI_DIVIDE)
                make_case(VK_F1, Rml::Input::KI_F1)
                make_case(VK_F2, Rml::Input::KI_F2)
                make_case(VK_F3, Rml::Input::KI_F3)
                make_case(VK_F4, Rml::Input::KI_F4)
                make_case(VK_F5, Rml::Input::KI_F5)
                make_case(VK_F6, Rml::Input::KI_F6)
                make_case(VK_F7, Rml::Input::KI_F7)
                make_case(VK_F8, Rml::Input::KI_F8)
                make_case(VK_F9, Rml::Input::KI_F9)
                make_case(VK_F10, Rml::Input::KI_F10)
                make_case(VK_F11, Rml::Input::KI_F11)
                make_case(VK_F12, Rml::Input::KI_F12)
                make_case(VK_F13, Rml::Input::KI_F13)
                make_case(VK_F14, Rml::Input::KI_F14)
                make_case(VK_F15, Rml::Input::KI_F15)
                make_case(VK_F16, Rml::Input::KI_F16)
                make_case(VK_F17, Rml::Input::KI_F17)
                make_case(VK_F18, Rml::Input::KI_F18)
                make_case(VK_F19, Rml::Input::KI_F19)
                make_case(VK_F20, Rml::Input::KI_F20)
                make_case(VK_F21, Rml::Input::KI_F21)
                make_case(VK_F22, Rml::Input::KI_F22)
                make_case(VK_F23, Rml::Input::KI_F23)
                make_case(VK_F24, Rml::Input::KI_F24)

                make_case(VK_NUMLOCK, Rml::Input::KI_NUMLOCK)
                make_case(VK_SCROLL, Rml::Input::KI_SCROLL)

                make_case(VK_OEM_NEC_EQUAL, Rml::Input::KI_OEM_NEC_EQUAL)

                make_case(VK_OEM_FJ_MASSHOU, Rml::Input::KI_OEM_FJ_MASSHOU)
                make_case(VK_OEM_FJ_TOUROKU, Rml::Input::KI_OEM_FJ_TOUROKU)
                make_case(VK_OEM_FJ_LOYA, Rml::Input::KI_OEM_FJ_LOYA)
                make_case(VK_OEM_FJ_ROYA, Rml::Input::KI_OEM_FJ_ROYA)

                make_case(VK_SHIFT, Rml::Input::KI_LSHIFT)
                make_case(VK_CONTROL, Rml::Input::KI_LCONTROL)
                make_case(VK_MENU, Rml::Input::KI_LMENU)

                make_case(VK_BROWSER_BACK, Rml::Input::KI_BROWSER_BACK)
                make_case(VK_BROWSER_FORWARD, Rml::Input::KI_BROWSER_FORWARD)
                make_case(VK_BROWSER_REFRESH, Rml::Input::KI_BROWSER_REFRESH)
                make_case(VK_BROWSER_STOP, Rml::Input::KI_BROWSER_STOP)
                make_case(VK_BROWSER_SEARCH, Rml::Input::KI_BROWSER_SEARCH)
                make_case(VK_BROWSER_FAVORITES, Rml::Input::KI_BROWSER_FAVORITES)
                make_case(VK_BROWSER_HOME, Rml::Input::KI_BROWSER_HOME)

                make_case(VK_VOLUME_MUTE, Rml::Input::KI_VOLUME_MUTE)
                make_case(VK_VOLUME_DOWN, Rml::Input::KI_VOLUME_DOWN)
                make_case(VK_VOLUME_UP, Rml::Input::KI_VOLUME_UP)
                make_case(VK_MEDIA_NEXT_TRACK, Rml::Input::KI_MEDIA_NEXT_TRACK)
                make_case(VK_MEDIA_PREV_TRACK, Rml::Input::KI_MEDIA_PREV_TRACK)
                make_case(VK_MEDIA_STOP, Rml::Input::KI_MEDIA_STOP)
                make_case(VK_MEDIA_PLAY_PAUSE, Rml::Input::KI_MEDIA_PLAY_PAUSE)
                make_case(VK_LAUNCH_MAIL, Rml::Input::KI_LAUNCH_MAIL)
                make_case(VK_LAUNCH_MEDIA_SELECT, Rml::Input::KI_LAUNCH_MEDIA_SELECT)
                make_case(VK_LAUNCH_APP1, Rml::Input::KI_LAUNCH_APP1)
                make_case(VK_LAUNCH_APP2, Rml::Input::KI_LAUNCH_APP2)

                make_case(VK_OEM_1, Rml::Input::KI_OEM_1)
                make_case(VK_OEM_PLUS, Rml::Input::KI_OEM_PLUS)
                make_case(VK_OEM_COMMA, Rml::Input::KI_OEM_COMMA)
                make_case(VK_OEM_MINUS, Rml::Input::KI_OEM_MINUS)
                make_case(VK_OEM_PERIOD, Rml::Input::KI_OEM_PERIOD)
                make_case(VK_OEM_2, Rml::Input::KI_OEM_2)
                make_case(VK_OEM_3, Rml::Input::KI_OEM_3)

                make_case(VK_OEM_4, Rml::Input::KI_OEM_4)
                make_case(VK_OEM_5, Rml::Input::KI_OEM_5)
                make_case(VK_OEM_6, Rml::Input::KI_OEM_6)
                make_case(VK_OEM_7, Rml::Input::KI_OEM_7)
                make_case(VK_OEM_8, Rml::Input::KI_OEM_8)

                make_case(VK_OEM_AX, Rml::Input::KI_OEM_AX)
                make_case(VK_OEM_102, Rml::Input::KI_OEM_102)
                make_case(VK_ICO_HELP, Rml::Input::KI_ICO_HELP)
                make_case(VK_ICO_00, Rml::Input::KI_ICO_00)

                make_case(VK_PROCESSKEY, Rml::Input::KI_PROCESSKEY)

                make_case(VK_ICO_CLEAR, Rml::Input::KI_ICO_CLEAR)

                make_case(VK_ATTN, Rml::Input::KI_ATTN)
                make_case(VK_CRSEL, Rml::Input::KI_CRSEL)
                make_case(VK_EXSEL, Rml::Input::KI_EXSEL)
                make_case(VK_EREOF, Rml::Input::KI_EREOF)
                make_case(VK_PLAY, Rml::Input::KI_PLAY)
                make_case(VK_ZOOM, Rml::Input::KI_ZOOM)
                make_case(VK_PA1, Rml::Input::KI_PA1)
                make_case(VK_OEM_CLEAR, Rml::Input::KI_OEM_CLEAR)
        }

        return Rml::Input::KI_UNKNOWN;
    }

    int convert_key(Rml::Input::KeyIdentifier rml_key_code) {
#define make_case(to, from) case from: return to;
        switch(rml_key_code) {
            make_case('A', Rml::Input::KI_A)
                make_case('B', Rml::Input::KI_B)
                make_case('C', Rml::Input::KI_C)
                make_case('D', Rml::Input::KI_D)
                make_case('E', Rml::Input::KI_E)
                make_case('F', Rml::Input::KI_F)
                make_case('G', Rml::Input::KI_G)
                make_case('H', Rml::Input::KI_H)
                make_case('I', Rml::Input::KI_I)
                make_case('J', Rml::Input::KI_J)
                make_case('K', Rml::Input::KI_K)
                make_case('L', Rml::Input::KI_L)
                make_case('M', Rml::Input::KI_M)
                make_case('N', Rml::Input::KI_N)
                make_case('O', Rml::Input::KI_O)
                make_case('P', Rml::Input::KI_P)
                make_case('Q', Rml::Input::KI_Q)
                make_case('R', Rml::Input::KI_R)
                make_case('S', Rml::Input::KI_S)
                make_case('T', Rml::Input::KI_T)
                make_case('U', Rml::Input::KI_U)
                make_case('V', Rml::Input::KI_V)
                make_case('W', Rml::Input::KI_W)
                make_case('X', Rml::Input::KI_X)
                make_case('Y', Rml::Input::KI_Y)
                make_case('Z', Rml::Input::KI_Z)

                make_case('0', Rml::Input::KI_0)
                make_case('1', Rml::Input::KI_1)
                make_case('2', Rml::Input::KI_2)
                make_case('3', Rml::Input::KI_3)
                make_case('4', Rml::Input::KI_4)
                make_case('5', Rml::Input::KI_5)
                make_case('6', Rml::Input::KI_6)
                make_case('7', Rml::Input::KI_7)
                make_case('8', Rml::Input::KI_8)
                make_case('9', Rml::Input::KI_9)

                make_case(VK_BACK, Rml::Input::KI_BACK)
                make_case(VK_TAB, Rml::Input::KI_TAB)

                make_case(VK_CLEAR, Rml::Input::KI_CLEAR)
                make_case(VK_RETURN, Rml::Input::KI_RETURN)

                make_case(VK_PAUSE, Rml::Input::KI_PAUSE)
                make_case(VK_CAPITAL, Rml::Input::KI_CAPITAL)

                make_case(VK_KANA, Rml::Input::KI_KANA)
                make_case(VK_JUNJA, Rml::Input::KI_JUNJA)
                make_case(VK_FINAL, Rml::Input::KI_FINAL)
                make_case(VK_HANJA, Rml::Input::KI_HANJA)

                make_case(VK_ESCAPE, Rml::Input::KI_ESCAPE)

                make_case(VK_CONVERT, Rml::Input::KI_CONVERT)
                make_case(VK_NONCONVERT, Rml::Input::KI_NONCONVERT)
                make_case(VK_ACCEPT, Rml::Input::KI_ACCEPT)
                make_case(VK_MODECHANGE, Rml::Input::KI_MODECHANGE)

                make_case(VK_SPACE, Rml::Input::KI_SPACE)
                make_case(VK_PRIOR, Rml::Input::KI_PRIOR)
                make_case(VK_NEXT, Rml::Input::KI_NEXT)
                make_case(VK_END, Rml::Input::KI_END)
                make_case(VK_HOME, Rml::Input::KI_HOME)
                make_case(VK_LEFT, Rml::Input::KI_LEFT)
                make_case(VK_UP, Rml::Input::KI_UP)
                make_case(VK_RIGHT, Rml::Input::KI_RIGHT)
                make_case(VK_DOWN, Rml::Input::KI_DOWN)
                make_case(VK_SELECT, Rml::Input::KI_SELECT)
                make_case(VK_PRINT, Rml::Input::KI_PRINT)
                make_case(VK_EXECUTE, Rml::Input::KI_EXECUTE)
                make_case(VK_SNAPSHOT, Rml::Input::KI_SNAPSHOT)
                make_case(VK_INSERT, Rml::Input::KI_INSERT)
                make_case(VK_DELETE, Rml::Input::KI_DELETE)
                make_case(VK_HELP, Rml::Input::KI_HELP)

                make_case(VK_LWIN, Rml::Input::KI_LWIN)
                make_case(VK_RWIN, Rml::Input::KI_RWIN)
                make_case(VK_APPS, Rml::Input::KI_APPS)

                make_case(VK_SLEEP, Rml::Input::KI_SLEEP)

                make_case(VK_NUMPAD0, Rml::Input::KI_NUMPAD0)
                make_case(VK_NUMPAD1, Rml::Input::KI_NUMPAD1)
                make_case(VK_NUMPAD2, Rml::Input::KI_NUMPAD2)
                make_case(VK_NUMPAD3, Rml::Input::KI_NUMPAD3)
                make_case(VK_NUMPAD4, Rml::Input::KI_NUMPAD4)
                make_case(VK_NUMPAD5, Rml::Input::KI_NUMPAD5)
                make_case(VK_NUMPAD6, Rml::Input::KI_NUMPAD6)
                make_case(VK_NUMPAD7, Rml::Input::KI_NUMPAD7)
                make_case(VK_NUMPAD8, Rml::Input::KI_NUMPAD8)
                make_case(VK_NUMPAD9, Rml::Input::KI_NUMPAD9)
                make_case(VK_MULTIPLY, Rml::Input::KI_MULTIPLY)
                make_case(VK_ADD, Rml::Input::KI_ADD)
                make_case(VK_SEPARATOR, Rml::Input::KI_SEPARATOR)
                make_case(VK_SUBTRACT, Rml::Input::KI_SUBTRACT)
                make_case(VK_DECIMAL, Rml::Input::KI_DECIMAL)
                make_case(VK_DIVIDE, Rml::Input::KI_DIVIDE)
                make_case(VK_F1, Rml::Input::KI_F1)
                make_case(VK_F2, Rml::Input::KI_F2)
                make_case(VK_F3, Rml::Input::KI_F3)
                make_case(VK_F4, Rml::Input::KI_F4)
                make_case(VK_F5, Rml::Input::KI_F5)
                make_case(VK_F6, Rml::Input::KI_F6)
                make_case(VK_F7, Rml::Input::KI_F7)
                make_case(VK_F8, Rml::Input::KI_F8)
                make_case(VK_F9, Rml::Input::KI_F9)
                make_case(VK_F10, Rml::Input::KI_F10)
                make_case(VK_F11, Rml::Input::KI_F11)
                make_case(VK_F12, Rml::Input::KI_F12)
                make_case(VK_F13, Rml::Input::KI_F13)
                make_case(VK_F14, Rml::Input::KI_F14)
                make_case(VK_F15, Rml::Input::KI_F15)
                make_case(VK_F16, Rml::Input::KI_F16)
                make_case(VK_F17, Rml::Input::KI_F17)
                make_case(VK_F18, Rml::Input::KI_F18)
                make_case(VK_F19, Rml::Input::KI_F19)
                make_case(VK_F20, Rml::Input::KI_F20)
                make_case(VK_F21, Rml::Input::KI_F21)
                make_case(VK_F22, Rml::Input::KI_F22)
                make_case(VK_F23, Rml::Input::KI_F23)
                make_case(VK_F24, Rml::Input::KI_F24)

                make_case(VK_NUMLOCK, Rml::Input::KI_NUMLOCK)
                make_case(VK_SCROLL, Rml::Input::KI_SCROLL)

                make_case(VK_OEM_NEC_EQUAL, Rml::Input::KI_OEM_NEC_EQUAL)

                make_case(VK_OEM_FJ_MASSHOU, Rml::Input::KI_OEM_FJ_MASSHOU)
                make_case(VK_OEM_FJ_TOUROKU, Rml::Input::KI_OEM_FJ_TOUROKU)
                make_case(VK_OEM_FJ_LOYA, Rml::Input::KI_OEM_FJ_LOYA)
                make_case(VK_OEM_FJ_ROYA, Rml::Input::KI_OEM_FJ_ROYA)

                make_case(VK_SHIFT, Rml::Input::KI_LSHIFT)
                make_case(VK_CONTROL, Rml::Input::KI_LCONTROL)
                make_case(VK_MENU, Rml::Input::KI_LMENU)

                make_case(VK_BROWSER_BACK, Rml::Input::KI_BROWSER_BACK)
                make_case(VK_BROWSER_FORWARD, Rml::Input::KI_BROWSER_FORWARD)
                make_case(VK_BROWSER_REFRESH, Rml::Input::KI_BROWSER_REFRESH)
                make_case(VK_BROWSER_STOP, Rml::Input::KI_BROWSER_STOP)
                make_case(VK_BROWSER_SEARCH, Rml::Input::KI_BROWSER_SEARCH)
                make_case(VK_BROWSER_FAVORITES, Rml::Input::KI_BROWSER_FAVORITES)
                make_case(VK_BROWSER_HOME, Rml::Input::KI_BROWSER_HOME)

                make_case(VK_VOLUME_MUTE, Rml::Input::KI_VOLUME_MUTE)
                make_case(VK_VOLUME_DOWN, Rml::Input::KI_VOLUME_DOWN)
                make_case(VK_VOLUME_UP, Rml::Input::KI_VOLUME_UP)
                make_case(VK_MEDIA_NEXT_TRACK, Rml::Input::KI_MEDIA_NEXT_TRACK)
                make_case(VK_MEDIA_PREV_TRACK, Rml::Input::KI_MEDIA_PREV_TRACK)
                make_case(VK_MEDIA_STOP, Rml::Input::KI_MEDIA_STOP)
                make_case(VK_MEDIA_PLAY_PAUSE, Rml::Input::KI_MEDIA_PLAY_PAUSE)
                make_case(VK_LAUNCH_MAIL, Rml::Input::KI_LAUNCH_MAIL)
                make_case(VK_LAUNCH_MEDIA_SELECT, Rml::Input::KI_LAUNCH_MEDIA_SELECT)
                make_case(VK_LAUNCH_APP1, Rml::Input::KI_LAUNCH_APP1)
                make_case(VK_LAUNCH_APP2, Rml::Input::KI_LAUNCH_APP2)

                make_case(VK_OEM_1, Rml::Input::KI_OEM_1)
                make_case(VK_OEM_PLUS, Rml::Input::KI_OEM_PLUS)
                make_case(VK_OEM_COMMA, Rml::Input::KI_OEM_COMMA)
                make_case(VK_OEM_MINUS, Rml::Input::KI_OEM_MINUS)
                make_case(VK_OEM_PERIOD, Rml::Input::KI_OEM_PERIOD)
                make_case(VK_OEM_2, Rml::Input::KI_OEM_2)
                make_case(VK_OEM_3, Rml::Input::KI_OEM_3)

                make_case(VK_OEM_4, Rml::Input::KI_OEM_4)
                make_case(VK_OEM_5, Rml::Input::KI_OEM_5)
                make_case(VK_OEM_6, Rml::Input::KI_OEM_6)
                make_case(VK_OEM_7, Rml::Input::KI_OEM_7)
                make_case(VK_OEM_8, Rml::Input::KI_OEM_8)

                make_case(VK_OEM_AX, Rml::Input::KI_OEM_AX)
                make_case(VK_OEM_102, Rml::Input::KI_OEM_102)
                make_case(VK_ICO_HELP, Rml::Input::KI_ICO_HELP)
                make_case(VK_ICO_00, Rml::Input::KI_ICO_00)

                make_case(VK_PROCESSKEY, Rml::Input::KI_PROCESSKEY)

                make_case(VK_ICO_CLEAR, Rml::Input::KI_ICO_CLEAR)

                make_case(VK_ATTN, Rml::Input::KI_ATTN)
                make_case(VK_CRSEL, Rml::Input::KI_CRSEL)
                make_case(VK_EXSEL, Rml::Input::KI_EXSEL)
                make_case(VK_EREOF, Rml::Input::KI_EREOF)
                make_case(VK_PLAY, Rml::Input::KI_PLAY)
                make_case(VK_ZOOM, Rml::Input::KI_ZOOM)
                make_case(VK_PA1, Rml::Input::KI_PA1)
                make_case(VK_OEM_CLEAR, Rml::Input::KI_OEM_CLEAR)
        }

        return Rml::Input::KI_UNKNOWN;
    }
}