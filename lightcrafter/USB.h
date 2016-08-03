#ifndef _USB_H_

#define _USB_H_

#include "dlpc350_usb.h"

namespace LightCrafter {
	namespace USB {
		bool isConnected();
		bool initialize();
		bool open();
		bool close();
		bool exit();
	};
};


#endif
