#ifndef _LC4500_UTIL_H_

#define _LC4500_UTIL_H_

#include <cstdint>

namespace {
	inline uint64_t operator"" _ui64(unsigned long long value) { return static_cast<uint64_t>(value); }
	inline uint32_t operator"" _ui32(unsigned long long value) { return static_cast<uint32_t>(value); }
	inline uint16_t operator"" _ui16(unsigned long long value) { return static_cast<uint16_t>(value); }
	inline uint8_t operator"" _ui8(unsigned long long value) { return static_cast<uint8_t>(value); }

	inline int64_t operator"" _i64(unsigned long long value) { return static_cast<int64_t>(value); }
	inline int32_t operator"" _i32(unsigned long long value) { return static_cast<int32_t>(value); }
	inline int16_t operator"" _i16(unsigned long long value) { return static_cast<int16_t>(value); }
	inline int8_t operator"" _i8(unsigned long long value) { return static_cast<int8_t>(value); }
}

namespace LightCrafter {
	namespace Util {
        extern void sleep(uint32_t ms);
	};
};

#endif
