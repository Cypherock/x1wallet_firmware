/**
 * \file
 * \brief a set of default configurations for various ATCA devices and interfaces
 *
 * \copyright (c) 2017 Microchip Technology Inc. and its subsidiaries.
 *            You may use this software and any derivatives exclusively with
 *            Microchip products.
 *
 * \page License
 *
 * (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use this
 * software and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIPS TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */


#ifndef ATCA_CFGS_H_
#define ATCA_CFGS_H_

#include "atca_iface.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ATCAIfaceCfg *cfg_atecc608a_iface;

/** \brief default configuration for an ECCx08A device on the first logical I2C bus */
extern ATCAIfaceCfg cfg_ateccx08a_i2c_default;
extern ATCAIfaceCfg cfg_ateccx08a_i2c_def;

/** \brief default configuration for an ECCx08A device on the logical SWI bus over UART*/
extern ATCAIfaceCfg cfg_ateccx08a_swi_default;

/** \brief default configuration for Kit protocol over a CDC interface */
extern ATCAIfaceCfg cfg_ateccx08a_kitcdc_default;

/** \brief default configuration for Kit protocol over a HID interface */
extern ATCAIfaceCfg cfg_ateccx08a_kithid_default;


/** \brief default configuration for a SHA204A device on the first logical I2C bus */
extern ATCAIfaceCfg cfg_atsha204a_i2c_default;

/** \brief default configuration for an SHA204A device on the logical SWI bus over UART*/
extern ATCAIfaceCfg cfg_atsha204a_swi_default;

/** \brief default configuration for Kit protocol over a CDC interface */
extern ATCAIfaceCfg cfg_atsha204a_kitcdc_default;

/** \brief default configuration for Kit protocol over a HID interface for SHA204 */
extern ATCAIfaceCfg cfg_atsha204a_kithid_default;

#ifdef __cplusplus
}
#endif
#endif /* ATCA_CFGS_H_ */