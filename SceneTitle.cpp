#include <tchar.h>
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Library/Input/InputManager.h"
#include "Library/ImGui/Include/imgui.h"
#include "Library/Text/DispString.h"
#include "Library/Text/Text.h"
#include "Library/Timer.h"

void SceneTitle::Initialize()
{
	sprite = new Sprite(L"Data/Texture/Title.png");
	uvSprite = std::make_unique<UVScrollSprite>(L"Data/Texture/Title.png");
	maskSprite = std::make_unique<MaskSprite>(L"Data/Texture/Title.png", L"Data/Texture/dissolve.png");
	maskSprite->maskConstant.edgeColor = { 1,0,0,1 };
	maskSprite->maskConstant.edgeThreshold = 0.1f;
}

void SceneTitle::Finalize()
{
	delete sprite;
}

void SceneTitle::Update()
{
	InputManager& input = InputManager::Instance();

	if (input.GetKeyPressed(Keyboard::Enter))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}

	
	uvScrollValue.x += 0.1 * Timer::Instance().DeltaTime();
	uvScrollValue.y += 0.1 * Timer::Instance().DeltaTime();
}

void SceneTitle::Render()
{
	sprite->Render(800, 450, 1280, 720, 1, 1, 1, 1, 0);
	uvSprite->Render(uvScrollValue, 0, 0, 640, 360, 1, 1, 1, 1, 0);

	maskSprite->maskConstant.dissolveThreshold -= Timer::Instance().DeltaTime();
	if (maskSprite->maskConstant.dissolveThreshold < 0.0f) maskSprite->maskConstant.dissolveThreshold = 1.0f;
	maskSprite->Render(800, 0, 640, 360, 1, 1, 1, 1, 0);
	
	DispString::Instance().Draw(L"HO感SHＨIN LiうB", { 800, 60 }, 32, TEXT_ALIGN::MIDDLE, { 0, 0, 0, 1 });

	DispString::Instance().Draw(L"HO感SHＨIN LiうB", { 800, 600 }, 32, TEXT_ALIGN::MIDDLE, { 1, 1, 1, 1 }, true);

	DispString::Instance().Draw(L"あいうえお", { 800, 450 }, 32, TEXT_ALIGN::LOWER_RIGHT,{ 1, 0, 0, 1 });

	Text::Instance().drawText(FONTNO::meirio, FONTSIZE::SIZE22, "テスト", { 100,200 });




	// --- デバッグ描画 ---
	DrawDebugGUI();
}

// デバッグ描画
void SceneTitle::DrawDebugGUI()
{
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("Title"))
				{
					SceneManager::Instance().ChangeScene(new SceneTitle);
				}
				if (ImGui::MenuItem("Game"))
				{
					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
				}
				if (ImGui::MenuItem("Animation"))
				{
					SceneManager::Instance().ChangeScene(new SceneAnimation);
				}
				if (ImGui::MenuItem("ContextBase"))
				{
					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneContextBase));
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
