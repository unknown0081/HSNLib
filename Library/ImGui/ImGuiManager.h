#pragma once
#include <wtypes.h>
#include "./Include/imgui.h"
#include "./Include/imgui_impl_win32.h"
#include "./Include/imgui_impl_dx11.h"

#include <vector>
#include <string>

//--------------------------------------------------------------
//  Graphics
//--------------------------------------------------------------
class ImGuiManager
{
private:
	ImGuiManager() {}
	~ImGuiManager() {}

public:
	static ImGuiManager& Instance()
	{
		static ImGuiManager instance;
		return instance;
	}

	// 初期化
	void Initialize(HWND hwnd);
	// 終了化
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Render();

	// DockSpace処理
	void DockSpace();
	// Console処理
	void Console();
};