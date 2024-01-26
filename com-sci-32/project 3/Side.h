#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline Side opponent(Side s)
{
	return Side(NSIDES - 1 - s);
}


#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
            std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#endif