#include "AudioManager.h"
#include "../ErrorLogger.h"

// 初期化処理
void AudioManager::Initialize()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), hrTrace(hr));

	// フラグ生成
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#if _DEBUG
	eflags |= DirectX::AudioEngine_Debug;
#endif

	// audioEngine 生成
	audioEngine = std::make_unique<DirectX::AudioEngine>(eflags);


	LoadMusic(static_cast<int>(MUSIC_LABEL::WEAPON), L"Data/Audio/Impact.wav");
}

