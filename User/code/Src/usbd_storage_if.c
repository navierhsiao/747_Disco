#include "usbd_storage_if.h"
#include "../../../BSP/system.h"

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200


const int8_t STORAGE_Inquirydata_HS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};

sdmmc_objectTypeDef *sd_object;
extern USBD_HandleTypeDef hUsbDeviceHS;

static int8_t STORAGE_Init_HS(uint8_t lun);
static int8_t STORAGE_GetCapacity_HS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_HS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_HS(uint8_t lun);
static int8_t STORAGE_Read_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_HS(void);

USBD_StorageTypeDef USBD_Storage_Interface_fops_HS =
{
  STORAGE_Init_HS,
  STORAGE_GetCapacity_HS,
  STORAGE_IsReady_HS,
  STORAGE_IsWriteProtected_HS,
  STORAGE_Read_HS,
  STORAGE_Write_HS,
  STORAGE_GetMaxLun_HS,
  (int8_t *)STORAGE_Inquirydata_HS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_HS(uint8_t lun)
{
  /* USER CODE BEGIN 9 */
  if(IS_SD_INSERTED==0)
  {
    sd_object=sdmmc_object_init();
  }
  return (USBD_OK);
  /* USER CODE END 9 */
}

/**
  * @brief  .
  * @param  lun: .
  * @param  block_num: .
  * @param  block_size: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_HS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 10 */
  int status=-1;

  if(IS_SD_INSERTED==0)
  {
    sd_object->sdmmc_get_cardInfo(sd_object);
    *block_num  = sd_object->cardInfo.LogBlockNbr-1;
    *block_size = sd_object->cardInfo.LogBlockSize;
    status=0;
  }
  
  return status;
  /* USER CODE END 10 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_HS(uint8_t lun)
{
  /* USER CODE BEGIN 11 */
  int status=-1;
  static int8_t prev_status = 0;

  if (IS_SD_INSERTED==0)
  {
    if (prev_status < 0)
    {
      sd_object=sdmmc_object_init();
      prev_status = 0;

    }

    sd_object->sdmmc_get_card_state(sd_object);
    if (sd_object->state == SD_TRANSFER_OK)
    {
      status = 0;
    }
  }
  else if (prev_status == 0)
  {
    prev_status = -1;
  }

  return status;
  /* USER CODE END 11 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_HS(uint8_t lun)
{
  /* USER CODE BEGIN 12 */
  return 0;
  /* USER CODE END 12 */
}

/**
  * @brief  .
  * @param  lun: .
  * @param  buf: .
  * @param  blk_addr: .
  * @param  blk_len: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  int state = -1;

  if (IS_SD_INSERTED==0)
  {
    sd_object->sdmmc_readBlocks_DMA(sd_object,(uint32_t *) buf,blk_addr,blk_len);
    
    while (sd_object->state != SD_TRANSFER_OK)
    {
      sd_object->sdmmc_get_card_state(sd_object);
    }

    state = 0;
  }
  return state;
}

/**
  * @brief  .
  * @param  lun: .
  * @param  buf: .
  * @param  blk_addr: .
  * @param  blk_len: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  int8_t state = -1;

  if (IS_SD_INSERTED==0)
  {
    sd_object->sdmmc_writeBlocks_DMA(sd_object,(uint32_t *) buf,blk_addr,blk_len);
    
    while (sd_object->state != SD_TRANSFER_OK)
    {
      sd_object->sdmmc_get_card_state(sd_object);
    }

    state = 0;
  }
  return state;
}

/**
  * @brief  .
  * @param  None
  * @retval .
  */
int8_t STORAGE_GetMaxLun_HS(void)
{
  /* USER CODE BEGIN 15 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 15 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
