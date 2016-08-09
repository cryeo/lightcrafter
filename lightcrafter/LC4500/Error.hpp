#ifndef _LC4500_ERROR_H_

#define _LC4500_ERROR_H_

#include "Util.hpp"
#include <vector>
#include <string>

namespace LC4500 {
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
