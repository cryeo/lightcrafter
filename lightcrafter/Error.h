#ifndef _ERROR_H_

#define _Error_H_

#include "dlpc350_usb.h"
#include <vector>
#include <initializer_list>
#include <string>

namespace LightCrafter {
	namespace Error {
		template<typename T, typename... Args>
		std::vector<T> makeList(Args... args) {
			return std::initializer_list<T>{args...};
		}
		namespace HSR {
			std::vector<std::string> initDone = makeList<std::string>("Error", "Successful");
			std::vector<std::string> DRCError = makeList<std::string>("No error has occurred", "Multiple overlapping bias or reset operations are accessing the same DMD block");		
			std::vector<std::string> forcedSwap = makeList<std::string>("No error has occurred", "Forced Swap Error occurred");
			std::vector<std::string> sequenceAbort = makeList<std::string>("No error has occurred", "Sequencer has detected an error condition that caused an abort");
			std::vector<std::string> sequenceError = makeList<std::string>("No error has occurred", "Sequencer detected an error");
		};
		namespace SSR {
			std::vector<std::string> memoryTest = makeList<std::string>("Internal Memory Test failed", "Internal Memory Test passed");
		};
		namespace MSR {
			std::vector<std::string> DMDParked = makeList<std::string>("DMD micromirrors are not parked", "DMD micromirrors are parked");
			std::vector<std::string> sequenceRunning = makeList<std::string>("Sequencer is stopped", "Sequencer is running normally");
			std::vector<std::string> bufferFrozen = makeList<std::string>("Frame buffer is not frozen", "Frame buffer is frozen");
			std::vector<std::string> gammaCorrection = makeList<std::string>("Gamma correction is disabled", "Gamma correction is enabled");
		};
	};
};

#endif
