/**************************************************************************//**
 * @file udelay.h
 * @brief Microsecond delay routine.
 * @version 4.1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#ifndef __UDELAY_H
#define __UDELAY_H

#include <stdint.h>

/***************************************************************************//**
 * @addtogroup Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Udelay
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void UDELAY_Calibrate(void);
void UDELAY_Delay(uint32_t usecs);

#ifdef __cplusplus
}
#endif

/** @} (end group Udelay) */
/** @} (end group Drivers) */

#endif
