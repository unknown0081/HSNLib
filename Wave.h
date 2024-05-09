#pragma once
#include <memory>
#include <random>
#include "SpinningTopEnemyManager.h"
#include "SpinningTopPlayerManager.h"
#include "StEnemy.h"
#include "StEnemyData.h"
#include "LightManager.h"
#include "ObsMarunoko.h"
#include "ObstacleManager.h"
#include "Library/2D/Sprite.h"

struct ScriptData
{
	virtual ~ScriptData() = default;
	virtual void Execute() = 0;			// 生成時に実行する関数
};

struct WaveScript
{
	float time;
	std::unique_ptr<ScriptData> data;
};

// 敵生成
struct ScriptEnemy : public ScriptData
{
	ScriptEnemy(int sType, int eType) : spawnType(sType), enemyType(eType) {};
	int spawnType;
	int enemyType;

	void Execute() override
	{
		StEnemy* enemy = new StEnemy(enemyType);
		float xPos;
		float zPos;

		bool noObstacleHit = false;

		while (true)
		{
			noObstacleHit = false;

			// 乱数生成器の設定
			std::random_device rd;  // 真の乱数生成器を初期化
			std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器を初期化
			std::uniform_real_distribution<float> dis(0.0f, enemySpawnErea[spawnType].radius); // 0からenemySpawnErea[i].radiusの間の一様乱数生成器を作成

			xPos = enemySpawnErea[spawnType].position.x;
			zPos = enemySpawnErea[spawnType].position.z;

			float rad = DirectX::XMConvertToRadians(rand() % 360);
			float dist = dis(gen);

			xPos += cosf(rad) * dist;
			zPos += sinf(rad) * dist;

			DirectX::XMFLOAT3 pos = { xPos, 0, zPos };

			// obsと衝突していないか確認
			int obsCount = ObstacleManager::Instance().GetObstacleCount();
			for (int i = 0; i < obsCount; i++)
			{
				Obstacle* obs = ObstacleManager::Instance().GetObstacle(i);

				if (!obs->isCollision) continue;

				// 衝突処理
				DirectX::XMFLOAT3 velocityA;
				if (Collision::StaticRepulsionSphereVsSphere(
					pos,
					enemy->GetRadius(),
					obs->GetPosition(),
					obs->GetRadius(),
					velocityA,
					120
				))
				{
					noObstacleHit = true;
					break;
				}
			}

			// 衝突していなければ抜ける
			if (!noObstacleHit) break;
		}
		
		enemy->SetPosition({ xPos, 100, zPos });
		enemy->generatePos = { xPos, 0.1f, zPos };
		enemy->spawnEffect->SetPosition(enemy->generatePos);

		// スポーン座標設定
		enemy->spawnPosition = { enemy->GetPosition().x, 0, enemy->GetPosition().z };
		SpinningTopEnemyManager::Instance().Register(enemy);
	}
};

struct ScriptMarunoko : public ScriptData
{
	ScriptMarunoko(int mType, DirectX::XMFLOAT3 pos, float speed) : type(mType), position(pos), moveSpeed(speed) {};
	int type;
	DirectX::XMFLOAT3 position;
	float moveSpeed;

	void Execute() override
	{
		ObsMarunoko* obstacle = new ObsMarunoko("Data/FBX/StMarunoko/StMarunoko.fbx", type, moveSpeed);
		obstacle->spawnPos = position;
		obstacle->SetPosition({ position.x, 100, position.z });
		obstacle->spawnEffect->SetPosition({ position.x, 0.1f, position.z });

		if (type != 0)
		{
			obstacle->isSpawnFinish = true;
			obstacle->position = position;
		}

		ObstacleManager::Instance().Register(obstacle);
	}
};

// ライトON
struct ScriptOnLight : public ScriptData
{
	ScriptOnLight() {}

	void Execute() override
	{
		LightManager::Instance().Clear();

		// ライト初期化
		Light* directionLight = new Light(LightType::Directional);
		directionLight->SetDirection(DirectX::XMFLOAT3(0.5, -1, -1));
		directionLight->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instance().Register(directionLight);

		LightManager::Instance().SetAmbientColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	}
};

