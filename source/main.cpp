#include "pch.h"
//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"
#include "EMath.h"
#include "PSphere.h"
#include "PTriangle.h"
#include "SceneGraph.h"
#include "PPlane.h"
#include "Quaternion.h"
#include "LDirection.h"
#include "LPoint.h"
#include "Camera.h"
#include "M_Lambert.h"
#include "M_LambertCookTorrance.h"
#include "M_LambertPhong.h"
#include "Albedo.h"
#include "PMesh.h"
#include <iostream>
#include "Scene.h"
#include "CatchInput.h"

using namespace Elite;

void InitEmptyScene()
{
	Scene* emptyScene{ new Scene() };

	emptyScene->AddCamera(new Camera{ FPoint3{-6.f, 3.f, 7.f}, FVector3{-0.5f, 0.f, 1.f}, 45.f, 4.f, 2.f }, true); // Left-angled
	emptyScene->AddCamera(new Camera{ FPoint3{6.f, 3.f, 7.f}, FVector3{0.5f, 0.f, 1.f}, 45.f, 4.f, 2.f }, false); // Right-angled
	emptyScene->AddCamera(new Camera{ FPoint3{0.f, 3.f, 7.f}, FVector3{0.f, 0.f, 1.f}, 45.f, 4.f, 2.f }, false); // Front-cam
	emptyScene->AddCamera(new Camera{ FPoint3{0.f, 5.f, -12.f}, FVector3{0.f, 0.3f, -1.f}, 45.f, 4.f, 2.f }, false); // Behind-cam

	RGBColor wallColor{ 0.47450f, 0.37647f, 0.298039f };
	M_Lambert* pLambert_Plane0{ new M_Lambert{wallColor, 0.5f} };
	M_Lambert* pLambert_Plane1{ new M_Lambert{wallColor, 0.5f} };
	M_Lambert* pLambert_Plane2{ new M_Lambert{wallColor, 0.5f} };
	M_Lambert* pLambert_Plane3{ new M_Lambert{wallColor, 0.5f} };
	M_Lambert* pLambert_Plane4{ new M_Lambert{wallColor, 0.5f} };

	M_LambertPhong* pLamberPhong_Plane0{ new M_LambertPhong{wallColor, 0.8f, 0.2f, 30} };
	M_LambertPhong* pLamberPhong_Plane1{ new M_LambertPhong{wallColor, 0.8f, 0.2f, 30} };
	M_LambertPhong* pLamberPhong_Plane2{ new M_LambertPhong{wallColor, 0.8f, 0.2f, 30} };
	M_LambertPhong* pLamberPhong_Plane3{ new M_LambertPhong{wallColor, 0.8f, 0.2f, 30} };

	emptyScene->AddElement(new PPlane{ FPoint3{0.f, -1.f, 0.f }, FVector3{0.f, 1.f, 0.f}, pLambert_Plane0, true }); // floor -1
	emptyScene->AddElement(new PPlane{ FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.f}, pLambert_Plane4, true }); // roof 10
	emptyScene->AddElement(new PPlane{ FPoint3{0.f, 0.f, -15.f }, FVector3{0.f, 0.f, 1.f}, pLambert_Plane1, true }); // back -15
	emptyScene->AddElement(new PPlane{ FPoint3{-6.f, 0.f, 0.f }, FVector3{1.f, 0.f, 0.f}, pLambert_Plane2, true }); // Left-wall [-6
	emptyScene->AddElement(new PPlane{ FPoint3{6.f, 0.f, 0.f }, FVector3{-1.f, 0.f, 0.f}, pLambert_Plane3, true }); // right wall 6]

	RGBColor lightColor1{ 0.964705f, 0.80392f, 0.545098f };
	RGBColor lightColor2{ 1.f, 1.f, 1.f };

	float intensity{ 20.f };
	emptyScene->AddElement(new LDirection{ FVector3{-1.f, -0.25f, -1.f}, RGBColor{1.f, 1.f, 1.f}, 2.f, false });
	emptyScene->AddElement(new LPoint{ FPoint3{3.f, 3.f, -12.f}, lightColor2, intensity });
	emptyScene->AddElement(new LPoint{ FPoint3{3.f, 2.7f, 0.f}, lightColor2, intensity });
	emptyScene->AddElement(new LPoint{ FPoint3{-3.f, 2.7f, 0.f}, lightColor2, intensity });
	emptyScene->AddElement(new LPoint{ FPoint3{-3.f, 3.f, -12.f}, lightColor2, intensity });

	SceneGraph::Instance().AddScene(emptyScene);
}

