#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

#include "LC4500/LC4500.hpp"
#include "LC4500/USB/USB.hpp"

using namespace std;
using namespace LC4500;

void showStatus(unique_ptr<DLPC350::HardwareStatus> &HSR, unique_ptr<DLPC350::SystemStatus> &SSR, unique_ptr<DLPC350::MainStatus> &MSR) {
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

void showVersion(unique_ptr<DLPC350::Version> &version) {
	printf("* Version\n");
	printf("** Application Software Revision : %d.%d.%d\n", version->app.major, version->app.minor, version->app.patch);
	printf("** API Software Revision : %d.%d.%d\n", version->api.major, version->api.minor, version->api.patch);
	printf("** Software Configuration Revision : %d.%d.%d\n", version->softwareConfig.major, version->softwareConfig.minor, version->softwareConfig.patch);
	printf("** Sequence Configuration Revision : %d.%d.%d\n", version->sequenceConfig.major, version->sequenceConfig.minor, version->sequenceConfig.patch);
}

template <typename Enumeration>
auto as_integer(Enumeration const value)
-> typename int
{
	return static_cast<int>(value);
}

namespace Logger {
	template<typename...Args>
	struct Log;
	template<typename T, typename...Args>
	struct Log<T, Args...> {
		void operator()(T head, Args... tail) {
			cout << " ";
			cout << head;
			Log<Args...>{}(tail...);
		}
	};
	template<>
	struct Log<> {
		void operator()() {
			cout << endl;
		}
	};
	template<typename T, typename... Args>
	void log(T t, Args... args) {
		cout << t;
		Log<Args...>{}(args...);
	}
};

int main() {
	USB::initialize();

	cout << "Try to connect" << endl;
	if (USB::open()) {
		cout << "Connected" << endl;

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
		cout << *tag << endl;

		auto numImages = LC4500::DLPC350::getNumImagesInFlash();
		if (!numImages) return -1;
		cout << static_cast<uint16_t>(*numImages) << endl;

		auto displayMode = LC4500::DLPC350::getDisplayMode();
		auto powerMode = LC4500::DLPC350::getPowerMode();

		if (displayMode && *powerMode == DLPC350::PowerMode::NORMAL) { // Normal mode
			if (*displayMode == DLPC350::DisplayMode::PATTERN) { // Pattern Sequence Mode
				auto patternTriggerMode = LC4500::DLPC350::getPatternTriggerMode();
				auto patternDisplayMode = LC4500::DLPC350::getPatternDisplayMode();

				if (*patternDisplayMode == DLPC350::PatternDisplayMode::EXTERNAL) {
				}
				else {
				}

				if (*patternTriggerMode == DLPC350::PatternTriggerMode::MODE0 ||
					*patternTriggerMode == DLPC350::PatternTriggerMode::MODE1 ||
					*patternTriggerMode == DLPC350::PatternTriggerMode::MODE2) { // Pattern sequence mode
				}
				else { // Pattern sequence mode with variable exposure display sequence
				}

			}
			else { // Video mode
			}
		}
		else { // Standby mode
		}

		using namespace Logger;

		log("setDisplayMode", LC4500::DLPC350::setDisplayMode(DLPC350::DisplayMode::PATTERN));
		log("setLEDCurrent", LC4500::DLPC350::setLEDCurrent(255, 255, 255));
		log("getPatternDisplayMode", as_integer(*LC4500::DLPC350::getPatternDisplayMode()));
		log("setPatternDisplayMode", LC4500::DLPC350::setPatternDisplayMode(DLPC350::PatternDisplayMode::INTERNAL));
		log("getPatternDisplayMode", as_integer(*LC4500::DLPC350::getPatternDisplayMode()));
		log("setPatternPeriod", LC4500::DLPC350::setPatternPeriod(2700, 3000));
		log("setPatternTriggerMode", LC4500::DLPC350::setPatternTriggerMode(DLPC350::PatternTriggerMode::MODE1));

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

		log("configurePatternSequence", LC4500::DLPC350::configurePatternSequence(ps));
		log("sendPatternDisplayLUT", LC4500::DLPC350::sendPatternDisplayLUT(ps));
		log("sendPatternImageLUT", LC4500::DLPC350::sendPatternImageLUT(ps));

		log("validatePatternSequence", LC4500::DLPC350::validatePatternSequence()->isValid());
		LC4500::DLPC350::setPatternSequenceStatus(DLPC350::PatternSequenceStatus::STOP);
		while (true) {
			LC4500::DLPC350::setPatternSequenceStatus(DLPC350::PatternSequenceStatus::START);
			if (*LC4500::DLPC350::getPatternSequenceStatus() == DLPC350::PatternSequenceStatus::START) break;
		}
		cout << "Started" << endl;
		int t;
		cin >> t;
		LC4500::DLPC350::setPatternSequenceStatus(DLPC350::PatternSequenceStatus::STOP);

		//cout << "Video Mode" << endl;
		//LC4500::DLPC350::setDisplayMode(DLPC350::DisplayMode::VIDEO);
		//cin >> t;
		//cout << "Set LED Enable" << endl;
		//LC4500::DLPC350::setLEDEnable(LightCrafter::DLPC350::LEDEnableMode::AUTO, true, true, true);
		//cin >> t;
		//cout << "Set LED 255, 0, 0" << endl;
		//LC4500::DLPC350::setLEDCurrent(255, 0, 0);
		//cin >> t;
		//cout << "Set LED 0, 255, 0" << endl;
		//LC4500::DLPC350::setLEDCurrent(0, 255, 0);
		//cin >> t;
		//cout << "Set LED 0, 0, 255" << endl;
		//LC4500::DLPC350::setLEDCurrent(0, 0, 255);
		//cin >> t;
		//cout << "Set LED 255, 255, 255" << endl;
		//LC4500::DLPC350::setLEDCurrent(255, 255, 255);
		//cin >> t;
		//cout << "Set Input source as Test Pattern" << endl;
		//LC4500::DLPC350::setInputSource(DLPC350::InputType::TEST_PATTERN);
		//cin >> t;
		//for (int i = 0; i <= 10; i++) {
		//	cout << "Set Input as Test Pattern " << i << endl;
		//	LC4500::DLPC350::setTestPattern(static_cast<DLPC350::TestPattern>(i));
		//	cin >> t;
		//}
		//cout << "Set Input source as Flash Image" << endl;
		//LC4500::DLPC350::setInputSource(DLPC350::InputType::FLASH);
		//cin >> t;
		//for (uint8_t i = 0; i < *LC4500::DLPC350::getNumImagesInFlash(); i++) {
		//	cout << "Set Input as Flash Image " << static_cast<uint16_t>(i) << endl;
		//	LC4500::DLPC350::setFlashImage(i);
		//	cin >> t;
		//}
	}
	cout << "Disconnect" << endl;
	USB::close();
	USB::exit();

	return 0;
}
