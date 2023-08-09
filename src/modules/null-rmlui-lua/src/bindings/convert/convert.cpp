#include "bindings/bindings.h"
#include "plugin/document/document.h"

namespace null::rml::modules::lua::convert_bindings {
	namespace functions {
		template <typename data_t>
		data_t* convert(Rml::Element* element) {
			if(data_t* test{ dynamic_cast<data_t*>(element) }; test) return test;
			return nullptr;
		}
	}

	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		sol::table as{ rmlui.create_named("element_conversion").create_named("as") };

		#define conversion(T) static_cast<T*(*)(Rml::Element*)>(&functions::convert)
		as["document"] = conversion(c_document);
		as["element_text"] = conversion(Rml::ElementText);
		as["element_data_grid"] = conversion(Rml::ElementDataGrid);
		as["element_data_grid_row"] = conversion(Rml::ElementDataGridRow);
		as["element_data_grid_cell"] = conversion(Rml::ElementDataGridCell);
		as["element_form"] = conversion(Rml::ElementForm);
		as["element_form_control"] = conversion(Rml::ElementFormControl);
		as["element_form_control_input"] = conversion(Rml::ElementFormControlInput);
		as["element_form_control_select"] = conversion(Rml::ElementFormControlSelect);
		as["element_form_control_data_select"] = conversion(Rml::ElementFormControlDataSelect);
		as["element_form_control_text_area"] = conversion(Rml::ElementFormControlTextArea);
		as["element_tab_set"] = conversion(Rml::ElementTabSet);
	}
}