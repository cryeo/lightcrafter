#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

#include "LC4500/LC4500.hpp"

using namespace LC4500;

void showStatus(std::unique_ptr<DLPC350::HardwareStatus> &HSR, std::unique_ptr<DLPC350::SystemStatus> &SSR, std::unique_ptr<DLPC350::MainStatus> &MSR) {
	using namespace LC4500::Error;

	printf("* Status\n");
	printf("** Hardware Status\n");
	printf("*** Internal Initialization : %s\n", HSR::initDone[static_cast<int>(HSR->initDone)].c_str());
	printf("*** DMD Reset Controller Error : %s\n", HSR::DRCError[static_cast<int>(HSR->DRCError)].c_str());
	printf("*** Forced Swap Error : %s\n", HSR::forcedSwap[static_cast<int>(HSR->forcedSwap)].c_str());
	printf("*** Sequencer Abort Status Flag : %s\n", HSR::sequenceAbort[static_cast<int>(HSR->sequenceAbort)].c_str());
	printf("*** Sequencer Error : %s\n", HSR::sequenceError[static_cast<int>(HSR->sequenceError)].c_str());
	printf("** System Status\n");
	printf("*** Internal Memory Test : %s\n", SSR::memoryTest[static_cast<int>(SSR->memoryTest)].c_str());
	printf("*** Main Status\n");
	printf("*** DMD Park Status : %s\n", MSR::DMDParked[static_cast<int>(MSR->DMDParked)].c_str());
	printf("*** Sequencer Run Flag : %s\n", MSR::sequenceRunning[static_cast<int>(MSR->sequenceRunning)].c_str());
	printf("*** Frame Buffer Swap Flag : %s\n", MSR::bufferFrozen[static_cast<int>(MSR->bufferFrozen)].c_str());
	printf("*** Gamma Correction Function Enable : %s\n", MSR::gammaCorrection[static_cast<int>(MSR->gammaCorrection)].c_str());
}

void showVersion(std::unique_ptr<DLPC350::Version> &version) {
	printf("* Version\n");
	printf("** Application Software Revision : %d.%d.%d\n", version->app.major, version->app.minor, version->app.patch);
	printf("** API Software Revision : %d.%d.%d\n", version->api.major, version->api.minor, version->api.patch);
	printf("** Software Configuration Revision : %d.%d.%d\n", version->softwareConfig.major, version->softwareConfig.minor, version->softwareConfig.patch);
	printf("** Sequence Configuration Revision : %d.%d.%d\n", version->sequenceConfig.major, version->sequenceConfig.minor, version->sequenceConfig.patch);
}

template <typename Enumeration>
inline int as_integer(Enumeration const value) { return static_cast<int>(value); }

namespace Logger {
	template<typename T>
	inline void log(T t) {
		std::cout << t << " ";
	}

	template<typename T, typename... Args>
	inline void log(T t, Args&&... args) {
		log(t);
		log(std::forward<Args>(args)...);
		std::cout << std::endl;
	}
};

