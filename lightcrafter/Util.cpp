#include "Util.h"

#include <chrono>
#include <thread>

namespace LightCrafter {
	namespace Util {
		inline void sleep(uint32_t ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
	};
};
