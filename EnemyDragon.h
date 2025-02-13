#pragma once

#include "Enemy.h"

class EnemyDragon : public Enemy
{
public:
	EnemyDragon();
	~EnemyDragon() override;

	// 更新処理
	void Update();

	// 描画処理
	void Render();

protected:
	// 死亡処理
	void OnDead() override;

private:
	float height = 1.0f;
};