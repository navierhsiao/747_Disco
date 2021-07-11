#ifndef __FT6X06_REG_H
#define __FT6X06_REG_H

#include <stdint.h>

/* Current mode register of the FT6X06 (R/W) */
#define FT6X06_DEV_MODE_REG         0x00U

/* Gesture ID register */
#define FT6X06_GEST_ID_REG          0x01U

/* Touch Data Status register : gives number of active touch points (0..2) */
#define FT6X06_TD_STAT_REG          0x02U

/* P1 X, Y coordinates, weight and misc registers */
#define FT6X06_P1_XH_REG            0x03U
#define FT6X06_P1_XL_REG            0x04U
#define FT6X06_P1_YH_REG            0x05U
#define FT6X06_P1_YL_REG            0x06U
#define FT6X06_P1_WEIGHT_REG        0x07U
#define FT6X06_P1_MISC_REG          0x08U

/* P2 X, Y coordinates, weight and misc registers */
#define FT6X06_P2_XH_REG            0x09U
#define FT6X06_P2_XL_REG            0x0AU
#define FT6X06_P2_YH_REG            0x0BU
#define FT6X06_P2_YL_REG            0x0CU
#define FT6X06_P2_WEIGHT_REG        0x0DU
#define FT6X06_P2_MISC_REG          0x0EU

/* Threshold for touch detection */
#define FT6X06_TH_GROUP_REG         0x80U

/* Filter function coefficients */
#define FT6X06_TH_DIFF_REG          0x85U

/* Control register */
#define FT6X06_CTRL_REG             0x86U

/* The time period of switching from Active mode to Monitor mode when there is no touching */
#define FT6X06_TIMEENTERMONITOR_REG 0x87U

/* Report rate in Active mode */
#define FT6X06_PERIODACTIVE_REG     0x88U

/* Report rate in Monitor mode */
#define FT6X06_PERIODMONITOR_REG    0x89U

/* The value of the minimum allowed angle while Rotating gesture mode */
#define FT6X06_RADIAN_VALUE_REG     0x91U

/* Maximum offset while Moving Left and Moving Right gesture */
#define FT6X06_OFFSET_LR_REG        0x92U

/* Maximum offset while Moving Up and Moving Down gesture */
#define FT6X06_OFFSET_UD_REG        0x93U

/* Minimum distance while Moving Left and Moving Right gesture */
#define FT6X06_DISTANCE_LR_REG      0x94U

/* Minimum distance while Moving Up and Moving Down gesture */
#define FT6X06_DISTANCE_UD_REG      0x95U

/* Maximum distance while Zoom In and Zoom Out gesture */
#define FT6X06_DISTANCE_ZOOM_REG    0x96U

/* High 8-bit of LIB Version info */
#define FT6X06_LIB_VER_H_REG        0xA1U

/* Low 8-bit of LIB Version info */
#define FT6X06_LIB_VER_L_REG        0xA2U

/* Chip Selecting */
#define FT6X06_CIPHER_REG           0xA3U

/* Interrupt mode register (used when in interrupt mode) */
#define FT6X06_GMODE_REG            0xA4U

/* Current power mode the FT6X06 system is in (R) */
#define FT6X06_PWR_MODE_REG         0xA5U

/* FT6X06 firmware version */
#define FT6X06_FIRMID_REG           0xA6U

/* FT6X06 Chip identification register */
#define FT6X06_CHIP_ID_REG          0xA8U

/* Release code version */
#define FT6X06_RELEASE_CODE_ID_REG  0xAFU

/* Current operating mode the FT6X06 system is in (R) */
#define FT6X06_STATE_REG            0xBCU

/**************** Base Function  *******************/

#define   FT6X06_DEV_MODE_BIT_MASK        0x70U
#define   FT6X06_DEV_MODE_BIT_POSITION    4U

#define   FT6X06_GEST_ID_BIT_MASK        0xFFU
#define   FT6X06_GEST_ID_BIT_POSITION    0U

#define   FT6X06_TD_STATUS_BIT_MASK        0x0FU
#define   FT6X06_TD_STATUS_BIT_POSITION    0U

#define   FT6X06_P1_XH_EF_BIT_MASK        0xC0U
#define   FT6X06_P1_XH_EF_BIT_POSITION    6U

#define   FT6X06_P1_XH_TP_BIT_MASK        0x0FU
#define   FT6X06_P1_XH_TP_BIT_POSITION    0U

#define   FT6X06_P1_XL_TP_BIT_MASK        0xFFU
#define   FT6X06_P1_XL_TP_BIT_POSITION    0U

#define   FT6X06_P1_YH_TID_BIT_MASK        0xF0U
#define   FT6X06_P1_YH_TID_BIT_POSITION    4U

#define   FT6X06_P1_YH_TP_BIT_MASK        0x0FU
#define   FT6X06_P1_YH_TP_BIT_POSITION    0U

