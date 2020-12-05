/* Copyright 2017 R. Thomas
 * Copyright 2017 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <map>
#include "LIEF/config.h"
#include "LIEF/logging.hpp"
#include "logging.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace LIEF {
namespace logging {

Logger* Logger::instance_ = nullptr;

Logger::Logger(Logger&&) = default;
Logger& Logger::operator=(Logger&&) = default;
Logger::~Logger() = default;

Logger::Logger(void) {
  // TODO(romain): Handle Android logcat sink
  if /* constexpr */ (lief_logging_support) {
    this->sink_ = spdlog::stderr_color_mt("console");

    this->sink_->set_level(spdlog::level::warn);
    this->sink_->set_pattern("%v");
    this->sink_->flush_on(spdlog::level::warn);
  }
}

Logger& Logger::instance() {
  if (instance_ == nullptr) {
    instance_ = new Logger{};
    std::atexit(destroy);
  }
  return *instance_;
}


void Logger::destroy(void) {
  delete instance_;
}

const char* to_string(LOGGING_LEVEL e) {
  const std::map<LOGGING_LEVEL, const char*> enumStrings {
    { LOGGING_LEVEL::LOG_TRACE,   "TRACE"    },
    { LOGGING_LEVEL::LOG_DEBUG,   "DEBUG"    },
    { LOGGING_LEVEL::LOG_INFO,    "INFO"     },
    { LOGGING_LEVEL::LOG_ERR,     "ERROR"    },
    { LOGGING_LEVEL::LOG_WARN,    "WARNING"  },
    { LOGGING_LEVEL::LOG_CRITICAL,"CRITICAL" },
  };
  auto   it  = enumStrings.find(e);
  return it == enumStrings.end() ? "UNDEFINED" : it->second;
}


void Logger::disable(void) {
  if /* constexpr */ (lief_logging_support) {
    Logger::instance().sink_->set_level(spdlog::level::off);
  }
}

void Logger::enable(void) {
  if /* constexpr */ (lief_logging_support) {
    Logger::instance().sink_->set_level(spdlog::level::warn);
  }
}

void Logger::set_level(LOGGING_LEVEL level) {
  if /* constexpr */ (not lief_logging_support) {
    return;
  }
  switch (level) {
    case LOG_TRACE:
      {
        Logger::instance().sink_->set_level(spdlog::level::trace);
        Logger::instance().sink_->flush_on(spdlog::level::trace);
        break;
      }

    case LOG_DEBUG:
      {
        Logger::instance().sink_->set_level(spdlog::level::debug);
        Logger::instance().sink_->flush_on(spdlog::level::debug);
        break;
      }

    case LOG_INFO:
      {
        Logger::instance().sink_->set_level(spdlog::level::info);
        Logger::instance().sink_->flush_on(spdlog::level::info);
        break;
      }

    default:
    case LOG_WARN:
      {
        Logger::instance().sink_->set_level(spdlog::level::warn);
        Logger::instance().sink_->flush_on(spdlog::level::warn);
        break;
      }

    case LOG_ERR:
      {
        Logger::instance().sink_->set_level(spdlog::level::err);
        Logger::instance().sink_->flush_on(spdlog::level::err);
        break;
      }

    case LOG_CRITICAL:
      {
        Logger::instance().sink_->set_level(spdlog::level::critical);
        Logger::instance().sink_->flush_on(spdlog::level::critical);
        break;
      }
  }
}

// Public interface

void disable(void) {
  Logger::disable();
}

void enable(void) {
  Logger::enable();
}

void set_level(LOGGING_LEVEL level) {
  Logger::set_level(level);
}

}
}


