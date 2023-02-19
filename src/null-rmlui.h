#pragma once
#if _DEBUG
	#include <RmlUi/Debugger.h>
#endif

#include <RmlUi/Core.h>

#include <backend/backend.h>
#include <file-interface/file-interface.h>
#include <system-interface/system-interface.h>
#include <render-interface/render-interface.h>

#include <compatibility/vec2.h>
#include <compatibility/color.h>

namespace null::rml {
	void set_default_interfaces(const utils::win::c_window& window);
	void load_system_font();

	void initialize();
	void shutdown();
}