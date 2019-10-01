#include "Game.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;


Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;


	Mesh* leftMargin = Object2D::CreateRectangle("leftMargin", corner, resolution.y * 0.9, 50, glm::vec3(0.5f, 0.5f, 0.5f), true);
	AddMeshToList(leftMargin);

	Mesh* topMargin = Object2D::CreateRectangle("topMargin", corner, 50, resolution.x, glm::vec3(0.5f, 0.5f, 0.5f), true);
	AddMeshToList(topMargin);

	Mesh* rightMargin = Object2D::CreateRectangle("rightMargin", corner, resolution.y * 0.9, 50, glm::vec3(0.5f, 0.5f, 0.5f), true);
	AddMeshToList(rightMargin);

	board = Object2D::CreateRectangle("board", corner, 30, 200, glm::vec3(0.3f, 0.1f, 0.6f), true);
	AddMeshToList(board);

	ball = Object2D::CreateCircle("ball", corner, 12, glm::vec3(0.2f, 0.8f, 0));
	AddMeshToList(ball);

	brick = Object2D::CreateRectangle("brick", glm::vec3(0, 0, 0), 30, 50, glm::vec3(1, 0, 0), true);

	InitializeBricks();
}

void Game::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::Update(float deltaTimeSeconds)
{
	static float trans_x = 150, trans_y = 250, trans_addVal = 0.75f, trans_rot = 0.1f, trans_rotAdd = 0.1f;
	static float scale_x = 1, scale_y = 1, scale_addVal = 0.05f;
	static float rot_x = 0, rot_addVal = 0.1f;
	glm::ivec2 resolution = window->GetResolution();
	// TODO: update steps for translation, rotation, scale, in order to create animations
	modelMatrix = glm::mat3(1);
	//modelMatrix *= Transform2D::Scale(0.5f, 7);
	modelMatrix *= Transform2D::Translate(0, 80);
	//modelMatrix *= Transform2D::Translate(trans_x, trans_y);
	//modelMatrix *= Transform2D::Translate(50, 50);
	//modelMatrix *= Transform2D::Rotate(trans_rot);
	//modelMatrix *= Transform2D::Translate(-50, -50);

	//trans_x += trans_addVal;
	//trans_y += trans_addVal;
	//trans_rot += trans_rotAdd;

	//if (trans_x > 150 || trans_y > 250 || trans_x < 50 || trans_y < 50) {
	//	trans_addVal *= -1;
		//trans_rot *= -1;
	//	trans_rotAdd *= -1;
	//}

	// TODO: create animations by multiplying current transform matrix with matrices from Transform 2D
	//modelMatrix *= Transform2D::Scale(2, 2);
	//modelMatrix *= Transform2D::Rotate(glm::pi<float>());
	RenderMesh2D(meshes["leftMargin"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(50, 680);
	//modelMatrix *= Transform2D::Scale(12.4f, 0.5f);

	//scale_x += scale_addVal;
	//scale_y += scale_addVal;

	//if (scale_x <= 0 || scale_y <= 0 || scale_x >= 1.5f || scale_y >= 1.5f) {
	//	scale_addVal *= -1;
	//}
	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D

	RenderMesh2D(meshes["topMargin"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	//modelMatrix *= Transform2D::Scale(0.5f, 7);
	modelMatrix *= Transform2D::Translate(1230, 80);
	//modelMatrix *= Transform2D::Translate(50, 50);
	//modelMatrix *= Transform2D::Rotate(rot_x);
	//modelMatrix *= Transform2D::Translate(-50, -50);

	//rot_x += rot_addVal;

	//if (rot_x >= 2.0f * glm::pi<float>()) {
	//	rot_x = 0;
	//}

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	RenderMesh2D(meshes["rightMargin"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(boardX, 20);
	//modelMatrix *= Transform2D::Scale(2, 0.2f);

	RenderMesh2D(meshes["board"], shaders["VertexColor"], modelMatrix);

	DrawBricks();

	modelMatrix = glm::mat3(1);
	if (attachedBall) {
		ballCenterX = boardX + 100;
		ballCenterY = 65;
		ballDirY = 5;
		ballDirX = 0;
	}
	else {
		if (ballCenterY >= 670) {
			float cos = (ballCenterX - resolution.x / 2) / (resolution.x / 2);
			if (ballDirX > 0) {
				ballDirX = 5 * glm::abs(cos);
			}
			else {
				ballDirX = -5 * glm::abs(cos);
			}
			ballDirY = -5 * glm::abs(1 - cos * cos);
		}
		if (ballCenterX <= 65 && ballCenterY >= 80) {
			float cos = (ballCenterY - (resolution.y - 80) / 2) / ((resolution.y - 80) / 2);
			if (ballDirY > 0) {
				ballDirY = 5 * glm::abs(1 - cos * cos);
			}
			else {
				ballDirY = -5 * glm::abs(1 - cos * cos);
			}
			ballDirX = 5 * glm::abs(cos);
		}
		if (ballCenterX >= 1220 && ballCenterY >= 80) {
			float cos = (ballCenterY - (resolution.y - 80) / 2) / ((resolution.y - 80) / 2);
			if (ballDirY > 0) {
				ballDirY = 5 * glm::abs(1 - cos * cos);
			}
			else {
				ballDirY = -5 * glm::abs(1 - cos * cos);
			}
			ballDirX = -5 * glm::abs(cos);
		}
		if (ballCenterY <= 60 && ballCenterX >= (boardX - 11) && ballCenterX <= (boardX + 200 + 11)) {
			float contactPoint = ballCenterX - boardX + 11;
			float cos = (contactPoint - 100) / 100;
			if (ballDirX > 0) {
				ballDirX = -5 * glm::abs(cos);
			}
			else {
				ballDirX = 5 * glm::abs(cos);
			}
			ballDirY = 5 * glm::abs(1 - cos * cos);
		}
		if (ballCenterY <= 10 || ballCenterX <= -10 || ballCenterY >= 1240) {
			attachedBall = true;
		}
		ballCenterX = ballCenterX + ballDirX;
		ballCenterY = ballCenterY + ballDirY;
	}
	modelMatrix *= Transform2D::Translate(ballCenterX, ballCenterY);
	RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
}

void Game::InitializeBricks() {
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			visibleBricks[i][j] = true;
		}
	}
}

void Game::DrawBricks() {
	glm::ivec2 resolution = window->GetResolution();
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (visibleBricks[i][j]) {
				glm::mat3 modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(150 + 100 * i, 150 + 50 * j);
				RenderMesh2D(brick, shaders["VertexColor"], modelMatrix);
			}
		}
	}
}

void Game::FrameEnd()
{

}

void Game::OnInputUpdate(float deltaTime, int mods)
{

}

void Game::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Game::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	boardX = mouseX;
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	attachedBall = false;
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Game::OnWindowResize(int width, int height)
{
}
