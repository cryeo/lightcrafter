#ifndef DLPC350_USB_H
#define DLPC350_USB_H

#define USB_MIN_PACKET_SIZE 64
#define USB_MAX_PACKET_SIZE 64

#define MY_VID 0x0451
#define MY_PID 0x6401

int DLPC350_USB_Open(void);
int DLPC350_USB_IsConnected();
int DLPC350_USB_Write();
int DLPC350_USB_Read();
int DLPC350_USB_Close();
int DLPC350_USB_Init();
int DLPC350_USB_Exit();

#endif //USB_H
