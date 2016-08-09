#ifndef _LC4500_DLPC350_H_
#define _LC4500_DLPC350_H_

#include "Transaction.hpp"
#include "PatternSequence.hpp"

#include <string>
#include <cstdint>
#include <memory>
#include <cassert>

namespace LC4500 {
	namespace DLPC350 {
		enum class LEDEnableMode : bool {
			AUTO = true,
			MANUAL = false
		};

		enum class PowerMode : bool {
			STANDBY = true, // Standby Mode
			NORMAL = false // Normal Operation
		};

		enum class DisplayMode : bool {
			PATTERN = true, // Pattern Display
			VIDEO = false // Video Display
		};

		enum class PatternDisplayMode : uint8_t {
			EXTERNAL = 0,  // Video port(RGB / FPD-Link)
			RESERVED1 = 1,
			RESERVED2 = 2,
			INTERNAL = 3  // Internal flash
		};

		enum class PatternTriggerMode : uint8_t {
			MODE0 = 0, // Pattern Trigger Mode 0: VSYNC serves to trigger the pattern display sequence.
			MODE1 = 1, // Pattern Trigger Mode 1: Internally or Externally (through TRIG_IN1 and TRIG_IN2) generated trigger.
			MODE2 = 2, // Pattern Trigger Mode 2: TRIG_IN_1 alternates between two patterns,while TRIG_IN_2 advances to the next pair of patterns.
			MODE3 = 3, // Pattern Trigger Mode 3: Internally or externally generated trigger for Variable Exposure display sequence.
			MODE4 = 4  // Pattern Trigger Mode 4: VSYNC triggered for Variable Exposure display sequence.
		};

		enum class PatternSequenceStatus : uint8_t {
			STOP = 0,
			PAUSE = 1,
			START = 2
		};

		enum class InputType : uint8_t {
			PARALLEL = 0,
			TEST_PATTERN = 1,
			FLASH = 2,
			FPDLINK = 3
		};

		enum class InputBitDepth : uint8_t {
			INTERNAL = 0,
			BITS30 = 0,
			BITS24 = 1,
			BITS20 = 2,
			BITS16 = 3,
			BITS10 = 4,
			BITS8 = 5
		};

		enum class TestPattern : uint8_t {
			SOLID_FIELD = 0,
			HORIZONTAL_RAMP = 1,
			VERTICAL_RAMP = 2,
			HORIZONTAL_LINES = 3,
			DIAGONAL_LINES = 4,
			VERTICAL_LINES = 5,
			GRID = 6,
			CHECKERBOARD = 7,
			RGB_RAMP = 8,
			COLOR_BARS = 9,
			STEP_BARS = 10
		};

		union HardwareStatus {
			uint8_t value;
			struct {
				bool initDone : 1; // 0 : error
				uint8_t : 1; // reserved
				bool DRCError : 1; // 1 : error
				bool forcedSwap : 1; // 1 : error
				uint8_t : 2; // reserved
				bool sequenceAbort : 1; // 1 : error
				bool sequenceError : 1; // 1 :: error
			};
			HardwareStatus() : value{ 0 } {}
		};

		union SystemStatus {
			uint8_t value;
			struct {
				bool memoryTest : 1; // 0 : error
				uint8_t : 7; // reserved
			};
			SystemStatus() : value{ 0 } {}
		};

		union MainStatus {
			uint8_t value;
			struct {
				bool DMDParked : 1; // 1 : parked
				bool sequenceRunning : 1; // 1 : running
				bool bufferFrozen : 1; // 1 : frozen
				bool gammaCorrection : 1; // 1 : enabled
				uint8_t : 4;
			};
			MainStatus() : value{ 0 } {}
		};

		struct Version {
			union {
				uint32_t value;
				struct {
					uint16_t patch : 16;
					uint8_t minor : 8;
					uint8_t major : 8;
				};
			} app, api, softwareConfig, sequenceConfig;
			Version() {}
			Version(uint32_t _app, uint32_t _api, uint32_t _sw, uint32_t _seq) : app{ _app }, api{ _api }, softwareConfig{ _sw }, sequenceConfig{ _seq } {}
		};

		union LEDEnable {
			uint8_t value;
			struct {
				bool red : 1;
				bool green : 1;
				bool blue : 1;
				LEDEnableMode mode : 1;
				uint8_t : 4;
			};
			LEDEnable() : value{ 0 } {}
			LEDEnable(uint8_t _value) : value(_value) {}
			LEDEnable(LEDEnableMode _mode, bool _red, bool _green, bool _blue) : mode(_mode), red(_red), green(_green), blue(_blue) {}
		};

