// Code in this file use source from and inspired by TheCherno/Hazel engine project
// https://github.com/TheCherno/Hazel/


#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Engine
{
	class ENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;

	};
}


#define LOG_ERR   error
#define LOG_WARN  warn
#define LOG_TRACE trace
#define LOG_INFO  info
#define LOG_FATAL fatal

#ifndef ENGINE_DISABLE_LOG
	#define ENGINE_LOG(type, ...) ::Engine::Log::GetCoreLogger()->type(__VA_ARGS__)
	#define GAME_LOG(type, ...)    ::Engine::Log::GetClientLogger()->type(__VA_ARGS__)
#else
	#define ENGINE_LOG(type, ...)
	#define GAME_LOG(type, ...)
#endif

#ifdef ENGINE_ENABLE_ASSERTS
	#define ENGINE_ASSERT(x, ...) \
      { if (!(x)) { ENGINE_LOG(LOG_ERR, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define APP_ASSERT(x, ...) \
      { if (!(x)) { APP_LOG(LOG_ERR, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ENGINE_ASSERT(x, ...)
	#define APP_ASSERT(x, ...)
#endif
