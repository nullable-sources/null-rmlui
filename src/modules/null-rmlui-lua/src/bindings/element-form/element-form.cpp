#include "bindings/bindings.h"

#include "plugin/event-listener/event-listener.h"

namespace null::rml::modules::lua::element_form_bindings {
	namespace functions {
		constexpr void set_attribute(auto& self, const std::string& name, const auto& value) {
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
				if(value) self.SetAttribute(name, true);
				else self.RemoveAttribute(name);
			} else {
				self.SetAttribute(name, value);
			}
		}
	}

	namespace options {
		struct select_options_proxy_node_t {
		public:
			Rml::Element* element{ };
			std::string value{ };
		};

		struct select_options_proxy_t {
		private:
			Rml::ElementFormControlSelect* element{ };

		public:
			select_options_proxy_t(Rml::ElementFormControlSelect& _element) : element{ &_element } { }

		public:
			select_options_proxy_node_t get(int index) const {
				if(Rml::Element* option{ element->GetOption(index) }; option != nullptr)
					return select_options_proxy_node_t{ option, option->GetAttribute("value", std::string{ }) };
				return select_options_proxy_node_t{ };
			}

			std::vector<select_options_proxy_node_t> pairs() const {
				return std::views::iota(0, element->GetNumOptions()) | std::views::transform([this](const int& i) { return get(i); }) | std::ranges::to<std::vector>();
			}
		};
	}

	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<Rml::ElementForm>("c_element_form", sol::no_constructor,
			"submit", sol::overload(
				&Rml::ElementForm::Submit,
				[](Rml::ElementForm& self) { self.Submit(); },
				[](Rml::ElementForm& self, const std::string& name) { self.Submit(name); }
			),

			sol::base_classes, sol::bases<Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementFormControl>("c_element_form_control", sol::no_constructor,
			"disabled", sol::property(&Rml::ElementFormControl::IsDisabled, &Rml::ElementFormControl::SetDisabled),
			"name", sol::property(&Rml::ElementFormControl::GetName, &Rml::ElementFormControl::SetName),
			"value", sol::property(&Rml::ElementFormControl::GetValue, &Rml::ElementFormControl::SetValue),
			"submitted", sol::readonly_property(&Rml::ElementFormControl::IsSubmitted),

			sol::base_classes, sol::bases<Rml::Element>()
		);

		#define has_attribute_getter(self_t, name) [](self_t& self) { return self.HasAttribute(name); }
		#define get_attribute_getter(self_t, name, def) [](self_t& self) { return self.GetAttribute(name, def); }
		#define set_attribute(self_t, name, def_t) [](self_t& self, const def_t& value) { functions::set_attribute(self, name, value); }
		rmlui.new_usertype<Rml::ElementFormControlInput>("c_element_form_control_input", sol::no_constructor,
			"checked", sol::property(has_attribute_getter(Rml::ElementFormControlInput, "checked"), set_attribute(Rml::ElementFormControlInput, "checked", bool)),
			"maxlength", sol::property(get_attribute_getter(Rml::ElementFormControlInput, "maxlength", -1), set_attribute(Rml::ElementFormControlInput, "maxlength", int)),
			"size", sol::property(get_attribute_getter(Rml::ElementFormControlInput, "size", 20), set_attribute(Rml::ElementFormControlInput, "size", int)),
			"max", sol::property(get_attribute_getter(Rml::ElementFormControlInput, "max", 100), set_attribute(Rml::ElementFormControlInput, "max", int)),
			"min", sol::property(get_attribute_getter(Rml::ElementFormControlInput, "min", 0), set_attribute(Rml::ElementFormControlInput, "min", int)),
			"step", sol::property(get_attribute_getter(Rml::ElementFormControlInput, "step", 1), set_attribute(Rml::ElementFormControlInput, "step", int)),

			sol::base_classes, sol::bases<Rml::ElementFormControl, Rml::Element>()
		);

		rmlui.new_usertype<options::select_options_proxy_t>("select_options_proxy_t", sol::no_constructor,
			sol::meta_function::index, &options::select_options_proxy_t::get,
			sol::meta_function::pairs, &options::select_options_proxy_t::pairs,
			sol::meta_function::ipairs, &options::select_options_proxy_t::pairs
		);

		rmlui.new_usertype<options::select_options_proxy_node_t>("select_options_proxy_node_t", sol::no_constructor,
			"element", &options::select_options_proxy_node_t::element,
			"value", &options::select_options_proxy_node_t::value
		);

		rmlui.new_usertype<Rml::ElementFormControlSelect>("c_element_form_control_select", sol::no_constructor,
			"add", [](Rml::ElementFormControlSelect& self, Rml::ElementPtr& element, sol::variadic_args va) {
				self.Add(std::move(element), va.size() > 0 ? va.get<int>() : -1);
				return 1;
			},
			"remove", &Rml::ElementFormControlSelect::Remove,
			"remove_all", &Rml::ElementFormControlSelect::RemoveAll,
			
			"selection", sol::property(&Rml::ElementFormControlSelect::GetSelection, &Rml::ElementFormControlSelect::SetSelection),
			"options", [](Rml::ElementFormControlSelect& self) { return options::select_options_proxy_t{ self }; },

			sol::base_classes, sol::bases<Rml::ElementFormControl, Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementFormControlDataSelect>("c_element_form_control_data_select", sol::no_constructor,
			"set_data_source", &Rml::ElementFormControlDataSelect::SetDataSource,

			sol::base_classes, sol::bases<Rml::ElementFormControlSelect, Rml::ElementFormControl, Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementFormControlTextArea>("c_element_form_control_text_area", sol::no_constructor,
			"cols", sol::property(&Rml::ElementFormControlTextArea::GetNumColumns, &Rml::ElementFormControlTextArea::SetNumColumns),
			"maxlength", sol::property(&Rml::ElementFormControlTextArea::GetMaxLength, &Rml::ElementFormControlTextArea::SetMaxLength),
			"rows", sol::property(&Rml::ElementFormControlTextArea::GetNumRows, &Rml::ElementFormControlTextArea::SetNumRows),
			"wordwrap", sol::property(&Rml::ElementFormControlTextArea::SetWordWrap, &Rml::ElementFormControlTextArea::GetWordWrap),

			sol::base_classes, sol::bases<Rml::ElementFormControl, Rml::Element>()
		);
	}
}