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
 *     some change for support solution. 
 * 
 *   
 *
 *****************************************************************************/

//#define DEBUG

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
#include "pixcir_hid_tangoc.h"

static struct usb_device *pix_udev= NULL;
static struct pix_usbtouch_dev *pix_priv_data = NULL;
static void pixcir_urbmsg_handle(struct pix_usbtouch_dev *pdev,
                                 unsigned char *pkt, int len);
static void pixcir_urb_complete(struct urb *urb);

static int pixcir_command_ctrol(struct pix_usbtouch_dev *usbtouch,
								   unsigned char request,unsigned char *command,
								   unsigned char  );


/*******************************************************************************/

static struct usbtouch_info pixcir_type = {
             .min_xc  = TOUCHSCREEN_MINX,
             .max_xc  = TOUCHSCREEN_MAXX,
             .min_yc  = TOUCHSCREEN_MINY,
             .max_yc  = TOUCHSCREEN_MAXY,
             .pixcir_urb_always = true,
};

static void pixcir_urbmsg_handle(struct pix_usbtouch_dev *pdev,
                                 unsigned char *pkt, int len)
{
      int z=50;
      int w=15;
      int i;
      int touch_active=0;
      unsigned char *cn_p =NULL;
    //  struct usbtouch_info *type = pdev->pix_info;
      
      pix_dbg("enter input point function--:%s: urb get pkt_data :\n",__func__);
      for(i=0;i<len;i++)
	pix_dbg(" (%02x) ",pkt[i]);
      pix_dbg("\n");
      
	  
      cn_p=pkt;   
      for (i=0 ;i<MUTITOUCH_MAX_FINGER_NUM ;i++)
     {
      	pdev->multouch_event[i].touch_statue = (*(cn_p+0) & 0xFF);
	pdev->multouch_event[i].finger_id =((*(cn_p+1) & 0xFF) &0x07);
	if (!conv_xy) {
		pdev->multouch_event[i].x = ((*(cn_p+3) & 0xFF) <<8) | (*(cn_p+2) & 0xFF) ;
		pdev->multouch_event[i].y = ((*(cn_p+5) & 0xFF) <<8) | (*(cn_p+4) & 0xFF) ;
	}else {
		pdev->multouch_event[i].x =  \
			fwx_to_screenx((((*(cn_p+3) & 0xFF) <<8) | (*(cn_p+2) & 0xFF)));
	       pdev->multouch_event[i].y =  \
 		   	fwy_to_screeny((((*(cn_p+5) & 0xFF) <<8) | (*(cn_p+4) & 0xFF)));
   		}
	if (swap_touchscreen_xy)
		pix_swap_xy(pdev->multouch_event[i].x , pdev->multouch_event[i].y);
	if (swap_direction_xy)
		pix_swap_direction(pdev->multouch_event[i].x ,pdev->multouch_event[i].y);
	/*if((pdev->multouch_event[i].x > TOUCHSCREEN_MAXX) || 
		(pdev->multouch_event[i].y >TOUCHSCREEN_MAXY))
	 	return;*/
	if (pdev->multouch_event[i].touch_statue==0x07) 
		touch_active++;
	cn_p += 6;
      	}

	  int j = 0;
    
/*
 (07)  (00)  (27)  (01)  (4d)  (00)
 (07)  (04)  (8d)  (00)  (28)  (00)
 (07)  (01)  (b7)  (01)  (27)  (00)
 (04)  (02)  (00)  (00)  (df)  (01)
 (04)  (03)  (00)  (00)  (df)  (01)
 (05)
i : 0 pixcir_urbmsg_handle:: input id[0] x[0]=505 y[0]=403
i : 1 pixcir_urbmsg_handle:: input id[4] x[4]=659 y[4]=440
i : 2 pixcir_urbmsg_handle:: input id[1] x[1]=361 y[1]=441
i : 4 pixcir_urbmsg_handle:: input id[3] x[3]=505 y[3]=403
*/
#if 1
//	  if (touch_active > 1 && touch_active < 5)
	  if ((touch_active == 2) || (touch_active == 3))
	  {
		  pdev->multouch_event[touch_active].touch_statue = 0x07;
		  pdev->multouch_event[touch_active].x	= pdev->multouch_event[0].x;
		  pdev->multouch_event[touch_active].y = pdev->multouch_event[0].y;
		  //pdev->multouch_event[0].x = 0;
		  //pdev->multouch_event[0].y = 0;
		  pix_dbg("\n\n		Change 2 3...... \n\n");
	  }
#endif
      if (touch_active)
      {
      		input_report_key(pdev->input, BTN_TOUCH, 1);

				for (i=0 ;i<MUTITOUCH_MAX_FINGER_NUM ;i++)
				{
				  if (pdev->multouch_event[i].touch_statue==0x07 && (touch_active ==1)){
					  	 j=i;
						input_report_abs(pdev->input, ABS_MT_TOUCH_MAJOR, z);
						input_report_abs(pdev->input, ABS_MT_WIDTH_MAJOR, w);
#if 1
						input_report_abs(pdev->input, ABS_MT_POSITION_X, 0);
						input_report_abs(pdev->input, ABS_MT_POSITION_Y, 1);
#else
						input_report_abs(pdev->input, ABS_MT_POSITION_X, pdev->multouch_event[i].x);
						input_report_abs(pdev->input, ABS_MT_POSITION_Y, pdev->multouch_event[i].y);
#endif
						input_report_abs(pdev->input,ABS_MT_TRACKING_ID,pdev->multouch_event[i].finger_id);
						pix_dbg("%s:: input id[%d] x[%d]=%2d y[%d]=%2d\n",
						__func__,pdev->multouch_event[i].finger_id,
						pdev->multouch_event[i].finger_id,pdev->multouch_event[i].x,
						pdev->multouch_event[i].finger_id,pdev->multouch_event[i].y);

						input_mt_sync(pdev->input);
				 }else	if (pdev->multouch_event[i].touch_statue==0x07){
					 input_report_abs(pdev->input, ABS_MT_TOUCH_MAJOR, z);
						 input_report_abs(pdev->input, ABS_MT_WIDTH_MAJOR, w);
						input_report_abs(pdev->input, ABS_MT_POSITION_X, pdev->multouch_event[i].x);
						input_report_abs(pdev->input, ABS_MT_POSITION_Y, pdev->multouch_event[i].y);
						input_report_abs(pdev->input,ABS_MT_TRACKING_ID,pdev->multouch_event[i].finger_id);
					pix_dbg("i : %d %s:: input id[%d] x[%d]=%2d y[%d]=%2d\n",
					i,__func__,pdev->multouch_event[i].finger_id,
					 pdev->multouch_event[i].finger_id,pdev->multouch_event[i].x,
					 pdev->multouch_event[i].finger_id,pdev->multouch_event[i].y);

						input_mt_sync(pdev->input);
				 }else
					 continue;

				}
				pix_dbg("\n");
	      
		}else
		{
			input_report_key(pdev->input, BTN_TOUCH, 0);
			input_report_abs(pdev->input, ABS_MT_TOUCH_MAJOR, 0);
			input_report_abs(pdev->input, ABS_MT_WIDTH_MAJOR, 0);
			input_mt_sync(pdev->input);
		}

#if 1
      if (touch_active == 1) {

    	   	if (j==0)
    	   		j=1;
    	   	else
    	   		j--;

      		for (i=0 ;i<1 ;i++)
      		{
				if (pdev->multouch_event[i].touch_statue==0x07)
				{
					input_report_abs(pdev->input, ABS_MT_TOUCH_MAJOR, z);
					input_report_abs(pdev->input, ABS_MT_WIDTH_MAJOR, w);
					input_report_abs(pdev->input, ABS_MT_POSITION_X, pdev->multouch_event[i].x);
					input_report_abs(pdev->input, ABS_MT_POSITION_Y, pdev->multouch_event[i].y);
					input_report_abs(pdev->input,ABS_MT_TRACKING_ID,pdev->multouch_event[j].finger_id);
					pix_dbg("%s:: input id[%d] x[%d]=%2d y[%d]=%2d\n",
					__func__,pdev->multouch_event[j].finger_id,
					pdev->multouch_event[j].finger_id,pdev->multouch_event[i].x,
					pdev->multouch_event[j].finger_id,pdev->multouch_event[i].y);

					input_mt_sync(pdev->input);
				}else
					continue;
			}

      		pix_dbg("\n");
      	}
#endif
      input_sync(pdev->input);
      memset(pdev->multouch_event,0,sizeof(pdev->multouch_event));
      touch_active=0;  
}

