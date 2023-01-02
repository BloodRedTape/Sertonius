#include "lighting_pass.hpp"

Array<VertexAttribute, 4> LightVertex::AttributesList{
	VertexAttribute::Float32x2,
	VertexAttribute::Float32x3,
	VertexAttribute::Float32x3,
	VertexAttribute::Float32x1
};

LightingPass::LightingPass(const RenderTargets& rts):
	m_RenderTargets(rts),
	m_SetLayout(
		DescriptorSetLayout::Create({
			ShaderBinding(0, 1, ShaderBindingType::Texture, ShaderStageBits::Fragment),
			ShaderBinding(1, 1, ShaderBindingType::Texture, ShaderStageBits::Fragment),
		})
	)
{
	FixedList<const Shader*, 2> shaders;
	shaders.Add(Shader::Create(ShaderLang::GLSL, ShaderStageBits::Vertex, File::ReadEntire("shaders/light.vert.glsl").Value()));
	shaders.Add(Shader::Create(ShaderLang::GLSL, ShaderStageBits::Fragment, File::ReadEntire("shaders/light.frag.glsl").Value()));

	GraphicsPipelineProperties props;
	props.VertexAttributes = LightVertex::AttributesList;
	props.Shaders = shaders;
	props.BlendFunction = BlendFunction::Add;
	props.SrcBlendFactor = BlendFactor::One;
	props.DstBlendFactor = BlendFactor::One;
	props.Layout = m_SetLayout.Get();
	props.DepthFunction = DepthFunction::Less;
	props.Pass = m_RenderTargets.LightingRenderPass.Get();

	m_Pipeline = GraphicsPipeline::Create(props);

	for (auto shader : shaders)
		delete shader;
}

List<LightVertex> BuildLightVertexData(const Scene& scene) {
	List<LightVertex> res;
	
	for (const RenderPointLight& light : scene.PointLights) {
		res.Add({
			{-1.f, 1.f},
			light.Position,
			light.Color,
			light.Radius
		});
		res.Add({
			{1.f, -1.f},
			light.Position,
			light.Color,
			light.Radius
		});
		res.Add({
			{-1.f,-1.f},
			light.Position,
			light.Color,
			light.Radius
		});
		res.Add({
			{-1.f, 1.f},
			light.Position,
			light.Color,
			light.Radius
		});
		res.Add({
			{1.f, -1.f},
			light.Position,
			light.Color,
			light.Radius
		});
		res.Add({
			{1.f, 1.f},
			light.Position,
			light.Color,
			light.Radius
		});
	}

	return res;
}

void LightingPass::CmdRender(CommandBuffer * cmd_buffer, const Scene & scene){
	m_Set->UpdateTextureBinding(0, 0, m_RenderTargets.Position.Get(), m_Sampler.Get());
	m_Set->UpdateTextureBinding(1, 0, m_RenderTargets.Normal.Get(), m_Sampler.Get());
	
	auto light_vertex_data = BuildLightVertexData(scene);

	if (m_LightGeometry->Size() < light_vertex_data.Size() * sizeof(light_vertex_data[0])) {
		m_LightGeometry->Realloc(light_vertex_data.Size() * sizeof(light_vertex_data[0]));
	}
	m_LightGeometry->Copy(light_vertex_data.Data(), light_vertex_data.Size() * sizeof(light_vertex_data[0]));

	cmd_buffer->Bind(m_Pipeline.Get());
	cmd_buffer->Bind(m_Set.Get());
	cmd_buffer->ClearColor(m_RenderTargets.Lighting.Get(), Color::Black);

	cmd_buffer->ChangeLayout(m_RenderTargets.Position.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Normal.Get(), TextureLayout::ShaderReadOnlyOptimal);
	cmd_buffer->BeginRenderPass(m_RenderTargets.LightingRenderPass.Get(), m_RenderTargets.LightingFrameBuffer.Get());
	{
		cmd_buffer->BindVertexBuffer(m_LightGeometry.Get());
		cmd_buffer->Draw(scene.PointLights.Size() * 6);
	}
	cmd_buffer->EndRenderPass();
	cmd_buffer->ChangeLayout(m_RenderTargets.Position.Get(), TextureLayout::ColorAttachmentOptimal);
	cmd_buffer->ChangeLayout(m_RenderTargets.Normal.Get(), TextureLayout::ColorAttachmentOptimal);
}
