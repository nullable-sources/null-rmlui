#pragma once
#include <memory>
#include <unordered_map>

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataVariable.h>
#include <RmlUi/Core/Variant.h>
#include <RmlUi/Core/DataTypes.h>

#include <sol/sol.hpp>


namespace null::rml::modules::lua {
	class c_object_definition;

	struct data_model_t {
	public:
		Rml::DataModelConstructor constructor{ };
		Rml::DataModelHandle handle{ };
		sol::state_view state_view{ nullptr };
		std::unique_ptr<c_object_definition> object_definition{ };

		//@note: sol data types are reference counted.  Hold onto them as we use them.
		sol::table table{ };
		std::unordered_map<std::string, sol::object> object_map{ };

	public:
		data_model_t(const sol::state_view& _state_view) : state_view{ state_view } {}
	};

	class c_object_definition final : public Rml::VariableDefinition {
	protected:
		data_model_t* model{ };
		sol::object object{ };

	public:
		c_object_definition(data_model_t* _model) : VariableDefinition{ Rml::DataVariableType::Scalar }, model{ _model } { }

	public:
		bool Get(void* ptr, Rml::Variant& variant) override;
		bool Set(void* ptr, const Rml::Variant& variant) override;
		int Size(void* ptr) override;
		Rml::DataVariable Child(void* ptr, const Rml::DataAddressEntry& address) override;
	};
}
