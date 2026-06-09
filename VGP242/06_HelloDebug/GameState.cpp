#include "GameState.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;
using namespace GoatedEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

enum class Shape
{
	None,
	AABB,
	AABBFilled,
	Sphere,
	GroundPlane,
	GroundCircle,
	Transform
};

const char* gShapenames[] =
{
	"None",
	"AABB",
	"AABBFilled",
	"Sphere",
	"GroundPlane",
	"GroundCircle",
	"Transform"
};

Shape gCurrentShape = Shape::None;
Color gShapeColor = Colors::White;
float gPlaneSize = 10.0f;


void GameState::Render()
{
	switch (gCurrentShape)
	{
	case Shape::None: break;
	case Shape::GroundPlane:
	{
		SimpleDraw::AddGroundPlane(gPlaneSize, gShapeColor);
	}
	break;
	default:
		break;
	}
	SimpleDraw::Render(mCamera);
}

float myVariable = 0.0f;
void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
		ImGui::ColorEdit4("ShapeColor", &gShapeColor.r);
		int currentShape = (int)gCurrentShape;
		if(ImGui::Combo("Shape", &currentShape, gShapenames, std::size(gShapenames)))
		{
			gCurrentShape = (Shape)currentShape;
		}

		ImGui::DragFloat("PlaneSize", &gPlaneSize, 1.0f, 1.0f, 10000.0f);
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}
