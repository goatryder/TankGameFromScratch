#pragma once

#include "Engine.h"
#include "Framework2D/Framework2DAPI.h"

#include "Framework2D/Structs/Vectors.h"
#include <Framework2D/Systems/SystemTimer.h>

namespace Framework2D {

	class LayerSystem;
	class Layer2D;

	class FRAMEWORK2D_API Game2D : public Engine::Application
	{
		LayerSystem* SystemLayer;
		Layer2D* HUDLayer;
		Layer2D* MainLayer;

		const VecInt2D GameBoundLeft;
		const VecInt2D GameBoundRight;

		friend Game2D* CreateGame(const char* Title, unsigned int Width, unsigned int Height,
			const VecInt2D& GameBoundLeft, const VecInt2D& GameBoundRight);
		Game2D(const char* Title, unsigned int Width, unsigned int Height, 
			const VecInt2D& GameBoundLeft, const VecInt2D& GameBoundRight);

	public:
		~Game2D();

		inline LayerSystem* GetSystemLayer() const { return SystemLayer; }
		inline Layer2D* GetHUDLayer() const { return HUDLayer; }
		inline Layer2D* GetMainLayer() const { return MainLayer; }

		inline const VecInt2D& GetGameBoundLeft() const { return GameBoundLeft; }
		inline const VecInt2D& GetGameBoundRight() const { return GameBoundRight; }

		static Game2D* Instance;

		// debug
		void MoveForward();
		void DelayedFunc();
		void DelayedFuncLoop();
		TimerHandle TimerHandle_1;
		TimerHandle TimerHandle_2;
		//
	};

	inline Game2D* GetGame() { return Game2D::Instance; }
	inline Game2D* CreateGame(const char* Title, unsigned int Width, unsigned int Height, 
		const VecInt2D& GameBoundLeft, const VecInt2D& GameBoundRight)
	{
		Game2D::Instance = new Game2D(Title, Width, Height, GameBoundLeft, GameBoundRight);
		return Game2D::Instance;
	}
}