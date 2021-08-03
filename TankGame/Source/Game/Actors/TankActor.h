#pragma once

#include <Framework2D/Gameplay/Actor/Actor.h>

#include <Game/Game.h>
#include <Framework2D/Gameplay/Actor/Components/EntityComponent.h>

namespace Game {

	using namespace Framework2D;

	class Bullet;

	/*
	 * This class is represent main AI/Player controlled actor - tank
	 *
	 */
	class Tank : public Framework2D::Actor
	{
	public:
		/*
		 * Constructor should be called only from Level::SpawnActorFromClass() method;
		 * Constructor can initialize Actor Components;
		 */
		Tank(const std::string& Name, const Vec2& Position);
		
		// frame per frame logic, no need to call Framework2D::Actor implementation
		virtual void OnTick(float DeltaTime) override;

		virtual void OnCollide(BaseEntity* Other, CollisionFilter Filter) override;

	protected:

		// direction
		EntityComponent<SpriteFlipFlop>* SpriteComp_Up;
		EntityComponent<SpriteFlipFlop>* SpriteComp_Down;
		EntityComponent<SpriteFlipFlop>* SpriteComp_Left;
		EntityComponent<SpriteFlipFlop>* SpriteComp_Right;

		EntityComponent<SpriteFlipFlop>* SpriteComp_Current;
		Direction CurrentDirection;

		inline EntityComponent<SpriteFlipFlop>* GetDirectionSpriteComp(Direction Dir);
		inline void ChangeDirection(Direction DirectionTo);

		bool bCanMove = false;
		float Speed;

		// movement
		inline void Move(float DeltaTime);

		// movement anim
		float MoveAnimRate;
		inline void EnableMoveAnim(bool bEnable, Direction Dir);

		bool bPossesedByPlayer = false;

	public:
		void MoveBegin(Direction DirectionTo);
		void MoveEnd(Direction DirectionTo);

		Bullet* ActiveBullet = nullptr;
		void Fire();

		Direction GetDirection() const { return CurrentDirection; }

	};
}
