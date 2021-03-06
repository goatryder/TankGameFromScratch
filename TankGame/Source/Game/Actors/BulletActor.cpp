#include "BulletActor.h"

#include <Game/Game.h>
#include <Framework2D/Gameplay/Level.h>
#include <Game/Actors/TankActor.h>
#include <Game/Actors/BoomActor.h>

namespace Game {

	Bullet::Bullet(const std::string& Name, const Vec2& Position, 
		Tank* Instigator, Direction SpawnDirection, float Speed, float Damage)
		: Actor(Name, Position), 
		Instigator(Instigator), Speed(Speed), Damage(Damage), 
		BulletDirection(SpawnDirection), DirectionVec(DirectionToVec2(SpawnDirection))
	{
		// Enable collision
		EnableCollision(CollisionType::CT_Projectile);
		
		AddToCollisionFilter(Instigator);

		const char* BulletTexPath;
		switch (SpawnDirection)
		{
		case Direction::LEFT:
			BulletTexPath = ResPath::T_BULLET_LEFT;
			break;
		case Direction::RIGHT:
			BulletTexPath = ResPath::T_BULLET_RIGHT;
			break;
		case Direction::DOWN:
			BulletTexPath = ResPath::T_BULLET_DOWN;
			break;
		default:
			BulletTexPath = ResPath::T_BULLET_UP;
		}

		// Create actor components
		BulletSpriteComp = new EntityComponent<SpriteEntity>((Actor*)this, "Sprite_" + Name, Position, BulletTexPath);

		// Initialize actor size
		SetSize(BulletSpriteComp->GetSize() * 2);
	}

	void Bullet::OnTick(float DeltaTime)
	{
		Vec2 NewPosition = GetPosition() + DirectionVec * (Speed * DeltaTime);
		SetPosition(NewPosition, true);
	}

	void Bullet::OnCollide(BaseEntity* Other, CollisionFilter Filter)
	{
		if (Other == Instigator) return; // tank can't shoot itself

		if (auto A = dynamic_cast<Actor*>(Other))
		{
			if constexpr (GameConst::BLUE_ON_BLUE) // Handle tanks blue on blue:
			{
				// FriendlyFire allowed. Do not check if we can damage other actor
				if (auto HealthComp = A->GetComponentByClass<HealthComponent>())
					HealthComp->OnDamage(Damage, BulletDirection, this);

			}
			else
			{
				// FriendlyFire Disallowed. Extra check to see if we can damage other actor

				// tank vs tank case:
				if (auto T = dynamic_cast<Tank*>(Other); T && Instigator && T->GetTankType() == Instigator->GetTankType())
				{
					// can't damage teammates
				}
				else
				{
					// no damage restriction, do some dmg
					if (auto HealthComp = A->GetComponentByClass<HealthComponent>())
						HealthComp->OnDamage(Damage, BulletDirection, this);
				}
			}
			
		}

		if (Instigator) Instigator->ActiveBullet = nullptr;
		
		// bullet hit fx
		Boom::SpawnBoomSmall(GetLevel(), Position);

		Destroy();
	}

	Bullet* Bullet::SpawnBasicBullet(Tank* TankFrom)
	{
		std::string Name = TankFrom->GetName() + "_Bullet";
		Direction BulletDirection = TankFrom->GetDirection();
		//Vec2 SpawnPos = TankFrom->GetSidePosition(TankFrom->GetDirection()) + DirectionToVec2(BulletDirection) * 20.f;
		Vec2 SpawnPos = TankFrom->GetSidePosition(TankFrom->GetDirection());
		
		constexpr float Speed = GameConst::BULLET_BASIC_SPEED;
		constexpr float Damage = GameConst::BULLET_BASIC_DAMAGE;

		Bullet* SpawnedBullet = TankFrom->GetLevel()->SpawnActorFromClass<Bullet>(Name, SpawnPos, Anchor::CENTER,
			TankFrom, BulletDirection, Speed, Damage);

		return SpawnedBullet;
	}
}