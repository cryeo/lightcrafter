#include <memory>
using namespace std;

#include "dlpc350_usb.h"
#ifdef Q_OS_WIN32
#include <setupapi.h>
#endif
#include "hidapi.h"

/***************************************************
*                  GLOBAL VARIABLES
****************************************************/

static hid_device *DeviceHandle;	//Handle to write
//In/Out buffers equal to HID endpoint size + 1
//First byte is for Windows internal use and it is always 0
unsigned char g_OutputBuffer[USB_MAX_PACKET_SIZE+1];
unsigned char g_InputBuffer[USB_MAX_PACKET_SIZE+1];


static int USBConnected = 0;      //Boolean true when device is connected

int DLPC350_USB_IsConnected()
{
    return USBConnected;
}

int DLPC350_USB_Init(void)
{
    return hid_init();
}

int DLPC350_USB_Exit(void)
{
    return hid_exit();
}

int DLPC350_USB_Open()
{
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    DeviceHandle = hid_open(MY_VID, MY_PID, NULL);

    if(DeviceHandle == NULL)
    {
        USBConnected = 0;
        return -1;
    }

    USBConnected = 1;

    return 0;
}

int DLPC350_USB_Write()
{
    int bytesWritten;

    if(DeviceHandle == NULL)
        return -1;

    if((bytesWritten = hid_write(DeviceHandle, g_OutputBuffer, USB_MIN_PACKET_SIZE+1)) == -1)
    {
        hid_close(DeviceHandle);
        USBConnected = 0;
        return -1;
    }

    return bytesWritten;
}

int DLPC350_USB_Read()
{
    int bytesRead;

    if(DeviceHandle == NULL)
        return -1;

    //clear out the input buffer
    memset((void*)&g_InputBuffer[0],0x00,USB_MIN_PACKET_SIZE+1);

    if((bytesRead = hid_read_timeout(DeviceHandle, g_InputBuffer, USB_MIN_PACKET_SIZE+1, 2000)) == -1)
    {
        hid_close(DeviceHandle);
        USBConnected = 0;
        return -1;
    }

    return bytesRead;
}

int DLPC350_USB_Close()
{
    hid_close(DeviceHandle);
    USBConnected = 0;

    return 0;
}

