#include <PCH_Framework.h>
#include "GameMode.h"

#include <Framework2D/Gameplay/Level.h>
#include <Framework2D/Gameplay/Controllers/PlayerController.h>
#include <Framework2D/Gameplay/Controllers/AIController.h>
#include <Framework2D/Gameplay/UI/HUD.h>

namespace Framework2D {

	GameMode::GameMode(Level* InLevel, PlayerController* InPlayerController, HUD* InHUD, AIController* InAIController)
		: m_Level(InLevel), m_PlayerController(InPlayerController), m_HUD(InHUD), m_AIController(InAIController)
	{

		m_Level->GM_Owner = this;
		m_PlayerController->GM_Owner = this;
		m_HUD->GM_Owner = this;
		m_AIController->GM_Owner = this;
	}

	GameMode::~GameMode() 
	{
	}

	inline void GameMode::Update(float DeltaTime)
	{
		m_Level->Update(DeltaTime);
		m_HUD->Update(DeltaTime);

		m_PlayerController->OnTick(DeltaTime);
		m_AIController->OnTick(DeltaTime);

		OnTick(DeltaTime);
	}

	void GameMode::Start()
	{
		GAME_LOG(info, "_____GameMode::Start_____");
	}

	void GameMode::Restart()
	{
		GAME_LOG(info, "_____GameMode::Restart_____");
	}

	void GameMode::End()
	{
		GAME_LOG(info, "_____GameMode::End_____");
	}
}