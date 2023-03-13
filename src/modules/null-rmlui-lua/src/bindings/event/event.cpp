#include <bindings/bindings.h>

namespace null::rml::modules::lua::event_bindings {
	namespace functions {
		std::unordered_map<std::string, sol::object> get_parameters(Rml::Event& self, sol::this_state this_state) {
			std::unordered_map<std::string, sol::object> result{ };

			for(auto& [key, value] : self.GetParameters()) {
				sol::object object{ };
				
				if(self.GetId() == Rml::EventId::Tabchange && value.GetType() == Rml::Variant::INT) object = sol::make_object(this_state, value.Get<int>());
				else object = make_object_from_variant(&value, this_state);

				result.insert(std::make_pair(key, object));
			}

			return result;
		}
	}

	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_enum("e_event_phase",
			"none", Rml::EventPhase::None,
			"capture", Rml::EventPhase::Capture,
			"target", Rml::EventPhase::Target,
			"nubble", Rml::EventPhase::Bubble
		);

		rmlui.new_usertype<Rml::Event>("c_event", sol::no_constructor,
			"stop_propagation", &Rml::Event::StopPropagation,
			"stop_immediate_propagation", &Rml::Event::StopImmediatePropagation,

			"current_element", sol::readonly_property(&Rml::Event::GetCurrentElement),
			"target_element", sol::readonly_property(&Rml::Event::GetTargetElement),
			"type", sol::readonly_property(&Rml::Event::GetType),
			"event_phase", sol::readonly_property(&Rml::Event::GetPhase),
			"interruptible", sol::readonly_property(&Rml::Event::IsInterruptible),
			"propagating", sol::readonly_property(&Rml::Event::IsPropagating),
			"immediate_propagating", sol::readonly_property(&Rml::Event::IsImmediatePropagating),
			"parameters", sol::readonly_property(&functions::get_parameters)
		);
	}
}