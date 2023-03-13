#include "../features/features.h"
#include "../ui/ui.h"
#include "lua.h"

namespace lua {
	void initialize() {
		null::lua::shared_state.open_libraries(sol::lib::base, sol::lib::string);

		null::lua::shared_state.new_usertype<features::process_info_t>("process_info_t", sol::no_constructor,
			"name", sol::readonly(&features::process_info_t::name),
			"pid", sol::readonly(&features::process_info_t::pid)
			);

		null::lua::shared_state.set_function("impl_inject", [](const std::string& pid, const std::string& dll_path) { features::inject(std::stoi(pid), dll_path); });
		null::lua::shared_state.set_function("impl_get_process_list", &features::get_processes_list);
		null::lua::shared_state.set_function("impl_select_dll", &features::select_dll);
		null::lua::shared_state.set_function("impl_minimize_window", [&]() { ShowWindow(ui::window.wnd_handle, SW_MINIMIZE); });
		null::lua::shared_state.set_function("impl_close_window", []() { PostQuitMessage(0); });

		null::rml::modules::lua::initialize(null::lua::shared_state);
	}
}