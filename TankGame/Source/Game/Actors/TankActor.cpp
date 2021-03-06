#include "TankActor.h"

#include <Framework2D/Systems/SystemTimer.h>
#include <Framework2D/Gameplay/Level.h>

#include <Game/Game.h>
#include <Game/Gameplay/TankiGameMode.h>
#include <Game/Gameplay/TankiAIController.h>
#include <Game/Gameplay/TankiPlayerController.h>

#include <Game/Actors/BoomActor.h>
#include <Game/Actors/BulletActor.h>
#include <Game/Actors/Pickups/PickupRespawnPointActor.h>
#include <Game/Actors/Boosters/BoosterTempInvincibilityActor.h>

namespace Game {

	using namespace GameConst;
	using namespace ResPath;


	TankSpawnPoint TankSpawnPoint::BottomLeftSpawnPoint(Vec2(GameConst::GAME_AREA_W0, GameConst::GAME_AREA_H1), Direction::UP, Anchor::BOTTOM_LEFT);
	TankSpawnPoint TankSpawnPoint::BottomRightSpawnPoint(Vec2(GameConst::GAME_AREA_W1, GameConst::GAME_AREA_H1), Direction::UP, Anchor::BOTTOM_RIGHT);
	TankSpawnPoint TankSpawnPoint::TopLeftSpawnPoint(Vec2(GameConst::GAME_AREA_W0, GameConst::GAME_AREA_H0), Direction::DOWN, Anchor::TOP_LEFT);
	TankSpawnPoint TankSpawnPoint::TopRightSpawnPoint(Vec2(GameConst::GAME_AREA_W1, GameConst::GAME_AREA_H0), Direction::DOWN, Anchor::TOP_RIGHT);
	TankSpawnPoint TankSpawnPoint::TopCenterSpawnPoint(Vec2(GameConst::GAME_AREA_W1 / 2, GameConst::GAME_AREA_H0), Direction::DOWN, Anchor::TOP);


	TankSkin TankSkin::PlayerBasicTankSkin{ T_TANK_UP_0, T_TANK_UP_1, T_TANK_DOWN_0, T_TANK_DOWN_1, T_TANK_RIGHT_0, T_TANK_RIGHT_1, T_TANK_LEFT_0, T_TANK_LEFT_1 };
	TankSkin TankSkin::EnemyBasicTankSkin{ T_TANK_EB_UP_0, T_TANK_EB_UP_1, T_TANK_EB_DOWN_0, T_TANK_EB_DOWN_1, T_TANK_EB_RIGHT_0, T_TANK_EB_RIGHT_1, T_TANK_EB_LEFT_0, T_TANK_EB_LEFT_1 };


	Tank::Tank(const std::string& Name, const Vec2& Position, 
		Direction StartDirection, float Speed, float MoveAnimRate, float Health, const TankSkin& Skin)
		: Actor(Name, Position), CurrentDirection(StartDirection), Speed(Speed), MoveAnimRate(MoveAnimRate)
	{
		// Enable collision
		EnableCollision(CollisionType::CT_Dynamic);

		// Create actor components
		SpriteComp_Up = new EntityComponent<SpriteFlipFlop>((Actor*)this, "FFSpriteUp" + Name, Position, MoveAnimRate, Skin.Up_0, Skin.Up_1);
		SpriteComp_Up->GetSprite()->SetAutoFlipFlopEnable(false);
		SpriteComp_Up->GetSprite()->SetEnableRender(false);

		SpriteComp_Down = new EntityComponent<SpriteFlipFlop>((Actor*)this, "FFSpriteDown" + Name, Position, MoveAnimRate, Skin.Down_0, Skin.Down_1);
		SpriteComp_Down->GetSprite()->SetAutoFlipFlopEnable(false);
		SpriteComp_Down->GetSprite()->SetEnableRender(false);
		
		SpriteComp_Left = new EntityComponent<SpriteFlipFlop>((Actor*)this, "FFSpriteLeft" + Name, Position, MoveAnimRate, Skin.Left_0, Skin.Left_1);
		SpriteComp_Left->GetSprite()->SetAutoFlipFlopEnable(false);
		SpriteComp_Left->GetSprite()->SetEnableRender(false);
		
		SpriteComp_Right = new EntityComponent<SpriteFlipFlop>((Actor*)this, "FFSpriteRight" + Name, Position, MoveAnimRate, Skin.Right_0, Skin.Right_1);
		SpriteComp_Right->GetSprite()->SetAutoFlipFlopEnable(false);
		SpriteComp_Right->GetSprite()->SetEnableRender(false);

		HealthComp = new HealthComponent((Actor*)this);
		HealthComp->SetHealth(Health, true);
		HealthComp->SetOnDeathCb(HEALTH_ON_DEATH_CB(Tank::OnDeath));

		// Initialize actor size
		SetSize(SpriteComp_Up->GetSize());

		// Initialize tank direction
		SpriteComp_Current = SpriteComp_Up;
		ChangeDirection(StartDirection);
	}

