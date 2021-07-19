#include <PCH_Framework.h>
#include "SolidEntity.h"

#include <Framework2D/Graphic2D/Render/Renderer.h>

namespace Framework2D {

	SolidEntity::SolidEntity(const std::string& Name, const Vec4& Color, const VecInt2D& Position, const VecInt2D& Size, bool bEnableRender)
		: BaseEntity(Name, Size, Position, bEnableRender), Color(Color)
	{

	}

	void SolidEntity::OnUpdate(float DeltaTime)
	{
	}

	void SolidEntity::OnEvent(Engine::Event& e)
	{
	}

	SolidEntity::~SolidEntity()
	{

	}

	inline VertexBatchColorQuad SolidEntity::GetVertexQuad()
	{
		VertexBatchColor v0 = { Position, Color };
		VertexBatchColor v1 = { {Position.X + Size.X, Position.Y}, Color };
		VertexBatchColor v2 = { {Position.X + Size.X, Position.Y + Size.Y}, Color };
		VertexBatchColor v3 = { {Position.X,          Position.Y + Size.Y}, Color };

		return { v0, v1, v2, v3 };
	}

}