static void pixcir_urb_complete(struct urb *urb)
{
	struct pix_usbtouch_dev *usbtouch = urb->context;
	int retval;
         
	switch (urb->status) {
	case 0:
		/* success */
		break;
	case -ETIME:
		/* this urb is timing out */
		printk("%s - urb timed out - was the device unplugged?",
		    __func__);
		return;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
	case -EPIPE:
		/* this urb is terminated, clean up */
		printk("%s - urb shutting down with status: %d",
		    __func__, urb->status);
		return;
	default:
		printk("%s - nonzero urb status received: %d",
		    __func__, urb->status);
		goto exit;
	}	
         pix_dbg("enter urb complete function%s. the urb data actual length=%d\n",__func__,urb->actual_length);	 
	usbtouch->pix_info->msg_handle(usbtouch, usbtouch->data, urb->actual_length);
	//usbtouch->pix_info->msg_handle(usbtouch, usbtouch->buffer, urb->actual_length);

exit:
	usb_mark_last_busy(interface_to_usbdev(usbtouch->pix_intf));
		retval = usb_submit_urb(urb, GFP_ATOMIC);
		if (retval)
			printk("%s - usb_submit_urb failed with result: %d",
		    		__func__, retval);
}

static int usbtouch_input_open(struct input_dev *input)
{
#if 0
	struct pix_usbtouch_dev *usbtouch = input_get_drvdata(input);
	int r;

	usbtouch->pixcir_urb->dev = interface_to_usbdev(usbtouch->pix_intf);

	r = usb_autopm_get_interface(usbtouch->pix_intf) ? -EIO : 0;
	if (r < 0)
		goto out;

	if (usbtouch->pix_info->pixcir_urb_always) {
		if (usb_submit_urb(usbtouch->pixcir_urb, GFP_KERNEL)) {
			r = -EIO;
			goto out_put;
		}
	}

	usbtouch->pix_intf->needs_remote_wakeup = 1;
out_put:
	usb_autopm_put_interface(usbtouch->pix_intf);
out:
#endif
	return 0;
}

