selected_process = nil
selected_dll = nil

function select_dll() selected_dll = impl_select_dll() end

function inject()
    if(selected_process ~= nil and selected_dll ~= nil) then
        impl_inject(selected_process, selected_dll)
    end
end

function select_process(event)
    if(event.parameters['value'] == 'on') then
        selected_process = event.target_element:get_attribute("id")
    end
end

function filter(document, element)
    local form = document:get_element_by_id('process-form')
    if(form == nil) then return end

    local radios = form:get_elements_by_tag_name('input')
    for i = 1, #radios do
        if(element:get_attribute('value') == '' or string.match(radios[i].inner_rml, element:get_attribute('value'))) then radios[i].style.display = 'inline-block'
        else radios[i].style.display = 'none' end
    end
end

function get_process_list(document)
    local form = document:get_element_by_id('process-form')
    if(form == nil) then return end

    local radios = form:get_elements_by_tag_name('input')
    for i = 1, #radios do form:remove_child(radios[i]) end

    local process_list = impl_get_process_list()
    for i = 1, #process_list do
        local element = document:create_element('input')
        form:append_child(element)
        element.inner_rml = '[' .. process_list[i].pid .. '] ' .. process_list[i].name
        element:set_attribute('type', 'radio')
        element:set_attribute('id', tostring(process_list[i].pid))
        element:add_event_listener('change', select_process, true)
    end
end