#include <optional>

#include <plugin/data-model/data-model.h>

namespace null::rml::modules::lua {
	bool c_object_definition::Get(void* ptr, Rml::Variant& variant) {
		sol::object* obj{ static_cast<sol::object*>(ptr) };

		if(obj->is<bool>())
			variant = obj->as<bool>();
		else if(obj->is<std::string>())
			variant = obj->as<std::string>();
		else if(obj->is<Rml::Vector2i>())
			variant = obj->as<Rml::Vector2i>();
		else if(obj->is<Rml::Vector2f>())
			variant = obj->as<Rml::Vector2f>();
		else if(obj->is<Rml::Colourb>())
			variant = obj->as<Rml::Colourb>();
		else if(obj->is<Rml::Colourf>())
			variant = obj->as<Rml::Colourf>();
		else if(obj->is<double>())
			variant = obj->as<double>();
		else // if(obj->get_type() == sol::type::lua_nil)
			variant = Rml::Variant{ };

		return true;
	}

	bool c_object_definition::Set(void* ptr, const Rml::Variant& variant) {
		sol::object* obj{ static_cast<sol::object*>(ptr) };

		if(obj->is<bool>())
			variant.GetInto<bool>(*static_cast<bool*>(ptr));
		else if(obj->is<std::string>())
			variant.GetInto<std::string>(*static_cast<std::string*>(ptr));
		else if(obj->is<Rml::Vector2i>())
			variant.GetInto<Rml::Vector2i>(*static_cast<Rml::Vector2i*>(ptr));
		else if(obj->is<Rml::Vector2f>())
			variant.GetInto<Rml::Vector2f>(*static_cast<Rml::Vector2f*>(ptr));
		else if(obj->is<Rml::Colourb>())
			variant.GetInto<Rml::Colourb>(*static_cast<Rml::Colourb*>(ptr));
		else if(obj->is<Rml::Colourf>())
			variant.GetInto<Rml::Colourf>(*static_cast<Rml::Colourf*>(ptr));
		else if(obj->is<double>())
			variant.GetInto<double>(*static_cast<double*>(ptr));
		else // if(obj->get_type() == sol::type::lua_nil)
			*obj = sol::make_object(model->state_view, sol::nil);

		return true;
	}

	int c_object_definition::Size(void* ptr) {
		//@note: non-table types are 1 entry long.
		sol::object* object{ static_cast<sol::object*>(ptr) };
		if(object->get_type() != sol::type::table)
			return 1;

		return static_cast<int>(object->as<sol::table>().size());
	}

	Rml::DataVariable c_object_definition::Child(void* ptr, const Rml::DataAddressEntry& address) {
		//@note: child should be called on a table.
		sol::object* object{ static_cast<sol::object*>(ptr) };
		if(object->get_type() != sol::type::table)
			return Rml::DataVariable{ };

		//@note: get our table object.
		//		 get the pointer as a string for use with holding onto the object.
		sol::table table{ object->as<sol::table>() };
		std::string tablestr{ std::to_string(reinterpret_cast<std::intptr_t>(table.pointer())) };

		//@note: accessing by name.
		if(address.index == -1) {
			//@note: try to get the object.
			sol::object e{ table.get<sol::object>(address.name) };
			if(e.get_type() == sol::type::lua_nil)
				return Rml::DataVariable{};

			//@note: hold a reference to it and return the pointer.
			auto iterator{ model->object_map.insert_or_assign(tablestr + "_" + std::to_string(address.index), e) };
			return Rml::DataVariable{ model->object_definition.get(), &(iterator.first->second) };
		} else { //@note: accessing by index.
			//@note: see if we have a key with the index.
			sol::object has_index{ table.get<sol::object>(address.index) };
			if(has_index.get_type() != sol::type::lua_nil) {
				auto iterator{ model->object_map.insert_or_assign(tablestr + "_" + std::to_string(address.index), has_index) };
				return Rml::DataVariable{ model->object_definition.get(), &(iterator.first->second) };
			}

			//@note: iterate through the entries and grab the nth entry.
			for(int idx{ }; auto& [k, v] : table.pairs()) {
				if(idx == address.index)
				{
					auto iterator{ model->object_map.insert_or_assign(tablestr + "_" + std::to_string(idx), v) };
					return Rml::DataVariable{ model->object_definition.get(), &(iterator.first->second) };
				}
				++idx;
			}

			//@note: index out of range.
			return Rml::DataVariable{ };
		}

		//@note: failure.
		return Rml::DataVariable{ };
	}
}
