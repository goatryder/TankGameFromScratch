#include "PhoenixBaseActor.h"

#include <Framework2D/Gameplay/Level.h>
#include <Game/Game.h>
#include <Framework2D/Gameplay/Actor/Components/EntityComponent.h>

namespace Game {

	PhoenixBase::PhoenixBase(const std::string& Name, const Vec2& Position)
		: Actor(Name, Position)
	{
		// Enable collision
		//EnableCollision(false);

		// Create actor components
		auto* TankUp = new EntityComponent<SpriteEntity>((Actor*)this, Name, Position,
			ResPath::T_TANK_UP_0);
		//auto TankLeft = new EntityComponent<SpriteFlipFlop>((Actor*)this, Name, Position, 0.3f, ResPath::T_TANK_LEFT_0, ResPath::T_TANK_LEFT_1);
		//TankLeft->SetRelativePosition({ 50, 0 });
		//auto BoomComp = new EntityComponent<SpriteSequence>((Actor*)this, Name, Position, 1.f,
		//	std::initializer_list<const char*>{ ResPath::T_BOOM_BIG_0, ResPath::T_BOOM_BIG_1, ResPath::T_BOOM_SMALL_0, ResPath::T_BOOM_SMALL_1, ResPath::T_BOOM_SMALL_2 });

		// Initialize actor size
		SetSize(TankUp->GetSize());
	}

	void PhoenixBase::OnTick(float DeltaTime)
	{

	}

	void PhoenixBase::OnCollide(BaseEntity* Other, CollisionFilter Filter)
	{

	}

}