#include <bindings/bindings.h>

#include <unordered_map>

namespace null::rml::modules::lua::element_derived_bindings {
	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_usertype<Rml::ElementText>("c_element_text", sol::no_constructor,
			"text", sol::property(&Rml::ElementText::GetText, &Rml::ElementText::SetText),

			sol::base_classes, sol::bases<Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementDataGrid>("c_element_data_grid", sol::no_constructor,
			"add_column", sol::resolve<bool(const std::string&, const std::string&, float, const std::string&)>(&Rml::ElementDataGrid::AddColumn),
			"set_data_source", &Rml::ElementDataGrid::SetDataSource,

			"rows", sol::readonly_property(&get_indexed_table<Rml::ElementDataGridRow, Rml::ElementDataGrid, &Rml::ElementDataGrid::GetRow, &Rml::ElementDataGrid::GetNumRows>),

			sol::base_classes, sol::bases<Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementDataGridRow>("c_element_data_grid_row", sol::no_constructor,
			"refresh_rows", &Rml::ElementDataGridRow::RefreshRows,

			"expanded", sol::property(&Rml::ElementDataGridRow::IsRowExpanded, [](Rml::ElementDataGridRow& self, bool expanded) { if(expanded) self.ExpandRow(); else self.CollapseRow(); }),
			"parent_relative_index", sol::readonly_property(&Rml::ElementDataGridRow::GetParentRelativeIndex),
			"table_relative_index", sol::readonly_property(&Rml::ElementDataGridRow::GetTableRelativeIndex),
			"parent_row", sol::readonly_property(&Rml::ElementDataGridRow::GetParentRow),
			"parent_grid", sol::readonly_property(&Rml::ElementDataGridRow::GetParentGrid),

			sol::base_classes, sol::bases<Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementDataGridCell>("c_element_data_grid_cell", sol::no_constructor,
			"column", sol::readonly_property(&Rml::ElementDataGridCell::GetColumn),

			sol::base_classes, sol::bases<Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementTabSet>("c_element_tab_set", sol::no_constructor,
			"set_panel", sol::resolve<void(int, const std::string&)>(&Rml::ElementTabSet::SetPanel),
			"set_tab", sol::resolve<void(int, const std::string&)>(&Rml::ElementTabSet::SetTab),
			"remove_tab", &Rml::ElementTabSet::RemoveTab,

			"active_tab", sol::property(&Rml::ElementTabSet::GetActiveTab, &Rml::ElementTabSet::SetActiveTab),
			"num_tabs", &Rml::ElementTabSet::GetNumTabs,

			sol::base_classes, sol::bases<Rml::Element>()
		);

		rmlui.new_usertype<Rml::ElementProgress>("c_element_progress", sol::no_constructor,
			"value", sol::property(&Rml::ElementProgress::GetValue, &Rml::ElementProgress::SetValue),
			"max", sol::property(&Rml::ElementProgress::GetMax, &Rml::ElementProgress::SetMax),

			sol::base_classes, sol::bases<Rml::Element>()
		);
	}
}