		union LEDCurrent {
			uint32_t value;
			struct {
				uint8_t red;
				uint8_t green;
				uint8_t blue;
			};
			LEDCurrent() : value{ 0 } {}
			LEDCurrent(uint32_t _value) : value(_value) {}
			LEDCurrent(uint8_t _red, uint8_t _green, uint8_t _blue) : red(_red), green(_green), blue(_blue) {}
		};

		union InputSource {
			uint8_t value;
			struct {
				InputType type : 3;
				InputBitDepth bitDepth : 3;
				uint8_t : 2;
			};
			InputSource() : value{ 0 } {}
			InputSource(uint8_t _value) : value(_value) {}
			InputSource(InputType _type, InputBitDepth _bitDepth) : type(_type), bitDepth(_bitDepth) {}
		};

		union PatternPeriod {
			struct {
				uint16_t exposure : 16;
				uint16_t frame : 16;
			};
			uint32_t value;
			PatternPeriod(uint32_t _value) : value(_value) {}
			PatternPeriod(uint16_t _exposure, uint16_t _frame) : exposure(_exposure), frame(_frame) {}
		};

		union Validation {
			uint8_t value;
			struct {
				bool invalidPeriod : 1;
				bool invalidPattern : 1;
				bool overlapTriggerOut1 : 1;
				bool missingBlackVector : 1;
				bool invalidPeriodDifference : 1;
				uint8_t : 3;
			};
			Validation() : value{ 0 } {}
			Validation(uint8_t _value) : value(_value) {}
			inline bool isValid() { return (value & ((1 << 5) - 1)) == 0; }
		};

		extern std::unique_ptr<HardwareStatus> getHardwareStatus();
		extern std::unique_ptr<SystemStatus> getSystemStatus();
		extern std::unique_ptr<MainStatus> getMainStatus();
		extern std::unique_ptr<Version> getVersion();
		extern std::unique_ptr<std::string> getFirmwareTag();

		extern std::unique_ptr<uint8_t> getNumImagesInFlash();

		extern std::unique_ptr<PowerMode> getPowerMode();
		extern bool setPowerMode(PowerMode mode);

		extern std::unique_ptr<DisplayMode> getDisplayMode();
		extern bool setDisplayMode(DisplayMode mode);

		extern std::unique_ptr<PatternTriggerMode> getPatternTriggerMode();
		extern bool setPatternTriggerMode(PatternTriggerMode mode);

		extern std::unique_ptr<PatternDisplayMode> getPatternDisplayMode();
		extern bool setPatternDisplayMode(PatternDisplayMode mode);

		extern std::unique_ptr<PatternSequenceStatus> getPatternSequenceStatus();
		extern bool setPatternSequenceStatus(PatternSequenceStatus mode);

		extern std::unique_ptr<LEDEnable> getLEDEnable();
		extern bool setLEDEnable(LEDEnableMode mode, bool redEnabled = true, bool greenEnabled = true, bool blueEnabled = true);

		extern std::unique_ptr<LEDCurrent> getLEDCurrent();
		extern bool setLEDCurrent(uint8_t red, uint8_t green, uint8_t blue);

		extern std::unique_ptr<InputSource> getInputSource();
		extern bool setInputSource(InputType type, InputBitDepth bitDepth = InputBitDepth::INTERNAL);

		extern std::unique_ptr<TestPattern> getTestPattern();
		extern bool setTestPattern(TestPattern pattern);

		extern std::unique_ptr<uint8_t> getFlashImage();
		extern bool setFlashImage(uint8_t index);

		extern std::unique_ptr<PatternPeriod> getPatternPeriod();
		extern bool setPatternPeriod(uint32_t exposure, uint32_t frame);

		extern bool openMailbox(uint8_t index);
		extern bool closeMailbox();
		extern bool setAddressInMailbox(uint8_t address);

		extern bool configurePatternSequence(PatternSequence &patternSequence, bool repeat = true, uint8_t triggerOut2PulsePerPattern = 1);
		extern bool sendPatternDisplayLUT(PatternSequence &patternSequence);
		extern bool sendPatternImageLUT(PatternSequence &patternSequence);

		extern std::unique_ptr<Validation> validatePatternSequence();
	};
};

#endif