static void usbtouch_input_close(struct input_dev *input)
{
#if 0
	struct pix_usbtouch_dev *usbtouch = input_get_drvdata(input);
	int r;

	if (usbtouch->pix_info->pixcir_urb_always)
		usb_kill_urb(usbtouch->pixcir_urb);
	r = usb_autopm_get_interface(usbtouch->pix_intf);
	usbtouch->pix_intf->needs_remote_wakeup = 0;
	if (!r)
		usb_autopm_put_interface(usbtouch->pix_intf);
#endif
     return ;
}

static int usbtouch_suspend
(struct usb_interface *intf, pm_message_t message)
{
	struct pix_usbtouch_dev *usbtouch = usb_get_intfdata(intf);

	usb_kill_urb(usbtouch->pixcir_urb);

	return 0;
}

static int usbtouch_resume(struct usb_interface *intf)
{
	struct pix_usbtouch_dev *usbtouch = usb_get_intfdata(intf);
	struct input_dev *input = usbtouch->input;
	int result = 0;

	mutex_lock(&input->mutex);
	if (input->users || usbtouch->pix_info->pixcir_urb_always)
		result = usb_submit_urb(usbtouch->pixcir_urb, GFP_NOIO);
	mutex_unlock(&input->mutex);

	return result;
}

static int usbtouch_reset_resume(struct usb_interface *intf)
{
	struct pix_usbtouch_dev *usbtouch = usb_get_intfdata(intf);
	struct input_dev *input = usbtouch->input;
	int err = 0;
	mutex_lock(&input->mutex);
	if (input->users)
		err = usb_submit_urb(usbtouch->pixcir_urb, GFP_NOIO);
	mutex_unlock(&input->mutex);

	return err;
}

static void usbtouch_free_buffers(struct usb_device *udev,
				  struct pix_usbtouch_dev *usbtouch)
{
	usb_free_coherent(udev, usbtouch->pix_info->rept_size,
			  usbtouch->data, usbtouch->data_dma);
	kfree(usbtouch->buffer);
}

