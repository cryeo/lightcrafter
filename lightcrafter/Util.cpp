#include "Util.h"

namespace LightCrafter {
	namespace Util {
        inline void sleep(int ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
	};
};