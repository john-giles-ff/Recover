/**
  ******************************************************************************
  * @file    usbh_def.h
  * @author  MCD Application Team
  * @brief   Definitions used in the USB host library
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef  USBH_DEF_H
#define  USBH_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../../../../../USB_HOST/Target/usbh_conf.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_DEF
  * @brief This file is includes USB descriptors
  * @{
  */

#ifndef NULL
#define NULL  0U
#endif

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef TRUE
#define TRUE  1U
#endif


#define ValBit(VAR,POS)                               (VAR & (1 << POS))
#define SetBit(VAR,POS)                               (VAR |= (1 << POS))
#define ClrBit(VAR,POS)                               (VAR &= ((1 << POS)^255))

#define LE16(addr)        (((uint16_t)(addr)[0]) | \
                           ((uint16_t)(((uint32_t)(addr)[1]) << 8)))

#define LE24(addr)        (((uint32_t)(addr)[0]) | \
                           (((uint32_t)(addr)[1]) << 8) | \
                           (((uint32_t)(addr)[2]) << 16))

#define LE32(addr)        (((uint32_t)(addr)[0]) | \
                           (((uint32_t)(addr)[1]) << 8) | \
                           (((uint32_t)(addr)[2]) << 16) | \
                           (((uint32_t)(addr)[3]) << 24))

#define LE64(addr)        (((uint64_t)(addr)[0]) | \
                           (((uint64_t)(addr)[1]) << 8) | \
                           (((uint64_t)(addr)[2]) << 16) | \
                           (((uint64_t)(addr)[3]) << 24) | \
                           (((uint64_t)(addr)[4]) << 32) | \
                           (((uint64_t)(addr)[5]) << 40) | \
                           (((uint64_t)(addr)[6]) << 48) | \
                           (((uint64_t)(addr)[7]) << 56))

#define LE16S(addr)       ((int16_t)(LE16((addr))))
#define LE24S(addr)       ((int32_t)(LE24((addr))))
#define LE32S(addr)       ((int32_t)(LE32((addr))))
#define LE64S(addr)       ((int64_t)(LE64((addr))))



#define  USB_LEN_DESC_HDR                                  0x02U
#define  USB_LEN_DEV_DESC                                  0x12U
#define  USB_LEN_CFG_DESC                                  0x09U
#define  USB_LEN_IF_DESC                                   0x09U
#define  USB_LEN_EP_DESC                                   0x07U
#define  USB_LEN_OTG_DESC                                  0x03U
#define  USB_LEN_SETUP_PKT                                 0x08U

/* bmRequestType :D7 Data Phase Transfer Direction  */
#define  USB_REQ_DIR_MASK                                  0x80U
#define  USB_H2D                                           0x00U
#define  USB_D2H                                           0x80U

/* bmRequestType D6..5 Type */
#define  USB_REQ_TYPE_STANDARD                             0x00U
#define  USB_REQ_TYPE_CLASS                                0x20U
#define  USB_REQ_TYPE_VENDOR                               0x40U
#define  USB_REQ_TYPE_RESERVED                             0x60U

/* bmRequestType D4..0 Recipient */
#define  USB_REQ_RECIPIENT_DEVICE                          0x00U
#define  USB_REQ_RECIPIENT_INTERFACE                       0x01U
#define  USB_REQ_RECIPIENT_ENDPOINT                        0x02U
#define  USB_REQ_RECIPIENT_OTHER                           0x03U

/* Table 9-4. Standard Request Codes  */
/* bRequest , Value */
#define  USB_REQ_GET_STATUS                                0x00U
#define  USB_REQ_CLEAR_FEATURE                             0x01U
#define  USB_REQ_SET_FEATURE                               0x03U
#define  USB_REQ_SET_ADDRESS                               0x05U
#define  USB_REQ_GET_DESCRIPTOR                            0x06U
#define  USB_REQ_SET_DESCRIPTOR                            0x07U
#define  USB_REQ_GET_CONFIGURATION                         0x08U
#define  USB_REQ_SET_CONFIGURATION                         0x09U
#define  USB_REQ_GET_INTERFACE                             0x0AU
#define  USB_REQ_SET_INTERFACE                             0x0BU
#define  USB_REQ_SYNCH_FRAME                               0x0CU

