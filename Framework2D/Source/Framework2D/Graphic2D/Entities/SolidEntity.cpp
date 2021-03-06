#include <PCH_Framework.h>
#include "SolidEntity.h"

namespace Framework2D {

	SolidEntity::SolidEntity(const std::string& Name, const Vec4& Color, const Vec2& Position, const Vec2& Size, bool bEnableRender)
		: BaseEntity(Name, Size, Position, bEnableRender), Color(Color)
	{
	}

	void SolidEntity::OnUpdate(float DeltaTime)
	{
	}

	void SolidEntity::OnCollide(BaseEntity* Other, CollisionFilter Filter)
	{
	}

	SolidEntity::~SolidEntity()
	{

	}

}
