#pragma once
#include <type_traits>
#include <RmlUi/Core.h>

#include "bindings/colour/colour.h"
#include "bindings/context/context.h"
#include "bindings/convert/convert.h"
#include "bindings/data-model/data-model.h"
#include "bindings/document/document.h"
#include "bindings/element/element.h"
#include "bindings/element-derived/element-derived.h"
#include "bindings/element-form/element-form.h"
#include "bindings/event/event.h"
#include "bindings/global/global.h"
#include "bindings/log/log.h"
#include "bindings/vector/vector.h"

#ifndef RMLUI_NO_THIRDPARTY_CONTAINERS
template <typename key, typename value>
struct sol::is_container<Rml::UnorderedMap<key, value>> : std::true_type { };

template <typename Key, typename value>
struct sol::is_container<Rml::SmallUnorderedMap<Key, value>> : std::true_type { };

template <typename t>
struct sol::is_container<Rml::UnorderedSet<t>> : std::true_type { };

template <typename t>
struct sol::is_container<Rml::SmallUnorderedSet<t>> : std::true_type { };

template <>
struct sol::is_container<Rml::ElementList> : std::true_type { };
#endif

namespace null::rml::modules::lua {
	template <typename type_t = Rml::Element>
	struct table_indexed_iterator_t {
		struct iterator_t {
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = type_t*;
			using pointer = type_t**;
			using reference = type_t*&;

		private:
			const table_indexed_iterator_t<type_t>* owner{ };
			mutable int pos{ };

		public:
			iterator_t(const table_indexed_iterator_t<type_t>* _owner, const int& _pos) : owner{ _owner }, pos{ _pos } { }

		public:
			auto operator++() const { pos = std::min(++pos, owner->func_max()); return *this; }
			auto operator++(int) const { pos = std::min(pos++, owner->func_max()); return *this; }

			auto operator==(const iterator_t& other) const {
				int max{ owner->func_max() };
				return std::min(pos, max) == std::min(other.pos, max);
			}

			auto operator*() const { return owner->func_get(pos); }
		};

	private:
		std::function<type_t* (int)> func_get{ };
		std::function<int()> func_max{ };

	public:
		table_indexed_iterator_t(std::function<type_t* (int)> get, std::function<int()> max) : func_get{ get }, func_max{ max } { assert((func_get) && (func_max)); }

	public:
		iterator_t begin() const { return iterator_t{ this, 0 }; }
		iterator_t end() const { return iterator_t{ this, func_max() }; }
		int size() const { return func_max(); }
	};

	template <typename type_t, typename self_t, auto get_fn, auto max_fn>
	sol::as_table_t<table_indexed_iterator_t<type_t>> get_indexed_table(self_t& self) {
		std::function<type_t* (int)> get{ };
		if constexpr (std::is_member_function_pointer_v<decltype(get_fn)>) {
			//@note: straight member function pointer like Rml::Element::GetChild.
			get = std::bind(get_fn, &self, std::placeholders::_1);
		} else {
			//@note: a helper function to convert the normal getter to type std::function<type_t* (int)>.
			get = std::invoke(get_fn, self);
		}

		std::function<int()> max{ };
		if constexpr (std::is_member_function_pointer_v<decltype(max_fn)>) {
			//@note: straight member function pointer like Rml::Element::GetNumChildren.
			max = std::bind(max_fn, &self);
		} else {
			//@npte: a helper function to convert the normal getter to type std::function<int ())>.
			max = std::invoke(max_fn, self);
		}

		return sol::as_table(table_indexed_iterator_t<type_t>{ get, max});
	}

	template <typename type_t, auto get_fn, auto max_fn>
	sol::as_table_t<table_indexed_iterator_t<type_t>> get_indexed_table() {
		return sol::as_table(table_indexed_iterator_t<type_t>{ std::invoke(get_fn), std::invoke(max_fn) });
	}

	sol::object make_object_from_variant(const Rml::Variant* variant, const sol::state_view& state_view);
}