// ライトOFF
struct ScriptOffLight : public ScriptData
{
	ScriptOffLight() {}

	void Execute() override
	{
		LightManager::Instance().Clear();

		// ライト初期化
		Light* directionLight = new Light(LightType::Directional);
		directionLight->SetDirection(DirectX::XMFLOAT3(0.5, -1, -1));
		directionLight->SetColor(DirectX::XMFLOAT4(0, 0, 0, 1));
		LightManager::Instance().Register(directionLight);

		// 点光源追加
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					Light* light = new Light(LightType::Point);
					light->SetPosition({ i * 50.0f - 25.0f, 7, j * 50.0f - 25.0f });
					light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
					light->SetRange(30.0f);
					LightManager::Instance().Register(light);
				}
			}
		}
		LightManager::Instance().SetAmbientColor({ 0,0,0,1.0f });
	}
};

struct SciptDestoryEnemy : public ScriptData
{
	SciptDestoryEnemy() {}
	void Execute() override
	{
		SpinningTopEnemyManager::Instance().Clear();
	}
};

struct ScriptDestoryObstacle : public ScriptData
{
	ScriptDestoryObstacle() {}
	void Execute() override
	{
		ObstacleManager::Instance().WaveClear();
	}
};

#define SET_ENEMY(time, spawnType, enemyType) {(time), std::unique_ptr<ScriptData>(new ScriptEnemy(spawnType, enemyType))}
#define SET_MARUNOKO(time, marunokoType, posX, posY, posZ, speed) {(time), std::unique_ptr<ScriptData>(new ScriptMarunoko(marunokoType, {posX, posY, posZ}, speed))}
#define SET_OnLight(time) {(time), std::unique_ptr<ScriptData>(new ScriptOnLight())}
#define SET_OffLight(time) {(time), std::unique_ptr<ScriptData>(new ScriptOffLight())}
#define SET_EnemyDestory(time) {(time), std::unique_ptr<ScriptData>(new SciptDestoryEnemy())}
#define SET_ObstacleDestory(time) {(time), std::unique_ptr<ScriptData>(new ScriptDestoryObstacle())}
#define SET_END		{0,nullptr}

class Wave
{
private:
	Wave() {}
	~Wave() {}

public:
	static Wave& Instance()
	{
		static Wave instance;
		return instance;
	}

	// 初期化処理
	void Init();

	// 更新処理
	void Update();

	// 描画処理
	void Render();

public:
	int state = 0;
	float timer = 0;

	WaveScript* pScript = nullptr;
	WaveScript** ppScript = nullptr;

	int nowWave = 0;				// 現在のウェーブ
	float waveLimitTime = 50.0f;	// ウェーブの時間50秒
	float waveLimitTimer = 0.0f;	// ウェーブの残り時間タイマー

	// ウェーブ表示用
	enum WaveShowState
	{
		showWaveInit,
		goCenter,
		waitCenter,
		goOutSide,
		showWaveEnd,
	};
	int showWaveState;

	float showWaveTextEaseTime = 0.3f;	// イージングに使用する時間
	float showWaveTextWaitTime = 1.0f;	// 中央でゆっくり移動する時間
	float showWaveTextTimer = 0.0f;		// ウェーブ表示の全体時間の管理用タイマー

	std::unique_ptr<Sprite> sprWaveLine;
	std::unique_ptr<Sprite> sprWaveCut1;
	std::unique_ptr<Sprite> sprWaveCut2;
	std::unique_ptr<Sprite> sprWaveCut3;
	std::unique_ptr<Sprite> sprWaveCut4;

	DirectX::XMFLOAT2 startPos = { -256, 296 };
	DirectX::XMFLOAT2 centerStartPos = { 480, 296 };
	DirectX::XMFLOAT2 centerEndPos = { 544, 296 };
	DirectX::XMFLOAT2 endPos = { 1280, 296 };
	DirectX::XMFLOAT2 cutCurrentPos = startPos;
};