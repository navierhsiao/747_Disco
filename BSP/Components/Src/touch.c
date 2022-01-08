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
void touch_read_reg(touch_objectTypeDef *object,uint16_t reg,uint8_t *data,uint8_t length);
void touch_write_reg(touch_objectTypeDef *object,uint16_t reg,uint8_t *data,uint8_t length);
uint8_t touch_probe_chip_reg_Addr(touch_objectTypeDef *object);

void touch_object_init(touch_objectTypeDef *object,uint8_t orientation)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    I2C_Object_Init(&object->i2c_object,i2c_object_attr);

    object->touch_get_chipID=touch_get_chipID;
    object->touch_scanState=touch_scanState;
    object->touch_read_reg=touch_read_reg;
    object->touch_write_reg=touch_write_reg;
    object->orientation=orientation;

    //touch INT-無上下拉
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    HAL_Delay(100);

    uint8_t result=touch_probe_chip_reg_Addr(object);
    // uint8_t temp[4]={0};
    // object->touch_read_reg(object,GT_PID_REG,temp,4);
    
    // object->touch_get_chipID(object);
}

void touch_get_chipID(touch_objectTypeDef *object)
{

}

uint8_t touch_probe_chip_reg_Addr(touch_objectTypeDef *object)
{
    uint8_t data[4]={0};
    uint8_t regTemp[2];
    uint16_t reg=GT_PID_REG;
    uint8_t chip_wr_addr1=0xBA;
    uint8_t chip_rd_addr1=0xBB;

    uint8_t chip_wr_addr2=0x28;
    uint8_t chip_rd_addr2=0x29;

    regTemp[0]=(uint8_t)(reg>>8);
    regTemp[1]=(uint8_t)(reg&0xFF);

    if(object->i2c_object.i2c_write(&object->i2c_object,chip_wr_addr2,regTemp,2)==HAL_OK)
    {
        object->chip_wr_reg=chip_wr_addr2;
        object->chip_rd_reg=chip_rd_addr2;
        object->i2c_object.i2c_read(&object->i2c_object,chip_rd_addr2,data,4);
        return 0;
    }
    else
    {
        if(object->i2c_object.i2c_write(&object->i2c_object,chip_wr_addr1,regTemp,2)==HAL_OK)
        {
            object->chip_wr_reg=chip_wr_addr1;
            object->chip_rd_reg=chip_rd_addr1;
            object->i2c_object.i2c_read(&object->i2c_object,chip_rd_addr1,data,4);
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

void touch_read_reg(touch_objectTypeDef *object,uint16_t reg,uint8_t *data,uint8_t length)
{
    uint8_t regTemp[2];
    regTemp[0]=(uint8_t)(reg>>8);
    regTemp[1]=(uint8_t)(reg&0xFF);

    object->i2c_object.i2c_write(&object->i2c_object,GT_CMD_WR,regTemp,2);
    object->i2c_object.i2c_read(&object->i2c_object,GT_CMD_RD,data,length);
}

void touch_write_reg(touch_objectTypeDef *object,uint16_t reg,uint8_t *data,uint8_t length)
{
    uint8_t buffer[2+length];
    buffer[0]=(uint8_t)(reg>>8);
    buffer[1]=(uint8_t)(reg&0xFF);

    for(int i=0;i<length;i++)
    {
    buffer[2+i]=data[i];
    }

    object->i2c_object.i2c_write(&object->i2c_object,GT_CMD_WR,buffer,2+length);
}

void touch_scanState(touch_objectTypeDef *object)
{

}