#pragma once
#include "Winbrew.h"
#include <chrono>

namespace gTime {
	inline auto sleep = Sleep;

	class MarcoPolo {
	public:
		MarcoPolo();

		float start() const;
		float since() const;
	private:
		mutable std::chrono::steady_clock::time_point mark;
	};
}