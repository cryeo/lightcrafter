#ifndef _ERROR_H_

#define _ERROR_H_

#include "dlpc350_usb.h"
#include "Util.h"
#include <vector>
#include <string>

namespace LightCrafter {
	namespace Error {
		namespace HSR {
			std::vector<std::string> initDone = Util::makeList<std::string>("Error", "Successful");
			std::vector<std::string> DRCError = Util::makeList<std::string>("No error has occurred", "Multiple overlapping bias or reset operations are accessing the same DMD block");		
			std::vector<std::string> forcedSwap = Util::makeList<std::string>("No error has occurred", "Forced Swap Error occurred");
			std::vector<std::string> sequenceAbort = Util::makeList<std::string>("No error has occurred", "Sequencer has detected an error condition that caused an abort");
			std::vector<std::string> sequenceError = Util::makeList<std::string>("No error has occurred", "Sequencer detected an error");
		};
		namespace SSR {
			std::vector<std::string> memoryTest = Util::makeList<std::string>("Internal Memory Test failed", "Internal Memory Test passed");
		};
		namespace MSR {
			std::vector<std::string> DMDParked = Util::makeList<std::string>("DMD micromirrors are not parked", "DMD micromirrors are parked");
			std::vector<std::string> sequenceRunning = Util::makeList<std::string>("Sequencer is stopped", "Sequencer is running normally");
			std::vector<std::string> bufferFrozen = Util::makeList<std::string>("Frame buffer is not frozen", "Frame buffer is frozen");
			std::vector<std::string> gammaCorrection = Util::makeList<std::string>("Gamma correction is disabled", "Gamma correction is enabled");
		};
	};
};

#endif
