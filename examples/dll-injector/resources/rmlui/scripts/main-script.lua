selected = {
    dll = '',
    pid = 0
}

function inject()
    if(selected.dll == '' or selected.pid == 0) then return end
    sdk.inject(selected.pid, selected.dll)
end

function select_path(event)
    if(event.parameters.value == 'on') then
        selected.dll = event.target_element:get_attribute("id")
    end
end

function select_pid(event)
    if(event.parameters.value == 'on') then
        selected.pid = event.target_element:get_attribute("id")
    end
end

function on_load(document)
    fill_processes(document)

    local span = document:get_element_by_id('build-architecture')
    span.inner_rml = sdk.get_architecture()
end