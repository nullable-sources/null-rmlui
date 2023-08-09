#include "bindings/bindings.h"

namespace null::rml::modules::lua::colour_bindings {
	template <typename data_t>
	using colour_tuple_t = std::tuple<data_t, data_t, data_t, data_t>;

	template <typename data_t, int default_alpha>
	colour_tuple_t<data_t> get_rgba(Rml::Colour<data_t, default_alpha>& self) {
		return colour_tuple_t<data_t>{ self.red, self.green, self.blue, self.alpha };
	}

	template <typename data_t, int default_alpha>
	void set_rgba(Rml::Colour<data_t, default_alpha>& self, const colour_tuple_t<data_t>& color) {
		sol::tie(self.red, self.green, self.blue, self.alpha) = color;
	}

	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<Rml::Colourb>("c_colourb", sol::constructors<Rml::Colourb(), Rml::Colourb(Rml::byte, Rml::byte, Rml::byte), Rml::Colourb(Rml::byte, Rml::byte, Rml::byte, Rml::byte)>(),
			sol::meta_function::addition, &Rml::Colourb::operator+,
			sol::meta_function::subtraction, &Rml::Colourb::operator-,
			sol::meta_function::multiplication, &Rml::Colourb::operator*,
			sol::meta_function::division, &Rml::Colourb::operator/,
			sol::meta_function::equal_to, &Rml::Colourb::operator==,

			"red", &Rml::Colourb::red,
			"green", &Rml::Colourb::green,
			"blue", &Rml::Colourb::blue,
			"alpha", &Rml::Colourb::alpha,
			"rgba", sol::property(static_cast<colour_tuple_t<Rml::byte>(*)(Rml::Colourb&)>(&get_rgba), static_cast<void(*)(Rml::Colourb&, const colour_tuple_t<Rml::byte>&)>(&set_rgba))
		);

		rmlui.new_usertype<Rml::Colourf>("c_colourf", sol::constructors<Rml::Colourf(), Rml::Colourf(float, float, float), Rml::Colourf(float, float, float, float)>(),
			sol::meta_function::addition, &Rml::Colourf::operator+,
			sol::meta_function::subtraction, &Rml::Colourf::operator-,
			sol::meta_function::multiplication, &Rml::Colourf::operator*,
			sol::meta_function::division, &Rml::Colourf::operator/,
			sol::meta_function::equal_to, &Rml::Colourf::operator==,

			"red", &Rml::Colourf::red,
			"green", &Rml::Colourf::green,
			"blue", &Rml::Colourf::blue,
			"alpha", &Rml::Colourf::alpha,
			"rgba", sol::property(static_cast<colour_tuple_t<float>(*)(Rml::Colourf&)>(&get_rgba), static_cast<void(*)(Rml::Colourf&, const colour_tuple_t<float>&)>(&set_rgba))
		);
	}
}