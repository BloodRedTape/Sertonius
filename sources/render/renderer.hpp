#pragma once

#include <graphics/api/graphics_pipeline.hpp>
#include <graphics/api/semaphore.hpp>
#include <graphics/api/framebuffer.hpp>

class Renderer {
private:
	const RenderPass *m_RenderPass;
public:
	Renderer(const RenderPass *pass):
		m_RenderPass(pass)
	{}

	void Render(const Framebuffer *framebuffer, Semaphore *wait, Semaphore *signal);
};