#include "DLPC350.h"
#include "USB.h"
#include "Error.h"
#include "Util.h"

#include "dlpc350_api.h"
#include "dlpc350_common.h"

#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

using namespace LightCrafter;

DLPC350::DLPC350() {
	LightCrafter::USB::initialize();
}

DLPC350::~DLPC350() {
	LightCrafter::USB::close();
	LightCrafter::USB::exit();
}

bool DLPC350::connect() {
	if (LightCrafter::USB::isConnected()) {
		if (!getStatus()) return false;
		showStatus();
		if (!getVersion()) return false;
		showVersion();
		if (!getFirmwareTag()) return false;
		std::cout << firmwareTag << std::endl;
		if (!getNumImagesInFlash()) return false;
		std::cout << "No. of Images : " << numImages << std::endl;

		if (getDisplayMode() && powerMode == NORMAL) { // Normal mode
			if (displayMode == PATTERN) { // Pattern sequence mode
				if (getPatternTriggerMode()) {
					if (!getPatternDisplayMode()) return false;

					if (patternDisplayMode == EXTERNAL) {
					}
					else {
					}

					if (patternTriggerMode <= 2) { // Pattern sequence mode
					}
					else { // Pattern sequence mode with variable exposure display sequence
					}
				}
			}
			else { // Video mode

			}
		}
		else { // Standby mode
		}

		return true;
	}
	return false;
}

bool DLPC350::getStatus() {
    unsigned char hw, sys, main;

    return (DLPC350_GetStatus(&HSR, &SSR, &MSR) == SUCCESS);
}

void DLPC350::showStatus() {
	using namespace LightCrafter::Error;

	std::printf("* Status\n");
	std::printf("** Hardware Status\n");
	std::printf("*** Internal Initialization : %s\n", HSR::initDone[static_cast<int>(HSR.initDone)]);
	std::printf("*** DMD Reset Controller Error : %s\n", HSR::DRCError[static_cast<int>(HSR.DRCError)]);
	std::printf("*** Forced Swap Error : %s\n", HSR::forcedSwap[static_cast<int>(HSR.forcedSwap)]);
	std::printf("*** Sequencer Abort Status Flag : %s\n", HSR::sequenceAbort[static_cast<int>(HSR.sequenceAbort)]);
	std::printf("*** Sequencer Error : %s\n", HSR::sequenceError[static_cast<int>(HSR.sequenceError)]);
	std::printf("** System Status\n");
	std::printf("*** Internal Memory Test : %s\n", SSR::memoryTest[static_cast<int>(SSR.memoryTest)]);
	std::printf("*** Main Status\n");
	std::printf("*** DMD Park Status : %s\n", MSR::DMDParked[static_cast<int>(MSR.DMDParked)]);
	std::printf("*** Sequencer Run Flag : %s\n", MSR::sequenceRunning[static_cast<int>(MSR.sequenceRunning)]);
	std::printf("*** Frame Buffer Swap Flag : %s\n", MSR::bufferFrozen[static_cast<int>(MSR.bufferFrozen)]);
	std::printf("*** Gamma Correction Function Enable : %s\n", MSR::gammaCorrection[static_cast<int>(MSR.gammaCorrection)]);
}


bool DLPC350::getVersion() {
	return (DLPC350_GetVersion(&version.app, &version.api, &version.softwareConfig, &version.sequenceConfig) == SUCCESS);
}

void DLPC350::showVersion() {
	std::printf("* Version\n");
	std::printf("** Application Software Revision : %d.%d.%d\n", version.app.major, version.app.minor, version.app.patch);
	std::printf("** API Software Revision : %d.%d.%d\n", version.api.major, version.api.minor, version.api.patch);
	std::printf("** Software Configuration Revision : %d.%d.%d\n", version.softwareConfig.major, version.softwareConfig.minor, version.softwareConfig.patch);
	std::printf("** Sequence Configuration Revision : %d.%d.%d\n", version.sequenceConfig.major, version.sequenceConfig.minor, version.sequenceConfig.patch);
}

bool DLPC350::getFirmwareTag() {
	unsigned char tag[33];
    if (DLPC350_GetFirmwareTagInfo(&tag[0]) == SUCCESS) {
		firmwareTag = std::string(reinterpret_cast<char *>(tag));
		return true;
	}
	return false;
}

bool DLPC350::getNumImagesInFlash() {
	return (DLPC350_GetNumImagesInFlash(&numImages) == SUCCESS);
}

bool DLPC350::getPowerMode() {
	return (DLPC350_GetPowerMode(reinterpret_cast<bool *>(&powerMode)) == SUCCESS);
}

bool DLPC350::getPatternTriggerMode() {
	return (DLPC350_GetPatternTriggerMode(&patternTriggerMode) == SUCCESS);
}

bool DLPC350::getPatternDisplayMode() {
	return (DLPC350_GetPatternDisplayMode(reinterpret_cast<bool *>(&patternDisplayMode)) == SUCCESS);
}

bool DLPC350::getDisplayMode() {
	return (DLPC350_GetMode(reinterpret_cast<bool *>(&displayMode)) == SUCCESS);
}

bool DLPC350::setDisplayMode(DisplayMode mode) {
	getDisplayMode();

	if (displayMode == PATTERN) {
		getPatternMode();
		if (patternMode != STOP) {
			setPatternMode(STOP);
		}
	}

	if (displayMode != mode) {
		int iteration = 0;

		if (DLPC350_SetMode(mode) != SUCCESS) return false;

		while (displayMode != mode) {
			getDisplayMode();
			Util::sleep(100);
			if (iteration++ > 5) return false;
		}
	}

	return true;
}

bool DLPC350::getPatternMode() {
	return (DLPC350_GetPatternDisplay(reinterpret_cast<unsigned int*>(&patternMode)) == SUCCESS);
}

bool DLPC350::setPatternMode(PatternMode mode) {
	if (DLPC350_PatternDisplay(mode) != SUCCESS) return false;

	int iteration = 0;
	while (patternMode != mode) {
		getPatternMode();
		Util::sleep(100);
		if (iteration++ > 5) return false;
	}
	return true;
}
