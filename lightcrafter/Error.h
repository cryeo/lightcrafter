#ifndef _LIGHTCRAFTER_ERROR_H_

#define _LIGHTCRAFTER_ERROR_H_

#include "Util.h"
#include <vector>
#include <string>

namespace LightCrafter {
	namespace Error {
		namespace HSR {
			extern std::vector<std::string> initDone;
			extern std::vector<std::string> DRCError;
			extern std::vector<std::string> forcedSwap;
			extern std::vector<std::string> sequenceAbort;
			extern std::vector<std::string> sequenceError;
		};
		namespace SSR {
			extern std::vector<std::string> memoryTest;
		};
		namespace MSR {
			extern std::vector<std::string> DMDParked;
			extern std::vector<std::string> sequenceRunning;
			extern std::vector<std::string> bufferFrozen;
			extern std::vector<std::string> gammaCorrection;
		};
	};
};

#endif
