#pragma once
#include "SpinningTopBase.h"
#include "Library/3D/SkinnedMesh.h"
#include "Collision.h"

#include "BehaviorTreeIF.h"
#include "BehaviorTree.h"

// エネミー
class SpinningTopEnemy : public SpinningTopBase, public IBTree
{
public:
	enum class KIND
	{
		NONE = 0,
		DEBUG_STOP,
		ROOT,
		Normal,
		Down,
		IDLE,
		CollisionAvoidance,
		Generate,
		PlayerPursuit,
		PlayerPositionGet,
		WaitChargeAttack,
		ChargeAttack,
		SeekPlayer,
		WanderSpawnArea,
		Death,
	};

public:
	SpinningTopEnemy() {};
	~SpinningTopEnemy() override {}

	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Render(bool drawShadow = false) = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive() {};

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	// 破棄
	void Destroy();

	// ----- SteeringBehavior -----
	DirectX::XMFLOAT3 SbSeek();
	DirectX::XMFLOAT3 SbArrival();
	DirectX::XMFLOAT3 SbWander();
	DirectX::XMFLOAT3 SbCollisionAvoidance();

public:
	// enemies での index
	int id;

	int enemyKind;

	bool isGenerateFinish = false;	// 生成が終わってるかどうか
	bool isPursuit = false;			// プレイヤーに突っ込んでいる最中かどうか

	bool createParts = false;	// 破壊時にパーツを生成するかどうか

	// プレイヤーとの距離
	float playerDistance;
	DirectX::XMFLOAT3 plPosition = { 0,0,0 };	// TODO: デバッグ用だから削除する

	float downRotationSpeed = 360.0f;
	float normalRoatationSpeed = 720.0f;
	float rotationSpeed = 720;	// 1秒に回転する角度

	float pursuitRadius = 8.0f;		// プレイヤーに突っ込み始める範囲
	float searchRadius = 12.0f;		// プレイヤー追従開始の範囲
	float notSearchRadius = 15.0f;	// プレイヤーの追従をあきらめる範囲


	// ----- SteeringBehavior -----
	DirectX::XMFLOAT3 steeringForce = { 0,0,0 };
	DirectX::XMFLOAT3 targetPosition = { 3,0,0 };
	DirectX::XMFLOAT3 spawnPosition = { 0,0,0 };

	float steeringMaxForce = 1.0f;
	float slowingArea = 1.0f;
	float circleDistance = 3.0f;
	float circleRadius = 2.5f;
	float wanderAngle = 0;
	int wanderAngleChange = 30;
	float wanderAngleChangeTimer = 0.0f;
	float wanderAngleChangeTime = 0.1f;

	float maxSeeAhead = 6.0f;
	float maxAvoidForce = 60.0f;

	float chargeAttackCoolTimer = 0.0f;
	float chargeAttackCoolTime = 1.0f;

	float waitChargeAttackTimer = 0.0f;
	float waitChargeAttackTime = 0.3f;

	float chargeAttackTimer = 0.0f;
	float chargeAttackTime = 2.0f;

	float downFrictionPower = 5.0f;
	bool isDown = false;		// ダウン状態かどうか
	float downTime = 5.0f;		// ダウン状態の時間
	float downTimer = 0.0f;		// ダウン状態要タイマー
	float downAngle = 30.0f;

	// ----- behaviorTree -----
	int behaviorType;	// ビヘイビアツリーのタイプ
	std::unique_ptr<BTree> aiTree;
	bool GetBTreeJudge(const int kind) override;
	IBTree::STATE ActBTree(const int kind) override;
};