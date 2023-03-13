#include "../features/features.h"
#include "../ui/ui.h"
#include "lua.h"
#include <modules/null-rmlui-lua/src/plugin/document/document.h>

namespace lua {
	void initialize() {
		null::lua::shared_state.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string);

		sol::table sdk{ null::lua::shared_state.create_named_table("sdk") };

		sdk.new_usertype<features::process_info_t>("process_info_t", sol::no_constructor,
			"name", sol::readonly(&features::process_info_t::name),
			"pid", sol::readonly(&features::process_info_t::pid),
			"type", sol::readonly(&features::process_info_t::type),
			"architecture", sol::readonly(&features::process_info_t::architecture)
			);

		sdk.new_usertype<features::dll_info_t>("dll_info_t", sol::no_constructor,
			"name", sol::readonly(&features::dll_info_t::name),
			"path", sol::readonly(&features::dll_info_t::path),
			"architecture", sol::readonly(&features::dll_info_t::architecture)
		);

		sdk.set_function("inject", [](const std::string& pid, const std::string& dll_path) { features::inject(std::stoi(pid), dll_path); });
		sdk.set("select_dll", &features::select_dll);
		sdk.set("get_processes", &features::get_processes_list);
		sdk.set("close", []() { PostQuitMessage(0); });
		sdk.set("collapse", [&]() { ShowWindow(ui::window.wnd_handle, SW_MINIMIZE); });
		sdk.set("get_architecture", []() {
#ifdef _WIN64
			return "x64";
#else
			return "x86";
#endif
			});

		null::rml::modules::lua::initialize(null::lua::shared_state);
	}
}