/******************************************************************************
 * pixcir driver for usbtouchscreen NOTE
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
 *     some change for support solution. 
 * 
 *   
 *
 * 
 ##############################NOTE######################################
 if you touch the HID-CTP and find CTP have no action even the driver mach 
 sucessfully ! First please find out the drivers/hid/usbhid/hid-core.c and
 modify the usbhid_probe() function like below.Because the hid core hold the
 device.you must change it to ignoer the pixcir hid device.

 ########################################################################
  
 static int usbhid_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_host_interface *interface = intf->cur_altsetting;
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usbhid_device *usbhid;
	struct hid_device *hid;
	unsigned int n, has_in = 0;
	size_t len;
	int ret;

	dbg_hid("HID probe called for ifnum %d\n",
			intf->altsetting->desc.bInterfaceNumber);

	  if(dev->descriptor. idVendor==0x040b && dev->descriptor.idProduct==0x2230) {
		dev_info(&dev->dev,"HID ignore the PIXCIR HID DEVICE!PID : %04x " \
			       "VID : %04xthe device has special driver!\n",
			       le16_to_cpu(dev->descriptor.idProduct), \
			       le16_to_cpu(dev->descriptor. idVendor));
		return -ENODEV;
        	   }
        
	for (n = 0; n < interface->desc.bNumEndpoints; n++)
		if (usb_endpoint_is_int_in(&interface->endpoint[n].desc))
			has_in++;
	if (!has_in) {
		hid_err(intf, "couldn't find an input interrupt endpoint\n");
		return -ENODEV;
	}
  ................................
  ................................
  ................................
  ................................

}

 ##############################NOTE######################################
 if you touch the HID-CTP and find CTP have no action even the driver mach 
 sucessfully ! Please make sure the pixcir_hid_tangoc.h have the right config
 Especially Resolution . 

 ########################################################################
