#ifndef __TOUCH_H
#define __TOUCH_H

#include "../../Hardware/Inc/i2c.h"

#define MAX_WIDTH           480
#define MAX_HEIGHT          854
#define MAX_TOUCH           3

#define GT_INT          HAL_GPIO_ReadPin(GPIOK,GPIO_PIN_7)  

#define GT_CMD_WR 		0XBA     	
#define GT_CMD_RD 		0XBB	

// #define GT_CMD_WR 		0X28	
// #define GT_CMD_RD 		0X29	

#define GT_CTRL_REG 	0X8040   	
#define GT_CFGS_REG 	0X8047   
#define GT_CHECK_REG 	0X80FF  
#define GT_PID_REG 		0X8140   

#define GT_GSTID_REG 	0X814E   	
#define GT_TP1_REG 		0X8150  
#define GT_TP2_REG 		0X8158		
#define GT_TP3_REG 		0X8160	
#define GT_TP4_REG 		0X8168	
#define GT_TP5_REG 		0X8170	


typedef struct touch_structDef
{
    i2c_objectTypeDef i2c_object;
    uint8_t     touch_state;
    uint16_t    x[MAX_TOUCH];
    uint16_t    y[MAX_TOUCH];
    uint16_t    chip_id;
    uint8_t     numOfPoints;
    uint8_t     orientation;
    uint8_t     chip_wr_reg;
    uint8_t     chip_rd_reg;

    void (*touch_get_chipID)    (struct touch_structDef *object);
    void (*touch_scanState)     (struct touch_structDef *object);
    void (*touch_read_reg)      (struct touch_structDef *object,uint16_t reg,uint8_t *data,uint8_t length);
    void (*touch_write_reg)     (struct touch_structDef *object,uint16_t reg,uint8_t *data,uint8_t length);
}touch_objectTypeDef;

void touch_object_init(touch_objectTypeDef *object,uint8_t orientation);
#endif