#include "../../system.h"
#include "ft6x06_reg.h"

touch_objectTypeDef touch;


const i2c_objectAttr i2c_object_attr={
    .Instance           =I2C4,
    .Timing             =0x70B0383C,
    .OwnAddress1        =0,   
    .AddressingMode     =I2C_ADDRESSINGMODE_7BIT,   
    .DualAddressMode    =I2C_DUALADDRESS_DISABLE,   
    .OwnAddress2        =0,      
    .OwnAddress2Masks   =I2C_OA2_NOMASK,  
    .GeneralCallMode    =I2C_GENERALCALL_DISABLE,   
    .NoStretchMode      =I2C_NOSTRETCH_DISABLE   
};

void touch_scanState(touch_objectTypeDef *object);
void touch_get_chipID(touch_objectTypeDef *object);

void touch_object_init(touch_objectTypeDef *object,uint8_t orientation)
{
    I2C_Object_Init(&object->i2c_object,i2c_object_attr);

    object->touch_get_chipID=touch_get_chipID;
    object->touch_scanState=touch_scanState;
    object->orientation=orientation;

    object->touch_get_chipID(object);
}

void touch_get_chipID(touch_objectTypeDef *object)
{
    object->i2c_object.i2c_readReg(&object->i2c_object,touch_addr,FT6X06_CHIP_ID_REG,&object->chip_id,I2C_MEMADD_SIZE_8BIT,1);
    object->chip_id &= FT6X06_CHIP_ID_BIT_MASK;
    object->chip_id = object->chip_id>>FT6X06_CHIP_ID_BIT_POSITION;
}

void touch_scanState(touch_objectTypeDef *object)
{
    uint8_t data[12]={0};
    object->i2c_object.i2c_readReg(&object->i2c_object,touch_addr,FT6X06_TD_STAT_REG,&object->touch_state,I2C_MEMADD_SIZE_8BIT,1);
    object->touch_state &= FT6X06_TD_STATUS_BIT_MASK;
    object->touch_state = object->touch_state>>FT6X06_TD_STATUS_BIT_POSITION;
    
    if(object->touch_state>MAX_TOUCH)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    if(object->touch_state==1)
    {
        object->i2c_object.i2c_readReg(&object->i2c_object,touch_addr,FT6X06_P1_XH_REG,data,I2C_MEMADD_SIZE_8BIT,4);

        if(object->orientation==OTM8009A_ORIENTATION_PORTRAIT)
        {
            //直立
            object->x[0]=(((uint16_t)data[0]&0xF)<<8) + data[1];
            object->y[0]=(((uint16_t)data[2]&0xF)<<8) + data[3];
        }
        else if(object->orientation==OTM8009A_ORIENTATION_LANDSCAPE)
        {
            //水平
            object->y[0]=(((uint16_t)data[0]&0xF)<<8) + data[1];
            object->x[0]=(((uint16_t)data[2]&0xF)<<8) + data[3];
        }     
    }
    else if(object->touch_state==2)
    {
        object->i2c_object.i2c_readReg(&object->i2c_object,touch_addr,FT6X06_P1_XH_REG,data,I2C_MEMADD_SIZE_8BIT,12);
        object->x[0]=(((uint32_t)data[0] & FT6X06_P1_XH_TP_BIT_MASK) << 8) | ((uint32_t)data[1] & FT6X06_P1_XL_TP_BIT_MASK);
        object->y[0]=(((uint32_t)data[2] & FT6X06_P1_YH_TP_BIT_MASK) << 8) | ((uint32_t)data[3] & FT6X06_P1_YL_TP_BIT_MASK);
        
        object->x[1]=(((uint32_t)data[6] & FT6X06_P1_XH_TP_BIT_MASK) << 8) | ((uint32_t)data[7] & FT6X06_P1_XL_TP_BIT_MASK);
        object->y[1]=(((uint32_t)data[8] & FT6X06_P1_YH_TP_BIT_MASK) << 8) | ((uint32_t)data[9] & FT6X06_P1_YL_TP_BIT_MASK);
        if(object->orientation==OTM8009A_ORIENTATION_PORTRAIT)
        {
            //直立
            object->x[0]=(((uint16_t)data[0]&0xF)<<8) + data[1];
            object->y[0]=(((uint16_t)data[2]&0xF)<<8) + data[3];

            object->x[1]=(((uint16_t)data[6]&0xF)<<8) + data[7];
            object->y[1]=(((uint16_t)data[8]&0xF)<<8) + data[9];
        }
        else if(object->orientation==OTM8009A_ORIENTATION_LANDSCAPE)
        {
            //水平
            object->y[0]=(((uint16_t)data[0]&0xF)<<8) + data[1];
            object->x[0]=(((uint16_t)data[2]&0xF)<<8) + data[3];

            object->y[1]=(((uint16_t)data[6]&0xF)<<8) + data[8];
            object->x[1]=(((uint16_t)data[7]&0xF)<<8) + data[9];
        }     
    }
}