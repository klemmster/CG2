#ifndef STOPWATCH_H_XU6R2ZE0
#define STOPWATCH_H_XU6R2ZE0
#include <chrono>
#include <iostream>

using namespace std::chrono;

class Stopwatch
{
public:
    Stopwatch (const std::string id):
        start(high_resolution_clock::now()),
        id(id){};
    virtual ~Stopwatch () { std::cout << id << " took Time: " <<  duration_cast<milliseconds>(end - start).count() << "ms\n";  };

    void stop() { end = high_resolution_clock::now(); };

private:
    const time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> end;
    const std::string id;
};

#endif /* end of include guard: STOPWATCH_H_XU6R2ZE0 */

