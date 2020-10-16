#pragma once

using namespace Elite;

class Camera
{
public:
	Camera(const FPoint3& startPos, float FOV_angle = 45, float speed = 10.f, float rotSpeed = 5.f, float FOV_changeSpeed = 10.f); // FOV in degrees
	Camera(const FPoint3& pos, const FPoint3& target, float FOV_angle = 45.f, float speed = 10.f, float rotSpeed = 5.f, float FOV_changeSpeed = 10.f);
	Camera(const FPoint3& pos, const FVector3& forward, float FOV_angle = 45.f, float speed = 10.f, float rotSpeed = 5.f, float FOV_changeSpeed = 10.f);
	void SetPosition(const FPoint3& newPos);
	FPoint3 GetPosition() const;
	
	void SetFOV(float newAngle);
	float GetFOV() const;
	void IncFOV(float increment);
	void ProcessInput(float elapsedSec);

	FMatrix4 GetTransform() const;

	void ResetPosition(); // sets camera back to spawnposition (obn changed accordingly);

private:
	// Position Vector
	FPoint3 m_Pos; // In World space: position
	FVector3 m_Forward;
	FVector3 m_Up;
	FVector3 m_Right;

	float m_FOV_Angle;
	float m_FOV_changeSpeed;

	// Axes transformationmatrix
	Elite::FMatrix4 m_OBN;
	Elite::FMatrix4 m_Original_OBN;

	void MoveLocal(const FVector3& translate);
	void RotateLocal(float x, float y, float z);
	void RotateGlobalY(float y);

	float m_Speed;
	float m_RotationSpeed;

	// R * T = Pivot
	// first rotate, then translate --> Pivot
	// first translate, then rotate --> Something else
};  

