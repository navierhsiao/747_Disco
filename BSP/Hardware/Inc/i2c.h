#ifndef __I2C_H
#define __I2C_H

enum i2c_error_state
{
    I2C_ERROR_NONE                  =0,
    I2C_ERROR                       =1,
    I2C_ERROR_ACKNOWLEDGE_FAILURE   =2,
    I2C_ERROR_PERIPH_FAILURE        =3,
    I2C_ERROR_BUSY                  =4
};

typedef struct
{
    I2C_TypeDef     *Instance;
    //init structure
    uint32_t        Timing;    
    uint32_t        OwnAddress1;   
    uint32_t        AddressingMode;   
    uint32_t        DualAddressMode;   
    uint32_t        OwnAddress2;      
    uint32_t        OwnAddress2Masks;  
    uint32_t        GeneralCallMode;   
    uint32_t        NoStretchMode;   
}i2c_objectAttr;

typedef struct
{
    I2C_HandleTypeDef hi2c;
    DMA_HandleTypeDef hdma_i2c_rx;
    DMA_HandleTypeDef hdma_i2c_tx;
}i2c_objectHandle;
typedef struct  i2c_objectStructDef
{
    i2c_objectHandle handle;
    i2c_objectAttr object_attr;

    uint8_t i2c_status;

    void (*i2c_init)        (struct i2c_objectStructDef*);
    void (*i2c_writeReg)    (struct i2c_objectStructDef*,uint16_t ,uint16_t ,uint8_t *,uint16_t ,uint16_t );
    void (*i2c_readReg)     (struct i2c_objectStructDef*,uint16_t ,uint16_t ,uint8_t *,uint16_t ,uint16_t );
    void (*i2c_isReady)     (struct i2c_objectStructDef*,uint16_t , uint32_t );
}i2c_objectTypeDef;

void I2C_Object_Init(i2c_objectTypeDef* object,i2c_objectAttr attr);

#endif