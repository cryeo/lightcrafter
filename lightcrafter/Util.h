#ifndef _UTIL_H_

#define _UTIL_H_

#include <vector>
#include <string>
#include <chrono>
#include <thread>

namespace LightCrafter {
	namespace Util {
		template<typename T, typename ...Args>
		inline std::vector<T> makeList(Args... args) {
			return std::vector<T>(args...);
		}

        void sleep(int ms);
	};
};

#endif
