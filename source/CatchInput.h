#pragma once

enum class Action
{
	ToggleCamera,
	ToggleScene,
	ResetCameras,
	MoveCamRight,
	MoveCamLeft,
	MoveCamUp,
	MoveCamDown,
	ToggleShadows,
	Null
};

namespace CatchInput
{
	Action GetAction();
	void PrintInputCommands();
}


