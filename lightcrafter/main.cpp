#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

#include "DLPC350.h"
#include "Error.h"
#include "USB.h"

using namespace std;
using namespace LightCrafter::Error;
using namespace LightCrafter;

template<class A, class B, class C>
void showStatus(A HSR, B SSR, C MSR) {
	using namespace LightCrafter::Error;

	std::printf("* Status\n");
	std::printf("** Hardware Status\n");
	std::printf("*** Internal Initialization : %s\n", HSR::initDone[static_cast<int>(HSR->initDone)].c_str());
	std::printf("*** DMD Reset Controller Error : %s\n", HSR::DRCError[static_cast<int>(HSR->DRCError)].c_str());
	std::printf("*** Forced Swap Error : %s\n", HSR::forcedSwap[static_cast<int>(HSR->forcedSwap)].c_str());
	std::printf("*** Sequencer Abort Status Flag : %s\n", HSR::sequenceAbort[static_cast<int>(HSR->sequenceAbort)].c_str());
	std::printf("*** Sequencer Error : %s\n", HSR::sequenceError[static_cast<int>(HSR->sequenceError)].c_str());
	std::printf("** System Status\n");
	std::printf("*** Internal Memory Test : %s\n", SSR::memoryTest[static_cast<int>(SSR->memoryTest)].c_str());
	std::printf("*** Main Status\n");
	std::printf("*** DMD Park Status : %s\n", MSR::DMDParked[static_cast<int>(MSR->DMDParked)].c_str());
	std::printf("*** Sequencer Run Flag : %s\n", MSR::sequenceRunning[static_cast<int>(MSR->sequenceRunning)].c_str());
	std::printf("*** Frame Buffer Swap Flag : %s\n", MSR::bufferFrozen[static_cast<int>(MSR->bufferFrozen)].c_str());
	std::printf("*** Gamma Correction Function Enable : %s\n", MSR::gammaCorrection[static_cast<int>(MSR->gammaCorrection)].c_str());
}

template<class A>
void showVersion(A version) {
	std::printf("* Version\n");
	std::printf("** Application Software Revision : %d.%d.%d\n", version->app.major, version->app.minor, version->app.patch);
	std::printf("** API Software Revision : %d.%d.%d\n", version->api.major, version->api.minor, version->api.patch);
	std::printf("** Software Configuration Revision : %d.%d.%d\n", version->softwareConfig.major, version->softwareConfig.minor, version->softwareConfig.patch);
	std::printf("** Sequence Configuration Revision : %d.%d.%d\n", version->sequenceConfig.major, version->sequenceConfig.minor, version->sequenceConfig.patch);
}

int main() {
	LightCrafter::DLPC350 lc;

	cout << "Connect" << endl;
	if (lc.connect()) {
		cout << "Connected" << endl;

		auto HSR = lc.getHardwareStatus();
		auto SSR = lc.getSystemStatus();
		auto MSR = lc.getMainStatus();
		if (!HSR && !SSR && !MSR) return -1;
		showStatus(HSR, SSR, MSR);

		auto version = lc.getVersion();
		if (!version) return -1;
		showVersion(version);

		auto tag = lc.getFirmwareTag();
		if (!tag) return -1;
		cout << *tag << endl;

		auto numImages = lc.getNumImagesInFlash();
		if (!numImages) return -1;
		cout << static_cast<uint16_t>(*numImages) << endl;

		auto displayMode = lc.getDisplayMode();
		auto powerMode = lc.getPowerMode();

		if (displayMode && *powerMode == DLPC350::PowerMode::NORMAL) { // Normal mode
			if (*displayMode == DLPC350::DisplayMode::PATTERN) { // Pattern Sequence Mode
				auto patternTriggerMode = lc.getPatternTriggerMode();
				auto patternDisplayMode = lc.getPatternDisplayMode();

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

		int t;

		cout << "Pattern Mode" << endl;
		lc.setDisplayMode(DLPC350::DisplayMode::PATTERN);
		lc.setPatternDisplayMode(DLPC350::PatternDisplayMode::INTERNAL);
		lc.setPatternPeriod(2700, 3000);
		lc.setPatternTriggerMode(DLPC350::PatternTriggerMode::MODE1);

		lc.clearPatternSequence();

		using Pattern = DLPC350::Pattern;
		using Bit = Pattern::BitIndex;
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G0);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G1);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G2);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G3);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G4);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G5);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G6);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::G7);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R0);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R1);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R2);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R3);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R4);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R5);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R6);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::R7);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::B0);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::B1);
		lc.addPatternToSequence(Pattern::Color::WHITE, Pattern::TriggerType::EXTERNAL_POSITIVE, 1, 3, Bit::B2);

		cout << lc.checkPatternSequence() << endl;
		cout << lc.sendPatternSequence() << endl;
		//cout << "Video Mode" << endl;
		//lc.setDisplayMode(DLPC350::DisplayMode::VIDEO);
		//cin >> t;
		//cout << "Set LED Enable" << endl;
		//lc.setLEDEnable(LightCrafter::DLPC350::LEDEnableMode::AUTO, true, true, true);
		//cin >> t;
		//cout << "Set LED 255, 0, 0" << endl;
		//lc.setLEDCurrent(255, 0, 0);
		//cin >> t;
		//cout << "Set LED 0, 255, 0" << endl;
		//lc.setLEDCurrent(0, 255, 0);
		//cin >> t;
		//cout << "Set LED 0, 0, 255" << endl;
		//lc.setLEDCurrent(0, 0, 255);
		//cin >> t;
		//cout << "Set LED 255, 255, 255" << endl;
		//lc.setLEDCurrent(255, 255, 255);
		//cin >> t;
		//cout << "Set Input source as Test Pattern" << endl;
		//lc.setInputSource(DLPC350::InputType::TEST_PATTERN);
		//cin >> t;
		//for (int i = 0; i <= 10; i++) {
		//	cout << "Set Input as Test Pattern " << i << endl;
		//	lc.setTestPattern(static_cast<DLPC350::TestPattern>(i));
		//	cin >> t;
		//}
		//cout << "Set Input source as Flash Image" << endl;
		//lc.setInputSource(DLPC350::InputType::FLASH);
		//cin >> t;
		//for (uint8_t i = 0; i < *lc.getNumImagesInFlash(); i++) {
		//	cout << "Set Input as Flash Image " << static_cast<uint16_t>(i) << endl;
		//	lc.setFlashImage(i);
		//	cin >> t;
		//}
	}
	cout << "Disconnect" << endl;
	lc.disconnect();

	return 0;
}
