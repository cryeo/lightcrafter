#ifndef _DLPC350_PATTERNSEQUENCE_H_
#define _DLPC350_PATTERNSEQUENCE_H_

#include "../USB/USB.hpp"

#include <cstdint>
#include <cassert>
#include <vector>

namespace LC4500 {
	namespace Internal {};

	namespace {
		namespace Internal {
			const size_t maxPatternInSequence = 128;
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

			struct Data {
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
				Data() : value(0) {}
				Data(Color _color,
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

			bool addPattern(
				Pattern::Color color,
				Pattern::TriggerType triggerType,
				uint8_t bitDepth,
				uint8_t imageIndex,
				Pattern::BitIndex startBit,
				bool invertPattern = false,
				bool insertBlack = true) {

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

				bool bufferSwap ;
				bool triggerOutPrevious = false;

				if (sizePattern() == 0) {
					bufferSwap = true;
				}
				else {
					bufferSwap = (lastPattern().imageIndex != imageIndex);
				}

				//auto patternDisplayMode = getPatternDisplayMode();

				//if (sizePattern() == 0) {
				//	if (*patternDisplayMode == PatternDisplayMode::INTERNAL) assert(triggerType != Pattern::TriggerType::NO_TRIGGER);
				//	bufferSwap = true;
				//}
				//else {
				//	bufferSwap = (lastPattern().imageIndex != imageIndex);
				//}

				//if (*patternDisplayMode == PatternDisplayMode::EXTERNAL) {
				//	triggerType = bufferSwap ? Pattern::TriggerType::EXTERNAL_POSITIVE : Pattern::TriggerType::NO_TRIGGER;
				//}

				uint8_t patternNumber = iStartBit / bitDepth;

				Pattern pattern(color, triggerType, bitDepth, imageIndex, patternNumber, invertPattern, insertBlack, bufferSwap, triggerOutPrevious);

				addPattern(pattern);

				return true;
			}

			void addPattern(Pattern& pat) {
				pattern[_sizePattern++] = pat;
				if (pat.data.bufferSwap) image[_sizeImage++] = pat.imageIndex;
			}

			inline size_t sizePattern() {
				return _sizePattern;
			}

			inline size_t sizeImage() {
				return _sizeImage;
			}

			inline Pattern& lastPattern() {
				assert(_sizePattern > 0);
				return pattern[_sizePattern - 1];
			}

			inline Pattern& getPattern(size_t index) {
				return pattern[index];
			}

			inline uint8_t getImage(size_t index) {
				return image[index];
			}

		private:
			size_t _sizePattern, _sizeImage;
			Pattern pattern[Internal::maxPatternInSequence];
			uint8_t image[USB::bufferSize];
		};
	};
};
#endif
