// Fuente: https://github.com/BennyQBD/ModernOpenGLTutorial
// Se modifico para que funcionara con la libreria chrono
#ifndef DEBUGTIMER_H_INCLUDED
#define DEBUGTIMER_H_INCLUDED

#include <chrono>
#include <iostream>

class DebugTimer
{
public:
    void Start()
    {
        startTime = std::chrono::system_clock::now();
    }
    
    void End(const std::string& message)
    {
        std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaTime = endTime - startTime;
        std::cout << message << deltaTime.count() << "ms" << std::endl;
    }
    
protected:
private:
    std::chrono::system_clock::time_point startTime;
};

#endif // DEBUGTIMER_H_INCLUDED