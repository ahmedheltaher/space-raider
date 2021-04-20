#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

void setInterval(std::function<void()> callback, unsigned int interval) { // for infinte
    std::thread([callback, interval]() {
        while (true) {
            callback();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

void setInterval(std::function<void()> callback, unsigned int interval, int times) { // With Limted Times
    std::thread([callback, interval, times]() {
        int counter = times;
        while (true) {
            if (counter <= 0) return;
            callback();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            counter--;
        }
    }).detach();
}

void setTimeout(std::function<void()> callback, unsigned int delay) {
    std::thread([callback, delay]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback();
    }).detach();
}

#endif // !TIME