/*********************************************************************************/
static int pixcir_command_ctrol(struct pix_usbtouch_dev *usbtouch,
								   unsigned char request,unsigned char *command,
								   unsigned char msg_dir)
{
	int retval = -1;
	unsigned int pipe;
	 int buf_size = 4 * sizeof(u8);
	unsigned char *debug_buf;
	unsigned char bm_request_type;
	struct usb_device *udev = interface_to_usbdev(usbtouch->pix_intf);
         mutex_init(&usbtouch->ctrl_urb_lock);
	mutex_lock(&usbtouch->ctrl_urb_lock);
	usbtouch->pix_info->pixcir_urb_always = false;
         debug_buf = kmalloc(buf_size,GFP_KERNEL);
	 if(!debug_buf) {
	 	dev_err(&udev->dev,"%s:debug_buf out of memory.\n",__func__);
		goto out_buf_malloc_err;
	 }
	 memcpy(debug_buf, command, sizeof(command));
	 bm_request_type = 0x21; 
	  pipe= usb_sndctrlpipe(udev, 0);
	  retval = usb_control_msg(udev, pipe, request, 
			      bm_request_type, 0x03<<8|0x00 , 0x00,debug_buf,  	 
			      buf_size,5000);
	 if (retval != buf_size) 
	       dev_err(&udev->dev,"%s:: usb control msg fail,retval=%d\n",__func__,retval);
	 
          mutex_unlock(&usbtouch->ctrl_urb_lock);
          if (retval != buf_size) {
		dev_err(&udev->dev,"%s:: usb control msg fail,retval=%d\n",__func__,retval);
		retval = 1;
	} else {
	      	retval = 0;   
	}
      out_buf_malloc_err:
		kfree(debug_buf);
		usbtouch->pix_info->pixcir_urb_always = true;
       		return retval;
	 /**********************
        if usb contral msg suscessful 
        	return 0 ;
        if usb contral msg fail 
        	return 1 ;
	*******************/
}
								   
#ifdef USBTOUCH_CLASS_DEBUG_ATTR								    
 								
static int pixcir_hid_debug_show(struct class *class,
				  struct class_attribute *attr,
				  char *buf)
{
// struct pix_usbtouch_dev *usbtouch =(struct pix_usbtouch_dev *) container_of(class, struct pix_usbtouch_dev, pixcir_hid_class);
	
	

return 0 ;
}
static int pixcir_hid_debug_store(struct class *class, 
	                      struct class_attribute *attr,
		                const char *buf, size_t count)
{
	printk("enter::%s\n",__func__);
	int ret = -1;
	char val ;
	struct pix_usbtouch_dev *usbtouch = pix_priv_data;
	 if(IS_ERR_OR_NULL(usbtouch))
	 {
        		printk("%s::pix_usbtouch_dev point err\n",__func__);
	 	return PTR_ERR(usbtouch);
          } 
	if(sscanf(buf,"%s",&val) <1)
		return -EINVAL;
	switch(val)
	{
	case 'c' :
	case 'C' :
	
 		     if(pixcir_command_ctrol(usbtouch,0x09,
			 	                 		  hid_cali_cmd,PIX_SET_REPORT) ==0)
			{				 				 
			      printk("%s:hid calibration action suscessful!\n",__func__);
 		             ret=0;
 		      }else {
			      printk("%s:hid calibration action fail!\n",__func__);
 		             ret=1;
 		      	} 
    
		break;
	case 't':
	case 'T':
         	   if(pixcir_command_ctrol(usbtouch,0x09,
			 	                   hid_reset_cmd,PIX_SET_REPORT)==0)
	    		{
		      		printk("%s:hid reset action suscessful!",__func__);
                    	ret=0; 
           		}else{
				printk("%s:hid reset action fail!",__func__);
                    	ret=1; 
                  	  }		
              break;
			  
	   case 's':
	   case 'S':
	   	      if(usbtouch->pix_info->pixcir_urb_always ==true)
		      	usbtouch->pix_info->pixcir_urb_always = false;
	    break;
		
	    case 'n':
	    case 'N':
			if(usbtouch->pix_info->pixcir_urb_always ==false)
				usbtouch->pix_info->pixcir_urb_always = true;
			if(dbg_flag)
				dbg_flag=~dbg_flag;
	    break;
				  
	    case 'd':
	    case 'D':
			if(!dbg_flag)
				dbg_flag=~dbg_flag;
	    break;
	}	

	return ret;

}
static CLASS_ATTR(pixcir_hid_debug, S_IRUGO|S_IWUGO, pixcir_hid_debug_show, pixcir_hid_debug_store);
#endif

