/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels 
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>

struct SDL_Window;
struct SDL_Surface;
class Camera;

namespace Elite
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void ProcessInput(float elapsedSec);
		void Update(float elapsedSec);
		void Render();

		bool SaveBackbufferToImage() const;

		void ClearBackground();

	private:
		bool threading{ false };
		const uint32_t m_AmountOfThreads{ 4 };

		SDL_Window* m_pWindow;
		SDL_Surface* m_pFrontBuffer;
		SDL_Surface* m_pBackBuffer;
		
		uint32_t* m_pBackBufferPixels;
		uint32_t m_Width;
		uint32_t m_Height;
		float m_AR;
		bool m_HardshadowsOn;

		static void SubRender(SDL_Surface* pBackBuffer, uint32_t* pBackBufferPixels, uint32_t width, uint32_t height, const FMatrix4& camMatrix, const FPoint3& camPos, float AR, uint32_t threadSize, uint32_t startIdx, bool hardShadows);
	};
}

#endif