#include "pch.h"
#include "Camera.h"
#include "SDL.h"
#include <iostream>
#include "CatchInput.h"

Camera::Camera(const FPoint3& startPos, float FOV_angle, float speed, float rotSpeed, float FOV_changeSpeed)
	: m_FOV_Angle{Elite::ToRadians(FOV_angle)}
	, m_Pos{startPos}
	, m_Speed{speed}
	, m_FOV_changeSpeed{Elite::ToRadians(FOV_changeSpeed)}
	, m_RotationSpeed{rotSpeed}
{
	m_OBN = Elite::FMatrix4{
		1, 0, 0, startPos.x, // Right
		0, 1, 0, startPos.y, // Up
		0, 0, 1, startPos.z, // Forward
		0, 0, 0, 1 // Translation
	};
}

Camera::Camera(const FPoint3& pos, const FPoint3& target, float FOV_angle, float speed, float rotSpeed, float FOV_changeSpeed)
	: m_FOV_Angle{Elite::ToRadians(FOV_angle)}
	, m_Pos{pos}
	, m_Speed{speed}
	, m_FOV_changeSpeed{Elite::ToRadians(FOV_changeSpeed)}
	, m_RotationSpeed{rotSpeed}
{
	// Forward towards target
	m_Forward = target - pos;

	// Right = Forward x GlobalUp
	m_Right = Elite::Cross(FVector3(0, 1, 0), m_Forward);

	// Up = m_Right x m_Forward
	m_Up = Elite::Cross(m_Forward, m_Right);

	Elite::Normalize(m_Right);
	Elite::Normalize(m_Up);

	m_OBN = Elite::FMatrix4{ m_Right, m_Up, m_Forward, FVector4{m_Pos.x, m_Pos.y, m_Pos.z , 1} };
	m_Original_OBN = m_OBN;
	
}

Camera::Camera(const FPoint3& pos, const FVector3& forward, float FOV_angle, float speed, float rotSpeed, float FOV_changeSpeed)
	: m_FOV_Angle{Elite::ToRadians(FOV_angle)}
	, m_Pos{pos}
	, m_Speed{speed}
	, m_FOV_changeSpeed{Elite::ToRadians(FOV_changeSpeed)}
	, m_RotationSpeed{rotSpeed}
	, m_Forward{forward}
{
	Elite::Normalize(m_Forward);

	m_Right = Elite::Cross(FVector3(0.f, 1.f, 0.f), m_Forward);
	m_Up = Elite::Cross(m_Forward, m_Right);

	Elite::Normalize(m_Right);
	Elite::Normalize(m_Up);

	m_OBN = Elite::FMatrix4{ m_Right, m_Up, m_Forward, FVector3{m_Pos} };
	m_Original_OBN = m_OBN;

}

void Camera::ProcessInput(float elapsedSec)
{
	//// Movement
	switch (CatchInput::GetAction())
	{
	case Action::MoveCamDown:
		MoveLocal(FVector3{ 0.f, -1.f, 0.f } * elapsedSec * m_Speed);
		break;

	case Action::MoveCamLeft:
		MoveLocal(FVector3{ -1.f, 0.f, 0.f } * elapsedSec * m_Speed);
		break;

	case Action::MoveCamUp:
		MoveLocal(FVector3{ 0.f, 1.f, 0.f } * elapsedSec * m_Speed);
		break;

	case Action::MoveCamRight:
		MoveLocal(FVector3{ 1.f, 0.f, 0.f } * elapsedSec * m_Speed);
		break;
	}

	// Rotation
	int mouseX{};
	int mouseY{};
	const Uint32 mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

	// Both mouse buttons pressed
	if ((SDL_BUTTON(SDL_BUTTON_RIGHT) & mouseState) && (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState))
	{
		MoveLocal(FVector3{ float(mouseX) * elapsedSec * m_Speed, float(-mouseY) * elapsedSec * m_Speed ,0.f });
		return;
	}

	// Only left mouse button pressed
	if (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState)
	{
		// If rotating
		if (abs(mouseX) > abs(mouseY))
		{
			//RotateLocal(0, -mouseX * m_RotationSpeed * elapsedSec, 0);
			RotateGlobalY(-mouseX * m_RotationSpeed * elapsedSec);
			return;
		}

		else
		{
			MoveLocal(FVector3{ 0.f, 0.f, float(mouseY) } *elapsedSec * m_Speed);
		}
	}

	if (SDL_BUTTON(SDL_BUTTON_RIGHT) & mouseState)
	{
		RotateLocal(mouseY * m_RotationSpeed * elapsedSec, 0, 0);
	}
}

#pragma region FOV_Changers
float Camera::GetFOV() const
{
	return tanf(m_FOV_Angle / 2);
}

void Camera::SetFOV(float newAngle)
{
	m_FOV_Angle = Elite::ToRadians(newAngle);
}

void Camera::IncFOV(float increment)
{
	SetFOV(m_FOV_Angle + (increment / 10000000.f));
}

#pragma endregion 

Elite::FMatrix4 Camera::GetTransform() const
{
	return m_OBN;
}
void Camera::MoveLocal(const FVector3& translate)
{
	m_OBN *= Elite::MakeTranslation(FVector3{ FVector4{ translate } });
}

void Camera::RotateLocal(float x, float y, float z)
{
	m_OBN *= FMatrix4{ Elite::MakeRotationZYX(x, y, z) };
}

void Camera::RotateGlobalY(float y)
{
	FMatrix4 rot = Elite::MakeRotationY(y);
	m_OBN =  rot * m_OBN;
}

void Camera::SetPosition(const FPoint3& point)
{
	// ultimate set
	m_Pos = point;
}

FPoint3 Camera::GetPosition() const
{
	return FPoint3{ m_OBN[3].xyz };
}

void Camera::ResetPosition()
{
	m_OBN = m_Original_OBN;
}