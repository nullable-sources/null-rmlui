-- <input type="radio" id="...">
--     <div class="info-content">
--         <span id="name">...</span>
--         <span id="architecture">...</span>
--     </div>
--     <span id="path">...</span>
-- </input>

function add_dll(document)
    local dll_info = sdk.select_dll()
    if(document:get_element_by_id(dll_info.path) ~= nil) then return end

    local form = document:get_element_by_id('dll-form')
    if(form == nil) then return end

    local element = document:create_element('input')
    element:set_attribute('type', 'radio')
    element:set_attribute('id', dll_info.path)
    
    local info_content = document:create_element("div")
    info_content:set_attribute('class', 'info-content')

    local name = document:create_element('span')
    name.inner_rml = dll_info.name
    name:set_attribute('id', 'name')
    info_content:append_child(name)

    local architecture = document:create_element('span')
    architecture.inner_rml = dll_info.architecture
    architecture:set_attribute('id', 'architecture')
    info_content:append_child(architecture)
    element:append_child(info_content)

    local path = document:create_element('span')
    path.inner_rml = dll_info.path
    path:set_attribute('id', 'path')
    element:append_child(path)

    element:add_event_listener('change', select_path, true)
    form:append_child(element)
end

function remove_dll(document)
    local form = document:get_element_by_id('dll-form')
    if(form == nil) then return end

    local dll = document:get_element_by_id(selected.dll)
    if(dll == nil) then return end

    form:remove_child(dll)
end