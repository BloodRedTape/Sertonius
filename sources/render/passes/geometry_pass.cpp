#include "render/passes/geometry_pass.hpp"
#include <core/fixed_list.hpp>
#include <core/os/file.hpp>

GeometryPass::GeometryPass(const RenderTargets& targets):
	m_RenderTargets(targets)
{

	FixedList<const Shader*, 2> shaders;
	shaders.Add(Shader::Create(ShaderLang::GLSL, ShaderStageBits::Vertex, File::ReadEntire("shaders/geom.vert.glsl").Value()));
	shaders.Add(Shader::Create(ShaderLang::GLSL, ShaderStageBits::Fragment, File::ReadEntire("shaders/geom.frag.glsl").Value()));

	GraphicsPipelineProperties props;
	props.VertexAttributes = Vertex::AttributesList;
	props.Shaders = shaders;
	props.Layout = m_Layout.Get();
	props.Pass = targets.GeometryRenderPass.Get();

	m_Pipeline = GraphicsPipeline::Create(props);

	for (auto shader : shaders)
		delete shader;
}

void GeometryPass::CmdRender(CommandBuffer* cmd_buffer, Span<Mesh> meshes){
	cmd_buffer->SetViewport(0, 0, m_RenderTargets.Size().x, m_RenderTargets.Size().y);
	cmd_buffer->SetScissor(0, 0, m_RenderTargets.Size().x, m_RenderTargets.Size().y);
	cmd_buffer->Bind(m_Pipeline.Get());
	cmd_buffer->BeginRenderPass(m_RenderTargets.GeometryRenderPass.Get(), m_RenderTargets.GeometryFrameBuffer.Get());
	for (const Mesh& mesh : meshes)
		mesh.CmdDraw(*cmd_buffer);
	cmd_buffer->EndRenderPass();
}
