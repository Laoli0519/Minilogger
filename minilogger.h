//Easy-to-use log classes,called minilogger
//Requires C++11 standard and above


#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <initializer_list>
// ============================================================
//  get system time 
// ============================================================
#ifdef WIN32
	#include <Windows.h>
#elif linux
	#include <time.h>
#endif


// ============================================================
// time in 24 hours YYYY:MM:DD hh:mm:ss format
// ============================================================
static std::string DateTimeStamp()
{
	char str[64];

	// get the time, and convert it to struct tm format
#ifdef WIN32
	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	sprintf_s(str, "[%u-%u-%u %d:%u:%u.%u]", currentTime.wYear, currentTime.wMonth, \
		currentTime.wDay, currentTime.wHour, \
		currentTime.wMinute, currentTime.wSecond, \
		currentTime.wMilliseconds);
#elif defined(linux)
	time_t a = time(0);
	struct tm* b = localtime(&a);

	// print the time to the string
	strftime(str, 64, "%Y-%m-%d %H:%M:%S", b);
#endif
	return str;
}

namespace MiniLog {
	const uint16_t log_level_debug = 1;
	const uint16_t log_level_info = 2;
	const uint16_t log_level_warning = 4;
	const uint16_t log_level_error = 8;
	const uint16_t log_level_fatal = 16;

	// ============================================================
	// New Logger with a new log file and new log title
	// ============================================================
	class Logger {
		public:
			Logger(const std::string p_filename,
				const std::string p_username,
				const uint16_t log_level = MiniLog::log_level_debug); 
			
			Logger(const std::string p_username, 
				const uint16_t log_level = MiniLog::log_level_debug); 

			~Logger();

		public:

			void set_log_level(const uint16_t level);

			template <typename T>
			void log(const T& fmt) ;

			template <typename T, typename... Args >
			void log(const char *fmt, const T& value, const Args &... args) ;

			template<typename... Args>
			void debug(const char *fmt, const Args &... args);

			template<typename... Args>
			void info(const char *fmt, const Args &... args);

			template<typename... Args>
			void warning(const char *fmt, const Args &... args);

			template<typename... Args>
			void error(const char *fmt, const Args &... args);

			template<typename... Args>
			void fatal(const char *fmt, const Args &... args);

			template<typename T>
			void debug(const T & fmt);

			template<typename T>
			void info(const T & fmt);

			template<typename T>
			void warning(const T & fmt);

			template<typename T>
			void error(const T & fmt);

			template<typename T>
			void fatal(const T & fmt);

			void flush();
		private:
			bool should_log(const uint16_t level);

		protected:
			std::ofstream 		m_logfile;
			const uint16_t 		m_log_level;
			const std::string 	m_username;
			std::ostream		m_out_stream;
	};

	
	Logger::Logger(const std::string p_filename,
		const std::string p_username,
		const uint16_t log_level):
			m_log_level(log_level), 
			m_username(p_username), 
			m_out_stream(std::cout.rdbuf()) {
		// now the tricky part... testing to see if a file is open or not.
	    // stupid C++. You need to open a file in read mode, and if it doesn't
	    // open correctly, you know that it doesn't exist.
		// a file is open or not.
		std::fstream filetester(p_filename.c_str(), std::ios::in);

		if (filetester.is_open()) {
			filetester.close();
			m_logfile.open(p_filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
		}
		else {
			m_logfile.open(p_filename.c_str(), std::ios::out | std::ios::binary);
		}
		std::streambuf *buf = m_logfile.rdbuf();
		m_out_stream.rdbuf(buf);
	}

	Logger::Logger(const std::string p_username, const uint16_t log_level):
			m_log_level(log_level),
			m_username(p_username),
			m_out_stream(std::cout.rdbuf()) {}

	Logger::~Logger() {
		if (m_logfile.is_open()) {
			m_logfile.close();
		}
	}

	inline bool Logger::should_log(const uint16_t level) {
		return level >= m_log_level;	
	}

	template <typename T, typename... Args >
	void Logger::log(const char *fmt, const T& value, const Args &... args) {
		for(int i = 0; '\0' != fmt[i]; ++i) {
			if('{' == fmt[i] && '}' == fmt[i + 1]) {
				m_out_stream << value;
				log(fmt + 2 + i, args...);	
				break;
			}
			else {
				m_out_stream << fmt[i];
			}
		}
	}

	template <typename T>
	void Logger::log(const T& fmt) {
		m_out_stream << fmt << std::endl;
	}

	template<typename... Args>
	void Logger::debug(const char *fmt, const Args &... args) {
		if(!should_log(MiniLog::log_level_debug)) {
			return ;
		}

		std::string message;
		message = DateTimeStamp() + " [" + m_username + "] [debug] " + fmt;
		log(message.c_str(), args...);
	}

	template<typename... Args>
	void Logger::info(const char *fmt, const Args &... args) {
		if(!should_log(MiniLog::log_level_info)) {
			return ;
		}

		std::string message;
		message = DateTimeStamp() + " [" + m_username + "] [info] " + fmt;
		log(message.c_str(), args...);
	}

	template<typename... Args>
	void Logger::warning(const char *fmt, const Args &... args) {
		if(!should_log(MiniLog::log_level_warning)) {
			return ;
		}

		std::string message;
		message = DateTimeStamp() + " [" + m_username + "] [warning] " + fmt;
		log(message.c_str(), args...);
	}

	template<typename... Args>
	void Logger::error(const char *fmt, const Args &... args) {
		if(!should_log(MiniLog::log_level_error)) {
			return ;
		}

		std::string message;

		message = DateTimeStamp() + " [" + m_username + "] [error] " + fmt;
		log(message.c_str(), args...);
	}

	template<typename... Args>
	void Logger::fatal(const char *fmt, const Args &... args) {
		if(!should_log(MiniLog::log_level_fatal)) {
			return ;
		}

		std::string message;
		message = DateTimeStamp() + " [" + m_username + "] [fatal] " + fmt;
		log(message.c_str(), args...);	
	}

	template<typename T>
	void Logger::debug(const T & fmt) {
		if (!should_log(MiniLog::log_level_debug)) {
			return;
		}

		std::stringstream message;
		message << DateTimeStamp() << " [" + m_username + "] [debug] " + fmt;
		log(message.str().c_str());
	}

	template<typename T>
	void Logger::info(const T & fmt) {
		if (!should_log(MiniLog::log_level_info)) {
			return;
		}

		std::stringstream message;
		message << DateTimeStamp() << " [" << m_username << "] [info] " << fmt;
		log(message.str().c_str());
	}

	template<typename T>
	void Logger::warning(const T & fmt) {
		if (!should_log(MiniLog::log_level_warning)) {
			return;
		}

		std::stringstream message;
		message << DateTimeStamp() << " [" << m_username << "] [warning] " << fmt;
		log(message.str().c_str());
	}

	template<typename T>
	void Logger::error(const T & fmt) {
		if (!should_log(MiniLog::log_level_error)) {
			return;
		}

		std::stringstream message;
		message << DateTimeStamp() << " [" << m_username << "] [error] " << fmt;
		log(message.str().c_str());
	}

	template<typename T>
	void Logger::fatal(const T & fmt) {
		if (!should_log(MiniLog::log_level_fatal)) {
			return;
		}

		std::stringstream message;
		message << DateTimeStamp() << " [" << m_username << "] [fatal] " << fmt;
		log(message.str().c_str());
	}

	inline void Logger::flush() {
		m_out_stream.flush();
	}
}

#endif
