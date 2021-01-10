#pragma once
#include <spdlog/spdlog.h>

namespace Core
{
	class Log
	{
	public:
		static void Init();

		template<typename ... Args>
		static void Trace(Args&& ... args) { s_Logger->trace(std::forward<Args>(args)...); }

		template<typename ... Args>
		static void Info(Args&& ... args) { s_Logger->trace(std::forward<Args>(args)...); }

		template<typename ... Args>
		static void Warn(Args&& ... args) { s_Logger->trace(std::forward<Args>(args)...); }

		template<typename ... Args>
		static void Error(Args&& ... args) { s_Logger->trace(std::forward<Args>(args)...); }

		template<typename ... Args>
		static void Critical(Args&& ... args) { s_Logger->trace(std::forward<Args>(args)...); }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}