#include "Util.hpp"

#include <chrono>
#include <thread>

namespace LC4500 {
	namespace Util {
		inline void sleep(uint32_t ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
	};
};
