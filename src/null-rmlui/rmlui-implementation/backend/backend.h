#pragma once
#include <RmlUi/Core.h>
#include <null-sdk.h>

#include "../system-interface/system-interface.h"
#include "../render-interface/render-interface.h"

namespace ntl::rml::backend {
    int wnd_proc(Rml::Context* context, HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

    std::string convert_to_utf8(std::wstring_view wstr);
    std::wstring convert_to_utf16(std::string_view str);

    Rml::Input::KeyIdentifier convert_key(int win32_key_code);
    int convert_key(Rml::Input::KeyIdentifier rml_key_code);
    int get_key_modifier_state();
}