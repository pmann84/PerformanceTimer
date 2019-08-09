#include <memory>
#include <iostream>
#include <thread>
#include <vector>

#include "timer.h"
#include <random>

////////////////////////////////////////////////////////////////////////////////////
// Random utility function
////////////////////////////////////////////////////////////////////////////////////
int random_int_in_range(int start, int end)
{
   const std::uniform_int_distribution<int> uniform_dist(start, end);
   std::random_device rd;
   std::default_random_engine rand(rd());
   return uniform_dist(rand);
}

////////////////////////////////////////////////////////////////////////////////////
// Client derived monitors
////////////////////////////////////////////////////////////////////////////////////
class cout_monitor : public timer_monitor
{
public:
   void add_measurement(std::chrono::milliseconds duration_in_ms) override
   {
      std::cout << "Duration: " << duration_in_ms.count() << std::endl;
   }
};

class performance_monitor : public timer_monitor
{
public:
   void add_measurement(std::chrono::milliseconds duration_in_ms) override
   {
      m_measurements.push_back(duration_in_ms.count());
   }

   std::vector<double> get_measurements() const
   {
      return m_measurements;
   }

private:
   std::vector<double> m_measurements;
};

////////////////////////////////////////////////////////////////////////////////////
// Entry point
////////////////////////////////////////////////////////////////////////////////////
int main()
{
   // Simple monitor that just spits out measurement to cout
   std::cout << "Cout Monitor: " << std::endl;
   cout_monitor coutmonitor;
   for(int i = 0; i < 5; ++i)
   {
      timer timer(coutmonitor);
      std::this_thread::sleep_for(std::chrono::milliseconds(random_int_in_range(0, 1000)));
   }

   // Timer with a monitor that can store the individual results
   std::cout << "Performance Monitor: " << std::endl;
   performance_monitor perf_monitor;
   for (int i = 0; i < 5; ++i)
   {
      timer timer(perf_monitor);
      std::this_thread::sleep_for(std::chrono::milliseconds(random_int_in_range(0, 1000)));
   }

   for(auto& m : perf_monitor.get_measurements())
   {
      std::cout << m << ", ";
   }
   std::cout << std::endl;

   return 0;
}