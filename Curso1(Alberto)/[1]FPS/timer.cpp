// Fuente: https://www.youtube.com/watch?v=Vyt-dM8gTRQ

#include "timer.h"

Timer* Timer::sInstance = nullptr;

Timer* Timer::Instance() {
    if (sInstance == nullptr) {
        sInstance = new Timer();
    }

    return sInstance;
}

void Timer::Release() {
    delete sInstance;
    sInstance = nullptr;
}

Timer::Timer() {
    mTimeScale = 1.0f;
    mDeltaTime = std::chrono::duration<float>(0.0f);
}

Timer::~Timer() {

}

void Timer::Start() {
    mStartTime = std::chrono::system_clock::now();
}

void Timer::Reset() {
    mStartTime = std::chrono::system_clock::now();
}

float Timer::DeltaTime() {
    return mDeltaTime.count();
}

void Timer::TimeScale(float t) {
    mTimeScale = t;
}

float Timer::TimeScale() {
    return mTimeScale;
}

void Timer::Tick() {
    mDeltaTime = std::chrono::system_clock::now() - mStartTime;
}

void Timer::Wait(float ms) {
    Timer* delay = Timer::Instance();
    while (delay->DeltaTime() < ms) delay->Tick();
}