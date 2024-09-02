#include "null-rmlui.h"

namespace ntl::rml::extensions {
    void i_widget_colorpicker::add_events() {
        canvas->AddEventListener(Rml::EventId::Mousedown, this);
        canvas->AddEventListener(Rml::EventId::Drag, this);

        if(copy_button) copy_button->AddEventListener(Rml::EventId::Click, this);
        if(paste_button) paste_button->AddEventListener(Rml::EventId::Click, this);

        hue_slider->AddEventListener(Rml::EventId::Change, this);
        alpha_slider->AddEventListener(Rml::EventId::Change, this);
        if(value_input) value_input->AddEventListener(Rml::EventId::Change, this);

        control_element->AddEventListener(Rml::EventId::Change, this);
    }

    void i_widget_colorpicker::remove_events() {
        canvas->RemoveEventListener(Rml::EventId::Mousedown, this);
        canvas->RemoveEventListener(Rml::EventId::Drag, this);

        if(copy_button) copy_button->RemoveEventListener(Rml::EventId::Click, this);
        if(paste_button) paste_button->RemoveEventListener(Rml::EventId::Click, this);

        hue_slider->RemoveEventListener(Rml::EventId::Change, this);
        alpha_slider->RemoveEventListener(Rml::EventId::Change, this);
        if(value_input) value_input->RemoveEventListener(Rml::EventId::Change, this);

        control_element->RemoveEventListener(Rml::EventId::Change, this);
    }

    void i_widget_colorpicker::on_update() {
        if(value_dirty) {
            const color_t<int> color = build_color();
            const std::string hex = format_to_hex(color);
            set_elements_value(hex);
            set_elements_color(color);

            value_dirty = false;
            value_from_input = false;

            indicator_dirty = true;
        }

        if(indicator_dirty && control_element->IsVisible()) {
            const vec2_t<float> box_size = canvas->GetBox().GetSize(Rml::BoxArea::Border);
            if(box_size == 0.f) return;

            const vec2_t<float> sv_offset = canvas->GetBox().GetSize(Rml::BoxArea::Border) * Rml::Vector2f(canvas->GetAttribute("saturation", 0.f), 1.f - canvas->GetAttribute("brightness", 0.f));
            const vec2_t<float> indicator_size = indicator->GetBox().GetSize(Rml::BoxArea::Border);
            indicator->SetOffset(math::min(math::max(sv_offset - indicator_size / 2.f, vec2_t(0.f)), box_size - indicator_size), canvas);

            indicator_dirty = false;
        }
    }

    void i_widget_colorpicker::format_elements() {
        Rml::ElementUtilities::FormatElement(indicator, canvas->GetBox().GetSize(Rml::BoxArea::Border));
    }

    void i_widget_colorpicker::set_elements_value(const std::string& value) {
        lock_value = true;
        control_element->SetAttribute("value", value);
        if(value_input && !value_from_input)
            value_input->SetAttribute("value", value);
        lock_value = false;
    }

    void i_widget_colorpicker::on_value_change(const std::string& value) {
        if(!lock_value && value.starts_with("#") && 7 >= value.size() <= 9) {
            const std::vector<int> color = value | std::views::drop(1) | std::views::chunk(2) | std::views::transform([](const auto& range) { return (int)std::strtoul(std::string{ range.begin(), range.end() }.c_str(), nullptr, 16); }) | std::ranges::to<std::vector>();

            const hsv_color_t hsv = hsv_color_t(color_t<int>(color));
            hue_slider->SetAttribute("value", hsv.h);
            canvas->SetAttribute("saturation", hsv.s);
            canvas->SetAttribute("brightness", hsv.v);
            if(color.size() == 4 && !ignore_alpha) alpha_slider->SetAttribute("value", hsv.a);
        }

        control_element->DispatchEvent(Rml::EventId::Change, { { "value", Rml::Variant{ value } } });
    }

    void i_widget_colorpicker::ProcessEvent(Rml::Event& event) {
        switch(event.GetId()) {
            case Rml::EventId::Drag:
            case Rml::EventId::Mousedown: {
                if(event.GetCurrentElement() == canvas) {
                    Rml::Vector2f offset = (Rml::Vector2f(event.GetParameter("mouse_x", 0.f), event.GetParameter("mouse_y", 0.f)) - canvas->GetAbsoluteOffset(Rml::BoxArea::Border)) / canvas->GetBox().GetSize(Rml::BoxArea::Border);
                    canvas->SetAttribute("saturation", std::clamp(offset.x, 0.f, 1.f));
                    canvas->SetAttribute("brightness", std::clamp(1.f - offset.y, 0.f, 1.f));
                    value_dirty = true;
                }
            } break;

            case Rml::EventId::Click: {
                if(!copy_button || !paste_button || event.GetParameter("button", -1) != 0) break;

                if(event.GetCurrentElement() == copy_button) {
                    const std::string hex = format_to_hex(build_color());
                    set_elements_value(hex);
                    system_interface->SetClipboardText(hex);
                } else if(event.GetCurrentElement() == paste_button) {
                    std::string clipboard{ };
                    system_interface->GetClipboardText(clipboard);
                    if(!clipboard.empty() && (clipboard.starts_with("#") && 7 >= clipboard.size() <= 9)) {
                        control_element->SetAttribute("value", clipboard);
                    }
                }
            } break;

            case Rml::EventId::Change: {
                if(value_input && event.GetCurrentElement() == value_input) {
                    on_value_change(value_input->GetAttribute("value", std::string{ }));
                    value_from_input = true;
                }
                value_dirty = true;
            } break;
        }
    }
}