/* Table 9-5. Descriptor Types of USB Specifications */
#ifndef USB_DESC_TYPE_DEVICE
#define  USB_DESC_TYPE_DEVICE                              0x01U
#endif
#ifndef USB_DESC_TYPE_CONFIGURATION
#define  USB_DESC_TYPE_CONFIGURATION                       0x02U
#endif
#ifndef USB_DESC_TYPE_STRING
#define  USB_DESC_TYPE_STRING                              0x03U
#endif
#ifndef USB_DESC_TYPE_INTERFACE
#define  USB_DESC_TYPE_INTERFACE                           0x04U
#endif
#ifndef USB_DESC_TYPE_ENDPOINT
#define  USB_DESC_TYPE_ENDPOINT                            0x05U
#endif
#ifndef USB_DESC_TYPE_DEVICE_QUALIFIER
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    0x06U
#endif
#ifndef USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           0x07U
#endif
#ifndef USB_DESC_TYPE_INTERFACE_POWER
#define  USB_DESC_TYPE_INTERFACE_POWER                     0x08U
#endif
#define  USB_DESC_TYPE_HID                                 0x21U
#define  USB_DESC_TYPE_HID_REPORT                          0x22U


#define USB_DEVICE_DESC_SIZE                               0x12U
#define USB_CONFIGURATION_DESC_SIZE                        0x09U
#define USB_HID_DESC_SIZE                                  0x09U
#define USB_INTERFACE_DESC_SIZE                            0x09U
#define USB_ENDPOINT_DESC_SIZE                             0x07U

/* Descriptor Type and Descriptor Index  */
/* Use the following values when calling the function USBH_GetDescriptor  */
#define  USB_DESC_DEVICE                    ((USB_DESC_TYPE_DEVICE << 8) & 0xFF00U)
#define  USB_DESC_CONFIGURATION             ((USB_DESC_TYPE_CONFIGURATION << 8) & 0xFF00U)
#define  USB_DESC_STRING                    ((USB_DESC_TYPE_STRING << 8) & 0xFF00U)
#define  USB_DESC_INTERFACE                 ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00U)
#define  USB_DESC_ENDPOINT                  ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00U)
#define  USB_DESC_DEVICE_QUALIFIER          ((USB_DESC_TYPE_DEVICE_QUALIFIER << 8) & 0xFF00U)
#define  USB_DESC_OTHER_SPEED_CONFIGURATION ((USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION << 8) & 0xFF00U)
#define  USB_DESC_INTERFACE_POWER           ((USB_DESC_TYPE_INTERFACE_POWER << 8) & 0xFF00U)
#define  USB_DESC_HID_REPORT                ((USB_DESC_TYPE_HID_REPORT << 8) & 0xFF00U)
#define  USB_DESC_HID                       ((USB_DESC_TYPE_HID << 8) & 0xFF00U)


#define  USB_EP_TYPE_CTRL                                  0x00U
#define  USB_EP_TYPE_ISOC                                  0x01U
#define  USB_EP_TYPE_BULK                                  0x02U
#define  USB_EP_TYPE_INTR                                  0x03U

#define  USB_EP_DIR_OUT                                    0x00U
#define  USB_EP_DIR_IN                                     0x80U
#define  USB_EP_DIR_MSK                                    0x80U

#ifndef USBH_MAX_PIPES_NBR
 #define USBH_MAX_PIPES_NBR                                15U
#endif /* USBH_MAX_PIPES_NBR */

#define USBH_DEVICE_ADDRESS_DEFAULT                        0x00U
#define USBH_DEVICE_ADDRESS                                0x01U

#define USBH_MAX_ERROR_COUNT                               0x02U


/**
  * @}
  */


#define USBH_CONFIGURATION_DESCRIPTOR_SIZE (USB_CONFIGURATION_DESC_SIZE \
                                           + USB_INTERFACE_DESC_SIZE\
                                           + (USBH_MAX_NUM_ENDPOINTS * USB_ENDPOINT_DESC_SIZE))


#define CONFIG_DESC_wTOTAL_LENGTH (ConfigurationDescriptorData.ConfigDescfield.\
                                          ConfigurationDescriptor.wTotalLength)


typedef union
{
  uint16_t w;
  struct BW
  {
    uint8_t msb;
    uint8_t lsb;
  }
  bw;
}
uint16_t_uint8_t;


typedef union _USB_Setup
{
  uint32_t d8[2];

  struct _SetupPkt_Struc
  {
    uint8_t           bmRequestType;
    uint8_t           bRequest;
    uint16_t_uint8_t  wValue;
    uint16_t_uint8_t  wIndex;
    uint16_t_uint8_t  wLength;
  } b;
}
USB_Setup_TypeDef;

