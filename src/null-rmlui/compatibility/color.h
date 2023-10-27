#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

template <>
struct null::compatibility::data_type_converter_t<null::sdk::i_color<float>, Rml::Colourf> {
    static inline Rml::Colourf convert(const null::sdk::i_color<float>& color) { return { color.r, color.g, color.b, color.a }; }
};

template <>
struct null::compatibility::data_type_converter_t<Rml::Colourf, null::sdk::i_color<float>> {
    static inline null::sdk::i_color<float> convert(const Rml::Colourf& color) { return { color.red, color.green, color.blue, color.alpha }; }
};

template <>
struct null::compatibility::data_type_converter_t<null::sdk::i_color<int>, Rml::Colourb> {
    static inline Rml::Colourb convert(const null::sdk::i_color<int>& color) { return { (byte)color.r, (byte)color.g, (byte)color.b, (byte)color.a }; }
};

template <>
struct null::compatibility::data_type_converter_t<Rml::Colourb, null::sdk::i_color<int>> {
    static inline null::sdk::i_color<int> convert(const Rml::Colourb& color) { return { color.red, color.green, color.blue, color.alpha }; }
};