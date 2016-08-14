#include "DLPC350.hpp"

namespace LC4500 {
	namespace DLPC350 {
		/**
		* getHardwareStatus
		* CMD2 : 0x1A, CMD3 : 0x0A
		*/
		std::unique_ptr<HardwareStatus> getHardwareStatus() {
			auto result = transactForGetValues<HardwareStatus>(0x1A0A);
			return std::unique_ptr<HardwareStatus>(new HardwareStatus(*result.get()));
		}

		/**
		* getSystemStatus
		* CMD2 : 0x1A, CMD3 : 0x0B
		*/
		std::unique_ptr<SystemStatus> getSystemStatus() {
			auto result = transactForGetValues<SystemStatus>(0x1A0B);
			return std::unique_ptr<SystemStatus>(new SystemStatus(*result.get()));
		}

		/**
		* getMainStatus
		* CMD2 : 0x1A, CMD3 : 0x0C
		*/
		std::unique_ptr<MainStatus> getMainStatus() {
			auto result = transactForGetValues<MainStatus>(0x1A0C);
			return std::unique_ptr<MainStatus>(new MainStatus(*result.get()));
		}

		/**
		* getVersion
		* CMD2 : 0x02, CMD3 : 0x05
		*/
		std::unique_ptr<Version> getVersion() {
			auto result = transactForGetValues<uint32_t>(0x0205);
			return std::unique_ptr<Version>(new Version(*(result.get()), *(result.get() + 1), *(result.get() + 2), *(result.get() + 3)));
		}

		/**
		* getFirmwareTag
		* CMD2 : 0x1A, CMD3 : 0xFF
		*/
		std::unique_ptr<std::string> getFirmwareTag() {
			auto result = transactForGetValues<char>(0x1AFF);
			return std::unique_ptr<std::string>(new std::string(result.get()));
		}

		/**
		* getNumImagesInFlash
		* CMD2 : 0x1A, CMD3 : 0x42
		*/
		std::unique_ptr<uint8_t> getNumImagesInFlash() {
			auto result = transactForGetValues(0x1A42);
			return std::unique_ptr<uint8_t>(new uint8_t(*result.get()));
		}

		/**
		* getPowerMode
		* CMD2 : 0x02, CMD3 : 0x00
		*/
		std::unique_ptr<PowerMode> getPowerMode() {
			auto result = transactForGetValues<PowerMode>(0x0200);
			return std::unique_ptr<PowerMode>(new PowerMode(*result.get()));
		}

		/**
		* setPowerMode
		* CMD2 : 0x02, CMD3 : 0x00, Param : 1
		*/
		bool setPowerMode(PowerMode mode) {
			auto result = transactForSetValues(0x0200, mode);
			return (result != nullptr);
		}

		/**
		* getDisplayMode
		* CMD2 : 0x1A, CMD3 : 0x1B
		*/
		std::unique_ptr<DisplayMode> getDisplayMode() {
			auto result = transactForGetValues<DisplayMode>(0x1A1B);
			return std::unique_ptr<DisplayMode>(new DisplayMode(*result.get()));
		}

		/**
		* setDisplayMode
		* CMD2 : 0x1A, CMD3 : 0x1B, Param : 1
		*/
		bool setDisplayMode(DisplayMode mode) {
			auto displayMode = getDisplayMode();

			if (*displayMode == DisplayMode::PATTERN) {
				auto patternSequenceStatus = getPatternSequenceStatus();
				if (*patternSequenceStatus != PatternSequenceStatus::STOP) {
					setPatternSequenceStatus(PatternSequenceStatus::STOP);
				}
			}

			if (*displayMode != mode) {				
				auto result = transactForSetValues<uint8_t>(0x1A1B, static_cast<uint8_t>(mode));
				return (result != nullptr);
			}

			return true;
		}

		/**
		* getPatternDisplayMode
		* CMD2 : 0x1A, CMD3 : 0x22
		*/
		std::unique_ptr<PatternDisplayMode> getPatternDisplayMode() {
			auto result = transactForGetValues<PatternDisplayMode>(0x1A22);
			return std::unique_ptr<PatternDisplayMode>(new PatternDisplayMode(*result.get()));
		}