#ifdef USBTOUCH_DRV_DEBUG_ATTR
static ssize_t pix_drvattr_debug(struct device_driver *drv, 
	                  const char *buf,size_t count)
{	
	printk("enter::%s\n",__func__);
	int ret = -1;
	char val ;
	struct pix_usbtouch_dev *usbtouch = pix_priv_data;
	 if(IS_ERR_OR_NULL(usbtouch))
	 {
        		printk("%s::pix_usbtouch_dev point err\n",__func__);
	 	return PTR_ERR(usbtouch);
          } 
	if(sscanf(buf,"%s",&val) <1)
		return -EINVAL;
	switch(val)
	{
	case 'c' :
	case 'C' :
	
 		     if(pixcir_command_ctrol(usbtouch,0x09,
			 	                 		  hid_cali_cmd,PIX_SET_REPORT) ==0)
			{				 				 
			      printk("%s:hid calibration action suscessful!\n",__func__);
 		             ret=0;
 		      }else {
			      printk("%s:hid calibration action fail!\n",__func__);
 		             ret=1;
 		      	} 
    
		break;
	case 't':
	case 'T':
         	   if(pixcir_command_ctrol(usbtouch,0x09,
			 	                   hid_reset_cmd,PIX_SET_REPORT)==0)
	    		{
		      		printk("%s:hid reset action suscessful!",__func__);
                    	ret=0; 
           		}else{
				printk("%s:hid reset action fail!",__func__);
                    	ret=1; 
                  	  }		
              break;
			  
	   case 's':
	   case 'S':
	   	      if(usbtouch->pix_info->pixcir_urb_always ==true)
		      	usbtouch->pix_info->pixcir_urb_always = false;
	    break;
		
	    case 'n':
	    case 'N':
			if(usbtouch->pix_info->pixcir_urb_always ==false)
				usbtouch->pix_info->pixcir_urb_always = true;
			if(dbg_flag)
				dbg_flag=~dbg_flag;
	    break;
				  
	    case 'd':
	    case 'D':
			if(!dbg_flag)
				dbg_flag=~dbg_flag;
	    break;
	}	

	return ret;

}

static ssize_t pix_drvattr_show(struct device_driver *drv,
	               const char *buf,size_t count)
{
	int ret;
	return ret;

}

