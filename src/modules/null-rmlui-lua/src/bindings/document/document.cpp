#include "bindings/bindings.h"
#include "plugin/document/document.h"

namespace null::rml::modules::lua::document_bindings {
	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_enum<Rml::ModalFlag>("e_modal_flag", {
				{ "none", Rml::ModalFlag::None },
				{ "modal", Rml::ModalFlag::Modal },
				{ "keep", Rml::ModalFlag::Keep }
			}
		);

		rmlui.new_enum<Rml::FocusFlag>("e_focus_flag", {
				{ "none", Rml::FocusFlag::None },
				{ "document", Rml::FocusFlag::Document },
				{ "keep", Rml::FocusFlag::Keep },
				{ "auto", Rml::FocusFlag::Auto }
			}
		);

		rmlui.new_usertype<c_document>("c_document", sol::no_constructor,
			"pull_to_front", &c_document::PullToFront,
			"push_to_back", &c_document::PushToBack,

			"show", sol::overload(
				[](c_document& self) { self.Show(); },
				[](c_document & self, Rml::ModalFlag modal) { self.Show(modal); },
				[](c_document & self, Rml::ModalFlag modal, Rml::FocusFlag focus) { self.Show(modal, focus); }
			),
			"hide", &c_document::Hide,
			"close", &c_document::Close,

			"create_element", &c_document::CreateElement,
			"create_text_node", &c_document::CreateTextNode,
			
			"reload_style_sheet", sol::overload(
				[](c_document& self) { self.ReloadStyleSheet(); },
				[](c_document& self, bool load) {
					self.ReloadStyleSheet();
					if(load) {
						//@note: dispatch the load event so we can re-bind any scripts that got wiped out.
						self.DispatchEvent(Rml::EventId::Load, Rml::Dictionary());
					}
				}
			),
			
			"load_external_script", &c_document::LoadExternalScript,
			"load_inline_script", sol::overload(
				[](c_document& self, const std::string& content) { self.LoadInlineScript(content, self.GetSourceURL(), 0); },
				[](c_document& self, const std::string& content, const std::string& source_path) { self.LoadInlineScript(content, source_path, 0); },
				&c_document::LoadInlineScript
			),
			
			"update_document", &c_document::UpdateDocument,
			"append_to_style_sheet", [](c_document& self, const std::string& content) {
				std::shared_ptr<Rml::StyleSheetContainer> combined{ Rml::Factory::InstanceStyleSheetString(content)->CombineStyleSheetContainer(*self.GetStyleSheetContainer()) };
				self.SetStyleSheetContainer(std::move(combined));
			},

			"title", sol::property(&c_document::GetTitle, &c_document::SetTitle),
			"context", sol::readonly_property(&c_document::GetContext),
			"url", sol::readonly_property(&c_document::GetSourceURL),
			"modal", sol::readonly_property(&c_document::IsModal),

			sol::base_classes, sol::bases<Rml::Element>()
		);
	}
}