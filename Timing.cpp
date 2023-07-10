#include "Timing.h"

float time::MarcoPolo::start() const {
	const std::chrono::steady_clock::time_point old = mark;
	mark = std::chrono::steady_clock::now();
	const std::chrono::duration<float> elapsed = mark - old;
	return elapsed.count();
}

float time::MarcoPolo::since() const {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - mark).count();
}