#include "BrickSolidActor.h"


namespace Game {

	BrickSolid::BrickSolid(const std::string& Name, const Vec2& Position)
		: Actor(Name, Position)
	{
		// Enable collision
		EnableCollision();

		// Create actor components
		BrickSpriteComp = new EntityComponent<SpriteEntity>((Actor*)this, Name, Position, ResPath::T_BRICK_BASE);

		// Initialize actor size
		SetSize(BrickSpriteComp->GetSize());
	}

	void BrickSolid::OnTick(float DeltaTime)
	{

	}

	void BrickSolid::OnCollide(BaseEntity* Other, CollisionFilter Filter)
	{
		if (Filter == CollisionFilter::CF_BLOCK)
		{
		}
	}

}