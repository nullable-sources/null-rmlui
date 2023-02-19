#pragma once
#if _DEBUG
	#include <RmlUi/Debugger.h>
#endif

#include <RmlUi/Core.h>

#include <rmlui-implementation/backend/backend.h>
#include <rmlui-implementation/file-interface/file-interface.h>
#include <rmlui-implementation/system-interface/system-interface.h>
#include <rmlui-implementation/render-interface/render-interface.h>

#include <extensions/decorators/decorators.h>

#include <compatibility/vec2.h>
#include <compatibility/color.h>

namespace null::rml {
	void set_default_interfaces(const utils::win::c_window& window);
	void load_system_font();

	void initialize();
	void shutdown();
}