int main() {
	USB::initialize();

	std::cout << "Try to connect" << std::endl;
	if (USB::open()) {
		std::cout << "Connected" << std::endl;

 		auto HSR = LC4500::DLPC350::getHardwareStatus();
		auto SSR = LC4500::DLPC350::getSystemStatus();
		auto MSR = LC4500::DLPC350::getMainStatus();
		if (!HSR && !SSR && !MSR) return -1;
		showStatus(HSR, SSR, MSR);

		auto version = LC4500::DLPC350::getVersion();
		if (!version) return -1;
		showVersion(version);

		auto tag = LC4500::DLPC350::getFirmwareTag();
		if (!tag) return -1;
		std::cout << "* Firmware tag : " << *tag << std::endl;

		auto numImages = LC4500::DLPC350::getNumImagesInFlash();
		if (!numImages) return -1;
		std::cout << "* The number of images in flash memory : " << as_integer(*numImages) << std::endl;

		auto displayMode = LC4500::DLPC350::getDisplayMode();
		auto powerMode = LC4500::DLPC350::getPowerMode();

		//if (displayMode && *powerMode == DLPC350::PowerMode::NORMAL) { // Normal mode
		//	if (*displayMode == DLPC350::DisplayMode::PATTERN) { // Pattern Sequence Mode
		//		auto patternTriggerMode = LC4500::DLPC350::getPatternTriggerMode();
		//		auto patternDisplayMode = LC4500::DLPC350::getPatternDisplayMode();

		//		if (*patternDisplayMode == DLPC350::PatternDisplayMode::EXTERNAL) {
		//		}
		//		else {
		//		}

		//		if (*patternTriggerMode == DLPC350::PatternTriggerMode::MODE0 ||
		//			*patternTriggerMode == DLPC350::PatternTriggerMode::MODE1 ||
		//			*patternTriggerMode == DLPC350::PatternTriggerMode::MODE2) { // Pattern sequence mode
		//		}
		//		else { // Pattern sequence mode with variable exposure display sequence
		//		}

		//	}
		//	else { // Video mode
		//	}
		//}
		//else { // Standby mode
		//}

		using namespace Logger;

		Logger::log("setDisplayMode", LC4500::DLPC350::setDisplayMode(DLPC350::DisplayMode::PATTERN));
		Logger::log("setLEDCurrent", LC4500::DLPC350::setLEDCurrent(255, 255, 255));
		auto a = LC4500::DLPC350::getPatternDisplayMode();
		Logger::log("getPatternDisplayMode", as_integer(*a));
		Logger::log("setPatternDisplayMode", LC4500::DLPC350::setPatternDisplayMode(DLPC350::PatternDisplayMode::INTERNAL));
		auto b = LC4500::DLPC350::getPatternDisplayMode();
		Logger::log("getPatternDisplayMode", as_integer(*b));
		Logger::log("setPatternPeriod", LC4500::DLPC350::setPatternPeriod(2700, 3000));
		Logger::log("setPatternTriggerMode", LC4500::DLPC350::setPatternTriggerMode(DLPC350::PatternTriggerMode::MODE1));

		using Bit = Pattern::BitIndex;
		PatternSequence ps;
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G0);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G1);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G2);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G3);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G4);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G5);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G6);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::G7);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R0);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R1);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R2);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R3);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R4);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R5);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R6);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::R7);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::B0);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::B1);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 2, Bit::B2);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G0);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G1);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G2);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G3);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G4);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G5);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G6);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G7);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R0);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R1);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R2);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R3);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R4);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R5);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R6);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R7);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::B0);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::B1);
		ps.addPattern(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::B2);

		Logger::log("configurePatternSequence", LC4500::DLPC350::configurePatternSequence(ps));
		Logger::log("sendPatternDisplayLUT", LC4500::DLPC350::sendPatternDisplayLUT(ps));
		Logger::log("sendPatternImageLUT", LC4500::DLPC350::sendPatternImageLUT(ps));

		Logger::log("validatePatternSequence", LC4500::DLPC350::validatePatternSequence()->isValid());
		LC4500::DLPC350::setPatternSequenceStatus(DLPC350::PatternSequenceStatus::STOP);
		while (true) {
			LC4500::DLPC350::setPatternSequenceStatus(DLPC350::PatternSequenceStatus::START);
			if (*LC4500::DLPC350::getPatternSequenceStatus() == DLPC350::PatternSequenceStatus::START) break;
		}
		std::cout << "Pattern sequence started" << std::endl;

		LC4500::DLPC350::setPatternSequenceStatus(DLPC350::PatternSequenceStatus::STOP);
		std::cout << "Pattern sequence stoped" << std::endl;
	}

	std::cout << "Disconnect" << std::endl;

	USB::close();
	USB::exit();

	return 0;
}
