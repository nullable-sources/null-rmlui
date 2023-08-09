#include "null-rmlui-lua.h"

#include "bindings/bindings.h"
#include "plugin/plugin.h"

namespace null::rml::modules::lua {
    void initialize(sol::state_view& state_view) {
        Rml::RegisterPlugin(new c_plugin{ state_view });
        register_bindings(state_view);
    }

    void register_bindings(sol::state_view& state_view) {
        global_bindings::bind(state_view);
        colour_bindings::bind(state_view);
        context_bindings::bind(state_view);
        data_model_bindings::bind(state_view);
        element_bindings::bind(state_view);
        element_derived_bindings::bind(state_view);
        element_form_bindings::bind(state_view);
        document_bindings::bind(state_view);
        event_bindings::bind(state_view);
        log_bindings::bind(state_view);
        vector_bindings::bind(state_view);
        convert_bindings::bind(state_view);
    }
}