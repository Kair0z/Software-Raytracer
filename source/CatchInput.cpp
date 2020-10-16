#include "pch.h"
#include "CatchInput.h"
#include "SDL.h"
#include <iostream>

Action CatchInput::GetAction() 
{
	const Uint8* pKeyState = SDL_GetKeyboardState(nullptr);

	if (pKeyState[SDL_SCANCODE_O]) return Action::ToggleShadows;
	if (pKeyState[SDL_SCANCODE_T]) return Action::ToggleCamera;
	if (pKeyState[SDL_SCANCODE_N]) return Action::ToggleScene;
	if (pKeyState[SDL_SCANCODE_R]) return Action::ResetCameras;
	if (pKeyState[SDL_SCANCODE_RIGHT]) return Action::MoveCamRight;
	if (pKeyState[SDL_SCANCODE_UP]) return Action::MoveCamUp;
	if (pKeyState[SDL_SCANCODE_LEFT]) return Action::MoveCamLeft;
	if (pKeyState[SDL_SCANCODE_DOWN]) return Action::MoveCamDown;

	return Action::Null;
}

void CatchInput::PrintInputCommands()
{
	std::cout << "Currently available Input-keys: \n";
	std::cout << "------------------------------- \n";
	std::cout << "- O: Toggle shadows \n";
	std::cout << "- T: Toggle Camera \n";
	std::cout << "- N: Change Scene \n";
	std::cout << "- R: Reset all camera positions \n";
	std::cout << "- LEFT-RIGHT: Move camera left & right \n";
	std::cout << "- UP-DOWN: Move Camera up & down \n";
	std::cout << "------------------------------- \n";

	std::cout << "Camera-functionality: \n";
	std::cout << "--------------------- \n";
	std::cout << "- LMB-Drag(vert): Move forward | Move backward \n";
	std::cout << "- LMB-Drag(Hor) : Turn left | Turn right \n";
	std::cout << "- RMB-Drag(vert): Turn up | Turn down \n";
	std::cout << "--------------------- \n";
}