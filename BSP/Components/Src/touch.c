#include "../../system.h"
#include "ft6x06_reg.h"

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

const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

void touch_object_init(touch_objectTypeDef *object,uint8_t orientation)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    I2C_Object_Init(&object->i2c_object,i2c_object_attr);

    object->touch_get_chipID=touch_get_chipID;
    object->touch_scanState=touch_scanState;
    object->touch_read_reg=touch_read_reg;
    object->touch_write_reg=touch_write_reg;
    object->orientation=orientation;

    object->numOfPoints=3;
    //touch INT-無上下拉
    __HAL_RCC_GPIOK_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    HAL_Delay(100);
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    HAL_Delay(200);

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
    uint8_t data0[4]={0};
    uint8_t data1[4]={0};
    uint8_t regTemp[2];
    uint16_t reg=GT_PID_REG;
    uint8_t chip_wr_addr1=0xBA;
    uint8_t chip_rd_addr1=0xBB;

    uint8_t chip_wr_addr2=0x28;
    uint8_t chip_rd_addr2=0x29;

    regTemp[0]=(uint8_t)(reg>>8);
    regTemp[1]=(uint8_t)(reg&0xFF);

    object->i2c_object.i2c_write(&object->i2c_object,chip_wr_addr1,regTemp,2);
    object->i2c_object.i2c_read(&object->i2c_object,chip_rd_addr1,data0,4);

    object->i2c_object.i2c_write(&object->i2c_object,chip_wr_addr2,regTemp,2);
    object->i2c_object.i2c_read(&object->i2c_object,chip_rd_addr2,data1,4);

    
    if(atoi(data0)==911)
    {
        object->chip_id=911;
        object->chip_wr_reg=chip_wr_addr1;
        object->chip_rd_reg=chip_rd_addr1;

        data0[0]=0x02;			
        object->touch_write_reg(object,GT_CTRL_REG,data0,1);
        object->touch_read_reg(object,GT_CFGS_REG,data0,1);
        HAL_Delay(10);
        data0[0]=0x00;	 
        object->touch_write_reg(object,GT_CTRL_REG,data0,1);
        return 0;
    } 
    else
    {
        if(atoi(data1)==911)
        {
            object->chip_id=911;
            object->chip_wr_reg=chip_wr_addr2;
            object->chip_rd_reg=chip_rd_addr2;

            data1[0]=0x02;			
            object->touch_write_reg(object,GT_CTRL_REG,data1,1);
            object->touch_read_reg(object,GT_CFGS_REG,data1,1);
            HAL_Delay(10);
            data1[0]=0x00;	 
            object->touch_write_reg(object,GT_CTRL_REG,data1,1);
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

    object->i2c_object.i2c_write(&object->i2c_object,object->chip_wr_reg,regTemp,2);
    object->i2c_object.i2c_read(&object->i2c_object,object->chip_rd_reg,data,length);
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

    object->i2c_object.i2c_write(&object->i2c_object,object->chip_wr_reg,buffer,2+length);
}

void touch_scanState(touch_objectTypeDef *object)
{
    uint8_t posData[4];
    uint8_t statusReg=0;

    object->touch_read_reg(object,GT_GSTID_REG,&statusReg,1);
    //檢測bit 7是否為1，若為1需要將其設為0，否則不會更新下次數據
    if(statusReg&(1<<7))
    {
        uint8_t wrData=0;
        object->touch_write_reg(object,GT_GSTID_REG,&wrData,1);
    }

    //檢測到至少有一點觸碰且觸碰的數量小於6
    if((statusReg&0xF)&&((statusReg&0xF)<6))
    {
    for(int i=0;i<object->numOfPoints;i++)
    {
        if(statusReg&(1<<i))	
        {
            object->touch_state=1;	
            object->touch_read_reg(object,GT9147_TPX_TBL[i],posData,4);
            //直
            if(object->orientation==0)
            {
                object->y[i]=854-(((uint16_t)posData[3]<<8)+posData[2]);
                object->x[i]=(((uint16_t)posData[1]<<8)+posData[0]);
            }
            //橫
            else if(object->orientation==1)
            {
                object->y[i]=(((uint16_t)posData[1]<<8)+posData[0]);
                object->x[i]=(((uint16_t)posData[3]<<8)+posData[2]);
            }         
        }		
    }
    }
    else
    { 
        //觸碰結束
        if((statusReg&0X8F)==0X80)
        { 
            if(object->touch_state=1)	
            {
                object->x[0]=0xffff;
                object->y[0]=0xffff;
                object->touch_state=0;
            }
        } 	
    }
}