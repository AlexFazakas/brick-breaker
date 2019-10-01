#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Game : public SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void InitializeBricks();
	void DrawBricks();

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	float translateX, translateY;
	float scaleX, scaleY;
	float angularStep;
	float boardX = 250;
	bool attachedBall = true;
	Mesh *board;
	Mesh *ball;
	Mesh *brick;
	float ballCenterX = 350;
	float ballCenterY = 65;
	float ballDirX = 0;
	float ballDirY = 5;
	int lives = 3;
	bool visibleBricks[5][10];
};