typedef  struct  _DescHeader
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
}
USBH_DescHeader_t;

typedef struct _DeviceDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  bcdUSB;        /* USB Specification Number which device complies too */
  uint8_t   bDeviceClass;
  uint8_t   bDeviceSubClass;
  uint8_t   bDeviceProtocol;
  /* If equal to Zero, each interface specifies its own class
  code if equal to 0xFF, the class code is vendor specified.
  Otherwise field is valid Class Code.*/
  uint8_t   bMaxPacketSize;
  uint16_t  idVendor;      /* Vendor ID (Assigned by USB Org) */
  uint16_t  idProduct;     /* Product ID (Assigned by Manufacturer) */
  uint16_t  bcdDevice;     /* Device Release Number */
  uint8_t   iManufacturer;  /* Index of Manufacturer String Descriptor */
  uint8_t   iProduct;       /* Index of Product String Descriptor */
  uint8_t   iSerialNumber;  /* Index of Serial Number String Descriptor */
  uint8_t   bNumConfigurations; /* Number of Possible Configurations */
}
USBH_DevDescTypeDef;

typedef struct _EndpointDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint8_t   bEndpointAddress;   /* indicates what endpoint this descriptor is describing */
  uint8_t   bmAttributes;       /* specifies the transfer type. */
  uint16_t  wMaxPacketSize;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
  uint8_t   bInterval;          /* is used to specify the polling interval of certain transfers. */
}
USBH_EpDescTypeDef;

typedef struct _InterfaceDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;    /* Value used to select alternative setting */
  uint8_t bNumEndpoints;        /* Number of Endpoints used for this interface */
  uint8_t bInterfaceClass;      /* Class Code (Assigned by USB Org) */
  uint8_t bInterfaceSubClass;   /* Subclass Code (Assigned by USB Org) */
  uint8_t bInterfaceProtocol;   /* Protocol Code */
  uint8_t iInterface;           /* Index of String Descriptor Describing this interface */
  USBH_EpDescTypeDef               Ep_Desc[USBH_MAX_NUM_ENDPOINTS];
}
USBH_InterfaceDescTypeDef;


typedef struct _ConfigurationDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  wTotalLength;        /* Total Length of Data Returned */
  uint8_t   bNumInterfaces;       /* Number of Interfaces */
  uint8_t   bConfigurationValue;  /* Value to use as an argument to select this configuration*/
  uint8_t   iConfiguration;       /*Index of String Descriptor Describing this configuration */
  uint8_t   bmAttributes;         /* D7 Bus Powered , D6 Self Powered, D5 Remote Wakeup , D4..0 Reserved (0)*/
  uint8_t   bMaxPower;            /*Maximum Power Consumption */
  USBH_InterfaceDescTypeDef        Itf_Desc[USBH_MAX_NUM_INTERFACES];
}
USBH_CfgDescTypeDef;


/* Following USB Host status */
typedef enum
{
  USBH_OK = 0,
  USBH_BUSY,
  USBH_FAIL,
  USBH_NOT_SUPPORTED,
  USBH_UNRECOVERED_ERROR,
  USBH_ERROR_SPEED_UNKNOWN,
}USBH_StatusTypeDef;


/** @defgroup USBH_CORE_Exported_Types
  * @{
  */

typedef enum
{
  USBH_SPEED_HIGH  = 0U,
  USBH_SPEED_FULL  = 1U,
  USBH_SPEED_LOW   = 2U,

}USBH_SpeedTypeDef;

/* Following states are used for gState */
typedef enum
{
  HOST_IDLE = 0U,
  HOST_DEV_WAIT_FOR_ATTACHMENT,
  HOST_DEV_ATTACHED,
  HOST_DEV_DISCONNECTED,
  HOST_DETECT_DEVICE_SPEED,
  HOST_ENUMERATION,
  HOST_CLASS_REQUEST,
  HOST_INPUT,
  HOST_SET_CONFIGURATION,
  HOST_SET_WAKEUP_FEATURE,
  HOST_CHECK_CLASS,
  HOST_CLASS,
  HOST_SUSPENDED,
  HOST_ABORT_STATE,
}HOST_StateTypeDef;

/* Following states are used for EnumerationState */
typedef enum
{
  ENUM_IDLE = 0U,
  ENUM_GET_FULL_DEV_DESC,
  ENUM_SET_ADDR,
  ENUM_GET_CFG_DESC,
  ENUM_GET_FULL_CFG_DESC,
  ENUM_GET_MFC_STRING_DESC,
  ENUM_GET_PRODUCT_STRING_DESC,
  ENUM_GET_SERIALNUM_STRING_DESC,
} ENUM_StateTypeDef;

