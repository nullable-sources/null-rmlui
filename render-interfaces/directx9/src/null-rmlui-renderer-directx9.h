#pragma once
#include "null-rmlui-renderer-directx9/mesh/mesh.h"

namespace null::rml::directx9 {
	class c_render : public i_render_interface {
	public:
		void set_secondary_texture(void* texture) override;
		void set_blend_state(bool enabled) override;
		void set_blend_function(bool enabled, float alpha) override;

	public:
		Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;
		void EnableScissorRegion(bool enable) override;

	public:
		void initialize() override;
	};
}