#ifndef __MT25TL01G_H
#define __MT25TL01G_H

#define CONF_MT25TL01G_READ_ENHANCE      0                       /* MMP performance enhance reade enable/disable */

#define CONF_QSPI_ODS                   MT25TL01G_CR_ODS_15

#define CONF_QSPI_DUMMY_CLOCK                 8U

/* Dummy cycles for STR read mode */
#define MT25TL01G_DUMMY_CYCLES_READ_QUAD      8U
#define MT25TL01G_DUMMY_CYCLES_READ           8U
/* Dummy cycles for DTR read mode */
#define MT25TL01G_DUMMY_CYCLES_READ_DTR       6U
#define MT25TL01G_DUMMY_CYCLES_READ_QUAD_DTR  8U

#define MT25TL01G_FLASH_SIZE                  0x8000000 /* 2 * 512 MBits => 2 * 64MBytes => 128MBytes*/
#define MT25TL01G_SECTOR_SIZE                 0x10000   /* 2 * 1024 sectors of 64KBytes */
#define MT25TL01G_SUBSECTOR_SIZE              0x1000    /* 2 * 16384 subsectors of 4kBytes */
#define MT25TL01G_PAGE_SIZE                   0x100     /* 2 * 262144 pages of 256 bytes */

#define MT25TL01G_DIE_ERASE_MAX_TIME          460000
#define MT25TL01G_SECTOR_ERASE_MAX_TIME       1000
#define MT25TL01G_SUBSECTOR_ERASE_MAX_TIME    400

/**
  * @brief  MT25TL01G Commands
  */
/* Reset Operations */
#define MT25TL01G_RESET_ENABLE_CMD                     0x66
#define MT25TL01G_RESET_MEMORY_CMD                     0x99

/* Identification Operations */
#define MT25TL01G_READ_ID_CMD                          0x9E
#define MT25TL01G_READ_ID_CMD2                         0x9F
#define MT25TL01G_MULTIPLE_IO_READ_ID_CMD              0xAF
#define MT25TL01G_READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5A

/* Read Operations */
#define MT25TL01G_READ_CMD                             0x03
#define MT25TL01G_READ_4_BYTE_ADDR_CMD                 0x13

#define MT25TL01G_FAST_READ_CMD                        0x0B
#define MT25TL01G_FAST_READ_DTR_CMD                    0x0D
#define MT25TL01G_FAST_READ_4_BYTE_ADDR_CMD            0x0C
#define MT25TL01G_FAST_READ_4_BYTE_DTR_CMD             0x0E

#define MT25TL01G_DUAL_OUT_FAST_READ_CMD               0x3B
#define MT25TL01G_DUAL_OUT_FAST_READ_DTR_CMD           0x3D
#define MT25TL01G_DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x3C

#define MT25TL01G_DUAL_INOUT_FAST_READ_CMD             0xBB
#define MT25TL01G_DUAL_INOUT_FAST_READ_DTR_CMD         0xBD
#define MT25TL01G_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xBC

#define MT25TL01G_QUAD_OUT_FAST_READ_CMD               0x6B
#define MT25TL01G_QUAD_OUT_FAST_READ_DTR_CMD           0x6D
#define MT25TL01G_QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x6C

#define MT25TL01G_QUAD_INOUT_FAST_READ_CMD             0xEB
#define MT25TL01G_QUAD_INOUT_FAST_READ_DTR_CMD         0xED
#define MT25TL01G_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xEC
#define MT25TL01G_QUAD_INOUT_FAST_READ_4_BYTE_DTR_CMD  0xEE
/* Write Operations */
#define MT25TL01G_WRITE_ENABLE_CMD                     0x06
#define MT25TL01G_WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define MT25TL01G_READ_STATUS_REG_CMD                  0x05
#define MT25TL01G_WRITE_STATUS_REG_CMD                 0x01

#define MT25TL01G_READ_LOCK_REG_CMD                    0xE8
#define MT25TL01G_WRITE_LOCK_REG_CMD                   0xE5

#define MT25TL01G_READ_FLAG_STATUS_REG_CMD             0x70
#define MT25TL01G_CLEAR_FLAG_STATUS_REG_CMD            0x50

