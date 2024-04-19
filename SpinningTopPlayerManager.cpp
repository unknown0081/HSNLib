#include "SpinningTopPlayerManager.h"

void SpinningTopPlayerManager::Update() {
	// XV
	for (StPlayer* player : players) {
		player->Update();
	}

	// jü
	for (StPlayer* player : removes) {
		std::vector<StPlayer*>::iterator it = std::find(players.begin(), players.end(), player);

		if (it != players.end()) {
			players.erase(it);
		}
		// eÛÌjü
		delete player;
	}
	// jüXgðNA
	removes.clear();
}

//@`æ
void SpinningTopPlayerManager::Render() {
	for (StPlayer* player : players) {
		player->Render();
	}
}

// vC[o^
void SpinningTopPlayerManager::Register(StPlayer* player) {
	players.emplace_back(player);
}


// vC[í
void SpinningTopPlayerManager::Remove(StPlayer* player) {
	// jüXgÉÇÁ
	removes.insert(player);
}

// vC[Sí
void SpinningTopPlayerManager::Clear() {
	for (StPlayer* player : players) {
		delete player;
	}
	players.clear();
}

// fobOpGUI`æ
void SpinningTopPlayerManager::DrawDebugGui() {
	for (StPlayer* player : players) {
		player->DrawDebugGui();
	}
}
