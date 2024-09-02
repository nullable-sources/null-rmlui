#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

template <typename coordinates_t>
struct ntl::compatibility::data_type_converter_t<vec4_t<coordinates_t>, Rml::Vector4<coordinates_t>> {
    static inline Rml::Vector4<coordinates_t> convert(const vec4_t<coordinates_t>& vec) { return { vec.x, vec.y, vec.z, vec.w }; }
};

template <typename coordinates_t>
struct ntl::compatibility::data_type_converter_t<Rml::Vector4<coordinates_t>, vec4_t<coordinates_t>> {
    static inline vec4_t<coordinates_t> convert(const Rml::Vector4<coordinates_t>& vec) { return { vec.x, vec.y, vec.z, vec.w }; }
};