	void Tank::OnTick(float DeltaTime)
	{
		Move(DeltaTime);
	}

	void Tank::OnDestroy()
	{
		if (Booster_RespawnProteciton) Booster_RespawnProteciton->Destroy();
		SystemTimer::RemoveTimer(TimerHandle_HitFX);
		SystemTimer::RemoveTimer(TimerHandle_FlashyFX);

		if (IsPossesedByPlayerController() && GetLevel())  // remove from player controller
		{
			if (auto GM = dynamic_cast<TankiGameMode*>(GetLevel()->GetGameMode()))
			{
				if (auto PC = GM->GetCustomPlayerTankController(); PC->GetControlledTank() == this)
					PC->OnTankDestroyed();
			}
		}
		else if (IsPossesedByAIController() && GetLevel())  // remove from ai controller
		{
			if (auto GM = dynamic_cast<TankiGameMode*>(GetLevel()->GetGameMode()))
			{
				GM->OnEnemyTankKilled();

				if (auto AICon = GM->GetCustomAIController())
					AICon->RemoveTank(this);
			}
		}
	}

	void Tank::DropPickable()
	{
		PickupRespawnPoint::SpawnPickupRespawnPoint(GetLevel(), GetCenterPosition());
	}

	void Tank::TankHitFX_Activate()
	{
		Boom::SpawnBoomBig(GetLevel(), Position);

		SpriteComp_Down->GetSprite()->SetColor(Vec4::RedColor);
		SpriteComp_Up->GetSprite()->SetColor(Vec4::RedColor);
		SpriteComp_Left->GetSprite()->SetColor(Vec4::RedColor);
		SpriteComp_Right->GetSprite()->SetColor(Vec4::RedColor);

		SystemTimer::SetTimer(TimerHandle_HitFX, TIMER_CALLBACK(Tank::TankHitFX_End), 0.5f);
	}

	void Tank::TankHitFX_End()
	{
		Destroy();
	}

	void Tank::TankFlashyFX_Activate()
	{
		if (SpriteComp_Flashy == nullptr) // create component on fly
		{
			SpriteComp_Flashy = new EntityComponent<SpriteFlipFlop>((Actor*)this, "FlashyFX" + Name, GetPosition(true), FLASHY_ANIM_SPEED, T_FLASHY_0, T_FLASHY_1);
		}

		SpriteComp_Flashy->GetEntity()->SetEnableRender(true);

		SystemTimer::SetTimer(TimerHandle_FlashyFX, TIMER_CALLBACK(Tank::TankFlashyFX_End), FLASHY_ANIM_TIME);
	}