void AddBallScene()
{
	InitEmptyScene();

	int sceneIdx{ SceneGraph::Instance().GetAmountOfScenes() - 1 };

	float specular{ 0.8f };
	float diffuse{ 0.2f };
	float phongExp{ 60.f };
	M_LambertPhong* pLamberPhong_Triangle0{ new M_LambertPhong{RGBColor{Albedo::Silver}, specular, diffuse, phongExp} };
	M_Lambert* pLambert_Triangle1{ new M_Lambert{RGBColor{1.f, 1.f, 1.f}, 1.f} };
	M_Lambert* pLambert_Triangle2{ new M_Lambert{RGBColor{1.f, 1.f, 1.f}, 1.f} };

	SceneGraph::Instance()[sceneIdx]->AddElement(new PTriangle{ FPoint3{-2.5f, 6.5f, -5.f}, std::vector<FPoint3>{FPoint3{1.f, 0.f, 0.f}, FPoint3{0.f, 1.f, 0.f}, FPoint3{0.f, 0.f, 0.f}}, pLamberPhong_Triangle0, true, Cullmode::NoCull });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PTriangle{ FPoint3{0.f, 6.5f, -5.f}, std::vector<FPoint3>{FPoint3{1.f, 0.f, 0.f}, FPoint3{0.f, 1.f, 0.f}, FPoint3{0.f, 0.f, 0.f}}, pLambert_Triangle1, true, Cullmode::Backface });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PTriangle{ FPoint3{2.5f, 6.5f, -5.f}, std::vector<FPoint3>{FPoint3{1.f, 0.f, 0.f}, FPoint3{0.f, 1.f, 0.f}, FPoint3{0.f, 0.f, 0.f}}, pLambert_Triangle2, true, Cullmode::Frontface });

	RGBColor albedo{ Albedo::Brass };
	M_LambertCookTorrance* pMat_Sphere0 = new M_LambertCookTorrance{ albedo, 1.f, true };
	M_LambertCookTorrance* pMat_Sphere1 = new M_LambertCookTorrance{ albedo, 0.6f, true };
	M_LambertCookTorrance* pMat_Sphere2 = new M_LambertCookTorrance{ albedo, 0.1f, true };
	M_LambertCookTorrance* pMat_Sphere3 = new M_LambertCookTorrance{ albedo, 1.f, false };
	M_LambertCookTorrance* pMat_Sphere4 = new M_LambertCookTorrance{ albedo, 0.6f, false };
	M_LambertCookTorrance* pMat_Sphere5 = new M_LambertCookTorrance{ albedo, 0.1f, false };

	SceneGraph::Instance()[sceneIdx]->AddElement(new PSphere{ FPoint3{-2.5f, 1.f, -5.f}, 1.f, pMat_Sphere0, true });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PSphere{ FPoint3{0.f, 1.f, -5.f}, 1.f, pMat_Sphere1, true });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PSphere{ FPoint3{2.5f, 1.f, -5.f}, 1.f, pMat_Sphere2, true });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PSphere{ FPoint3{-2.5f, 4.f, -5.f}, 1.f, pMat_Sphere3, true });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PSphere{ FPoint3{0.f, 4.f, -5.f}, 1.f, pMat_Sphere4, true });
	SceneGraph::Instance()[sceneIdx]->AddElement(new PSphere{ FPoint3{2.5f, 4.f, -5.f}, 1.f, pMat_Sphere5, true });
}

void AddBunnyScene()
{
	InitEmptyScene();
	int sceneIdx{ SceneGraph::Instance().GetAmountOfScenes() - 1 };

	M_Lambert* pLambert_Mesh{ new M_Lambert{Albedo::DeepRuby, 1.f} };
	M_Lambert* pLambert_Mesh1{ new M_Lambert{Albedo::Jade, 1.f} };
	M_Lambert* pLambert_Mesh2{ new M_Lambert{Albedo::Egyptian, 1.f} };
	M_Lambert* pLambert_Mesh3{ new M_Lambert{Albedo::Gold, 1.f} };
	M_Lambert* pLambert_Mesh4{ new M_Lambert{Albedo::VantaBlack, 1.f} };

	M_LambertPhong* pLambertPhong_Mesh{ new M_LambertPhong{RGBColor{0.f, 1.f, 1.f}, 0.8f, 0.2f, 30.f} };
	M_LambertCookTorrance* pLambertCookTorrance_Mesh{ new M_LambertCookTorrance{Albedo::Gold, 0.2f, false} };

	SceneGraph::Instance()[sceneIdx]->AddElement(new PMesh{ "lowpoly_bunny", pLambert_Mesh1, FPoint3{3.f, -1.f, -4.f}, true, Cullmode::NoCull, 0.5f });
	//SceneGraph::Instance()[sceneIdx]->AddElement(new PMesh{ "lowpoly_bunny", pLambert_Mesh2, FPoint3{-3.f, -1.f, -4.f}, true, Cullmode::NoCull, 0.7f });
}

void AddCustomScene()
{
	Scene* pScene = new Scene{};

	pScene->AddCamera(new Camera{ FPoint3{0.f, 0.f, 40.f}, FVector3{0.f, 0.f, 1.f}, 90.f, 10.f, 10.f }, true);

	M_LambertCookTorrance* pMat = new M_LambertCookTorrance{ Albedo::Silver, 0.5f, false };
	pScene->AddElement(new PSphere{ FPoint3{}, 10.f, pMat, true });
	pScene->AddElement(new PPlane{ FPoint3{0.f, 0.f, -20.f}, FVector3{0.f, 0.f, 1.f}, new M_Lambert{RGBColor{1.f, 0.f, 0.f}, 1.f}, true });

	pScene->AddElement(new LDirection{ FVector3{-1.f, -1.f, -1.f}, RGBColor{1.f, 1.f, 1.f}, 3.14, true });
	pScene->AddElement(new LPoint{ FPoint3{5.f, 0.f, 15.f}, RGBColor{1.f, 1.f, 1.f}, 40.f, true });
	pScene->AddElement(new LPoint{ FPoint3{5.f, 0.f, -15.f}, RGBColor{1.f, 1.f, 1.f}, 40.f, true });

	SceneGraph::Instance().AddScene(pScene);
}

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	
	SDL_Window* pWindow = SDL_CreateWindow(
		"Raytracer - Arne Van Kerschaver",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);

	AddCustomScene();
	AddBallScene();
	AddBunnyScene();

	CatchInput::PrintInputCommands();

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;

	while (isLooping)
	{
		float elapsedSec = pTimer->GetElapsed();

		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if(e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				break;
			}
		}

		//--------- Render ---------
		if (SceneGraph::Instance().GetCurrentScene()->HasCamera())
		{
			pRenderer->Update(elapsedSec);
			SceneGraph::Instance().Update(elapsedSec);

			pRenderer->ProcessInput(elapsedSec);
			SceneGraph::Instance().ProcessInput(elapsedSec);
		}

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}