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
        selected_process = event.target_element:GetAttribute("id")
    end
end

function filter(document, element)
    local form = document:GetElementById('process-form')
    if(form == nil) then return end

    print(form:GetAttribute('value'))

    local radios = form:GetElementsByTagName('input')
    for i = 1, #radios do
        if(element:GetAttribute('value') == '' or string.match(radios[i].inner_rml, element:GetAttribute('value'))) then radios[i].style.display = 'inline-block'
        else radios[i].style.display = 'none' end
    end
end

function get_process_list(document)
    local form = document:GetElementById('process-form')
    if(form == nil) then return end

    local radios = form:GetElementsByTagName('input')
    for i = 1, #radios do form:RemoveChild(radios[i]) end

    local process_list = impl_get_process_list()
    for i = 1, #process_list do
        form:AppendChild(document:CreateElement('input'))
        local element = form.last_child
        element.inner_rml = '[' .. process_list[i].pid .. '] ' .. process_list[i].name
        element:SetAttribute('type', 'radio')
        element:SetAttribute('id', process_list[i].pid)
        element:AddEventListener('change', select_process, true)
    end
end