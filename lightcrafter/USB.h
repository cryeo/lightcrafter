#ifndef _USB_H_

#define _USB_H_

namespace LightCrafter {
	namespace USB {
		extern bool isConnected();
		extern bool initialize();
		extern bool open();
		extern bool close();
		extern bool exit();
	};
};

#endif
