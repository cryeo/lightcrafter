#ifndef _LIGHTCRAFTER_DLPC350_H_

#define _LIGHTCRAFTER_DLPC350_H_

#include "USB.h"

#include <string>
#include <cstdint>
#include <memory>
#include <cassert>

#define MASK_BITS(N) ((1 << N) - 1)

#define MAX_PATTERN_SEQUENCES 128

namespace LightCrafter {
	class DLPC350 {
	public:
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
		};
		
		union SystemStatus {
			uint8_t value;
			struct {
				bool memoryTest : 1; // 0 : error
				uint8_t : 7; // reserved
			};
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

		struct Pattern {
			enum class Color : uint8_t {
				PASS = 0,
				RED = 1,
				GREEN = 2,
				YELLOW = 3,
				BLUE = 4,
				MAGENTA = 5,
				CYAN = 6,
				WHITE = 7
			};

			enum class TriggerType : uint8_t {
				INTERNAL = 0,
				EXTERNAL_POSITIVE = 1,
				EXTERNAL_NEGATIVE = 2,
				NO_TRIGGER = 3
			};

			enum class BitIndex : uint8_t {
				G0 = 0,
				G1 = 1,
				G2 = 2,
				G3 = 3,
				G4 = 4,
				G5 = 5,
				G6 = 6,
				G7 = 7,
				R0 = 8,
				R1 = 9,
				R2 = 10,
				R3 = 11,
				R4 = 12,
				R5 = 13,
				R6 = 14,
				R7 = 15,
				B0 = 16,
				B1 = 17,
				B2 = 18,
				B3 = 19,
				B4 = 20,
				B5 = 21,
				B6 = 22,
				B7 = 23
			};

			struct PatternData {
				union {
					uint32_t value;
					struct {
						TriggerType triggerType : 2;
						uint8_t patternNumber : 6;
						uint8_t bitDepth : 4;
						Color color : 4;
						bool invertPattern : 1;
						bool insertBlack : 1;
						bool bufferSwap : 1;
						bool triggerOutPrevious : 1;
						uint8_t : 4;
					};
				};
				PatternData() : value(0) {}
				PatternData(Color _color,
					TriggerType _triggerType,
					uint8_t _bitDepth,
					uint8_t _patternNumber,
					bool _invertPattern,
					bool _insertBlack,
					bool _bufferSwap,
					bool _triggerOutPrevious) :
					color(_color),
					triggerType(_triggerType),
					bitDepth(_bitDepth),
					patternNumber(_patternNumber),
					invertPattern(_invertPattern),
					insertBlack(_insertBlack),
					bufferSwap(_bufferSwap),
					triggerOutPrevious(_triggerOutPrevious) {}

			} data;
			uint8_t imageIndex;

			Pattern() {}
			Pattern(Color _color,
				TriggerType _triggerType,
				uint8_t _bitDepth,
				uint8_t _imageIndex,
				uint8_t _patternNumber,
				bool _invertPattern,
				bool _insertBlack,
				bool _bufferSwap,
				bool _triggerOutPrevious) :
				data(_color, _triggerType, _bitDepth, _patternNumber, 
					 _invertPattern, _insertBlack, _bufferSwap, 
					 _triggerOutPrevious),
				imageIndex(_imageIndex) {}
		};

		class PatternSequence {
		public:
			PatternSequence() : _sizePattern(0), _sizeImage(0), image{ 0 } {}

			void clear() {
				_sizePattern = 0;
				_sizeImage = 0;
			}

			void addPattern(Pattern& pat) {
				pattern[_sizePattern++] = pat;
				if (pat.data.bufferSwap) image[_sizeImage++] = pat.imageIndex;
			}

			inline size_t size() {
				return _sizePattern;
			}

			inline size_t sizeImage() {
				return _sizeImage;
			}

			inline Pattern& last() {
				assert(_sizePattern > 0);
				return pattern[_sizePattern - 1];
			}

			inline Pattern& get(size_t index) {
				return pattern[index];
			}

			inline uint8_t getImage(size_t index) {
				return image[index];
			}

