#include "Game.h"

#include <Framework2D.h>

#include <Framework2D/Graphic2D/Layer2D.h>
#include <Framework2D/Graphic2D/Render/ResourceLoader.h>
#include <Framework2D/Graphic2D/Render/Renderer.h>

#include <Framework2D/Graphic2D/Entities/SolidEntity.h>
#include <Framework2D/Graphic2D/Groups/GroupSolid.h>

#include <Framework2D/Graphic2D/Entities/SpriteEntity.h>
#include <Framework2D/Graphic2D/Groups/GroupSprite.h>

namespace Game {

	class GameInitializer : public Framework2D::Initializer
	{
	public:
		void LoadTextures()
		{
			using namespace Framework2D;
			using namespace ResPath;

			ResourceLoader::LoadTexture({ T_TANK_DOWN_0
				, T_TANK_DOWN_1
				, T_TANK_UP_0
				, T_TANK_UP_1
				, T_TANK_LEFT_0
				, T_TANK_LEFT_1
				, T_TANK_RIGHT_0
				, T_TANK_RIGHT_1

				, T_BRICK_0
				, T_BRICK_1
				, T_BRICK_BASE

				, T_BULLET_UP
				, T_BULLET_DOWN
				, T_BULLET_RIGHT
				, T_BULLET_LEFT

				, T_TANK_EB_DOWN_0
				, T_TANK_EB_DOWN_1

				, T_TANK_EB_UP_0
				, T_TANK_EB_UP_1
				, T_TANK_EB_LEFT_0
				, T_TANK_EB_LEFT_1
				, T_TANK_EB_RIGHT_0
				, T_TANK_EB_RIGHT_1

				, T_PHOENIX_PNG

				, T_BOOM_SMALL_0
				, T_BOOM_SMALL_1
				, T_BOOM_SMALL_2
				, T_BOOM_BIG_0
				, T_BOOM_BIG_1

				, T_FLASHY_0
				, T_FLASHY_1

				, T_BOOST_HP

				, T_WON_UI
				, T_LOOSE_UI
				, T_TANK_ICO_UI

				, T_UI_NUM_0
				, T_UI_NUM_1
				, T_UI_NUM_2
				, T_UI_NUM_3
				, T_UI_NUM_4
				, T_UI_NUM_5
				, T_UI_NUM_6
				, T_UI_NUM_7
				, T_UI_NUM_8
				, T_UI_NUM_9
				
				});
		}

		void RegisterShaders()
		{
			using namespace Framework2D;
			using namespace ResPath;

			// Register Shaders
			ResourceLoader::RegisterShader(ShaderType::QuadBatchColor, S_BATCH_COLOR);
			ResourceLoader::RegisterShader(ShaderType::QuadBatchTexture, S_BATCH_TEXTURE);

			// Preset Shader uniforms.
			auto shader = ResourceLoader::GetShader(ShaderType::QuadBatchColor);
			shader->Bind();
			shader->SetUniformMat4f("u_ViewProjModel", RendererStatics::ProjViewModel);
			shader->Unbind();

			shader = ResourceLoader::GetShader(ShaderType::QuadBatchTexture);
			shader->Bind();
			shader->SetUniformMat4f("u_ViewProjModel", RendererStatics::ProjViewModel);

			std::vector<int> TexSlotArr;
			int TexSlotNum = Texture::GetMaxTextureBind();
			TexSlotArr.reserve(TexSlotNum);
			for (int i = 0; i < TexSlotNum; ++i) TexSlotArr.push_back(i);
			shader->SetUniform1iv("u_Textures", TexSlotNum, TexSlotArr.data());

			shader->Unbind();
		}

		void Init() override
		{
			using namespace Framework2D;
			using namespace ResPath;

			Initializer::Init();
			GAME_LOG(LOG_WARN, "Game::GameInitializer Init()");

			RegisterShaders();
			LoadTextures();
			
			Game2D* game = GetGame();
			Layer2D* MainLayer = game->GetMainLayer();

			GroupSolid* GroupTest = new GroupSolid("TestGroupSolid");
			MainLayer->AddGroup(GroupTest);

			SolidEntity* SolidTest_0 = new SolidEntity("TestSolid", Vec4::YellowColor, VecInt2D::VecZero, VecInt2D(50), true);
			SolidEntity* SolidTest_1 = new SolidEntity("TestSolid2", Vec4::CyanColor, VecInt2D::VecZero, VecInt2D(20), true);
			SolidEntity* SolidTest_2 = new SolidEntity("TestSolid3", Vec4::MagentaColor, VecInt2D(200, 100), VecInt2D(70), true);

			GroupTest->AddSolid(SolidTest_0);
			GroupTest->AddSolid(SolidTest_1);
			GroupTest->AddSolid(SolidTest_2);

			GroupSprite* GroupTest_2 = new GroupSprite("TestGroupSprite");
			MainLayer->AddGroup(GroupTest_2);

			SpriteEntity* SpriteTest_0 = new SpriteEntity("TestSprite", T_FLASHY_0, VecInt2D(0, 0));
			SpriteEntity* SpriteTest_1 = new SpriteEntity("TestSprite2", T_TANK_EB_DOWN_0, VecInt2D(540, 0));

			GroupTest_2->AddSprite(SpriteTest_0);
			GroupTest_2->AddSprite(SpriteTest_1);
		}
	};

}

Framework2D::Initializer* Framework2D::CreateInitializer()
{
	return new Game::GameInitializer();
}

Engine::Application* Engine::CreateApplication()
{
	auto App = Framework2D::CreateGame("Tanki From Shet", 640, 480);

	auto Initializer = Framework2D::CreateInitializer();
	Initializer->Init();
	delete Initializer;

	return App;
}
