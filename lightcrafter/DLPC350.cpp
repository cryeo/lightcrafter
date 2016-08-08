#include "DLPC350.h"

#include "Error.h"
#include "Util.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

namespace LightCrafter {
	using TransactionType = USB::TransactionType;

	DLPC350::DLPC350() {
		USB::initialize();
	}

	DLPC350::~DLPC350() {
		USB::exit();
	}

	bool DLPC350::connect() {
		return USB::open();
	}

	bool DLPC350::disconnect() {
		return USB::close();
	}

	/**
	* transact
	*
	* @param
	*
	* @return
	*
	*/
	std::shared_ptr<uint8_t> DLPC350::transact(USB::Transaction &tran) {
		uint16_t result = USB::write(tran);

		if (tran.head.flags.reply) {
			if (result <= 0) return nullptr;

			auto receivedTransaction = USB::read();

			if (receivedTransaction == nullptr) return nullptr;
			if (receivedTransaction->head.flags.error || 
				(receivedTransaction->head.flags.rw == USB::TransactionType::READ &&	receivedTransaction->head.length == 0)) return nullptr;

			std::shared_ptr<uint8_t> ret(new uint8_t[BUFFER_SIZE], std::default_delete<uint8_t[]>());
			memcpy(ret.get(), receivedTransaction->data.raw, BUFFER_SIZE);

			return ret;
		}
		else {
			return nullptr;
		}
	}

	std::shared_ptr<uint8_t> DLPC350::transactForGet(uint8_t cmd2, uint8_t cmd3) {
		auto send = USB::transaction(TransactionType::READ, cmd2, cmd3);
		return transact(send);
	}

