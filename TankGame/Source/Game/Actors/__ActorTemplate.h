#pragma once

#include <Framework2D/Gameplay/Actor/Actor.h>

#include <Game/Game.h>
#include <Framework2D/Gameplay/Actor/Components/EntityComponent.h>

namespace Game {

	using namespace Framework2D;

	/*
	 * This class is represent 
	 * 
	 */
	class __ActorTemplate : public Framework2D::Actor
	{
	protected:


	public:
		/* 
		 * Constructor should be called only from Level::SpawnActorFromClass() method;
		 * Constructor can initialize Actor Components;
		 */ 
		__ActorTemplate(const std::string& Name, const Vec2& Position);

		// frame per frame logic, no need to call Framework2D::Actor implementation
		virtual void OnTick(float DeltaTime) override;

		// will be called when actor overlaps with other entity with enabled collision
		virtual void OnCollide(BaseEntity* Other, CollisionFilter Filter) override;

		// will be called when Destroy() called
		virtual void OnDestroy() override;
	};
}
