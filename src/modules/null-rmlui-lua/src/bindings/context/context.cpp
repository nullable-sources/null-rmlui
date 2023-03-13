#include <bindings/bindings.h>
#include <plugin/document/document.h>
#include <plugin/data-model/data-model.h>

namespace null::rml::modules::lua::context_bindings {
	namespace document {
		std::function<c_document* (int)> get_document(Rml::Context& self) {
			return std::function<c_document* (int)>{ [&self](int idx) -> auto { return dynamic_cast<c_document*>(self.GetDocument(idx)); } };
		}
	}
	
	namespace data_model {
		void bind_table(data_model_t* data, sol::table& table) {
			for(auto& [key, value] : table) {
				std::string string_key{ key.as<std::string>() };
				auto iterator{ data->object_map.insert_or_assign(string_key, value) };

				if(value.get_type() == sol::type::function) {
					data->constructor.BindEventCallback(string_key,
						[string_key, cb = sol::protected_function{ value }, state = sol::state_view{ table.lua_state() }](Rml::DataModelHandle, Rml::Event& event, const Rml::VariantList& varlist)
						{
							if(!cb.valid()) return;

							sol::protected_function_result result{ cb(event, sol::as_args(varlist | std::views::transform([&](const auto& variant) { return make_object_from_variant(&variant, state); }) | std::ranges::to<std::vector>())) };
							if(!result.valid())
								error_handler(cb.lua_state(), std::move(result));
						}
					);
				} else {
					data->constructor.BindCustomDataVariable(string_key, Rml::DataVariable(data->object_definition.get(), &(iterator.first->second)));
				}
			}
		}

		std::unique_ptr<data_model_t> open_data_model(Rml::Context& self, const std::string& name, sol::object model, sol::this_state this_state) {
			sol::state_view state_view{ this_state };

			//@note: create data model.
			Rml::DataModelConstructor constructor{ self.CreateDataModel(name) };
			std::unique_ptr<data_model_t> data{ std::make_unique<data_model_t>(state_view) };

			//@note: already created? get existing.
			if(!constructor) {
				constructor = self.GetDataModel(name);
				if(!constructor)
					return data;
			}

			data->constructor = constructor;
			data->handle = constructor.GetModelHandle();
			data->object_definition = std::make_unique<c_object_definition>(data.get());

			//@note: only bind table.
			if(model.get_type() == sol::type::table) {
				data->table = model.as<sol::table>();
				bind_table(data.get(), data->table);
			}

			return data;
		}
	}

	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<Rml::Context>("c_context", sol::no_constructor,
			"create_document", sol::overload(
					&Rml::Context::CreateDocument,
					[](Rml::Context& self) { return self.CreateDocument(); }
				),
			"get_document", [](Rml::Context& self, const std::string& name) { return dynamic_cast<c_document*>(self.GetDocument(name)); },
			"load_document", [](Rml::Context& self, const std::string& document) { return dynamic_cast<c_document*>(self.LoadDocument(document)); },
			"documents", sol::readonly_property(&get_indexed_table<c_document, Rml::Context, &document::get_document, &Rml::Context::GetNumDocuments>),
			"unload_all_documents", &Rml::Context::UnloadAllDocuments,
			"unload_document", &Rml::Context::UnloadDocument,
			"pull_document_to_dront", &Rml::Context::PullDocumentToFront,
			"push_document_to_back", &Rml::Context::PushDocumentToBack,
			"unfocus_document", &Rml::Context::UnfocusDocument,
			
			"render", &Rml::Context::Render,
			"update", &Rml::Context::Update,
			
			"open_data_model", &data_model::open_data_model,

			"is_mouse_interacting", &Rml::Context::IsMouseInteracting,
			"enable_mouse_cursor", &Rml::Context::EnableMouseCursor,
			
			"add_event_listener", &Rml::Context::AddEventListener,
			"process_mouse_move", &Rml::Context::ProcessMouseMove,
			"process_mouse_button_down", &Rml::Context::ProcessMouseButtonDown,
			"process_mouse_button_up", &Rml::Context::ProcessMouseButtonUp,
			"process_mouse_wheel", &Rml::Context::ProcessMouseWheel,
			"process_mouse_leave", &Rml::Context::ProcessMouseLeave,
			"process_key_down", &Rml::Context::ProcessKeyDown,
			"process_key_up", &Rml::Context::ProcessKeyUp,
			"process_text_input", sol::resolve<bool(const std::string&)>(&Rml::Context::ProcessTextInput),
			
			"activate_theme", &Rml::Context::ActivateTheme,
			"is_theme_active", &Rml::Context::IsThemeActive,
			
			"get_element_at_point", sol::overload(
				[](Rml::Context& self, Rml::Vector2f point) { return self.GetElementAtPoint(point); },
				[](Rml::Context& self, Rml::Vector2f point, const Rml::Element& ignore_element) { return self.GetElementAtPoint(point, &ignore_element); },
				[](Rml::Context& self, Rml::Vector2f point, const Rml::Element& ignore_element, Rml::Element& element) { return self.GetElementAtPoint(point, &ignore_element, &element); }
			),

			"dimensions", sol::property(&Rml::Context::GetDimensions, &Rml::Context::SetDimensions),
			"dp_ratio", sol::property(&Rml::Context::GetDensityIndependentPixelRatio, &Rml::Context::SetDensityIndependentPixelRatio),
			"clip_region", sol::property(&Rml::Context::GetActiveClipRegion, &Rml::Context::SetActiveClipRegion),

			"focus_element", sol::readonly_property(&Rml::Context::GetFocusElement),
			"hover_element", sol::readonly_property(&Rml::Context::GetHoverElement),
			"name", sol::readonly_property(&Rml::Context::GetName),
			"root_element", sol::readonly_property(&Rml::Context::GetRootElement)
		);
	}
}