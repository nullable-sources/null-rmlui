#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

template <typename coordinates_t>
struct null::compatibility::data_type_converter_t<rect_t<coordinates_t>, Rml::Rectangle<coordinates_t>> {
    static inline Rml::Rectangle<coordinates_t> convert(const rect_t<coordinates_t>& rect) { return Rml::Rectangle<coordinates_t>::FromCorners(Rml::Vector2<coordinates_t>(rect.min.x, rect.min.y), Rml::Vector2<coordinates_t>(rect.max.x, rect.max.y)); }
};

template <typename coordinates_t>
struct null::compatibility::data_type_converter_t<Rml::Rectangle<coordinates_t>, rect_t<coordinates_t>> {
    static inline rect_t<coordinates_t> convert(const Rml::Rectangle<coordinates_t>& rect) { return { rect.p0.x, rect.p0.y, rect.p1.x, rect.p1.y }; }
};