	void Tank::TankFlashyFX_End()
	{
		ENGINE_ASSERT(SpriteComp_Flashy, "TankFlashyFX_End() Tank::SpriteComp_Flashy is nullptr!");
		SpriteComp_Flashy->GetEntity()->SetEnableRender(false);
	}

	void Tank::RespawnProtection_Activate()
	{
		if (Booster_RespawnProteciton == nullptr)
		{
			const std::string BoosterName = "BoosterInvincibility_" + GetName();
			Booster_RespawnProteciton = BoosterTempInvincibility::SpawnBoosterTempInvincibility(GetLevel(), BoosterName);
		}
		
		Booster_RespawnProteciton->ActivateBoost(this, true);
		TankFlashyFX_Activate();
	}

	void Tank::OnDeath()
	{
		bCanMove = false;
			
		if (bDropPickableOnDeath) DropPickable();

		TankHitFX_Activate();
	}

	inline EntityComponent<SpriteFlipFlop>* Tank::GetDirectionSpriteComp(Direction Dir)
	{
		switch (Dir)
		{
		case Direction::RIGHT: 
			return SpriteComp_Right;
		
		case Direction::DOWN:
			return SpriteComp_Down;
		
		case Direction::LEFT:
			return SpriteComp_Left;
		
		default:
			return SpriteComp_Up;
		}
	}

	inline void Tank::ChangeDirection(Direction DirectionTo)
	{
		CurrentDirection = DirectionTo;

		SpriteComp_Current->GetEntity()->SetEnableRender(false);
		SpriteComp_Current = GetDirectionSpriteComp(DirectionTo);
		SpriteComp_Current->GetEntity()->SetEnableRender(true);
	}

	inline void Tank::EnableMoveAnim(bool bEnable, Direction Dir)
	{
		auto SpriteComp = GetDirectionSpriteComp(Dir);
		SpriteComp->GetSprite()->SetAutoFlipFlopEnable(bEnable);
	}

	inline void Tank::Move(float DeltaTime)
	{
		if (!bCanMove) return;

		Vec2 DeltaPos = DirectionToVec2(CurrentDirection) * (Speed * DeltaTime);
		Vec2 NewPos = GetPosition() + DeltaPos;
		SetPosition(NewPos, true);
	}

	void Tank::MoveBegin(Direction DirectionTo)
	{
		if (HealthComp->IsDead()) return;

		ChangeDirection(DirectionTo);
		EnableMoveAnim(true, DirectionTo);
		
		bCanMove = true;
	}

	void Tank::MoveEnd(Direction DirectionTo)
	{
		EnableMoveAnim(false, DirectionTo);

		if (DirectionTo == CurrentDirection) // execute only if last key pressed is last key released
			bCanMove = false;
	}

	void Tank::Fire()
	{
		if (IsPossesedByPlayerController() && ActiveBullet) return;  // player can only shoot if prev bullet is destroyed

		ActiveBullet = Bullet::SpawnBasicBullet(this);
	}

	Tank* Tank::SpawnBasicTank(Level* Level, const TankSpawnPoint& Point, TankType Type)
	{
		ENGINE_ASSERT(Level, "SpawnBasicPlayerTank Level is nullptr!");
		if (!Level) return nullptr;

		TankSkin* Skin;
		std::string Name;
		switch (Type)
		{
		case TankType::PlayerTank:
			Skin = &TankSkin::PlayerBasicTankSkin;
			Name = "PlayerTank";
			break;

		case TankType::EnemyTank:
		default:
			Skin = &TankSkin::EnemyBasicTankSkin;
			Name = "EnemyTank";
			break;
		}

		auto SpawnedTank = Level->SpawnActorFromClass<Tank>(Name, Point.SpawnPosition, Point.SpawnAnchor, 
			Point.SpawnDirection, TANK_BASIC_SPEED, TANK_BASIC_ANIM_SPEED, TANK_BASIC_HEALTH, *Skin);
		
		SpawnedTank->Type = Type;
		
		return SpawnedTank;
	}
}