#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

template <>
struct ntl::compatibility::data_type_converter_t<ntl::sdk::i_color<float>, Rml::Colourf> {
    static inline Rml::Colourf convert(const ntl::sdk::i_color<float>& color) { return { color.r, color.g, color.b, color.a }; }
};

template <>
struct ntl::compatibility::data_type_converter_t<Rml::Colourf, ntl::sdk::i_color<float>> {
    static inline ntl::sdk::i_color<float> convert(const Rml::Colourf& color) { return { color.red, color.green, color.blue, color.alpha }; }
};

template <>
struct ntl::compatibility::data_type_converter_t<ntl::sdk::i_color<int>, Rml::Colourb> {
    static inline Rml::Colourb convert(const ntl::sdk::i_color<int>& color) { return { (byte)color.r, (byte)color.g, (byte)color.b, (byte)color.a }; }
};

template <>
struct ntl::compatibility::data_type_converter_t<Rml::Colourb, ntl::sdk::i_color<int>> {
    static inline ntl::sdk::i_color<int> convert(const Rml::Colourb& color) { return { color.red, color.green, color.blue, color.alpha }; }
};

template <>
struct ntl::compatibility::data_type_converter_t<ntl::sdk::i_color<int>, Rml::ColourbPremultiplied> {
    static inline Rml::ColourbPremultiplied convert(const ntl::sdk::i_color<int>& color) { return { (byte)color.r, (byte)color.g, (byte)color.b, (byte)color.a }; }
};

template <>
struct ntl::compatibility::data_type_converter_t<Rml::ColourbPremultiplied, ntl::sdk::i_color<int>> {
    static inline ntl::sdk::i_color<int> convert(const Rml::ColourbPremultiplied& color) { return { color.red, color.green, color.blue, color.alpha }; }
};