#include <bindings/bindings.h>

#include <RmlUi/Core.h>

#include <plugin/document/document.h>
#include <plugin/event-listener/event-listener.h>

#include <unordered_map>

namespace null::rml::modules::lua::element_bindings {
	namespace functions {
		void add_event_listener(Rml::Element& self, const std::string& event, sol::protected_function func, const bool in_capture_phase = false) {
			self.AddEventListener(event, new c_event_listener{ func, &self }, in_capture_phase);
		}

		void add_event_listener(Rml::Element& self, const std::string& event, const std::string& code, sol::this_state this_state) {
			sol::state_view state{ this_state };
			self.AddEventListener(event, new c_event_listener{ state, code, &self }, false);
		}

		void add_event_listener(Rml::Element& self, const std::string& event, const std::string& code, sol::this_state this_state, const bool in_capture_phase) {
			sol::state_view state{ this_state };
			self.AddEventListener(event, new c_event_listener{ state, code, &self }, in_capture_phase);
		}

		auto get_elements_by_tag_name(Rml::Element& self, const std::string& tag) {
			Rml::ElementList result{ };
			self.GetElementsByTagName(result, tag);
			return result;
		}

		auto get_elements_by_class_name(Rml::Element& self, const std::string& class_name) {
			Rml::ElementList result{ };
			self.GetElementsByClassName(result, class_name);
			return result;
		}

		auto query_selector_all(Rml::Element& self, const std::string& selector) {
			Rml::ElementList result{ };
			self.QuerySelectorAll(result, selector);
			return result;
		}
	}

	namespace child {
		std::function<int()> get_num_children(Rml::Element& self) {
			return std::function<int()>{ std::bind(&Rml::Element::GetNumChildren, &self, false) };
		}
	}

	namespace style {
		struct iterator_t {
		public:
			Rml::PropertiesIteratorView view;

		public:
			iterator_t(Rml::PropertiesIteratorView&& self) : view{ std::move(self) } { }
		};

		std::tuple<sol::object, sol::object> next_pair(sol::user<iterator_t&> iterator_state, sol::this_state this_state) {
			if(iterator_state.value().view.AtEnd())
				return std::make_tuple(sol::object{ sol::lua_nil }, sol::object{ sol::lua_nil });

			std::tuple<sol::object, sol::object> result{ std::make_tuple(sol::object{ this_state, sol::in_place, iterator_state.value().view.GetName() }, sol::object{ this_state, sol::in_place, iterator_state.value().view.GetProperty().ToString() }) };
			++iterator_state.value().view;

			return result;
		}

		struct style_proxy_t {
		public:
			Rml::Element* element{ };

		public:
			style_proxy_t(Rml::Element& _element) : element{ &_element } { }

		public:
			std::string get(const std::string& name) {
				if(const Rml::Property* property{ element->GetProperty(name) }; property != nullptr) return property->ToString();
				return { };
			}

			void set(const std::string& name, const std::string& value) { element->SetProperty(name, value); }

			auto pairs() { return std::make_tuple(&next_pair, std::move(iterator_t{ std::move(element->IterateLocalProperties()) }), sol::lua_nil); }
		};
	}

	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<Rml::EventListener>("c_event_listener", sol::no_constructor,
			"on_attach", &Rml::EventListener::OnAttach,
			"on_detach", &Rml::EventListener::OnDetach,
			"process_event", &Rml::EventListener::ProcessEvent
		);

		rmlui.new_usertype<style::style_proxy_t>("style_proxy_t", sol::no_constructor,
			sol::meta_function::index, &style::style_proxy_t::get,
			sol::meta_function::new_index, &style::style_proxy_t::set,
			sol::meta_function::pairs, &style::style_proxy_t::pairs
		);