		private:
			size_t _sizePattern, _sizeImage;
			Pattern pattern[MAX_PATTERN_SEQUENCES];
			uint8_t image[MAX_PACKET_SIZE];
		};

		DLPC350();
		virtual ~DLPC350();

		bool connect();
		bool disconnect();

		std::shared_ptr<HardwareStatus> getHardwareStatus();
		std::shared_ptr<SystemStatus> getSystemStatus();
		std::shared_ptr<MainStatus> getMainStatus();
		std::shared_ptr<Version> getVersion();
		std::shared_ptr<std::string> getFirmwareTag();

		std::shared_ptr<uint8_t> getNumImagesInFlash();
		
		
		std::shared_ptr<PowerMode> getPowerMode();
		bool setPowerMode(PowerMode mode);
		
		std::shared_ptr<DisplayMode> getDisplayMode();
		bool setDisplayMode(DisplayMode mode);

		std::shared_ptr<PatternTriggerMode> getPatternTriggerMode();
		bool setPatternTriggerMode(PatternTriggerMode mode);

		std::shared_ptr<PatternDisplayMode> getPatternDisplayMode();
		bool setPatternDisplayMode(PatternDisplayMode mode);

		std::shared_ptr<PatternSequenceStatus> getPatternSequenceStatus();
		bool setPatternSequenceStatus(PatternSequenceStatus mode);

		std::shared_ptr<LEDEnable> getLEDEnable();
		bool setLEDEnable(LEDEnableMode mode, bool redEnabled = true, bool greenEnabled = true, bool blueEnabled = true);

		std::shared_ptr<LEDCurrent> getLEDCurrent();
		bool setLEDCurrent(uint8_t red, uint8_t green, uint8_t blue);

		std::shared_ptr<InputSource> getInputSource();
		bool setInputSource(InputType type, InputBitDepth bitDepth = InputBitDepth::INTERNAL);

		std::shared_ptr<TestPattern> getTestPattern();
		bool setTestPattern(TestPattern pattern);

		std::shared_ptr<uint8_t> getFlashImage();
		bool setFlashImage(uint8_t index);

		std::shared_ptr<PatternPeriod> getPatternPeriod();
		bool setPatternPeriod(uint16_t exposure, uint16_t frame);

		bool openMailbox(uint8_t index);
		bool closeMailbox();
		bool setAddressInMailbox(uint8_t address);

		bool addPatternToSequence(
			Pattern::Color color,
			Pattern::TriggerType triggerType,
			uint8_t bitDepth,
			uint8_t imageIndex,
			Pattern::BitIndex startBit,
			bool invertPattern = false,
			bool insertBlack = true);

		bool clearPatternSequence();
		bool configurePatternSequence(PatternSequence &patternSequence, bool repeat = true, uint8_t triggerOut2PulsePerPattern = 1);
		bool checkPatternSequence();
		bool sendPatternSequence();
		bool sendPatternSequence(PatternSequence &patternSequence);
		bool sendImageSequence(PatternSequence &patternSequence);


		union Validation {
			uint8_t value;
			struct {
				bool invalidPeriod : 1;
				bool invalidPattern : 1;
				bool overlapTriggerOut1: 1;
				bool missingBlackVector : 1;
				bool invalidPeriodDifference: 1;
				uint8_t : 3;
			};
			Validation(uint8_t _value) : value(_value) {}
			inline bool isValid() { return (value & ((1 << 5) - 1)) == 0; }
		};

		std::shared_ptr<Validation> validatePatternSequence();

	private:
		std::shared_ptr<uint8_t> transact(USB::Transaction &tran);
		
		std::shared_ptr<uint8_t> transactForGet(uint8_t cmd2, uint8_t cmd3);
		
		template<typename... ParamList>
		std::shared_ptr<uint8_t> transactForSet(uint8_t cmd2, uint8_t cmd3, ParamList&&... params) {
			auto send = USB::transaction(TransactionType::WRITE, cmd2, cmd3, std::forward<ParamList>(params)...);
			auto result = transact(send);
			return result;
		}

		PatternSequence patternSequence;
	};
};

#endif


