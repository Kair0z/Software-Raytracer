#include "pch.h"
//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"
#include "Ray.h"
#include <iostream>
#include <iomanip>
#include "SceneGraph.h"
#include "EMath.h"
#include "Camera.h"
#include "CatchInput.h"
#include <future>

Elite::Renderer::Renderer(SDL_Window * pWindow)
	:m_pWindow{ pWindow }
	, m_pFrontBuffer{ SDL_GetWindowSurface(pWindow) }
	, m_pBackBuffer{}
	, m_pBackBufferPixels{}
	, m_Width{}
	, m_Height{}
	, m_HardshadowsOn{false}
{
	//Initialize
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
	m_AR = float(m_Width) / float(m_Height);
}

void Elite::Renderer::Update(float elapsedSec)
{
	Render();
}

void Elite::Renderer::ProcessInput(float elapsedSec)
{
	switch (CatchInput::GetAction())
	{
	case Action::ToggleShadows:
		m_HardshadowsOn = !m_HardshadowsOn;
		break;
	}
}

void Elite::Renderer::SubRender(SDL_Surface* pBackBuffer, uint32_t* pBackBufferPixels, uint32_t width, uint32_t height, const FMatrix4& camMat, const FPoint3& camPos, float AR, uint32_t threadSize, uint32_t startIdx, bool hardShadows)
{
	float FOV = SceneGraph::Instance().GetCurrentScene()->GetActiveCam()->GetFOV();

	uint32_t r = startIdx / width;
	uint32_t c = startIdx % width;

	HitInfo hitInfo;
	RGBColor finalColor;

	for (r; r < height; ++r)
	{
		float samplePointY{ 1 - 2 * ((r + 0.5f) / height) };
		samplePointY *= FOV;

		for (c; c < width; ++c)
		{
			float samplePointX{ 2 * ((c + 0.5f) / width) - 1 };
			samplePointX *= AR * FOV;

			FPoint4 samplePoint_Transformed{ camMat * FPoint4 { samplePointX, samplePointY, -1.f, 1.f } };
			FVector3 fromView{ samplePoint_Transformed.x - camPos.x, samplePoint_Transformed.y - camPos.y, samplePoint_Transformed.z - camPos.z };

			Normalize(fromView);

			Ray ray{ camPos, fromView, 0.001f };
			// 0.f == tMin

			if (SceneGraph::Instance().GetCurrentScene()->Hit(ray, hitInfo) != nullptr) // Hit returns closestObj* or nullptr if nothing's been hit
			{
				finalColor = SceneGraph::Instance().GetCurrentScene()->Shade(hitInfo, -fromView, hardShadows);
				finalColor.MaxToOne();

				pBackBufferPixels[c + (r * width)] = SDL_MapRGB(pBackBuffer->format,
					static_cast<uint8_t>(255.f * (finalColor.r)),
					static_cast<uint8_t>(255.f * (finalColor.g)),
					static_cast<uint8_t>(255.f * (finalColor.b)));
			}
		}
	}
}

void Elite::Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);
	FMatrix4 camMatrix = SceneGraph::Instance().GetCurrentScene()->GetActiveCam()->GetTransform();
	FPoint3 camPos = SceneGraph::Instance().GetCurrentScene()->GetActiveCam()->GetPosition();

	ClearBackground();

	if (threading)
	{
		static std::vector<std::future<void>> threadStates{ m_AmountOfThreads };

		uint32_t allPixels = m_Width * m_Height;

		for (int i{}; i < m_AmountOfThreads; ++i)
		{
			uint32_t size = static_cast<float>(allPixels) / static_cast<float>(m_AmountOfThreads);
			uint32_t startIdx = i * size;

			threadStates.push_back(std::async(SubRender, m_pBackBuffer, m_pBackBufferPixels, m_Width, m_Height, camMatrix, camPos, m_AR, size, startIdx, m_HardshadowsOn));
		}


		for (auto& thread : threadStates)
		{
			thread.wait();
		}
	}
	else
	{
		HitInfo hitInfo;

		float FOV{ SceneGraph::Instance().GetCurrentScene()->GetActiveCam()->GetFOV() };

		//Loop over all the pixels
		for (uint32_t r = 0; r < m_Height; ++r)
		{
			// Calc samplepoint in every pixel (y) 
			float samplePointY{ 1 - 2 * ((r + 0.5f) / m_Height) };
			samplePointY *= FOV;

			for (uint32_t c = 0; c < m_Width; ++c)
			{
				RGBColor finalColor;

				// Calc samplepoint in every pixel (x)
				float samplePointX{ 2 * ((c + 0.5f) / m_Width) - 1 };
				samplePointX *= m_AR * FOV;

				// ray Origin = camPos, dir = camPos --> samplepoint
				// sends out a ray with direction away from the user (Right-handed-frame)
				FPoint3 startPoint{ SceneGraph::Instance().GetCurrentScene()->GetActiveCam()->GetPosition() };

				FPoint4 samplePoint_Transformed{ SceneGraph::Instance().GetCurrentScene()->GetActiveCam()->GetTransform() * FPoint4 { samplePointX, samplePointY, -1.f, 1.f } };
				FVector3 fromView{ samplePoint_Transformed.x - startPoint.x, samplePoint_Transformed.y - startPoint.y, samplePoint_Transformed.z - startPoint.z };
				Normalize(fromView);

				Ray ray{ startPoint, fromView, 0.001f };
				// 0.f == tMin

				if (SceneGraph::Instance().GetCurrentScene()->Hit(ray, hitInfo) != nullptr) // Hit returns closestObj* or nullptr if nothing's been hit
				{
					finalColor = SceneGraph::Instance().GetCurrentScene()->Shade(hitInfo, -fromView, m_HardshadowsOn);
					finalColor.MaxToOne();

					m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
						static_cast<uint8_t>(255.f * (finalColor.r)),
						static_cast<uint8_t>(255.f * (finalColor.g)),
						static_cast<uint8_t>(255.f * (finalColor.b)));
				}

			}
		}

	}
	
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

void Elite::Renderer::ClearBackground()
{
	for (uint32_t r = 0; r < m_Height; ++r)
	{
		for (uint32_t c = 0; c < m_Width; ++c)
		{
			m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format, 0, 0, 0);
		}
	}
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}