#define   FT6X06_P1_YL_TP_BIT_MASK        0xFFU
#define   FT6X06_P1_YL_TP_BIT_POSITION    0U

#define   FT6X06_P1_WEIGHT_BIT_MASK        0xFFU
#define   FT6X06_P1_WEIGHT_BIT_POSITION    0U

#define   FT6X06_P1_MISC_BIT_MASK        0xF0U
#define   FT6X06_P1_MISC_BIT_POSITION    4U

#define   FT6X06_P2_XH_EF_BIT_MASK        0xC0U
#define   FT6X06_P2_XH_EF_BIT_POSITION    6U

#define   FT6X06_P2_XH_TP_BIT_MASK        0x0FU
#define   FT6X06_P2_XH_TP_BIT_POSITION    0U

#define   FT6X06_P2_XL_TP_BIT_MASK        0xFFU
#define   FT6X06_P2_XL_TP_BIT_POSITION    0U

#define   FT6X06_P2_YH_TID_BIT_MASK        0xF0U
#define   FT6X06_P2_YH_TID_BIT_POSITION    4U

#define   FT6X06_P2_YH_TP_BIT_MASK        0x0FU
#define   FT6X06_P2_YH_TP_BIT_POSITION    0U

#define   FT6X06_P2_YL_TP_BIT_MASK        0xFFU
#define   FT6X06_P2_YL_TP_BIT_POSITION    0U

#define   FT6X06_P2_WEIGHT_BIT_MASK        0xFFU
#define   FT6X06_P2_WEIGHT_BIT_POSITION    0U

#define   FT6X06_P2_MISC_BIT_MASK        0xF0U
#define   FT6X06_P2_MISC_BIT_POSITION    4U

#define   FT6X06_TH_GROUP_BIT_MASK        0xFFU
#define   FT6X06_TH_GROUP_BIT_POSITION    0U

#define   FT6X06_TH_DIFF_BIT_MASK        0xFFU
#define   FT6X06_TH_DIFF_BIT_POSITION    0U

#define   FT6X06_CTRL_BIT_MASK           0xFFU
#define   FT6X06_CTRL_BIT_POSITION       0U

#define   FT6X06_TIMEENTERMONITOR_BIT_MASK           0xFFU
#define   FT6X06_TIMEENTERMONITOR_BIT_POSITION       0U

#define   FT6X06_PERIODACTIVE_BIT_MASK           0xFFU
#define   FT6X06_PERIODACTIVE_BIT_POSITION       0U

#define   FT6X06_PERIODMONITOR_BIT_MASK           0xFFU
#define   FT6X06_PERIODMONITOR_BIT_POSITION       0U

#define   FT6X06_RADIAN_VALUE_BIT_MASK           0xFFU
#define   FT6X06_RADIAN_VALUE_BIT_POSITION       0U

#define   FT6X06_OFFSET_LR_BIT_MASK           0xFFU
#define   FT6X06_OFFSET_LR_BIT_POSITION       0U

#define   FT6X06_OFFSET_UD_BIT_MASK           0xFFU
#define   FT6X06_OFFSET_UD_BIT_POSITION       0U

#define   FT6X06_DISTANCE_LR_BIT_MASK           0xFFU
#define   FT6X06_DISTANCE_LR_BIT_POSITION       0U

#define   FT6X06_DISTANCE_UD_BIT_MASK           0xFFU
#define   FT6X06_DISTANCE_UD_BIT_POSITION       0U

#define   FT6X06_DISTANCE_ZOOM_BIT_MASK           0xFFU
#define   FT6X06_DISTANCE_ZOOM_BIT_POSITION       0U

#define   FT6X06_LIB_VER_H_BIT_MASK           0xFFU
#define   FT6X06_LIB_VER_H_BIT_POSITION       0U

#define   FT6X06_LIB_VER_L_BIT_MASK           0xFFU
#define   FT6X06_LIB_VER_L_BIT_POSITION       0U

#define   FT6X06_CIPHER_BIT_MASK           0xFFU
#define   FT6X06_CIPHER_BIT_POSITION       0U

#define   FT6X06_G_MODE_BIT_MASK           0xFFU
#define   FT6X06_G_MODE_BIT_POSITION       0U

#define   FT6X06_PWR_MODE_BIT_MASK           0xFFU
#define   FT6X06_PWR_MODE_BIT_POSITION       0U

#define   FT6X06_FIRMID_BIT_MASK           0xFFU
#define   FT6X06_FIRMID_BIT_POSITION       0U

#define   FT6X06_CHIP_ID_BIT_MASK           0xFFU
#define   FT6X06_CHIP_ID_BIT_POSITION       0U

#define   FT6X06_RC_ID_BIT_MASK           0xFFU
#define   FT6X06_RC_ID_BIT_POSITION       0U

#define   FT6X06_STATE_BIT_MASK           0xFFU
#define   FT6X06_STATE_BIT_POSITION       0U

#endif