	/**
	* getHardwareStatus
	* CMD2 : 0x1A, CMD3 : 0x0A
	* @param
	*
	* @return
	*
	*/
	using HardwareStatus = DLPC350::HardwareStatus;
	std::shared_ptr<HardwareStatus> DLPC350::getHardwareStatus() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x0A);
		uint8_t *ptr = result.get();
		return std::shared_ptr<HardwareStatus>(new HardwareStatus{ *ptr });
	}

	/**
	* getSystemStatus
	* CMD2 : 0x1A, CMD3 : 0x0B
	* @param
	*
	* @return
	*
	*/
	using SystemStatus = DLPC350::SystemStatus;
	std::shared_ptr<SystemStatus> DLPC350::getSystemStatus() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x0B);
		uint8_t *ptr = result.get();
		return std::shared_ptr<SystemStatus>(new SystemStatus{ *ptr });
	}

	/**
	* getMainStatus
	* CMD2 : 0x1A, CMD3 : 0x0C
	* @param
	*
	* @return
	*
	*/
	using MainStatus = DLPC350::MainStatus;
	std::shared_ptr<MainStatus> DLPC350::getMainStatus() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x0C);
		uint8_t *ptr = result.get();
		return std::shared_ptr<MainStatus>(new MainStatus{ *ptr });
	}

	/**
	* getVersion
	* CMD2 : 0x02, CMD3 : 0x05
	* @param
	*
	* @return
	*
	*/
	using Version = DLPC350::Version;
	std::shared_ptr<Version> DLPC350::getVersion() {
		std::shared_ptr<uint8_t> result = transactForGet(0x02, 0x05);
		uint32_t *ptr = reinterpret_cast<uint32_t *>(result.get());
		return std::shared_ptr<Version>(new Version{ *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3) });
	}

	/**
	* getFirmwareTag
	* CMD2 : 0x1A, CMD3 : 0xFF
	* @param
	*
	* @return
	*
	*/
	std::shared_ptr<std::string> DLPC350::getFirmwareTag() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0xFF);
		char *ptr = reinterpret_cast<char *>(result.get());
		return std::shared_ptr<std::string>(new std::string(ptr));
	}

	/**
	* getNumImagesInFlash
	* CMD2 : 0x1A, CMD3 : 0x42
	* @param
	*
	* @return
	*
	*/
	std::shared_ptr<uint8_t> DLPC350::getNumImagesInFlash() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x42);
		return result;;
	}

	/**
	* getPowerMode
	* CMD2 : 0x02, CMD3 : 0x00
	* @param
	*
	* @return
	*
	*/
	using PowerMode = DLPC350::PowerMode;
	std::shared_ptr<PowerMode> DLPC350::getPowerMode() {
		std::shared_ptr<uint8_t> result = transactForGet(0x02, 0x00);
		uint8_t *ptr = result.get();
		return std::shared_ptr<PowerMode>(new PowerMode(static_cast<PowerMode>(*ptr)));
	}

	/**
	* setPowerMode
	* CMD2 : 0x02, CMD3 : 0x00, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setPowerMode(PowerMode mode) {
		std::shared_ptr<uint8_t> result = transactForSet(0x02, 0x00, static_cast<uint8_t>(mode));
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getDisplayMode
	* CMD2 : 0x1A, CMD3 : 0x1B
	* @param
	*
	* @return
	*
	*/
	using DisplayMode = DLPC350::DisplayMode;
	std::shared_ptr<DisplayMode> DLPC350::getDisplayMode() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x1B);
		uint8_t *ptr = result.get();
		return std::shared_ptr<DisplayMode>(new DisplayMode(static_cast<DisplayMode>(*ptr)));
	}

	/**
	* setDisplayMode
	* CMD2 : 0x1A, CMD3 : 0x1B, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setDisplayMode(DisplayMode mode) {
		auto displayMode = getDisplayMode();

		if (*displayMode == DisplayMode::PATTERN) {
			auto patternSequenceStatus = getPatternSequenceStatus();
			if (*patternSequenceStatus != PatternSequenceStatus::STOP) {
				setPatternSequenceStatus(PatternSequenceStatus::STOP);
			}
		}

		if (*displayMode != mode) {
			std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x1B, static_cast<uint8_t>(mode));
			uint8_t *ptr = result.get();
			return (ptr != nullptr);
		}

		return true;
	}

	/**
	* getPatternDisplayMode
	* CMD2 : 0x1A, CMD3 : 0x22
	* @param
	*
	* @return
	*
	*/
	using PatternDisplayMode = DLPC350::PatternDisplayMode;
	std::shared_ptr<PatternDisplayMode> DLPC350::getPatternDisplayMode() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x22);
		uint8_t *ptr = result.get();
		return std::shared_ptr<PatternDisplayMode>(new PatternDisplayMode(static_cast<PatternDisplayMode>(*ptr)));
	}

	/**
	* setPatternDisplayMode
	* CMD2 : 0x1A, CMD3 : 0x22, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setPatternDisplayMode(PatternDisplayMode mode) {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x22, static_cast<uint8_t>(mode));
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getPatternTriggerMode
	* CMD2 : 0x1A, CMD3 : 0x23
	* @param
	*
	* @return
	*
	*/
	using PatternTriggerMode = DLPC350::PatternTriggerMode;
	std::shared_ptr<PatternTriggerMode> DLPC350::getPatternTriggerMode() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x23);
		uint8_t *ptr = result.get();
		return std::shared_ptr<PatternTriggerMode>(new PatternTriggerMode(static_cast<PatternTriggerMode>(*ptr)));
	}

	/**
	* setPatternTriggerMode
	* CMD2 : 0x1A, CMD3 : 0x23, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setPatternTriggerMode(PatternTriggerMode mode) {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x23, static_cast<uint8_t>(mode));
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getPatternSequenceStatus
	* CMD2 : 0x1A, CMD3 : 0x23
	* @param
	*
	* @return
	*
	*/
	using PatternSequenceStatus = DLPC350::PatternSequenceStatus;
	std::shared_ptr<PatternSequenceStatus> DLPC350::getPatternSequenceStatus() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x24);
		uint8_t *ptr = result.get();
		return std::shared_ptr<PatternSequenceStatus>(new PatternSequenceStatus(static_cast<PatternSequenceStatus>(*ptr)));
	}

	/**
	* setPatternSequenceStatus
	* CMD2 : 0x1A, CMD3 : 0x23, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setPatternSequenceStatus(PatternSequenceStatus mode) {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x24, static_cast<uint8_t>(mode));
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getLEDEnable
	* CMD2 : 0x1A, CMD3 : 0x07
	* @param
	*
	* @return
	*
	*/
	using LEDEnable = DLPC350::LEDEnable;
	std::shared_ptr<LEDEnable> DLPC350::getLEDEnable() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x07);
		uint8_t *ptr = result.get();
		return std::shared_ptr<LEDEnable>(new LEDEnable{ *ptr });
	}

	/**
	* setLEDEnable
	* CMD2 : 0x1A, CMD3 : 0x07, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setLEDEnable(LEDEnableMode mode, bool redEnabled, bool greenEnabled, bool blueEnabled) {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x07, LEDEnable(mode, redEnabled, greenEnabled, blueEnabled).value);
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getLEDCurrent
	* CMD2 : 0x0B, CMD3 : 0x01
	* @param
	*
	* @return
	*
	*/
	using LEDCurrent = DLPC350::LEDCurrent;
	std::shared_ptr<LEDCurrent> DLPC350::getLEDCurrent() {
		std::shared_ptr<uint8_t> result = transactForGet(0x0B, 0x01);
		uint32_t *ptr = reinterpret_cast<uint32_t *>(result.get());

		return std::shared_ptr<LEDCurrent>(new LEDCurrent(*ptr));
	}

	/**
	* setLEDCurrent
	* CMD2 : 0x0B, CMD3 : 0x01, Param : 3
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setLEDCurrent(uint8_t red, uint8_t green, uint8_t blue) {
		red = 255 - red;
		green = 255 - green;
		blue = 255 - blue;
		std::shared_ptr<uint8_t> result = transactForSet(0x0B, 0x01, red, green, blue);
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getInputSource
	* CMD2 : 0x1A, CMD3 : 0x00
	* @param
	*
	* @return
	*
	*/
	using InputSource = DLPC350::InputSource;
	std::shared_ptr<InputSource> DLPC350::getInputSource() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x00);
		uint8_t *ptr = result.get();

		return std::shared_ptr<InputSource>(new InputSource(*ptr));
	}

	/**
	* setInputSource
	* CMD2 : 0x1A, CMD3 : 0x00, Param : 1
	* @param
	*
	* @return
	*
	*/
	using InputType = DLPC350::InputType;
	using InputBitDepth = DLPC350::InputBitDepth;
	bool DLPC350::setInputSource(InputType type, InputBitDepth bitDepth) {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x00, InputSource(type, bitDepth).value);
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getTestPattern
	* CMD2 : 0x12, CMD3 : 0x03
	* @param
	*
	* @return
	*
	*/
	using TestPattern = DLPC350::TestPattern;
	std::shared_ptr<TestPattern> DLPC350::getTestPattern() {
		assert(getInputSource()->type == InputType::TEST_PATTERN);
		std::shared_ptr<uint8_t> result = transactForGet(0x12, 0x03);
		uint8_t *ptr = result.get();
		return std::shared_ptr<TestPattern>(new TestPattern(static_cast<TestPattern>(*ptr)));
	}

	/**
	* setTestPattern
	* CMD2 : 0x12, CMD3 : 0x03, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setTestPattern(TestPattern pattern) {
		assert(getInputSource()->type == InputType::TEST_PATTERN);
		std::shared_ptr<uint8_t> result = transactForSet(0x12, 0x03, static_cast<uint8_t>(pattern));
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getFlashImage
	* CMD2 : 0x1A, CMD3 : 0x39
	* @param
	*
	* @return
	*
	*/
	std::shared_ptr<uint8_t> DLPC350::getFlashImage() {
		assert(getInputSource()->type == InputType::FLASH);
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x39);
		return result;
	}

	/**
	* setFlashImage
	* CMD2 : 0x1A, CMD3 : 0x39, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setFlashImage(uint8_t index) {
		assert(getInputSource()->type == InputType::FLASH);
		assert(0 <= index && index < *getNumImagesInFlash());
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x39, index);
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* getPatternPeriod
	* CMD2 : 0x1A, CMD3 : 0x29
	* @param
	*
	* @return
	*
	*/
	using PatternPeriod = DLPC350::PatternPeriod;
	std::shared_ptr<PatternPeriod> DLPC350::getPatternPeriod() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x29);
		uint16_t *ptr = reinterpret_cast<uint16_t *>(result.get());

		return std::shared_ptr<PatternPeriod>(new PatternPeriod(*ptr, *(ptr + 1)));
	}

	/**
	* setPatternPeriod
	* CMD2 : 0x1A, CMD3 : 0x29, Param : 1
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setPatternPeriod(uint16_t exposure, uint16_t frame) {
		assert(exposure <= frame);
		assert(frame - exposure > 230);

		uint8_t *exposurePtr = reinterpret_cast<uint8_t *>(&exposure);
		uint8_t *framePtr = reinterpret_cast<uint8_t *>(&frame);

		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x29, *exposurePtr, *(exposurePtr + 1), *(exposurePtr + 2), *(exposurePtr + 3), *framePtr, *(framePtr + 1), *(framePtr + 2), *(framePtr + 3));
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	using Pattern = DLPC350::Pattern;
	bool DLPC350::addPatternToSequence(
		Pattern::Color color,
		Pattern::TriggerType triggerType,
		uint8_t bitDepth,
		uint8_t imageIndex,
		Pattern::BitIndex startBit,
		bool invertPattern,
		bool insertBlack) {

		uint8_t iStartBit = static_cast<uint8_t>(startBit);

		std::vector<Pattern::BitIndex> checks;
		if (bitDepth == 5) {
			checks = { Pattern::BitIndex::G0, Pattern::BitIndex::G6, Pattern::BitIndex::R4, Pattern::BitIndex::B2 };
		}
		else if (bitDepth == 7) {
			checks = { Pattern::BitIndex::G0, Pattern::BitIndex::R0, Pattern::BitIndex::B0 };
		}

		for (auto check : checks) {
			uint8_t bit = static_cast<uint8_t>(check);
			assert(iStartBit != bit);
		}

		bool bufferSwap;
		bool triggerOutPrevious = false;

		auto patternDisplayMode = getPatternDisplayMode();

		if (patternSequence.size() == 0) {
			if (*patternDisplayMode == DLPC350::PatternDisplayMode::INTERNAL) assert(triggerType != Pattern::TriggerType::NO_TRIGGER);
			bufferSwap = true;
		}
		else {
			uint8_t previousImageIndex = patternSequence.last().imageIndex;
			bufferSwap = (previousImageIndex != imageIndex);
		}

		if (*patternDisplayMode == DLPC350::PatternDisplayMode::EXTERNAL) {
			triggerType = bufferSwap ? Pattern::TriggerType::EXTERNAL_POSITIVE : Pattern::TriggerType::NO_TRIGGER;
		}

		uint8_t patternNumber = iStartBit / bitDepth;

		Pattern pattern(color, triggerType, bitDepth, imageIndex, patternNumber, invertPattern, insertBlack, bufferSwap, triggerOutPrevious);
		patternSequence.addPattern(pattern);

		return true;
	}

	bool DLPC350::clearPatternSequence() {
		patternSequence.clear();
		return true;
	}

	bool DLPC350::checkPatternSequence() {
		assert(patternSequence.size() > 0);

		size_t size = patternSequence.size();

		uint16_t minimumExposurebyBitDepth[8] = { 235, 700, 1570, 1700, 2000, 2500, 4500, 8333 };
		uint16_t numExposuredPattern = 1;
		uint8_t maxBitDepth = 1;

		size_t nSplashImages = 0;
		uint8_t splashImages[MAX_PACKET_SIZE] = { 0, };

		auto patternPeriod = getPatternPeriod();

		for (size_t i = 0; i < size; i++) {
			Pattern &pattern = patternSequence.get(i);

			if (pattern.data.triggerOutPrevious == false) {
				maxBitDepth = std::max(maxBitDepth, pattern.data.bitDepth);
				if (numExposuredPattern > 1) {
					assert(patternPeriod->exposure / numExposuredPattern >= minimumExposurebyBitDepth[maxBitDepth - 1]);
				}
				numExposuredPattern = 1;
				maxBitDepth = 0;
			}
			else {
				numExposuredPattern++;
				maxBitDepth = std::max(maxBitDepth, pattern.data.bitDepth);
			}

			if (pattern.data.bufferSwap) {
				splashImages[nSplashImages++] = pattern.imageIndex;
			}
		}

		if (numExposuredPattern > 1) {
			assert(patternPeriod->exposure / numExposuredPattern >= minimumExposurebyBitDepth[maxBitDepth - 1]);
		}

		return true;
	}

	bool DLPC350::sendPatternSequence() {
		if (!configurePatternSequence(patternSequence, false)) return false;
		if (!sendPatternSequence(patternSequence)) return false;
		if (!sendImageSequence(patternSequence)) return false;
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
	bool DLPC350::configurePatternSequence(PatternSequence &patternSequence, bool repeat, uint8_t triggerOut2PulsePerPattern) {
		std::shared_ptr<uint8_t> result;
		if (repeat) {
			result = transactForSet(0x1A, 0x31,
				static_cast<uint8_t>(patternSequence.size() - 1),
				static_cast<uint8_t>(repeat),
				static_cast<uint8_t>(triggerOut2PulsePerPattern - 1),
				static_cast<uint8_t>(patternSequence.sizeImage() - 1));
		}
		else {
			result = transactForSet(0x1A, 0x31,
				static_cast<uint8_t>(patternSequence.size() - 1),
				static_cast<uint8_t>(repeat),
				static_cast<uint8_t>(patternSequence.size() - 1),
				static_cast<uint8_t>(patternSequence.sizeImage() - 1));
		}
		uint8_t *ptr = result.get();
		return (ptr != nullptr);
	}

	/**
	* openMailbox
	* CMD2 : 0x1A, CMD3 : 0x33
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::openMailbox(uint8_t index) {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x33, index);
		return (result != nullptr);
	}

	/**
	* closeMailBox
	* CMD2 : 0x1A, CMD3 : 0x33
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::closeMailbox() {
		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x33);
		return (result != nullptr);
	}

	/**
	* setAddressInMailbox
	* CMD2 : 0x1A, CMD3 : 0x32
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::setAddressInMailbox(uint8_t address) {
		assert(address <= 127);

		std::shared_ptr<uint8_t> result = transactForSet(0x1A, 0x32, address);
		return (result != nullptr);
	}

	/**
	* sendPatternSequence
	* CMD2 : 0x1A, CMD3 : 0x34
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::sendPatternSequence(PatternSequence &patternSequence) {
		if (!openMailbox(2)) return false;

		setAddressInMailbox(0);

		auto send = USB::transaction(TransactionType::WRITE, 0x1A, 0x34);
		
		uint16_t *length = &send.head.length;
		uint8_t *param = &send.data.raw[2];
		
		for (size_t i = 0; i < patternSequence.size(); i++) {
			Pattern& pattern = patternSequence.get(i);
			uint8_t *value = reinterpret_cast<uint8_t *>(&pattern.data.value);
			*length += 3;
			*(param++) = *(value++);
			*(param++) = *(value++);
			*(param++) = *(value++);
		}

		auto result = transact(send);

		closeMailbox();

		return (result != nullptr);
	}

	/**
	* sendImageSequence
	* CMD2 : 0x1A, CMD3 : 0x34
	* @param
	*
	* @return
	*
	*/
	bool DLPC350::sendImageSequence(PatternSequence &patternSequence) {
		if (!openMailbox(1)) return false;

		setAddressInMailbox(0);

		auto send = USB::transaction(TransactionType::WRITE, 0x1A, 0x34);

		uint16_t *length = &send.head.length;
		uint8_t *param = &send.data.raw[2];

		for (size_t i = 0; i < patternSequence.sizeImage(); i++) {
			*length += 1;
			*(param++) = patternSequence.getImage(i);
		}

		auto result = transact(send);

		closeMailbox();

		return (result != nullptr);
	}

	/**
	* validatePatternSequence
	* CMD2 : 0x1A, CMD3 : 0x1A
	* @param
	*
	* @return
	*
	*/
	using Validation = DLPC350::Validation;
	std::shared_ptr<Validation> DLPC350::validatePatternSequence() {
		std::shared_ptr<uint8_t> result = transactForGet(0x1A, 0x1A);

		return std::shared_ptr<Validation>(new Validation(*result));
	}
};