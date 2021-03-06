// Code in this file use source from and inspired by TheCherno/Hazel engine project
// https://github.com/TheCherno/Hazel/


#pragma once

#include "Core.h"

#include <Engine/LayerStack.h>

#include <memory>
#include <chrono>

namespace Engine {

	class ENGINE_API Application
	{
		bool bIsRunning = true;

		std::unique_ptr<class Window> CustomWindow;
		
		bool OnWindowClose(class WindowCloseEvent& e);

		bool bIsEnterKeyPressed = false;
		bool bIsAltKeyPressed = false;

		bool OnFullscreenKeyPressed(class KeyPressedEvent& e);
		bool OnFullscreenKeyReleassed(class KeyReleasedEvent& e);

	protected:
		LayerStack Layers;

	public:
		Application(const char* Title = "Engine App", unsigned int WindowWidth = 640, unsigned int WindowHeight = 480);
		virtual ~Application();

		/** Main loop */
		void Run();

		/** Dispatch to event such as window close, fullscreen hotkey press */
		void DistpatchEvents(Event& e);

		inline void PushLayer(Layer* layer) { Layers.PushLayer(layer); }
		inline void PushOverlay(Layer* layer) { Layers.PushOverlay(layer); }

		inline float GetWidth() const;
		inline float GetHeight() const;

	// tick seconds
	private:
		std::chrono::steady_clock::time_point T_Start;
		std::chrono::steady_clock::time_point T_Prev;
		std::chrono::steady_clock::time_point T_Next;
		float DeltaTime;

	public:
		inline float GetDeltaTime() { return DeltaTime; }
		float GetElapsedTime() { return std::chrono::duration<float, std::ratio<1>>(T_Next - T_Start).count(); }
	//

	};

	static Application* ApplicationInstance;
	inline Application* GetApplication() { return ApplicationInstance; }

	Application* CreateApplication();

}
