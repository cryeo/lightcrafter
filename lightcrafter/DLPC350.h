#ifndef _DLPC350_H_

#define _DLPC350_H_

#include <string>

#define SUCCESS 0
#define FAIL -1

namespace LightCrafter {
	class DLPC350 {
	public:
		DLPC350();
		virtual ~DLPC350();
		bool connect();
	private:
		// Hardware Status Register
		union {
			struct {
  				bool initDone : 1; // 0 : error
  				bool : 1;
  				bool DRCError: 1; // 1 : error
  				bool forcedSwap : 1; // 1 : error
  				bool : 1;
  				bool : 1;
  				bool sequenceAbort : 1; // 1 : error
  				bool sequenceError : 1; // 1 :: error
			};
			unsigned char value;
			inline unsigned char* operator&() const { return const_cast<unsigned char*>(&value); }
		} HSR;

		// System Status Register 
		union {
			struct {
  				bool memoryTest : 1; // 0 : error
			};
			unsigned char value;
			inline unsigned char* operator&() const { return const_cast<unsigned char*>(&value); }
		} SSR;

		// Main Status Register
		union {
			struct {
  				bool DMDParked : 1; // 1 : parked
  				bool sequenceRunning: 1; // 1 : running
  				bool bufferFrozen : 1; // 1 : frozen
  				bool gammaCorrection : 1; // 1 : enabled
			};
			unsigned char value;
			inline unsigned char* operator&() const { return const_cast<unsigned char*>(&value); }
		} MSR;

		// Version
		struct {
			union {
				struct {
					unsigned int patch : 16;
					unsigned int minor : 8;
					unsigned int major : 8;
				};
				unsigned int value;
				inline unsigned int* operator&() const { return const_cast<unsigned int*>(&value); }
			} app, api, softwareConfig, sequenceConfig;
		} version;

		std::string firmwareTag;
		unsigned int numImages;
		enum : bool {
			STANDBY = true,
			NORMAL = false
		} powerMode;

		// 0 = Pattern Trigger Mode 0: VSYNC serves to trigger the pattern display sequence.
		// 1 = Pattern Trigger Mode 1: Internally or Externally (through TRIG_IN1 and TRIG_IN2) generated trigger.
		// 2 = Pattern Trigger Mode 2: TRIG_IN_1 alternates between two patterns,while TRIG_IN_2 advances to the next pair of patterns.
		// 3 = Pattern Trigger Mode 3: Internally or externally generated trigger for Variable Exposure display sequence.
		// 4 = Pattern Trigger Mode 4: VSYNC triggered for Variable Exposure display sequence.
		int patternTriggerMode;

		enum : bool {
			EXTERNAL = true, // Video port(RGB / FPD-Link)
			INTERNAL = false // Internal flash
		} patternDisplayMode;

		bool getStatus();
		bool getVersion();
		bool getFirmwareTag();

		bool getNumImagesInFlash();

		bool getPowerMode();
		bool getPatternTriggerMode();
		bool getPatternDisplayMode();

		//***** Display Mode
		enum DisplayMode : bool {
			PATTERN = true,
			VIDEO = false
		};
		DisplayMode displayMode;
		bool getDisplayMode();
		bool setDisplayMode(DisplayMode mode);
		bool _setDisplayMode(DisplayMode mode);

		//***** The state of Pattern Display Sequence
		enum PatternMode : unsigned int {
			STOP = 0,
			PAUSE = 1,
			START = 2
		};
		PatternMode patternMode;
		bool getPatternMode();
		bool setPatternMode(PatternMode mode);
		bool _setPatternMode(PatternMode mode);
		
		// FOR DEBUG
		void showStatus();
		void showVersion();
	};
};

#endif