#define MT25TL01G_READ_NONVOL_CFG_REG_CMD              0xB5
#define MT25TL01G_WRITE_NONVOL_CFG_REG_CMD             0xB1

#define MT25TL01G_READ_VOL_CFG_REG_CMD                 0x85
#define MT25TL01G_WRITE_VOL_CFG_REG_CMD                0x81

#define MT25TL01G_READ_ENHANCED_VOL_CFG_REG_CMD        0x65
#define MT25TL01G_WRITE_ENHANCED_VOL_CFG_REG_CMD       0x61

#define MT25TL01G_READ_EXT_ADDR_REG_CMD                0xC8
#define MT25TL01G_WRITE_EXT_ADDR_REG_CMD               0xC5

/* Program Operations */
#define MT25TL01G_PAGE_PROG_CMD                        0x02
#define MT25TL01G_PAGE_PROG_4_BYTE_ADDR_CMD            0x12

#define MT25TL01G_DUAL_IN_FAST_PROG_CMD                0xA2
#define MT25TL01G_EXT_DUAL_IN_FAST_PROG_CMD            0xD2

#define MT25TL01G_QUAD_IN_FAST_PROG_CMD                0x32
#define MT25TL01G_EXT_QUAD_IN_FAST_PROG_CMD            0x38
#define MT25TL01G_QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    0x34

/* Erase Operations */
#define MT25TL01G_SUBSECTOR_ERASE_CMD_4K               0x20
#define MT25TL01G_SUBSECTOR_ERASE_4_BYTE_ADDR_CMD_4K   0x21

#define MT25TL01G_SUBSECTOR_ERASE_CMD_32K              0x52

#define MT25TL01G_SECTOR_ERASE_CMD                     0xD8
#define MT25TL01G_SECTOR_ERASE_4_BYTE_ADDR_CMD         0xDC

#define MT25TL01G_DIE_ERASE_CMD                        0xC7

#define MT25TL01G_PROG_ERASE_RESUME_CMD                0x7A
#define MT25TL01G_PROG_ERASE_SUSPEND_CMD               0x75

/* One-Time Programmable Operations */
#define MT25TL01G_READ_OTP_ARRAY_CMD                   0x4B
#define MT25TL01G_PROG_OTP_ARRAY_CMD                   0x42

/* 4-byte Address Mode Operations */
#define MT25TL01G_ENTER_4_BYTE_ADDR_MODE_CMD           0xB7
#define MT25TL01G_EXIT_4_BYTE_ADDR_MODE_CMD            0xE9

/* Quad Operations */
#define MT25TL01G_ENTER_QUAD_CMD                       0x35
#define MT25TL01G_EXIT_QUAD_CMD                        0xF5
#define MT25TL01G_ENTER_DEEP_POWER_DOWN                0xB9
#define MT25TL01G_RELEASE_FROM_DEEP_POWER_DOWN         0xAB

/*ADVANCED SECTOR PROTECTION Operations*/
#define MT25TL01G_READ_SECTOR_PROTECTION_CMD           0x2D
#define MT25TL01G_PROGRAM_SECTOR_PROTECTION            0x2C
#define MT25TL01G_READ_PASSWORD_CMD                    0x27
#define MT25TL01G_WRITE_PASSWORD_CMD                   0x28
#define MT25TL01G_UNLOCK_PASSWORD_CMD                  0x29
#define MT25TL01G_READ_GLOBAL_FREEZE_BIT               0xA7
#define MT25TL01G_READ_VOLATILE_LOCK_BITS              0xE8
#define MT25TL01G_WRITE_VOLATILE_LOCK_BITS             0xE5
    /*ADVANCED SECTOR PROTECTION Operations with 4-Byte Address*/
#define MT25TL01G_WRITE_4_BYTE_VOLATILE_LOCK_BITS      0xE1
#define MT25TL01G_READ_4_BYTE_VOLATILE_LOCK_BITS       0xE0
    /*One Time Programmable Operations */
#define MT25TL01G_READ_OTP_ARRAY                       0x4B
#define MT25TL01G_PROGRAM_OTP_ARRAY                    0x42


/**
  * @brief  MT25TL01G Registers
  */
