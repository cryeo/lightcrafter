#ifndef _LIGHTCRAFTER_USB_H_
#define _LIGHTCRAFTER_USB_H_

#include "hidapi.h"
#include <cstdint>
#include <algorithm>

#define VENDOR_ID 0x0451
#define PRODUCT_ID 0x6401

#define MAX_PACKET_SIZE 64
#define BUFFER_SIZE (MAX_PACKET_SIZE + 1)
#define TIMEOUT 2000

#define HID_MAX_MESSAGE_SIZE 512

namespace LightCrafter {
	namespace USB {
		enum class TransactionType : bool {
			WRITE = 0,
			READ = 1
		};

		struct Transaction {
			struct {
				struct {
					uint8_t destination : 3;
					uint8_t reserved : 2;
					bool error : 1;
					bool reply : 1;
					TransactionType rw : 1;
				} flags;
				uint8_t sequence;
				uint16_t length;
			} head;
			union {
				struct {
					uint8_t cmd3;
					uint8_t cmd2;
				} command;
				uint8_t raw[HID_MAX_MESSAGE_SIZE];
			} data;
			Transaction() : head{ 0 }, data{ 0 } {}
		};

		extern bool isConnected();

		extern bool initialize();
		extern bool exit();

		extern bool open();
		extern bool close();

		extern Transaction* read();
		extern int16_t write(Transaction &tran, uint32_t offset = 0);

		template<typename... ParamList>
		extern Transaction transaction(TransactionType type, uint8_t cmd2, uint8_t cmd3, ParamList&&... params) {
			Transaction tran;

			tran.head.flags.rw = type;
			tran.head.flags.destination = 0;
			tran.head.flags.reserved = 0;
			tran.head.flags.error = 0;
			tran.head.flags.reply = 1;

			tran.head.sequence = 0;
			tran.head.length = 2;

			tran.data.command.cmd2 = cmd2;
			tran.data.command.cmd3 = cmd3;

			for (uint8_t param : std::vector<uint8_t>({ params... })) {
				tran.data.raw[tran.head.length++] = static_cast<uint8_t>(param);
			}

			return tran;
		}

		extern hid_device* device;
		extern uint8_t readBuffer[BUFFER_SIZE];
		extern uint8_t writeBuffer[BUFFER_SIZE];
	};

	using TransactionType = USB::TransactionType;
	using Transaction = USB::Transaction;
};

#endif
