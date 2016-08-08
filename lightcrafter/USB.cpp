#include "USB.h"

namespace LightCrafter {
	hid_device* USB::device = nullptr;
	uint8_t USB::readBuffer[BUFFER_SIZE] = { 0, };
	uint8_t USB::writeBuffer[BUFFER_SIZE] = { 0, };

	inline bool USB::isConnected() {
		return (device != nullptr);
	}

	inline bool USB::initialize() {
		return (hid_init() == 0);
	}

	inline bool USB::exit() {
		return (hid_exit() == 0);
	}

	inline bool USB::open() {
		device = hid_open(VENDOR_ID, PRODUCT_ID, nullptr);
		if (device == nullptr) return false;
		return true;
	}

	inline bool USB::close() {
		hid_close(device);
		device = nullptr;
		return true;
	}

	inline USB::Transaction* USB::read() {
		if (device == nullptr) return nullptr;

		memset(readBuffer, 0, sizeof(uint8_t) * BUFFER_SIZE);
		int32_t readBytes = hid_read_timeout(device, readBuffer, BUFFER_SIZE, TIMEOUT);
		
		if (readBytes == -1) {
			USB::close();
			return nullptr;
		}

		return reinterpret_cast<USB::Transaction*>(readBuffer);
	}

	inline int16_t USB::write(USB::Transaction &tran, uint32_t offset) {

		if (device == nullptr) return -1;

		int16_t maxDataSize = MAX_PACKET_SIZE - sizeof(tran.head);
		int16_t totalWrittenBytes = 0;
		int16_t writtenBytes = std::min(static_cast<int16_t>(tran.head.length), maxDataSize);

		memset(writeBuffer, 0, sizeof(uint8_t) * BUFFER_SIZE);
		memcpy(writeBuffer + 1, &tran, sizeof(tran.head) + writtenBytes);
		
		if (hid_write(device, writeBuffer, BUFFER_SIZE) == -1) {
			USB::close();
			return -1;
		}

		totalWrittenBytes += writtenBytes;

		while (totalWrittenBytes < tran.head.length) {
			memset(writeBuffer, 0, sizeof(uint8_t) * BUFFER_SIZE);
			writtenBytes = std::min(MAX_PACKET_SIZE, static_cast<int16_t>(tran.head.length) - totalWrittenBytes);
			memcpy(writeBuffer + 1, &tran.data.raw[totalWrittenBytes], writtenBytes);
			if (hid_write(device, writeBuffer, BUFFER_SIZE) == -1) {
				USB::close();
				return -1;
			}
			totalWrittenBytes += writtenBytes;
		}

		return writtenBytes + sizeof(tran.head);
	}
};

