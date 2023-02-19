#pragma once
#include <RmlUi/Core.h>
#include <null-sdk.h>

#include <rmlui-implementation/system-interface/system-interface.h>
#include <rmlui-implementation/render-interface/render-interface.h>

namespace null::rml::backend {
    int wnd_proc(Rml::Context* context, HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

    std::string convert_to_utf8(const std::wstring_view& wstr);
    std::wstring convert_to_utf16(const std::string_view& str);

    Rml::Input::KeyIdentifier convert_key(const int& win32_key_code);
    int get_key_modifier_state();
}