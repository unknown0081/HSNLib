#pragma once
#include "Obstacle.h"

class ObsParts : public Obstacle
{
public:
	ObsParts(const char* filePath, bool pParts = false);
	~ObsParts() override;

	void Update() override;

private:
	enum class STATE
	{
		BREAK,		// 破壊されて生成された時
		WAIT,		// 通常時
		GO_PLAYER,	// プレイヤーに向かって移動
	};
	STATE state = STATE::BREAK;

	void TranslationBreak();
	void UpdateBreak();
	void TranslationWait();
	void UpdateWait();
	void TranslationGoPlayer();
	void UpdateGoPlayer();

	// 抵抗を与える
	void Friction();

	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	float frictionPower = 5.0f;

	float rotationSpeed = 0;	// 1秒に回転する角度

	int maxVelocity = 20.0f;
	int maxFrictionPower = 5.0f;
	int minFrictionPower = 3.0f;
	int maxRotationSpeed = 120.0f;
	int minRotationSpeed = 30.0f;

	float canGoPlayerTimer = 0.0f;
	float canGoPlayerTime = 1.0f;

	float toPlayerDistance = 100.0f;	// プレイヤーに向かうと判断する距離
	float goPlayerSpeed = 20.0f;

	bool playerParts = false;
};