		rmlui.new_usertype<Rml::Element>("c_element", sol::no_constructor,
			"add_event_listener", sol::overload(
				[](Rml::Element& s, const std::string& e, sol::protected_function f) { functions::add_event_listener(s, e, f, false); },
				sol::resolve<void(Rml::Element&, const std::string&, sol::protected_function, bool)>(&functions::add_event_listener),
				sol::resolve<void(Rml::Element&, const std::string&, const std::string&, sol::this_state)>(&functions::add_event_listener),
				sol::resolve<void(Rml::Element&, const std::string&, const std::string&, sol::this_state, bool)>(&functions::add_event_listener)
			),
			"dispatch_event", sol::resolve<bool(const std::string&, const Rml::Dictionary&)>(&Rml::Element::DispatchEvent),
			"process_default_action", &Rml::Element::ProcessDefaultAction,
			
			"append_child", [](Rml::Element& self, Rml::ElementPtr& e) { self.AppendChild(std::move(e)); },
			"remove_child", &Rml::Element::RemoveChild,
			"replace_child", [](Rml::Element& self, Rml::ElementPtr& inserted_element, Rml::Element* replaced_element) { self.ReplaceChild(std::move(inserted_element), replaced_element); },
			"has_child_nodes", &Rml::Element::HasChildNodes,
			"insert_before", [](Rml::Element& self, Rml::ElementPtr& element, Rml::Element* adjacent_element) { self.InsertBefore(std::move(element), adjacent_element); },
			
			"blur", &Rml::Element::Blur,
			"click", &Rml::Element::Click,
			"focus", &Rml::Element::Focus,
			
			"get_attribute", [](Rml::Element& self, const std::string& name, sol::this_state this_state) { return make_object_from_variant(self.GetAttribute(name), this_state); },
			"set_attribute", static_cast<void(Rml::Element::*)(const std::string&, const std::string&)>(&Rml::Element::SetAttribute),
			"remove_attribute", &Rml::Element::RemoveAttribute,
			"has_attribute", &Rml::Element::HasAttribute,
			
			"get_active_pseudo_classes", &Rml::Element::GetActivePseudoClasses,
			"set_pseudo_class", &Rml::Element::SetPseudoClass,
			"are_pseudo_classes_set", &Rml::Element::ArePseudoClassesSet,
			"is_pseudo_class_set", &Rml::Element::IsPseudoClassSet,
			"set_class", &Rml::Element::SetClass,
			"is_class_set", &Rml::Element::IsClassSet,

			"get_element_by_id", &Rml::Element::GetElementById,
			"get_elements_by_tag_name", &functions::get_elements_by_tag_name,
			"get_elements_by_class_name", &functions::get_elements_by_class_name,
			"query_selector", &Rml::Element::QuerySelector,
			"query_selector_all", &functions::query_selector_all,
			
			"scroll_into_view", [](Rml::Element& self, sol::variadic_args va) { if(va.size() == 0) self.ScrollIntoView(true); else self.ScrollIntoView(va[0].as<bool>()); },
			"clone", &Rml::Element::Clone,
			"closest", &Rml::Element::Closest,
			"is_point_within_element", &Rml::Element::IsPointWithinElement,

			"class_name", sol::property(&Rml::Element::GetClassNames, &Rml::Element::SetClassNames),
			"id", sol::property(&Rml::Element::GetId, &Rml::Element::SetId),
			"inner_rml", sol::property(sol::resolve<std::string() const>(&Rml::Element::GetInnerRML), &Rml::Element::SetInnerRML),
			"scroll_left", sol::property(&Rml::Element::GetScrollLeft, &Rml::Element::SetScrollLeft),
			"scroll_top", sol::property(&Rml::Element::GetScrollTop, &Rml::Element::SetScrollTop),

			"attributes", sol::readonly_property([](Rml::Element& self, sol::this_state this_state) { return self.GetAttributes() | std::views::transform([&](auto& pair) { return std::make_pair(pair.first, make_object_from_variant(&pair.second, this_state)); }) | std::ranges::to<std::unordered_map>(); }),
			"child_nodes", sol::readonly_property(&get_indexed_table<Rml::Element, Rml::Element, &Rml::Element::GetChild, &child::get_num_children>),
			"client_left", sol::readonly_property(&Rml::Element::GetClientLeft),
			"client_height", sol::readonly_property(&Rml::Element::GetClientHeight),
			"client_top", sol::readonly_property(&Rml::Element::GetClientTop),
			"client_width", sol::readonly_property(&Rml::Element::GetClientWidth),
			"first_child", sol::readonly_property(&Rml::Element::GetFirstChild),
			"last_child", sol::readonly_property(&Rml::Element::GetLastChild),
			"next_sibling", sol::readonly_property(&Rml::Element::GetNextSibling),
			"offset_height", sol::readonly_property(&Rml::Element::GetOffsetHeight),
			"offset_left", sol::readonly_property(&Rml::Element::GetOffsetLeft),
			"offset_parent", sol::readonly_property(&Rml::Element::GetOffsetParent),
			"offset_top", sol::readonly_property(&Rml::Element::GetOffsetTop),
			"offset_width", sol::readonly_property(&Rml::Element::GetOffsetWidth),
			"owner_document", sol::readonly_property([](Rml::Element& self) { return dynamic_cast<c_document*>(self.GetOwnerDocument()); }),
			"parent_node", sol::readonly_property(&Rml::Element::GetParentNode),
			"previous_sibling", sol::readonly_property(&Rml::Element::GetPreviousSibling),
			"scroll_height", sol::readonly_property(&Rml::Element::GetScrollHeight),
			"scroll_width", sol::readonly_property(&Rml::Element::GetScrollWidth),
			"style", sol::readonly_property([](Rml::Element& self) { return style::style_proxy_t{ self }; }),
			"tag_name", sol::readonly_property(&Rml::Element::GetTagName),
			"address", sol::readonly_property(&Rml::Element::GetAddress),
			"absolute_left", sol::readonly_property(&Rml::Element::GetAbsoluteLeft),
			"absolute_top", sol::readonly_property(&Rml::Element::GetAbsoluteTop),
			"baseline", sol::readonly_property(&Rml::Element::GetBaseline),
			"line_height", sol::readonly_property(&Rml::Element::GetLineHeight),
			"visible", sol::readonly_property(&Rml::Element::IsVisible),
			"z_index", sol::readonly_property(&Rml::Element::GetZIndex)
		);
	}
}