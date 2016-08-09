#ifndef _LC4500_USB_H_
#define _LC4500_USB_H_

#include "hidapi.h"

#include <cstdint>
#include <memory>

namespace LC4500 {
	namespace USB {
		namespace Internal {}

		namespace {
			using Buffer = std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>>;

			namespace Internal {
				const uint16_t vendorId = 0x0451;
				const uint16_t productId = 0x6401;
				const int32_t readTimeout = 2000;

				hid_device* device;
			};

			constexpr size_t maxPacketSize = 64;
			constexpr size_t bufferSize = maxPacketSize + 1;

			extern bool isConnected();

			extern bool initialize();
			extern bool exit();

			extern bool open();
			extern bool close();

			extern Buffer read();
			extern int32_t write(Buffer &data);

		};
	};
};

#endif