/* Status Register */
#define MT25TL01G_SR_WIP                      ((uint8_t)0x01)    /*!< Write in progress */
#define MT25TL01G_SR_WREN                     ((uint8_t)0x02)    /*!< Write enable latch */
#define MT25TL01G_SR_BLOCKPR                  ((uint8_t)0x5C)    /*!< Block protected against program and erase operations */
#define MT25TL01G_SR_PRBOTTOM                 ((uint8_t)0x20)    /*!< Protected memory area defined by BLOCKPR starts from top or bottom */
#define MT25TL01G_SR_SRWREN                   ((uint8_t)0x80)    /*!< Status register write enable/disable */

/* Non volatile Configuration Register */
#define MT25TL01G_NVCR_NBADDR                 ((uint16_t)0x0001) /*!< 3-bytes or 4-bytes addressing */
#define MT25TL01G_NVCR_SEGMENT                ((uint16_t)0x0002) /*!< Upper or lower 128Mb segment selected by default */
#define MT25TL01G_NVCR_DUAL                   ((uint16_t)0x0004) /*!< Dual I/O protocol */
#define MT25TL01G_NVCR_QUAB                   ((uint16_t)0x0008) /*!< Quad I/O protocol */
#define MT25TL01G_NVCR_RH                     ((uint16_t)0x0010) /*!< Reset/hold */
#define MT25TL01G_NVCR_DTRP                   ((uint16_t)0x0020) /*!< Double transfer rate protocol */
#define MT25TL01G_NVCR_ODS                    ((uint16_t)0x01C0) /*!< Output driver strength */
#define MT25TL01G_NVCR_XIP                    ((uint16_t)0x0E00) /*!< XIP mode at power-on reset */
#define MT25TL01G_NVCR_NB_DUMMY               ((uint16_t)0xF000) /*!< Number of dummy clock cycles */

/* Volatile Configuration Register */
#define MT25TL01G_VCR_WRAP                    ((uint8_t)0x03)    /*!< Wrap */
#define MT25TL01G_VCR_XIP                     ((uint8_t)0x08)    /*!< XIP */
#define MT25TL01G_VCR_NB_DUMMY                ((uint8_t)0xF0)    /*!< Number of dummy clock cycles */

/* Extended Address Register */
#define MT25TL01G_EAR_HIGHEST_SE              ((uint8_t)0x03)    /*!< Select the Highest 128Mb segment */
#define MT25TL01G_EAR_THIRD_SEG               ((uint8_t)0x02)    /*!< Select the Third 128Mb segment */
#define MT25TL01G_EAR_SECOND_SEG              ((uint8_t)0x01)    /*!< Select the Second 128Mb segment */
#define MT25TL01G_EAR_LOWEST_SEG              ((uint8_t)0x00)    /*!< Select the Lowest 128Mb segment (default) */

/* Enhanced Volatile Configuration Register */
#define MT25TL01G_EVCR_ODS                    ((uint8_t)0x07)    /*!< Output driver strength */
#define MT25TL01G_EVCR_RH                     ((uint8_t)0x10)    /*!< Reset/hold */
#define MT25TL01G_EVCR_DTRP                   ((uint8_t)0x20)    /*!< Double transfer rate protocol */
#define MT25TL01G_EVCR_DUAL                   ((uint8_t)0x40)    /*!< Dual I/O protocol */
#define MT25TL01G_EVCR_QUAD                   ((uint8_t)0x80)    /*!< Quad I/O protocol */

/* Flag Status Register */
#define MT25TL01G_FSR_NBADDR                  ((uint8_t)0x01)    /*!< 3-bytes or 4-bytes addressing */
#define MT25TL01G_FSR_PRERR                   ((uint8_t)0x02)    /*!< Protection error */
#define MT25TL01G_FSR_PGSUS                   ((uint8_t)0x04)    /*!< Program operation suspended */
#define MT25TL01G_FSR_PGERR                   ((uint8_t)0x10)    /*!< Program error */
#define MT25TL01G_FSR_ERERR                   ((uint8_t)0x20)    /*!< Erase error */
#define MT25TL01G_FSR_ERSUS                   ((uint8_t)0x40)    /*!< Erase operation suspended */
#define MT25TL01G_FSR_READY                   ((uint8_t)0x80)    /*!< Ready or command in progress */


void mt25tl01g_Init(void);
void MT25TL01G_WriteBuffer(uint8_t *buf, uint32_t writeAddr, uint16_t writeSize);

#endif