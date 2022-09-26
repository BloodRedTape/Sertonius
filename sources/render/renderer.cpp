#include "render/renderer.hpp"
#include <graphics/api/gpu.hpp>
#include <core/unique_ptr.hpp>

Renderer::Renderer(const Window &window):
	m_Window(window),
	m_RenderTargets(window)
{}

Semaphore acq, pst;
Fence fence;

//int s = (fence.Signal(), 1);

void Renderer::Render(Span<MeshComponent> meshes){
	m_RenderTargets.AcquireNext(&acq);

	m_CmdBuffer->Begin();
	m_GeometryPass.CmdRender(m_CmdBuffer.Get(), meshes);
	m_CompositePass.CmdRender(m_CmdBuffer.Get());
	m_CmdBuffer->End();


	GPU::Execute(m_CmdBuffer.Get(), acq, pst, fence);
	fence.WaitAndReset();
	m_RenderTargets.PresentCurrent(&pst);
}
