-- <input type="radio" id="...">
--     <span id="name">...</span>
--     <span id="pid">...</span>
--     <span id="type">...</span>
--     <span id="architecture">...</span>
-- </input>

function fill_processes(document)
    local form = document:get_element_by_id('process-form')
    if(form == nil) then return end

    local radios = form:get_elements_by_tag_name('input')
    for i = 1, #radios do form:remove_child(radios[i]) end

    local process_list = sdk.get_processes()
    for i = 1, #process_list do
        local element = document:create_element('input')
        element:set_attribute('type', 'radio')
        element:set_attribute('id', tostring(process_list[i].pid))
        
        local name = document:create_element('span')
        name.inner_rml = process_list[i].name
        name:set_attribute('id', 'name')
        element:append_child(name)

        local pid = document:create_element('span')
        pid.inner_rml = tostring(process_list[i].pid)
        pid:set_attribute('id', 'pid')
        element:append_child(pid)

        local type = document:create_element('span')
        type.inner_rml = process_list[i].type
        type:set_attribute('id', 'type')
        element:append_child(type)

        local architecture = document:create_element('span')
        architecture.inner_rml = process_list[i].architecture
        architecture:set_attribute('id', 'architecture')
        element:append_child(architecture)

        element:add_event_listener('change', select_pid, true)
        form:append_child(element)
    end

    filter_processes(document)
end