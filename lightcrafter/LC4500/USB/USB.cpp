#include "USB.hpp"

namespace LC4500 {
	namespace USB {
		hid_device* device = nullptr;

		inline bool isConnected() { return (device != nullptr); }

		inline bool initialize() { return (hid_init() == 0); }

		inline bool exit() { return (hid_exit() == 0); }

		inline bool open() { return ((device = hid_open(vendorId, productId, nullptr)) != nullptr); }

		inline bool close() { hid_close(device); device = nullptr; return true; }

		inline Buffer read() {
			if (!isConnected()) return nullptr;

			Buffer ret(new uint8_t[bufferSize]);
			int32_t readBytes = hid_read_timeout(device, ret.get(), bufferSize, readTimeout);

			if (readBytes == -1) {
				close();
				return nullptr;
			}

			return ret;
		}

		inline int32_t write(Buffer &data) {
			if (!isConnected()) return -1;

			int32_t writtenBytes = hid_write(device, data.get(), bufferSize);

			if (writtenBytes == -1) {
				close();
				return -1;
			}

			return writtenBytes;
		}
	};
};

