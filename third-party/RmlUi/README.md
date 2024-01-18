# Using guide
- compile RmlCore/RmlDebugger/RmlSvg
- link include directories and .lib files

# Changes
- in the latest version of RmlUI scrollbar ignores the right and bottom padding, to roll back to the behavior of version 5.1, use RMLUI_OLD_SCROLL_PADDING (used by default)
- The standard RmlUI supports only percentage and integer color components, support for floating-point color components is added through RMLUI_FLOAT_COLOR_PARSER (used by default)