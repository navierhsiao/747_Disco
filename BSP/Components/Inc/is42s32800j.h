#ifndef __IS42S32800J_H
#define __IS42S32800J_H

#include "../../Hardware/Inc/sdram.h"

#define IS42S32800J_BURST_LENGTH_1              0x00000000U
#define IS42S32800J_BURST_LENGTH_2              0x00000001U
#define IS42S32800J_BURST_LENGTH_4              0x00000002U
#define IS42S32800J_BURST_LENGTH_8              0x00000004U
#define IS42S32800J_BURST_TYPE_SEQUENTIAL       0x00000000U
#define IS42S32800J_BURST_TYPE_INTERLEAVED      0x00000008U
#define IS42S32800J_CAS_LATENCY_2               0x00000020U
#define IS42S32800J_CAS_LATENCY_3               0x00000030U
#define IS42S32800J_OPERATING_MODE_STANDARD     0x00000000U
#define IS42S32800J_WRITEBURST_MODE_PROGRAMMED  0x00000000U 
#define IS42S32800J_WRITEBURST_MODE_SINGLE      0x00000200U 

#define IS42S32800J_NORMAL_MODE_CMD             0x00000000U
#define IS42S32800J_CLK_ENABLE_CMD              0x00000001U
#define IS42S32800J_PALL_CMD                    0x00000002U
#define IS42S32800J_AUTOREFRESH_MODE_CMD        0x00000003U
#define IS42S32800J_LOAD_MODE_CMD               0x00000004U
#define IS42S32800J_SELFREFRESH_MODE_CMD        0x00000005U
#define IS42S32800J_POWERDOWN_MODE_CMD          0x00000006U

typedef struct 
{
    uint32_t TargetBank;           /*!< Target Bank                             */
    uint32_t RefreshMode;          /*!< Refresh Mode                            */
    uint32_t RefreshRate;          /*!< Refresh Rate                            */
    uint32_t BurstLength;          /*!< Burst Length                            */
    uint32_t BurstType;            /*!< Burst Type                              */
    uint32_t CASLatency;           /*!< CAS Latency                             */
    uint32_t OperationMode;        /*!< Operation Mode                          */
    uint32_t WriteBurstMode;       /*!< Write Burst Mode                        */
} IS42S32800J_Context_t;

void IS42S32800J_Init(void);

#endif