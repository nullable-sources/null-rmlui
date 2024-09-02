#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

template <typename coordinates_t>
struct ntl::compatibility::data_type_converter_t<vec2_t<coordinates_t>, Rml::Vector2<coordinates_t>> {
    static inline Rml::Vector2<coordinates_t> convert(const vec2_t<coordinates_t>& vec) { return { vec.x, vec.y }; }
};

template <typename coordinates_t>
struct ntl::compatibility::data_type_converter_t<Rml::Vector2<coordinates_t>, vec2_t<coordinates_t>> {
    static inline vec2_t<coordinates_t> convert(const Rml::Vector2<coordinates_t>& vec) { return { vec.x, vec.y }; }
};