		/**
		* setPatternDisplayMode
		* CMD2 : 0x1A, CMD3 : 0x22, Param : 1
		*/
		bool setPatternDisplayMode(PatternDisplayMode mode) {
			auto result = transactForSetValues<uint8_t>(0x1A22, static_cast<uint8_t>(mode));
			return (result != nullptr);
		}

		/**
		* getPatternTriggerMode
		* CMD2 : 0x1A, CMD3 : 0x23
		*/
		std::unique_ptr<PatternTriggerMode> getPatternTriggerMode() {
			auto result = transactForGetValues<PatternTriggerMode>(0x1A23);
			return std::unique_ptr<PatternTriggerMode>(new PatternTriggerMode(*result.get()));
		}

		/**
		* setPatternTriggerMode
		* CMD2 : 0x1A, CMD3 : 0x23, Param : 1
		*/
		bool setPatternTriggerMode(PatternTriggerMode mode) {
			auto result = transactForSetValues<uint8_t>(0x1A23, static_cast<uint8_t>(mode));
			return (result != nullptr);
		}

		/**
		* getPatternSequenceStatus
		* CMD2 : 0x1A, CMD3 : 0x23
		*/
		std::unique_ptr<PatternSequenceStatus> getPatternSequenceStatus() {
			auto result = transactForGetValues<PatternSequenceStatus>(0x1A24);
			return std::unique_ptr<PatternSequenceStatus>(new PatternSequenceStatus(*result.get()));
		}

		/**
		* setPatternSequenceStatus
		* CMD2 : 0x1A, CMD3 : 0x23, Param : 1
		*/
		bool setPatternSequenceStatus(PatternSequenceStatus mode) {
			auto result = transactForSetValues<uint8_t>(0x1A24, static_cast<uint8_t>(mode));
			return (result != nullptr);
		}

		/**
		* getLEDEnable
		* CMD2 : 0x1A, CMD3 : 0x07
		*/
		using LEDEnable = LEDEnable;
		std::unique_ptr<LEDEnable> getLEDEnable() {
			auto result = transactForGetValues<LEDEnable>(0x1A07);
			return std::unique_ptr<LEDEnable>(new LEDEnable(*result.get()));
		}

		/**
		* setLEDEnable
		* CMD2 : 0x1A, CMD3 : 0x07, Param : 1
		*/
		bool setLEDEnable(LEDEnableMode mode, bool redEnabled, bool greenEnabled, bool blueEnabled) {
			auto result = transactForSetValues<uint8_t>(0x1A07, LEDEnable(mode, redEnabled, greenEnabled, blueEnabled).value);
			return (result != nullptr);
		}

		/**
		* getLEDCurrent
		* CMD2 : 0x0B, CMD3 : 0x01
		*/
		using LEDCurrent = LEDCurrent;
		std::unique_ptr<LEDCurrent> getLEDCurrent() {
			auto result = transactForGetValues<uint32_t>(0x0B01);
			return std::unique_ptr<LEDCurrent>(new LEDCurrent(*result.get()));
		}

		/**
		* setLEDCurrent
		* CMD2 : 0x0B, CMD3 : 0x01, Param : 3
		*/
		bool setLEDCurrent(uint8_t red, uint8_t green, uint8_t blue) {
			auto result = transactForSetValues<uint8_t, uint8_t, uint8_t>(0x0B01, 255 - red, 255 - green,	255 - blue);
			return (result != nullptr);
		}

		/**
		* getInputSource
		* CMD2 : 0x1A, CMD3 : 0x00
		*/
		std::unique_ptr<InputSource> getInputSource() {
			auto result = transactForGetValues<InputSource>(0x1A00);
			return std::unique_ptr<InputSource>(new InputSource(*result.get()));
		}

