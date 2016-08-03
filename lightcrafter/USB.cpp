#include "USB.h"

namespace LightCrafter {
	namespace USB {
		inline bool isConnected() {
			return (DLPC350_USB_IsConnected() == 1);
		}

		inline bool initialize() {
			return (DLPC350_USB_Init() == 0);
		}

		inline bool open() {
			return (DLPC350_USB_Open() == 0);
		}

		inline bool close() {
			return (DLPC350_USB_Close() == 0);
		}

		inline bool exit() {
			return (DLPC350_USB_Exit() == 0);
		}
	};
};

