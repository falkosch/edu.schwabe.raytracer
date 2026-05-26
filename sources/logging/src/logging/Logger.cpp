#include "logging/Logger.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace logging
{
  Logger& Logger::instance()
  {
    static Logger logger;
    return logger;
  }

  Logger::Logger()
    : startTime_(std::chrono::steady_clock::now())
  {
  }

  Logger::~Logger()
  {
    stop();
  }

  void Logger::enqueue(LogEntry entry)
  {
    {
      std::lock_guard lock(mutex_);
      queue_.push(std::move(entry));
    }
    if (started_)
    {
      cv_.notify_one();
    }
  }

  void Logger::setLogFile(const std::string& path)
  {
    logFile_.open(path, std::ios::out | std::ios::trunc);
  }

  void Logger::start()
  {
    started_ = true;
    workerThread_ = std::jthread([this](const std::stop_token& stopToken)
    {
      workerLoop(stopToken);
    });
  }

  void Logger::stop()
  {
    if (workerThread_.joinable())
    {
      workerThread_.request_stop();
      cv_.notify_all();
      workerThread_.join();
    }

    std::lock_guard lock(mutex_);
    while (!queue_.empty())
    {
      auto entry = std::move(queue_.front());
      queue_.pop();
      processEntry(entry);
    }
    if (logFile_.is_open())
    {
      logFile_.flush();
      logFile_.close();
    }
  }

  DisplayBuffer& Logger::displayBuffer()
  {
    return displayBuffer_;
  }

  void Logger::workerLoop(const std::stop_token& stopToken)
  {
    while (!stopToken.stop_requested())
    {
      std::unique_lock lock(mutex_);
      cv_.wait(lock, stopToken, [this]
      {
        return !queue_.empty();
      });

      std::queue<LogEntry> batch;
      std::swap(batch, queue_);
      lock.unlock();

      while (!batch.empty())
      {
        auto entry = std::move(batch.front());
        batch.pop();
        processEntry(entry);
      }
    }

    std::lock_guard lock(mutex_);
    while (!queue_.empty())
    {
      auto entry = std::move(queue_.front());
      queue_.pop();
      processEntry(entry);
    }
  }

  std::string Logger::formatEntry(const LogEntry& entry) const
  {
    const auto message = entry.messageFactory();

    const auto elapsed = std::chrono::duration<double>(entry.timestamp - startTime_).count();

    const char* levelStr;
    switch (entry.level)
    {
    case LogLevel::Error: levelStr = "ERROR";
      break;
    case LogLevel::Warn: levelStr = "WARN";
      break;
    case LogLevel::Info: levelStr = "INFO";
      break;
    case LogLevel::Debug: levelStr = "DEBUG";
      break;
    default: levelStr = "?";
      break;
    }

    std::ostringstream oss;
    oss << "[" << std::fixed << std::setprecision(3) << elapsed
      << "] - " << levelStr << " " << entry.scope
      << " : " << message;
    return oss.str();
  }

  void Logger::processEntry(const LogEntry& entry)
  {
    auto formatted = formatEntry(entry);

    if (entry.level == LogLevel::Error || entry.level == LogLevel::Warn)
    {
      std::cerr << formatted << std::endl;
    }
    else
    {
      std::cout << formatted << std::endl;
    }

    if (logFile_.is_open())
    {
      logFile_ << formatted << "\n";
    }

    if (entry.level != LogLevel::Debug)
    {
      displayBuffer_.push(DisplayLine{entry.level, std::move(formatted)});
    }
  }
}
