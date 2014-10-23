/******************************************************************************
 * pixcir driver for usbtouchscreen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * if you have any questions .please mail to the driver author .
 *
 * 
 * - the driver is support for pixcir Tangoc -HID solution.
 * - the driver is support for 5 finger mutiltouch screen.
 * - the drvier just adapt the DEVICE-A mutiltouch ,if your 
 *    system need the DEVICE-B mutiltouch ,you must have
 *    some change for support solution. 
 * 
 *   
 *
 *****************************************************************************/
#ifndef _LINUX_PIXCIR_HID_TANGOC_H
#define _LINUX_PIXCIR_HID_TANGOC_H

#include <linux/module.h>

#define DRIVER_VERSION		"Pixcir-hid-driver-tangoC-v0.1"
#define DRIVER_AUTHOR		"HANSAN <jjsu@pixcir.com.ch>"
#define DRIVER_DESC		"USB Touchscreen Driver"
/**
 *
 * USBTOUCH_DRV_DEBUG_ATTR    - if you define it ,you will add debug 
 *                                                  interface by driver attr
 * USBTOUCH_CLASS_DEBUG_ATTR - if you define it ,you will add debug 
 *                                                  interface by class attr
 */
#define USBTOUCH_DRV_DEBUG_ATTR
#define USBTOUCH_CLASS_DEBUG_ATTR

/*****************************************************************************/

#define PIX_HID_IDVENOR        0x040b
#define PIX_HID_IDPRODUCT    0x2230

/**
 *
 * MUTITOUCH_MAX_FINGER_NUM - the max finger touch event num
 * TOUCHSCREEN_MINX   - the minimum value of the Screen  x-axis 
 * TOUCHSCREEN_MAXX   - the maximum value of the Screen x-axis 
 * TOUCHSCREEN_MINY   - the minimum value of the Screen  y-axis 
 * TOUCHSCREEN_MAXY   - the maximum value of the Screen y-axis 
 * Above four params base on your actual system display resolution
 * 
 * PIX_FW_MAX_X - Maximum x-axis form Pixcir Tangoc-hid tp firmware config
 * PIX_FW_MAX_Y - Maximum y-axis form Pixcir Tangoc-hid tp firmware config
 * Above two params bse on pixcir firware config 
 */
#define MUTITOUCH_MAX_FINGER_NUM 5
#define TOUCHSCREEN_MINX 0
#define TOUCHSCREEN_MAXX 800//10239//1024 //600//
#define TOUCHSCREEN_MINY 0
#define TOUCHSCREEN_MAXY 480//6143//768//480//
#define PIX_FW_MAX_X 800//10239//1024//(32*512) //1024
#define PIX_FW_MAX_Y 480//6143//768//(24*512) //768

/** 
 * If the actual display resolution donnot match with pixcir firm-
    -ware config ,you can use follow to change it .
 * swap_touchscreen_xy -Set '1' to exchange of the x-axis and y-axis else set '0'
 * swap_direction_xy - Set '1' to swapped x-axis and y-axis direction else set '0' 
 * conv_xy - Set '1' use it to conversion TP resolution to actual display's else set '0'
 * fwx_to_screenx - you can use it to conversion TP resolution to actual display's.
 * @fw_sx - the x-axis value from pixcir tp during touching tp
 * pix_swap_xy -Exchange of the x-axis and y-axis if you need it 
 * pix_swap_direction - Swapped x-axis and y-axis direction if you need it
 */
static int swap_touchscreen_xy = 0;
static int swap_direction_xy = 0;
static int conv_xy = 0;
module_param(conv_xy, bool, 0644);
MODULE_PARM_DESC(conv_xy, "If set X and Y axes are swapped.");

#define fwx_to_screenx(fw_sx) ((fw_sx*TOUCHSCREEN_MAXX)/PIX_FW_MAX_X)	   
#define fwy_to_screeny(fw_sy) ((fw_sy*TOUCHSCREEN_MAXY)/PIX_FW_MAX_Y)	

#define pix_swap_xy(x,y) do {int __tmp; __tmp=x;x=y;y=__tmp;}while(0)
#define pix_swap_direction(x,y) do{if(x) x=TOUCHSCREEN_MAXX-x; if(y) y=TOUCHSCREEN_MAXY-y ;}while(0)

/**
 * touch_event - the touch event desc as pixcir hid touchsceen solution 
 * usbtouch_info - the usbtouch misc param information desc
 * pix_usbtouch_dev - the pixcir Private data desc
 */
struct touch_event {
       int touch_statue;
	int finger_id; 
	int x ;
       int y;
};
struct usbtouch_info {
	int min_xc, max_xc;
	int min_yc, max_yc;
	int rept_size;
	bool pixcir_urb_always;
	//void (*msg_handle) (struct pix_usbtouch_dev *pdev, unsigned char *pkt, int len);
	void (*msg_handle) (void *pdev, void *pkt, int len);
};

/* Pixcir private usbtouch device */
struct pix_usbtouch_dev {
	unsigned char *data;
	dma_addr_t data_dma;
	unsigned char *buffer;
	int buf_len;
	struct mutex ctrl_urb_lock;	/* protect urb_buf */
	struct class *pixcir_hid_class;
	struct urb *pixcir_urb;
	struct usb_interface *pix_intf;
	struct input_dev *input;
	struct usbtouch_info *pix_info;
	struct touch_event multouch_event[MUTITOUCH_MAX_FINGER_NUM];
	char name[128];
	char phys[64];
	void *priv;
};

#define CLTR_SIZE 0x40
#define PIX_GET_REPORT   1  //from device to host
#define PIX_SET_REPORT 0  //from host  to device 


static unsigned char hid_cali_cmd[32]= {0x00,0x00,0x01,0x3a,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
								  	      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								  	      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static unsigned char hid_reset_cmd[32]={0x02,0x00,0x01,0x3a,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
									        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
										 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#define err_bit(err) ( do{ int __bit = 0; while(err&(1<<__bit++)) break; --__bit; }while(0) )
static int dbg_flag = 0;
#define pix_dbg(fmt...) do {  if(dbg_flag) printk(fmt); }while(0)

					

















#endif
