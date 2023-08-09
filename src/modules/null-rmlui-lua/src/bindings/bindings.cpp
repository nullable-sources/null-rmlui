#include "bindings/bindings.h"

namespace null::rml::modules::lua {
	sol::object make_object_from_variant(const Rml::Variant* variant, const sol::state_view& state_view) {
		if(!variant) return sol::make_object(state_view, sol::nil);

		switch (variant->GetType()) {
			case Rml::Variant::BOOL: return sol::make_object(state_view, variant->Get<bool>());
			
			case Rml::Variant::BYTE:
			case Rml::Variant::CHAR:
			case Rml::Variant::INT: return sol::make_object(state_view, variant->Get<int>());
			
			case Rml::Variant::INT64: return sol::make_object(state_view, variant->Get<int64_t>());
			case Rml::Variant::UINT: return sol::make_object(state_view, variant->Get<unsigned int>());
			case Rml::Variant::UINT64: return sol::make_object(state_view, variant->Get<uint64_t>());
			
			case Rml::Variant::FLOAT:
			case Rml::Variant::DOUBLE: return sol::make_object(state_view, variant->Get<double>());
			
			case Rml::Variant::COLOURB: return sol::make_object_userdata<Rml::Colourb>(state_view, variant->Get<Rml::Colourb>());
			case Rml::Variant::COLOURF: return sol::make_object_userdata<Rml::Colourf>(state_view, variant->Get<Rml::Colourf>());
			
			case Rml::Variant::STRING: return sol::make_object(state_view, variant->GetReference<std::string>());
			
			case Rml::Variant::VECTOR2: return sol::make_object_userdata<Rml::Vector2f>(state_view, variant->Get<Rml::Vector2f>());
			
			case Rml::Variant::VOIDPTR: return sol::make_object(state_view, variant->Get<void*>());
			
			default: return sol::make_object(state_view, sol::nil);
		}

		return sol::make_object(state_view, sol::nil);
	}
}
