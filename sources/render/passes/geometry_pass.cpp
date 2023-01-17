#include "render/passes/geometry_pass.hpp"
#include "framework/assets_manager.hpp"
#include "render/common.hpp"
#include "render/hot_reloader.hpp"
#include <core/fixed_list.hpp>
#include <core/os/file.hpp>

GeometryPass::GeometryPass(const RenderTargets& targets) :
	m_RenderTargets(targets),
	m_SetLayout(
		DescriptorSetLayout::Create({
			ShaderBinding(0, 1, ShaderBindingType::UniformBuffer, ShaderStageBits::Vertex),
			ShaderBinding(1, 1, ShaderBindingType::UniformBuffer, ShaderStageBits::Vertex),
			ShaderBinding(2, 1, ShaderBindingType::UniformBuffer, ShaderStageBits::Fragment),
			ShaderBinding(3, 1, ShaderBindingType::Texture,       ShaderStageBits::Fragment),
			ShaderBinding(4, 1, ShaderBindingType::Texture,       ShaderStageBits::Fragment),
		})
	),
	m_DefaultNormal(
		Texture2D::Create(Image(1, 1, Color(0, 0, 0.5f)), TextureUsageBits::Sampled, TextureLayout::ShaderReadOnlyOptimal)
	)
{

	HotReloadGraphicsPipelineProperties props;
	props.VertexAttributes = Vertex::AttributesList;
	props.BlendFunction = BlendFunction::Add;
	props.SrcBlendFactor = BlendFactor::SrcAlpha;
	props.DstBlendFactor = BlendFactor::OneMinusSrcAlpha;
	props.Layout = m_SetLayout.Get();
	props.DepthFunction = DepthFunction::Less;
	props.Pass = targets.GeometryRenderPass.Get();

	props.ShaderProperties.Add({ ShaderStageBits::Vertex, "shaders/geom.vert.glsl", DefaultCompileOptions});
	props.ShaderProperties.Add({ ShaderStageBits::Fragment, "shaders/geom.frag.glsl", DefaultCompileOptions});
	
	HotReloader::Get().Add(Move(props), [this](GraphicsPipeline *pipeline) {
		m_Pipeline = pipeline;
	});
}

void GeometryPass::CmdRender(CommandBuffer* cmd_buffer, const Scene &scene){
	m_SetPool.NextFrame();
	m_ModelUniformBufferPool.Reset();
	m_MaterialUniformBufferPool.Reset();

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
		model_uniform->Update({ render_mesh.Transform });

		const Mesh *mesh = AssetsManager::Get(render_mesh.Mesh);
		mesh->Bind(*cmd_buffer);

		for (const MeshSection& section : mesh->Sections()) {
			const Material* mat = AssetsManager::Get(MaterialHandle{ section.MaterialIndex });

			UniformBuffer<MaterialUniform>* material_uniform = m_MaterialUniformBufferPool.AllocOrReuse();
			material_uniform->Update({ mat->ColorValue });

			auto set = m_SetPool.Alloc();

			set->UpdateUniformBinding(0, 0, m_CameraUniform);
			set->UpdateUniformBinding(1, 0, *model_uniform);
			set->UpdateUniformBinding(2, 0, *material_uniform);
			const Texture2D* albedo = AssetsManager::Get(TextureHandle(mat->ColorTextureIndex));
			set->UpdateTextureBinding(3, 0, albedo ? albedo : Texture2D::White(), m_Sampler.Get());
			const Texture2D* normal = AssetsManager::Get(TextureHandle(mat->NormalTextureIndex));
			set->UpdateTextureBinding(4, 0, normal ? normal : m_DefaultNormal.Get(), m_Sampler.Get());

			cmd_buffer->Bind(set);

			cmd_buffer->DrawIndexed(section.IndicesCount, section.BaseIndex, section.BaseVertex);
		}
		
	}
	cmd_buffer->EndRenderPass();
}