static DRIVER_ATTR(pixcir_drv_dbg,S_IRUGO | S_IWUGO,pix_drvattr_show,pix_drvattr_debug);
static struct attribute **pix_attr[]={
	&driver_attr_pixcir_drv_dbg,
	NULL
};
static struct attribute_group pix_attrgp = {
	.attrs = pix_attr,
};
static struct attribute_group **drv_group[]= {
	&pix_attrgp,
	NULL
};
#endif 
/*********************************************************************************/
static int usbtouch_probe(struct usb_interface *intf,
			  const struct usb_device_id *id)
{
	struct pix_usbtouch_dev *usbtouch;
	struct input_dev *input_dev;
	struct usb_endpoint_descriptor *pix_int_endpoint;
	struct usb_host_interface *cur_interface;
	struct usb_device *udev;
	int err = -ENOMEM;
	if(IS_ERR_OR_NULL(intf)){
	     err = PTR_ERR(intf);
	    goto out_err_pt;
	} 
	udev = interface_to_usbdev(intf);
	if(IS_ERR_OR_NULL(udev)){
		err =  PTR_ERR(udev);
		    goto out_err_pt;
			}
	cur_interface=intf->cur_altsetting;		
	if(IS_ERR_OR_NULL(cur_interface)){
		err= PTR_ERR(udev);
		goto out_err_pt;
	}
	pix_int_endpoint = &cur_interface->endpoint->desc;
	if(IS_ERR_OR_NULL(pix_int_endpoint)){
		err= PTR_ERR(pix_int_endpoint);
		goto out_err_pt;
	}
	 if(udev->descriptor.idVendor == PIX_HID_IDVENOR && \
		 udev->descriptor.idProduct == PIX_HID_IDPRODUCT && \
		 cur_interface->desc.bInterfaceNumber == 0x00 && \
		 cur_interface->endpoint->desc.bEndpointAddress ==0x81) {
		dev_info(&udev->dev,"find pixcir interface sucessfully\n");
             	}else {
             	           err = -ENODEV;
			  goto out_err_pt;
             	}
	
	if(!usb_endpoint_is_int_in(pix_int_endpoint)){
		dev_err(&udev->dev,"%s:: no interrupt in endpoint \n",__func__);
		err = -ENODEV;
		goto out_err_pt;
	}
	usbtouch = kzalloc(sizeof(struct pix_usbtouch_dev), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!usbtouch || !input_dev)
		goto out_free;
	usbtouch->pix_info = &pixcir_type;
	if (!usbtouch->pix_info->msg_handle)
		usbtouch->pix_info->msg_handle = pixcir_urbmsg_handle;
	usbtouch->pix_info->rept_size = pix_int_endpoint->wMaxPacketSize;
	usbtouch->pix_intf= intf;
	usbtouch->input = input_dev;

	if (udev->manufacturer)
	strlcpy(usbtouch->name, udev->manufacturer, sizeof(usbtouch->name));

	if (udev->product) {
		if (udev->manufacturer)
			strlcat(usbtouch->name, " ", sizeof(usbtouch->name));
		strlcat(usbtouch->name, udev->product, sizeof(usbtouch->name));
	}

	if (!strlen(usbtouch->name))
		snprintf(usbtouch->name, sizeof(usbtouch->name),
			"USB Touchscreen %04x:%04x",
			 le16_to_cpu(udev->descriptor.idVendor),
			 le16_to_cpu(udev->descriptor.idProduct));

	usb_make_path(udev, usbtouch->phys, sizeof(usbtouch->phys));
	strlcat(usbtouch->phys, "/input0", sizeof(usbtouch->phys));

	input_dev->name = usbtouch->name;
	input_dev->phys = usbtouch->phys;
	usb_to_input_id(udev, &input_dev->id);
	input_dev->dev.parent = &intf->dev;
	input_set_drvdata(input_dev, usbtouch);
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) |BIT_MASK(EV_SYN);	                         
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	input_set_abs_params(input_dev, ABS_X, usbtouch->pix_info->min_xc, usbtouch->pix_info->max_xc, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, usbtouch->pix_info->min_yc, usbtouch->pix_info->max_yc, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, usbtouch->pix_info->min_xc,  usbtouch->pix_info->max_xc, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, usbtouch->pix_info->min_yc,  usbtouch->pix_info->max_yc, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0,  10, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0,  255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MINOR, 0,  255, 0, 0);
	err = input_register_device(usbtouch->input);
	if (err) {
		printk("%s - input_register_device failed, err: %d", __func__, err);
		goto out_unregister_input;
	}

        	usb_set_intfdata(intf, usbtouch);
	pix_udev=udev;
	pix_priv_data=usbtouch;
        usbtouch->data = usb_alloc_coherent(udev,usbtouch->pix_info->rept_size,
					    GFP_ATOMIC, &usbtouch->data_dma);
	if (!usbtouch->data)
		goto out_free_buffers;
         usbtouch->buffer = kmalloc(usbtouch->pix_info->rept_size, GFP_KERNEL);
	if(!usbtouch->buffer )
		goto out_free_buf;
	usbtouch->pixcir_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!usbtouch->pixcir_urb) {
		printk("%s - usb_alloc_urb failed: usbtouch->pixcir_urb", __func__);
		goto out_free_urb;
	}
         
	usb_fill_int_urb(usbtouch->pixcir_urb, udev, \
			usb_rcvintpipe(udev, pix_int_endpoint->bEndpointAddress), \
			usbtouch->data, usbtouch->pix_info->rept_size, \
			pixcir_urb_complete, usbtouch, pix_int_endpoint->bInterval);
	/*usb_fill_int_urb(usbtouch->pixcir_urb, udev, \
			usb_rcvintpipe(udev, pix_int_endpoint->bEndpointAddress), \
			usbtouch->buffer, usbtouch->pix_info->rept_size, \
			pixcir_urb_complete, usbtouch, pix_int_endpoint->bInterval);
			*/
	usbtouch->pixcir_urb->dev = udev;
	usbtouch->pixcir_urb->transfer_dma = usbtouch->data_dma;
	usbtouch->pixcir_urb->transfer_buffer = usbtouch->buffer;
	usbtouch->pixcir_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP  ;
	#if 1
	if (usbtouch->pix_info->pixcir_urb_always) {
		/* this can't fail */
		usb_autopm_get_interface(intf);
	
		err = usb_submit_urb(usbtouch->pixcir_urb, GFP_KERNEL);
         	if (err) {
			usb_autopm_put_interface(intf);
			printk("%s - usb_submit_urb failed with result: %d",
			    __func__, err);
			goto out_unregister_input;
		}
		
	}
	
        input_dev->open = usbtouch_input_open;
        input_dev->close = usbtouch_input_close;
	#endif
	
	#ifdef USBTOUCH_CLASS_DEBUG_ATTR
	usbtouch->pixcir_hid_class = class_create(THIS_MODULE,"pixcir_hid_class");
	if (!usbtouch->pixcir_hid_class) {
		printk("Error !creating pixcir_hid_class fail ");
	        goto out_err_class_create;
	}else {
		printk("%s:: pixcir_hid_class  sucessful !\n",__func__);
		}

	err = class_create_file(usbtouch->pixcir_hid_class, &class_attr_pixcir_hid_debug);
	if (err) {
		printk("%s::Error creating pixcir hid class version file! \n",__func__);
		goto our_err_class_file;
	}else {
		printk("%s:: pixcir_hid_class creat file sucessful !\n",__func__);
		}
	#endif
	
        dev_info(&udev->dev,"pixcir hid driver probe suscessful!\n" \
			              "pixcir_idVendor:0x%04x_idProduct:0x%04x_bEndpointAdress = 0x%02x\n", \
			              udev->descriptor.idVendor, \
				     udev->descriptor.idProduct, \
			              intf->cur_altsetting->endpoint->desc.bEndpointAddress);	
	return 0;
	
