#include "render/passes/geometry_pass.hpp"
#include <core/fixed_list.hpp>
#include <core/os/file.hpp>

GeometryPass::GeometryPass(const RenderTargets& targets) :
	m_RenderTargets(targets),
	m_SetLayout(
		DescriptorSetLayout::Create({
			ShaderBinding(0, 1, ShaderBindingType::UniformBuffer, ShaderStageBits::Vertex),
			ShaderBinding(1, 1, ShaderBindingType::UniformBuffer, ShaderStageBits::Vertex),
		})
	)
{

	FixedList<const Shader*, 2> shaders;
	shaders.Add(Shader::Create(ShaderLang::GLSL, ShaderStageBits::Vertex, File::ReadEntire("shaders/geom.vert.glsl").Value()));
	shaders.Add(Shader::Create(ShaderLang::GLSL, ShaderStageBits::Fragment, File::ReadEntire("shaders/geom.frag.glsl").Value()));

	GraphicsPipelineProperties props;
	props.VertexAttributes = Vertex::AttributesList;
	props.Shaders = shaders;
	props.Layout = m_SetLayout.Get();
	props.DepthFunction = DepthFunction::Less;
	props.Pass = targets.GeometryRenderPass.Get();

	m_Pipeline = GraphicsPipeline::Create(props);

	for (auto shader : shaders)
		delete shader;
}

void GeometryPass::CmdRender(CommandBuffer* cmd_buffer, const Scene &scene){
	m_SetPool.NextFrame();
	m_ModelUniformBufferPool.Reset();

	cmd_buffer->ClearColor(m_RenderTargets.Albedo.Get(), Color::Black);
	cmd_buffer->ClearColor(m_RenderTargets.Normal.Get(), Color::Black);
	cmd_buffer->ClearColor(m_RenderTargets.Position.Get(), Color::Black);
	cmd_buffer->ClearDepthStencil(m_RenderTargets.Depth.Get(), 1.f);

	m_CameraUniform.CmdUpdate(*cmd_buffer, {
		scene.Camera.MakeViewMatrix(),
		scene.Camera.MakeProjectionMatrix(),
	});

	cmd_buffer->SetViewport(0, 0, m_RenderTargets.Size().x, m_RenderTargets.Size().y);
	cmd_buffer->SetScissor(0, 0, m_RenderTargets.Size().x, m_RenderTargets.Size().y);
	cmd_buffer->Bind(m_Pipeline.Get());
	cmd_buffer->BeginRenderPass(m_RenderTargets.GeometryRenderPass.Get(), m_RenderTargets.GeometryFrameBuffer.Get());

	for (const RenderMesh& render_mesh : scene.Meshes) {
		UniformBuffer<ModelUniform>* model_uniform = m_ModelUniformBufferPool.AllocOrReuse();
		model_uniform->Update({ render_mesh.MakeTransformMatrix() });

		auto set = m_SetPool.Alloc();
		set->UpdateUniformBinding(0, 0, m_CameraUniform);
		set->UpdateUniformBinding(1, 0, *model_uniform);
		cmd_buffer->Bind(set);

		render_mesh.Mesh->CmdDraw(*cmd_buffer);
	}
	cmd_buffer->EndRenderPass();
}
