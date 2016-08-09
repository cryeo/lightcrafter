#include "USB.hpp"

namespace LC4500 {
	namespace USB {
		namespace {
			inline bool isConnected() { return (Internal::device != nullptr); }

			inline bool initialize() { return (hid_init() == 0); }

			inline bool exit() { return (hid_exit() == 0); }

			inline bool open() { return ((Internal::device = hid_open(Internal::vendorId, Internal::productId, nullptr)) != nullptr); }

			inline bool close() { hid_close(Internal::device); Internal::device = nullptr; return true; }

			inline Buffer read() {
				if (!isConnected()) return nullptr;

				Buffer ret(new uint8_t[bufferSize]);
				int32_t readBytes = hid_read_timeout(Internal::device, ret.get(), bufferSize, Internal::readTimeout);

				if (readBytes == -1) {
					close();
					return nullptr;
				}

				return ret;
			}

			inline int32_t write(Buffer &data) {
				if (!isConnected()) return -1;

				int32_t writtenBytes = hid_write(Internal::device, data.get(), bufferSize);

				if (writtenBytes == -1) {
					close();
					return -1;
				}

				return writtenBytes;
			}
		};
	};
};

