filter = {
    name = '',
    types = {},
    architectures = {}
}

function update_name_filter(document, element, event)
    filter.name = element:get_attribute('value')
    if(event.parameters.linebreak == true) then fill_processes(document)
    else filter_processes(document) end
end

function update_type_and_architecture_filter(document, element)
    -- @note: clear existed filters
    filter.types = {}
    filter.architectures = {}

    for value in string.gmatch(element:get_attribute('value'), "([^&]+)") do
        if(value == 'x64' or value == 'x86') then
            table.insert(filter.architectures, value)
        else
            table.insert(filter.types, value)
            rmlui.log.message(rmlui.e_log_type.info, 'cant fint \'process-form\' element.')
        end
    end

    filter_processes(document)
end

function filter_processes(document)
    local form = document:get_element_by_id('process-form')
    if(form == nil) then rmlui.log.message(rmlui.e_log_type.error, 'cant fint \'process-form\' element.') return end

    local radios = form:get_elements_by_tag_name('input')
    for i = 1, #radios do
        local name = radios[i].child_nodes[1]
        local pid = radios[i].child_nodes[2]
        local type = radios[i].child_nodes[3]
        local architecture = radios[i].child_nodes[4]
        
        local type_filter = true
        if(next(filter.types) ~= nil and #filter.types ~= 2) then
            for j = 1, #filter.types do
                if(filter.types[j] ~= type.inner_rml) then
                    type_filter = false
                end
            end
        end

        local architecture_filter = true
        if(next(filter.architectures) ~= nil and #filter.architectures ~= 2) then
            for j = 1, #filter.architectures do
                if(filter.architectures[j] ~= architecture.inner_rml) then
                    architecture_filter = false
                end
            end
        end

        if(type_filter == false or architecture_filter == false) then
            radios[i].style.display = 'none'
            goto continue
        end

        -- @note: filtering the name or pid
        if(string.match(name.inner_rml, filter.name) == nil and string.match(pid.inner_rml, filter.name) == nil) then
            radios[i].style.display = 'none'
            goto continue
        end

        radios[i].style.display = 'flex'
        ::continue::
    end

    local showed = 0
    local finded = 0
    for i = 1, #radios do
        if(radios[i].style.display == 'flex') then
            finded = finded + 1

            if(showed >= 50) then
                radios[i].style.display = 'none'
            else
                showed = showed + 1
            end
        end
    end

    local showed_span = document:get_element_by_id('showed-value')
    if(showed_span == nil) then return end
    showed_span.inner_rml = tostring(showed)

    local finded_span = document:get_element_by_id('finded-value')
    if(finded_span == nil) then return end
    finded_span.inner_rml = tostring(finded)

    local all_span = document:get_element_by_id('all-value')
    if(all_span == nil) then return end
    all_span.inner_rml = tostring(#radios)
end