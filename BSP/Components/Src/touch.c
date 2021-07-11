#include "../../system.h"
#include "ft6x06_reg.h"

touch_objectTypeDef touch;
i2c_objectTypeDef i2c_object;

const i2c_objectAttr i2c_object_attr={
    .Instance           =I2C4,
    .Timing             =0x10C0ECFF,
    .OwnAddress1        =0,   
    .AddressingMode     =I2C_ADDRESSINGMODE_7BIT,   
    .DualAddressMode    =I2C_DUALADDRESS_DISABLE,   
    .OwnAddress2        =0,      
    .OwnAddress2Masks   =I2C_OA2_NOMASK,  
    .GeneralCallMode    =I2C_GENERALCALL_DISABLE,   
    .NoStretchMode      =I2C_NOSTRETCH_DISABLE   
};

int touch_get_horizontal_offset(touch_objectTypeDef* obj);

void touch_init(void)
{

    I2C_Object_Init(&i2c_object,i2c_object_attr);

    i2c_object.i2c_init;
    touch.get_touch_offset=touch_get_horizontal_offset;
    touch.get_touch_offset(&touch);
}

int touch_get_horizontal_offset(touch_objectTypeDef* obj)
{
    return 3;
}