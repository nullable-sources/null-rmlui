#include "bindings/bindings.h"

#include "plugin/document/document.h"
#include "plugin/data-model/data-model.h"

namespace null::rml::modules::lua::data_model_bindings {
	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<data_model_t>("data_model_t", sol::no_constructor,
			sol::meta_function::index, [](data_model_t& self, const std::string& name, sol::this_state this_state) { return self.table.get<sol::object>(name); },
			sol::meta_function::new_index, [](data_model_t& self, const std::string& name, sol::object value, sol::this_state this_state) {
				self.handle.DirtyVariable(name);
				self.table.set(name, value);
			}
		);
	}
}