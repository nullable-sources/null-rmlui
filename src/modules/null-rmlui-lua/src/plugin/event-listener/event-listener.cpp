#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>

#include <string>
#include <format>

#include <plugin/event-listener/event-listener.h>
#include <plugin/document/document.h>

namespace null::rml::modules::lua {
	c_event_listener::c_event_listener(sol::state_view& state_view, const std::string& code, Rml::Element* _element) : element{ _element } {
		if(!element) return;

		Rml::Context* context{ element->GetContext() };
		Rml::ElementDocument* document{ element->GetOwnerDocument() };

		//@note: wrap our code so we pass event, element, and document.
		std::string function_code{ std::format("--[{}][{}] {}", context ? context->GetName() : "unknown context", document ? document->GetSourceURL() : "unknown document", element->GetTagName()) };

		if(const std::string& id = element->GetId(); !id.empty()) {
			function_code.append("#").append(id);
		}

		if(std::string classes{ element->GetClassNames() }; !classes.empty()) {
			function_code.append(".").append(classes | std::views::transform([](const char& c) { return c == ' ' ? '.' : c; }) | std::ranges::to<std::string>());
		}

		function_code.append("\n  return function (event,element,document) ").append(code).append(" end");

		//@note: run the script and get our function to call.
		//		 we would have liked to call c_document::RunLuaScript, but we don't know our owner_document at this point!
		//		 just get the function now.  When we process the event, we will move it to the environment.
		if(auto result{ state_view.safe_script(function_code, error_handler) }; result.valid()) {
			if(sol::object obj{ result.get<sol::object>() }; obj.get_type() == sol::type::function) func = obj.as<sol::protected_function>();
			else Rml::Log::Message(Rml::Log::LT_ERROR, "[lua] a function wasn't returned for the event listener.");
		}
	}

	void c_event_listener::ProcessEvent(Rml::Event& event) {
		c_document* document{ dynamic_cast<c_document*>(element->GetOwnerDocument()) };
		if(document != nullptr && func.valid()) {
			//@note: move our event into the Lua environment.
			sol::set_environment(document->environment, func);

			//@note: call the event!
			if(auto result{ func.call(event, element, document) }; !result.valid())
				error_handler(func.lua_state(), std::move(result));
		}
	}
}
