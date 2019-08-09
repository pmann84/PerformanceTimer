# PerformanceTimer

This is a simple RAII timer for C++, which provides a hook to gather timed measurements.

While this implementation works fine, it is worth noting that you should always check what you are measuring. As this implementation relys on destruction of the timer object when it goes out of scope, depending on the order of initialisation of other objects in the scope it may time the destruction of these other objects in the same scope. 
In C++ class destructors are called in reverse order to which they were initialised i.e. the last constructed object gets destructed first, so where you place your timer might be important.
