#include "bindings/bindings.h"

namespace null::rml::modules::lua::vector_bindings {
	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<Rml::Vector2i>("c_vector2i", sol::constructors<Rml::Vector2i(), Rml::Vector2i(int, int)>(),
			sol::meta_function::addition, &Rml::Vector2i::operator+,
			sol::meta_function::subtraction, sol::resolve<Rml::Vector2i(Rml::Vector2i) const>(&Rml::Vector2i::operator-),
			sol::meta_function::multiplication, sol::overload(
				sol::resolve<Rml::Vector2i(int) const>(&Rml::Vector2i::operator*),
				sol::resolve<Rml::Vector2i(Rml::Vector2i) const>(&Rml::Vector2i::operator*)
			),
			sol::meta_function::division, sol::overload(
				sol::resolve<Rml::Vector2i(int) const>(&Rml::Vector2i::operator/),
				sol::resolve<Rml::Vector2i(Rml::Vector2i) const>(&Rml::Vector2i::operator/)
			),
			sol::meta_function::unary_minus, sol::resolve<Rml::Vector2i() const>(&Rml::Vector2i::operator-),

			"x", &Rml::Vector2i::x,
			"y", &Rml::Vector2i::y,

			"magnitude", &Rml::Vector2i::Magnitude
		);

		rmlui.new_usertype<Rml::Vector2f>("c_vector2f", sol::constructors<Rml::Vector2f(), Rml::Vector2f(float, float)>(),
			sol::meta_function::addition, &Rml::Vector2f::operator+,
			sol::meta_function::subtraction, sol::resolve<Rml::Vector2f(Rml::Vector2f) const>(&Rml::Vector2f::operator-),
			sol::meta_function::multiplication, sol::overload(
				sol::resolve<Rml::Vector2f(float) const>(&Rml::Vector2f::operator*),
				sol::resolve<Rml::Vector2f(Rml::Vector2f) const>(&Rml::Vector2f::operator*)
			),
			sol::meta_function::division, sol::overload(
				sol::resolve<Rml::Vector2f(float) const>(&Rml::Vector2f::operator/),
				sol::resolve<Rml::Vector2f(Rml::Vector2f) const>(&Rml::Vector2f::operator/)
			),
			sol::meta_function::unary_minus, sol::resolve<Rml::Vector2f() const>(&Rml::Vector2f::operator-),

			"x", &Rml::Vector2f::x,
			"y", &Rml::Vector2f::y,

			"magnitude", &Rml::Vector2f::Magnitude
		);
	}
}