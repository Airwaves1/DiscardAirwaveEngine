#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Airwave
{
class PerformanceProfiler
{
  public:
    void start(const std::string &name)
    {
        auto now         = std::chrono::high_resolution_clock::now();
        startTimes[name] = now;
    }

    void stop(const std::string &name)
    {
        auto end      = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTimes[name]).count();
        totalDurations[name] += duration;
        callCounts[name]++;
    }

    void report()
    {
        for (const auto &entry : totalDurations)
        {
            std::cout << "Function: " << entry.first << ", Total Duration: " << entry.second << "μs"
                      << ", Call Count: " << callCounts[entry.first]
                      << ", Average Duration: " << (entry.second / callCounts[entry.first]) << "μs" << std::endl;
        }
    }

  private:
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> startTimes;
    std::unordered_map<std::string, long long> totalDurations;
    std::unordered_map<std::string, int> callCounts;
};
} // namespace Airwave