		/**
		* setInputSource
		* CMD2 : 0x1A, CMD3 : 0x00, Param : 1
		*/
		bool setInputSource(InputType type, InputBitDepth bitDepth) {
			auto result = transactForSetValues<uint8_t>(0x1A00, InputSource(type, bitDepth).value);
			return (result != nullptr);
		}

		/**
		* getTestPattern
		* CMD2 : 0x12, CMD3 : 0x03
		*/
		std::unique_ptr<TestPattern> getTestPattern() {
			assert(getInputSource()->type == InputType::TEST_PATTERN);
			auto result = transactForGetValues<TestPattern>(0x1203);
			return std::unique_ptr<TestPattern>(new TestPattern(*result.get()));
		}

		/**
		* setTestPattern
		* CMD2 : 0x12, CMD3 : 0x03, Param : 1
		*/
		bool setTestPattern(TestPattern pattern) {
			assert(getInputSource()->type == InputType::TEST_PATTERN);
			auto result = transactForSetValues<uint8_t>(0x1203, static_cast<uint8_t>(pattern));
			return (result != nullptr);
		}

		/**
		* getFlashImage
		* CMD2 : 0x1A, CMD3 : 0x39
		*/
		std::unique_ptr<uint8_t> getFlashImage() {
			assert(getInputSource()->type == InputType::FLASH);
			auto result = transactForGetValues(0x1A39);
			return std::unique_ptr<uint8_t>(new uint8_t(*result.get()));
		}

		/**
		* setFlashImage
		* CMD2 : 0x1A, CMD3 : 0x39, Param : 1
		*/
		bool setFlashImage(uint8_t index) {
			assert(getInputSource()->type == InputType::FLASH);
			assert(0 <= index && index < *getNumImagesInFlash());

			auto result = transactForSetValues<uint8_t>(0x1A39, std::forward<uint8_t>(index));
			return (result != nullptr);
		}

		/**
		* getPatternPeriod
		* CMD2 : 0x1A, CMD3 : 0x29
		*/
		std::unique_ptr<PatternPeriod> getPatternPeriod() {
			auto result = transactForGetValues<uint16_t>(0x1A29);
			return std::unique_ptr<PatternPeriod>(new PatternPeriod(*result.get(), *(result.get() + 1)));
		}

		/**
		* setPatternPeriod
		* CMD2 : 0x1A, CMD3 : 0x29, Param : 1
		*/
		bool setPatternPeriod(uint32_t exposure, uint32_t frame) {
			assert(exposure <= frame);
			assert(frame - exposure > 230);

			auto result = transactForSetValues<uint32_t, uint32_t>(0x1A29, std::forward<uint32_t>(exposure), std::forward<uint32_t>(frame));
			uint8_t *ptr = result.get();
			return (ptr != nullptr);
		}

		//bool checkPatternSequence() {
		//	assert(patternSequence.sizePattern() > 0);

		//	size_t size = patternSequence.size();

		//	uint16_t minimumExposurebyBitDepth[8] = { 235, 700, 1570, 1700, 2000, 2500, 4500, 8333 };
		//	uint16_t numExposuredPattern = 1;
		//	uint8_t maxBitDepth = 1;

		//	auto patternPeriod = getPatternPeriod();

		//	for (size_t i = 0; i < size; i++) {
		//		Pattern &pattern = patternSequence.get(i);

		//		if (pattern.data.triggerOutPrevious == false) {
		//			maxBitDepth = std::max(maxBitDepth, pattern.data.bitDepth);
		//			if (numExposuredPattern > 1) {
		//				assert(patternPeriod->exposure / numExposuredPattern >= minimumExposurebyBitDepth[maxBitDepth - 1]);
		//			}
		//			numExposuredPattern = 1;
		//			maxBitDepth = 0;
		//		}
		//		else {
		//			numExposuredPattern++;
		//			maxBitDepth = std::max(maxBitDepth, pattern.data.bitDepth);
		//		}
		//	}

		//	if (numExposuredPattern > 1) {
		//		assert(patternPeriod->exposure / numExposuredPattern >= minimumExposurebyBitDepth[maxBitDepth - 1]);
		//	}