out_free_urb:
	usb_free_urb(usbtouch->pixcir_urb);
out_free_buffers:
	usbtouch_free_buffers(udev, usbtouch);
out_free_buf:
	kfree(usbtouch->buffer);
out_unregister_input:
	input_unregister_device(input_dev);
	input_dev = NULL;
out_free:
	input_free_device(input_dev);
	kfree(usbtouch);
	 usb_set_intfdata(intf, NULL);
out_err_pt:
	return err;
	
#ifdef USBTOUCH_CLASS_DEBUG_ATTR
our_err_class_file:
	class_remove_file(usbtouch->pixcir_hid_class,&class_attr_pixcir_hid_debug);
out_err_class_create:                
	class_destroy(usbtouch->pixcir_hid_class);
#endif

}

static void usbtouch_disconnect(struct usb_interface *intf)
{
	struct pix_usbtouch_dev *usbtouch = usb_get_intfdata(intf);

	printk("%s - called", __func__);

	if (!usbtouch)
		return;
	printk("%s - usbtouch is initialized, cleaning up", __func__);
	usb_set_intfdata(intf, NULL);
	/* this will stop IO via close */
         usb_kill_urb(usbtouch->pixcir_urb);
	input_unregister_device(usbtouch->input);	
	usb_free_urb(usbtouch->pixcir_urb);
	//if (usbtouch->pix_info->exit)
	//	usbtouch->pix_info->exit(usbtouch);
	usbtouch_free_buffers(interface_to_usbdev(intf), usbtouch); 
	#ifdef USBTOUCH_CLASS_DEBUG_ATTR
	class_remove_file(usbtouch->pixcir_hid_class,&class_attr_pixcir_hid_debug);
        	class_destroy(usbtouch->pixcir_hid_class);
	#endif
	kfree(usbtouch);
}

static const struct usb_device_id pixcir_devices[ ] = {
      	{ USB_DEVICE(PIX_HID_IDVENOR, PIX_HID_IDPRODUCT) },
         { }
};
MODULE_DEVICE_TABLE(usb, pixcir_devices);

static struct usb_driver usbtouch_driver = {
	.name		= "pixcir_hid_tangoc",
	.probe		= usbtouch_probe,
	.disconnect	= usbtouch_disconnect,
	.suspend	= usbtouch_suspend,
	.resume		= usbtouch_resume,
	.reset_resume	= usbtouch_reset_resume,
	.id_table	= pixcir_devices,
	#ifdef USBTOUCH_DRV_DEBUG_ATTR
	.drvwrap          = {
		.driver.groups = drv_group,
		},
	#endif
	.supports_autosuspend = 1,
};

static int __init usbtouch_init(void)
{
	return usb_register(&usbtouch_driver);
}

static void __exit usbtouch_cleanup(void)
{
	
	usb_deregister(&usbtouch_driver);
}

module_init(usbtouch_init);
module_exit(usbtouch_cleanup);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

MODULE_ALIAS("touchkitusb");
MODULE_ALIAS("itmtouch");
MODULE_ALIAS("mtouchusb");
