#ifndef _LC4500_TRANSACTION_H_
#define _LC4500_TRANSACTION_H_

#include <cstdint>
#include <algorithm>

#include "../USB/USB.hpp"

namespace LC4500 {
	namespace Internal {};

	namespace {
		namespace Internal {
			constexpr size_t maxTransactionDataSize = 512;
		};

		struct Transaction {
			enum class Type : bool { WRITE = 0, READ = 1 };
			Transaction() : flags{ 0 }, sequence{ 0 }, length{ 0 }, data{ 0 } {}

			template<typename... ParamList>
			Transaction(Type _type, uint16_t cmd, ParamList&&... params) : flags{ 0, 0, 0, 0, _type }, sequence{ 0 }, length{ 2 }, command{ cmd } {
				if (sizeof...(params) > 0) {
					addData(std::forward<ParamList>(params)...);
				}
			}

			inline void addData() {}

			template<typename T>
			inline void addData(T first) {
				T* ptr = reinterpret_cast<T*>(&data[length]);
				*ptr = first;
				length += sizeof(first);
			}

			template<typename T, typename... ParamList>
			inline void addData(T first, ParamList&&... params) {
				addData(first);
				if (sizeof...(params) > 0) {
					addData(std::forward<ParamList>(params)...);
				}
			}

			struct {
				uint8_t destination : 3;
				uint8_t reserved : 2;
				bool error : 1;
				bool reply : 1;
				Type rw : 1;
			} flags;

			uint8_t sequence;
			uint16_t length;

			union {
				uint16_t command;
				uint8_t data[Internal::maxTransactionDataSize];
			};
		};
		
		extern inline std::unique_ptr<Transaction> read() {
			auto received = USB::read();

			auto ret = std::unique_ptr<Transaction>(new Transaction(), std::default_delete<Transaction>());
			memcpy(ret.get(), received.get(), USB::bufferSize);

			if (received == nullptr) return nullptr;

			return ret;
		}

		extern inline int32_t write(Transaction &tran) {
			uint16_t headerBytes = sizeof(tran.flags) + sizeof(tran.sequence) + sizeof(tran.length);
			uint16_t maxDataSize = USB::maxPacketSize - headerBytes;
			uint16_t totalWrittenBytes = 0;
			uint16_t writtenBytes = std::min(tran.length, maxDataSize);

			USB::Buffer buffer(new uint8_t[USB::bufferSize]);
			memcpy(buffer.get() + 1, &tran, sizeof(uint8_t) * writtenBytes);
			if (USB::write(buffer) == -1) return -1;

			totalWrittenBytes += writtenBytes;

			while (totalWrittenBytes < tran.length) {
				writtenBytes = std::min(static_cast<uint16_t>(USB::maxPacketSize), static_cast<uint16_t>(tran.length - totalWrittenBytes));
				memcpy(buffer.get() + 1, &tran.data[totalWrittenBytes], sizeof(uint8_t) * writtenBytes);
				totalWrittenBytes += writtenBytes;
			}

			return totalWrittenBytes + headerBytes;
		}

		template<class T>
		using TransactionData = std::unique_ptr<T, std::default_delete<T[]>>;

		template<class T = uint8_t>
		extern TransactionData<T> transact(Transaction &tran) {
			int32_t result = write(tran);

			if (tran.flags.reply) {
				if (result <= 0) return nullptr;

				auto received = read();

				if (received == nullptr) return nullptr;
				if (received->flags.error ||
					(received->flags.rw == Transaction::Type::READ && received->length == 0)) return nullptr;

				TransactionData<T> ret(new T[USB::maxPacketSize]);
				memcpy(ret.get(), received->data, USB::maxPacketSize);

				return ret;
			}
			else {
				return nullptr;
			}
		}

		template<class T = uint8_t>
		extern inline TransactionData<T> transactForGetValues(uint16_t cmd) {
			auto send = Transaction(Transaction::Type::READ, cmd);
			return transact<T>(send);
		}

		template<class T = uint8_t, typename... ParamList>
		extern inline TransactionData<T> transactForSetValues(uint16_t cmd, ParamList&&... params) {
			auto send = Transaction(Transaction::Type::WRITE, cmd, std::forward<ParamList>(params)...);
			return transact<T>(send);
		}
	};
};

#endif