		//	return true;
		//}

		bool sendPatternSequence(PatternSequence &patternSequence) {
			if (!configurePatternSequence(patternSequence)) return false;
			if (!sendPatternDisplayLUT(patternSequence)) return false;
			if (!sendPatternImageLUT(patternSequence)) return false;
			return true;
		}

		/**
		* configurePatternSequence
		* CMD2 : 0x1A, CMD3 : 0x31, Param : 4
		* @param
		*
		* @return
		*
		*/
		bool configurePatternSequence(PatternSequence &patternSequence, bool repeat, uint8_t triggerOut2PulsePerPattern) {
			if (repeat) {
				triggerOut2PulsePerPattern = static_cast<uint8_t>(patternSequence.sizePattern());
			}
			auto result = transactForSetValues<uint8_t, uint8_t, uint8_t, uint8_t>(0x1A31,
				static_cast<uint8_t>(patternSequence.sizePattern() - 1),
				static_cast<uint8_t>(repeat),
				static_cast<uint8_t>(triggerOut2PulsePerPattern - 1),
				static_cast<uint8_t>(patternSequence.sizeImage() - 1));
			return (result != nullptr);
		}

		/**
		* sendPatternDisplayLUT
		* CMD2 : 0x1A, CMD3 : 0x34
		* @param
		*
		* @return
		*
		*/
		bool sendPatternDisplayLUT(PatternSequence &patternSequence) {
			if (!openMailbox(2)) return false;

			setAddressInMailbox(0);

			auto send = Transaction(Transaction::Type::WRITE, 0x1A34);

			for (size_t i = 0; i < patternSequence.sizePattern(); i++) {
				Pattern& pattern = patternSequence.getPattern(i);
				uint8_t *value = reinterpret_cast<uint8_t *>(&pattern.data.value);
				for (size_t j = 0; j < 3; j++) {
					send.data[send.length++] = *(value++);
				}
			}

			auto result = transact(send);

			closeMailbox();

			return (result != nullptr);
		}

		/**
		* sendPatternImageLUT
		* CMD2 : 0x1A, CMD3 : 0x34
		* @param
		*
		* @return
		*
		*/
		bool sendPatternImageLUT(PatternSequence &patternSequence) {
			if (!openMailbox(1)) return false;
			setAddressInMailbox(0);

			auto send = Transaction(Transaction::Type::WRITE, 0x1A34);

			for (size_t i = 0; i < patternSequence.sizeImage(); i++) {
				send.data[send.length++] = patternSequence.getImage(i);
			}

			if (send.length == 4) {
				std::swap(send.data[2], send.data[3]);
			}

			auto result = transact(send);

			closeMailbox();

			return (result != nullptr);
		}

		/**
		* validatePatternSequence
		* CMD2 : 0x1A, CMD3 : 0x1A
		*/
		std::unique_ptr<PatternSequenceValidation> validatePatternSequence() {
			auto result = transactForGetValues<PatternSequenceValidation>(0x1A1A);
			return std::unique_ptr<PatternSequenceValidation>(new PatternSequenceValidation(*result.get()));
		}

		/**
		* openMailbox
		* CMD2 : 0x1A, CMD3 : 0x33
		*/
		bool openMailbox(uint8_t index) {
			auto result = transactForSetValues<uint8_t>(0x1A33, std::forward<uint8_t>(index));
			return (result != nullptr);
		}

		/**
		* closeMailBox
		* CMD2 : 0x1A, CMD3 : 0x33
		*/
		bool closeMailbox() {
			auto result = transactForSetValues(0x1A33);
			return (result != nullptr);
		}

		/**
		* setAddressInMailbox
		* CMD2 : 0x1A, CMD3 : 0x32
		*/
		bool setAddressInMailbox(uint8_t address) {
			assert(address <= 127);

			auto result = transactForSetValues<uint8_t>(0x1A32, std::forward<uint8_t>(address));
			return (result != nullptr);
		}
	};
};