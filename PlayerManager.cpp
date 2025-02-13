#include "PlayerManager.h"

// XV
void PlayerManager::Update()
{
	// XV
	for (Player* player : players)
	{
		player->Update();
	}

	// jü
	for (Player* player : removes)
	{
		std::vector<Player*>::iterator it = std::find(players.begin(), players.end(), player);

		if (it != players.end())
		{
			players.erase(it);
		}
		// eÛÌjü
		delete player;
	}
	// jüXgðNA
	removes.clear();
}

//@`æ
void PlayerManager::Render()
{
	for (Player* player : players)
	{
		player->Render();
	}
}

// vC[o^
void PlayerManager::Register(Player* player)
{
	players.emplace_back(player);
}


// vC[í
void PlayerManager::Remove(Player* player)
{
	// jüXgÉÇÁ
	removes.insert(player);
}

// vC[Sí
void PlayerManager::Clear()
{
	for (Player* player : players)
	{
		delete player;
	}
	players.clear();
}

// fobOpGUI`æ
void PlayerManager::DrawDebugGui()
{
	for (Player* player : players)
	{
		player->DrawDebugGui();
	}
}
