#pragma once

#include <thread>

#define SLEEP(ms) std::this_thread::sleep_for(std::chrono::milliseconds((ms)))