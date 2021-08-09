/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "logger.hpp"
#include "spdlog/sinks/syslog_sink.h"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

Logger* Logger::m_singleton = NULL;
//------------------------------------------------------------------------------
void Logger::_init(
    const char* app, const bool log_stdout, bool const log_rot_file) {
  int num_sinks = 0;
  spdlog::set_async_mode(2048);
#if TRACE_IS_ON
  spdlog::level::level_enum llevel = spdlog::level::trace;
#elif DEBUG_IS_ON
  spdlog::level::level_enum llevel = spdlog::level::debug;
#elif INFO_IS_ON
  spdlog::level::level_enum llevel = spdlog::level::info;
#else
  spdlog::level::level_enum llevel = spdlog::level::warn;
#endif
  if (log_stdout) {
    std::string filename = fmt::format("./{}.log", app);
    m_sinks.push_back(
        std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
    m_sinks[num_sinks++].get()->set_level(llevel);
  }
  if (log_rot_file) {
    std::string filename = fmt::format("./{}.log", app);
    m_sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        filename, 5 * 1024 * 1024, 3));
    m_sinks[num_sinks++].get()->set_level(llevel);
  }

  std::stringstream ss;
  ss << "[%Y-%m-%dT%H:%M:%S.%f] [" << app << "] [%n] [%l] %v";

  m_async_cmd = new _Logger("async_c", m_sinks, ss.str().c_str());
  m_itti      = new _Logger("itti   ", m_sinks, ss.str().c_str());
  m_nrf_app   = new _Logger("nrf_app", m_sinks, ss.str().c_str());
  m_system    = new _Logger("system ", m_sinks, ss.str().c_str());
  m_nrf_sbi   = new _Logger("sbi_srv", m_sinks, ss.str().c_str());
}

//------------------------------------------------------------------------------
_Logger::_Logger(
    const char* category, std::vector<spdlog::sink_ptr>& sinks,
    const char* pattern)
    : m_log(category, sinks.begin(), sinks.end()) {
  m_log.set_pattern(pattern);
#if TRACE_IS_ON
  m_log.set_level(spdlog::level::trace);
#elif DEBUG_IS_ON
  m_log.set_level(spdlog::level::debug);
#elif INFO_IS_ON
  m_log.set_level(spdlog::level::info);
#else
  m_log.set_level(spdlog::level::warn);
#endif
}

//------------------------------------------------------------------------------
void _Logger::trace(const char* format, ...) {
#if TRACE_IS_ON
  va_list args;
  va_start(args, format);
  log(_ltTrace, format, args);
  va_end(args);
#endif
}

//------------------------------------------------------------------------------
void _Logger::debug(const char* format, ...) {
#if DEBUG_IS_ON
  va_list args;
  va_start(args, format);
  log(_ltDebug, format, args);
  va_end(args);
#endif
}

//------------------------------------------------------------------------------
void _Logger::info(const char* format, ...) {
#if INFO_IS_ON
  va_list args;
  va_start(args, format);
  log(_ltInfo, format, args);
  va_end(args);
#endif
}

//------------------------------------------------------------------------------
void _Logger::startup(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(_ltStartup, format, args);
  va_end(args);
}

//------------------------------------------------------------------------------
void _Logger::warn(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(_ltWarn, format, args);
  va_end(args);
}

//------------------------------------------------------------------------------
void _Logger::error(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(_ltError, format, args);
  va_end(args);
}

//------------------------------------------------------------------------------
void _Logger::log(_LogType lt, const char* format, va_list& args) {
  char buffer[2048];

  vsnprintf(buffer, sizeof(buffer), format, args);

  switch (lt) {
    case _ltTrace:
      m_log.trace(buffer);
      break;
    case _ltDebug:
      m_log.debug(buffer);
      break;
    case _ltInfo:
      m_log.info(buffer);
      break;
    case _ltStartup:
      m_log.warn(buffer);
      break;
    case _ltWarn:
      m_log.error(buffer);
      break;
    case _ltError:
      m_log.critical(buffer);
      break;
  }
}
