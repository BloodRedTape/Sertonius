#pragma once

#include <core/list.hpp>
#include <core/print.hpp>
#include <core/optional.hpp>
#include <core/unique_ptr.hpp>
#include <core/function.hpp>
#include <graphics/api/compute_pipeline.hpp>
#include <graphics/api/graphics_pipeline.hpp>


struct ShaderProperties {
	ShaderStageBits::Value Stage = ShaderStageBits::Vertex;
	StringView ShaderPath = "";
	ShaderCompileOptions Options = {ShaderLang::GLSL};
};

struct HotReloadPipeline {
	virtual ~HotReloadPipeline() = default;

	virtual void Reload() = 0;
};

struct HotReloadGraphicsPipelineProperties: GraphicsPipelineProperties{
	List<ShaderProperties> ShaderProperties;
};

struct HotReloadComputePipelineProperties: ComputePipelineProperties{
	ShaderProperties ShaderProperties;
};

class HotReloader {
private:
	List<UniquePtr<HotReloadPipeline>> m_Pipelines;
public:
	
	template <typename CallbackType>
	void Add(HotReloadGraphicsPipelineProperties props, CallbackType callback) {
		struct Pipeline: HotReloadPipeline{
			HotReloadGraphicsPipelineProperties Properties;
			CallbackType Callback;

			Pipeline(HotReloadGraphicsPipelineProperties props, CallbackType callback):
				Properties(Move(props)),
				Callback(Move(callback))
			{}
			
			void Reload()override{
				FixedList<UniquePtr<Shader>, 10> shaders;

				for (const auto& props : Properties.ShaderProperties) {
					auto sources = File::ReadEntire(props.ShaderPath);

					if (!sources.HasValue())
						return Println("Can't read shader from: %", props.ShaderPath);

					Shader* shader = Shader::Create(props.Stage, sources.Value(), props.Options);

					if(!shader)
						return Println("Failed shader compilation of: %", props.ShaderPath);
					shaders.Add(shader);
				}

				FixedList<const Shader*, 10> raw_shaders;
				for (auto& shader : shaders)
					raw_shaders.Add(shader.Get());

				Properties.Shaders = raw_shaders;

				Callback(GraphicsPipeline::Create(Properties));
			}
		};

		Pipeline* reloader = new Pipeline(Move(props), callback);
		reloader->Reload();
		m_Pipelines.Add(reloader);
	}

	template <typename CallbackType>
	void Add(const HotReloadComputePipelineProperties& props, CallbackType callback) {
		struct Pipeline: HotReloadPipeline{
			HotReloadComputePipelineProperties Properties;
			CallbackType Callback;

			Pipeline(HotReloadComputePipelineProperties props, CallbackType callback):
				Properties(Move(props)),
				Callback(Move(callback))
			{}
			
			void Reload()override {
				
				const auto& props = Properties.ShaderProperties;
				auto sources = File::ReadEntire(props.ShaderPath);

				if (!sources.HasValue())
					return Println("Can't read shader from: %", props.ShaderPath);

				UniquePtr<Shader> shader = Shader::Create(props.Stage, sources.Value(), props.Options);

				if (!shader)
					return Println("Failed shader compilation of: %", props.ShaderPath);

				Properties.ComputeShader = shader.Get();

				Callback(ComputePipeline::Create(Properties));
			}
		};

		Pipeline* reloader = new Pipeline(Move(props), callback);
		reloader->Reload();
		m_Pipelines.Add(reloader);
	}
	
	void Realod() {
		for (auto& pipeline : m_Pipelines)
			pipeline->Reload();
	}

	static HotReloader& Get() {
		static HotReloader instance;
		return instance;
	}
};