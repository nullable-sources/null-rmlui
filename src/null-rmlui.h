#pragma once
#include "null-rmlui/rmlui-implementation/backend/backend.h"
#include "null-rmlui/rmlui-implementation/file-interface/file-interface.h"
#include "null-rmlui/rmlui-implementation/system-interface/system-interface.h"
#include "null-rmlui/rmlui-implementation/render-interface/render-interface.h"
#include "null-rmlui/rmlui-implementation/render-interface/post-processing/layers.h"

#include "null-rmlui/extensions/extensions.h"

#include "null-rmlui/compatibility/vec4.h"
#include "null-rmlui/compatibility/vec2.h"
#include "null-rmlui/compatibility/color.h"

namespace null::rml {
	void set_default_interfaces(const utils::win::c_window& window);
	void load_system_font();

	void initialize();
	void shutdown();
}