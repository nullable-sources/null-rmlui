# Using guide
- compile RmlCore/RmlDebugger/RmlSvg
- link include directories and .lib files

# Changes
- Added additional methods for data models. Enabled via RMLUI_DATAMODELS_EXTENSIONS. (used by default)
- Some elements have changed access qualifiers and added method override capability. Enabled via RMLUI_ELEMENTS_EXTENSIONS. (used by default)
- In the latest version of RmlUI scrollbar ignores the right and bottom padding, to roll back to the behavior of version 5.1, use RMLUI_OLD_SCROLL_PADDING (used by default)
- The standard RmlUI supports only percentage and integer color components, support for floating-point color components is added through RMLUI_FLOAT_COLOR_PARSER (used by default)
- RmlUi throws assert when using nested data-models (I personally have not noticed any errors because of this), to disable assert, you can use RMLUI_ALLOW_NESTED_DATAMODELS (used by default)
- RMLUI_FIX_DRAG_CLONE_POSITION (used by default) - corrects the drag_clone position when creating