#include "render/renderer.hpp"
#include <graphics/api/gpu.hpp>
#include <core/unique_ptr.hpp>


void Renderer::Render(const Framebuffer *framebuffer, Semaphore *wait, Semaphore *signal){
	
	UniquePtr<CommandPool> cmd_pool(CommandPool::Create());
	CommandBuffer* cmd_buffer = cmd_pool->Alloc();
	
	cmd_buffer->Begin();
	cmd_buffer->ClearColor(framebuffer->Attachments()[0], Color::Red);
	cmd_buffer->End();
	Fence fence;
	GPU::Execute(cmd_buffer, *wait, *signal, fence);
	fence.WaitFor();
}