/* Following states are used for CtrlXferStateMachine */
typedef enum
{
  CTRL_IDLE = 0U,
  CTRL_SETUP,
  CTRL_SETUP_WAIT,
  CTRL_DATA_IN,
  CTRL_DATA_IN_WAIT,
  CTRL_DATA_OUT,
  CTRL_DATA_OUT_WAIT,
  CTRL_STATUS_IN,
  CTRL_STATUS_IN_WAIT,
  CTRL_STATUS_OUT,
  CTRL_STATUS_OUT_WAIT,
  CTRL_ERROR,
  CTRL_STALLED,
  CTRL_COMPLETE
}CTRL_StateTypeDef;


/* Following states are used for RequestState */
typedef enum
{
  CMD_IDLE = 0U,
  CMD_SEND,
  CMD_WAIT
} CMD_StateTypeDef;

typedef enum {
  USBH_URB_IDLE = 0U,
  USBH_URB_DONE,
  USBH_URB_NOTREADY,
  USBH_URB_NYET,
  USBH_URB_ERROR,
  USBH_URB_STALL
}USBH_URBStateTypeDef;

typedef enum
{
  USBH_PORT_EVENT = 1U,
  USBH_URB_EVENT,
  USBH_CONTROL_EVENT,
  USBH_CLASS_EVENT,
  USBH_STATE_CHANGED_EVENT,
}
USBH_OSEventTypeDef;

/* Control request structure */
typedef struct
{
  uint8_t               pipe_in;
  uint8_t               pipe_out;
  uint8_t               pipe_size;
  uint8_t               *buff;
  uint16_t              length;
  uint16_t              timer;
  USB_Setup_TypeDef     setup;
  CTRL_StateTypeDef     state;
  uint8_t               errorcount;

} USBH_CtrlTypeDef;

/* Attached device structure */
typedef struct
{
#if (USBH_KEEP_CFG_DESCRIPTOR == 1U)
  uint8_t                           CfgDesc_Raw[USBH_MAX_SIZE_CONFIGURATION];
#endif
  uint8_t                           Data[USBH_MAX_DATA_BUFFER];
  uint8_t                           address;
  uint8_t                           speed;
  __IO uint8_t                      is_connected;
  uint8_t                           PortEnabled;
  uint8_t                           current_interface;
  USBH_DevDescTypeDef               DevDesc;
  USBH_CfgDescTypeDef               CfgDesc;

}USBH_DeviceTypeDef;

struct _USBH_HandleTypeDef;

/* USB Host Class structure */
typedef struct
{
  const char          *Name;
  uint8_t              ClassCode;
  USBH_StatusTypeDef  (*Init)        (struct _USBH_HandleTypeDef *phost);
  USBH_StatusTypeDef  (*DeInit)      (struct _USBH_HandleTypeDef *phost);
  USBH_StatusTypeDef  (*Requests)    (struct _USBH_HandleTypeDef *phost);
  USBH_StatusTypeDef  (*BgndProcess) (struct _USBH_HandleTypeDef *phost);
  USBH_StatusTypeDef  (*SOFProcess) (struct _USBH_HandleTypeDef *phost);
  void*                pData;
} USBH_ClassTypeDef;

/* USB Host handle structure */
typedef struct _USBH_HandleTypeDef
{
  __IO HOST_StateTypeDef     gState;       /*  Host State Machine Value */
  ENUM_StateTypeDef     EnumState;    /* Enumeration state Machine */
  CMD_StateTypeDef      RequestState;
  USBH_CtrlTypeDef      Control;
  USBH_DeviceTypeDef    device;
  USBH_ClassTypeDef*    pClass[USBH_MAX_NUM_SUPPORTED_CLASS];
  USBH_ClassTypeDef*    pActiveClass;
  uint32_t              ClassNumber;
  uint32_t              Pipes[15];
  __IO uint32_t         Timer;
  uint8_t               id;
  void*                 pData;
  void                 (* pUser )(struct _USBH_HandleTypeDef *pHandle, uint8_t id);

#if (USBH_USE_OS == 1U)
  osMessageQId          os_event;
  osThreadId            thread;
#endif

} USBH_HandleTypeDef;


#if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */

#ifdef __cplusplus
}
#endif

#endif /* USBH_DEF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

