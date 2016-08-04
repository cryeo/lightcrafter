#ifndef _UTIL_H_

#define _UTIL_H_

#include <vector>
#include <string>
#include <chrono>
#include <thread>

namespace LightCrafter {
	namespace Util {
		template<typename T, typename ...Args>
		extern inline std::vector<T> makeList(Args... args) {
			return std::initializer_list<T>{args...};
		}

        extern void sleep(uint32_t ms);
	};
};

#endif
