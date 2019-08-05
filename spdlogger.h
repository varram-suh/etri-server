#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace util
{
	class Log
	{
	// singleton
	private:

	public:
		static Log* getInstance()
		{
            static Log* instance;
            static std::once_flag initFlag;
			std::call_once(initFlag, []() { instance = new Log(); });
			return instance;
		}

	// function
	private:
		std::shared_ptr<spdlog::logger>	m_loggerConsole;

	public:
		Log()
		{
            m_loggerConsole = spdlog::stdout_color_mt("console"); 
            m_loggerConsole->set_level(spdlog::level::info); // Set specific logger's log level
		}

		template <typename... Args>
		void info(const char* message, const Args&... args)
		{
            m_loggerConsole->info(message, args...);
		}
		template <typename... Args>
		void critical(const char* message, const Args&... args)
		{
            m_loggerConsole->critical(message, args...);
		}
		template <typename... Args>
		void debug(const char* message, const Args&... args)
		{
            m_loggerConsole->debug(message, args...);
		}
		template <typename... Args>
		void trace(const char* message, const Args&... args)
		{
            m_loggerConsole->trace(message, args...);
		}
	};

	// macro
	template <typename... Args>
	void Info(const char* message, const Args&... args)
	{
		Log::getInstance()->info(message, args...);
	}
	template <typename... Args>
	void Debug(const char* message, const Args&... args)
	{
		Log::getInstance()->debug(message, args...);
	}
	template <typename... Args>
	void Trace(const char* message, const Args&... args)
	{
		Log::getInstance()->debug(message, args...);
	}
	template <typename... Args>
	void Critical(const char* message, const Args&... args)
	{
		Log::getInstance()->critical(message, args...);
	}
}


// define macro
#define CONSOLE_INFO(message, ...)  util::Info(message, ##__VA_ARGS__)
#define CONSOLE_DEBUG(message, ...)  util::Debug(message, ##__VA_ARGS__)
#define CONSOLE_CRITICAL(message, ...)  util::Critical(message, ##__VA_ARGS__)
#define CONSOLE_TRACE(message, ...)  util::Trace(message, ##__VA_ARGS__)

