#pragma once

#include <chrono>

////////////////////////////////////////////////////////////////////////////////////
// Base class that can handle measurements
////////////////////////////////////////////////////////////////////////////////////
class timer_monitor
{
public:
   virtual ~timer_monitor() = default;
   virtual void add_measurement(std::chrono::milliseconds duration_in_ms) = 0;
};

////////////////////////////////////////////////////////////////////////////////////
// RAII Timer
////////////////////////////////////////////////////////////////////////////////////
/// NOTE: Does this measure the destructor calls of other objects in the timer scope? 
/// Destructors are called in reverse order to which they were initialised, the last 
/// constructed object gets destructed first, same as in stack unwinding when exception 
/// is thrown so where you place your timing is important.
class timer
{
public:
   using TimePointT = std::chrono::time_point<std::chrono::high_resolution_clock>;

   timer(timer_monitor& monitor) : m_monitor(monitor)
   {
      m_start_time_point = std::chrono::high_resolution_clock::now();
   }
   ~timer()
   {
      stop();
   }

private:
   void stop() const
   {
      // Calculate time and return it to the monitor
      const auto end_time_point = std::chrono::high_resolution_clock::now();
      std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_point - m_start_time_point);
      // Notify the monitor
      m_monitor.add_measurement(duration);
   }

private:
   // start time point
   TimePointT m_start_time_point;
   // results monitor
   timer_monitor& m_monitor;
};
