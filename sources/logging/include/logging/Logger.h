#pragma once

#include "DisplayBuffer.h"
#include "LogEntry.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace logging {

  class Logger {
  public:
    static Logger &instance();

    void enqueue(LogEntry entry);

    void setLogFile(const std::string &path);

    void start();

    void stop();

    DisplayBuffer &displayBuffer();

  private:
    Logger();
    ~Logger();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    std::chrono::steady_clock::time_point startTime_;

    std::queue<LogEntry> queue_;
    mutable std::mutex mutex_;
    std::condition_variable_any cv_;

    std::jthread workerThread_;
    std::atomic<bool> started_{false};

    std::ofstream logFile_;
    DisplayBuffer displayBuffer_;

    void workerLoop(std::stop_token stopToken);
    void processEntry(LogEntry &entry);
    std::string formatEntry(LogEntry &entry);
  };

}
