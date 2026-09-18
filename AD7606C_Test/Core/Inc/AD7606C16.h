/*
 * ADC7606C-16.h
 *
 *  Created on: Jul 15, 2026
 *      Author: SRIVATHSJAYAN
 */

#ifndef AD7606C16_H_
#define AD7606C16_H_
#include "main.h"
#include<string.h>
#include<stdio.h>

typedef enum
{
    AD7606_REG_STATUS                  = 0x01U,
    AD7606_REG_CONFIG                  = 0x02U,

    AD7606_REG_RANGE_CH1_CH2           = 0x03U,
    AD7606_REG_RANGE_CH3_CH4           = 0x04U,
    AD7606_REG_RANGE_CH5_CH6           = 0x05U,
    AD7606_REG_RANGE_CH7_CH8           = 0x06U,

    AD7606_REG_BANDWIDTH               = 0x07U,
    AD7606_REG_OVERSAMPLING            = 0x08U,

    AD7606_REG_CH1_GAIN                = 0x09U,
    AD7606_REG_CH2_GAIN                = 0x0AU,
    AD7606_REG_CH3_GAIN                = 0x0BU,
    AD7606_REG_CH4_GAIN                = 0x0CU,
    AD7606_REG_CH5_GAIN                = 0x0DU,
    AD7606_REG_CH6_GAIN                = 0x0EU,
    AD7606_REG_CH7_GAIN                = 0x0FU,
    AD7606_REG_CH8_GAIN                = 0x10U,

    AD7606_REG_CH1_OFFSET              = 0x11U,
    AD7606_REG_CH2_OFFSET              = 0x12U,
    AD7606_REG_CH3_OFFSET              = 0x13U,
    AD7606_REG_CH4_OFFSET              = 0x14U,
    AD7606_REG_CH5_OFFSET              = 0x15U,
    AD7606_REG_CH6_OFFSET              = 0x16U,
    AD7606_REG_CH7_OFFSET              = 0x17U,
    AD7606_REG_CH8_OFFSET              = 0x18U,

    AD7606_REG_CH1_PHASE               = 0x19U,
    AD7606_REG_CH2_PHASE               = 0x1AU,
    AD7606_REG_CH3_PHASE               = 0x1BU,
    AD7606_REG_CH4_PHASE               = 0x1CU,
    AD7606_REG_CH5_PHASE               = 0x1DU,
    AD7606_REG_CH6_PHASE               = 0x1EU,
    AD7606_REG_CH7_PHASE               = 0x1FU,
    AD7606_REG_CH8_PHASE               = 0x20U,

    AD7606_REG_DIGITAL_DIAG_ENABLE     = 0x21U,
    AD7606_REG_DIGITAL_DIAG_ERR        = 0x22U,

    AD7606_REG_OPEN_DETECT_ENABLE      = 0x23U,
    AD7606_REG_OPEN_DETECTED           = 0x24U,

    /* 0x25 - 0x27 Reserved */

    AD7606_REG_DIAG_MUX_CH1_CH2        = 0x28U,
    AD7606_REG_DIAG_MUX_CH3_CH4        = 0x29U,
    AD7606_REG_DIAG_MUX_CH5_CH6        = 0x2AU,
    AD7606_REG_DIAG_MUX_CH7_CH8        = 0x2BU,

    AD7606_REG_OPEN_DETECT_QUEUE       = 0x2CU,
    AD7606_REG_FS_CLK_COUNTER          = 0x2DU,
    AD7606_REG_OS_CLK_COUNTER          = 0x2EU,
    AD7606_REG_ID                      = 0x2FU

} AD7606C16_RegTypeDef_t;
//@range
typedef enum
{
    AD7606C_RANGE_P2V5  = 0x00U,    /*!< ±2.5 V Single-Ended */
	AD7606C_RANGE_P5V   = 0x01U,    /*!< ±5 V Single-Ended */
	AD7606C_RANGE_P6V25 = 0x02U,    /*!< ±6.25 V Single-Ended */
	AD7606C_RANGE_P10V  = 0x03U,    /*!< ±10 V Single-Ended */
	AD7606C_RANGE_P12V5 = 0x04U,    /*!< ±12.5 V Single-Ended */
	AD7606C_RANGE_5V    = 0x05U,    /*!< 0 V to 5 V Single-Ended */
	AD7606C_RANGE_10V   = 0x06U,    /*!< 0 V to 10 V Single-Ended */
	AD7606C_RANGE_12V5  = 0x07U,    /*!< 0 V to 12.5 V Single-Ended */
	AD7606C_RANGE_D5V   = 0x08U,    /*!< ±5 V Differential */
	AD7606C_RANGE_D10V  = 0x09U,    /*!< ±10 V Differential */
	AD7606C_RANGE_D12V5 = 0x0AU,    /*!< ±12.5 V Differential */
	AD7606C_RANGE_D20V  = 0x0BU     /*!< ±20 V Differential */
} AD7606C16_Range_t;
//@channel_no
typedef enum
{
	AD7606C_CHANNEL_NO_1 = 0U,
	AD7606C_CHANNEL_NO_2 = 1U,
	AD7606C_CHANNEL_NO_3 = 2U,
	AD7606C_CHANNEL_NO_4 = 3U,
	AD7606C_CHANNEL_NO_5 = 4U,
	AD7606C_CHANNEL_NO_6 = 5U,
	AD7606C_CHANNEL_NO_7 = 6U,
	AD7606C_CHANNEL_NO_8 = 7U

} AD7606C16_Channel_t;
//@error
typedef enum
{
	AD7606C_ERR_ROM_CRC = 0U,
	AD7606C_ERR_MM_CRC,
	AD7606C_ERR_INT_CRC,
	AD7606C_ERR_SPI_WRITE,
	AD7606C_ERR_SPI_READ,
	AD7606C_ERR_BUSY_STUCK_HIGH
} AD7606C16_Error_t;
//@diag_mux
typedef enum
{
	AD7606C_DIAG_MUX_ADC_VOLTAGE_IN = 0x00,
	AD7606C_DIAG_MUX_TEMP,
	AD7606C_DIAG_MUX_VREF,
	AD7606C_DIAG_MUX_ALDO,
	AD7606C_DIAG_MUX_DLDO,
	AD7606C_DIAG_MUX_VDRIVE,
	AD7606C_DIAG_MUX_AGND,
	AD7606C_DIAG_MUX_AVCCC
} AD7606C16_Diag_Mux_t;
typedef struct
{
	uint8_t op_mode;
	uint8_t dout_mode;
	uint8_t ext_os_clk;
	uint8_t status_header;

} AD7606C16_InitTypeDef_t;
typedef struct
{
	SPI_HandleTypeDef *hspi;

	GPIO_TypeDef *BUSY_Port;
	uint16_t BUSY_Pin;

	GPIO_TypeDef *RST_Port;
	uint16_t RST_Pin;

	GPIO_TypeDef *CONVST_Port;
	uint16_t CONVST_Pin;

	GPIO_TypeDef *CS_Port;
	uint16_t CS_Pin;

	AD7606C16_InitTypeDef_t Init;

} AD7606C16_Handle_t;

#define RESET 0x00U
#define SET 0x01U

#define __UPPER_NIBBLE_MASK 0xF0
#define __LOWER_NIBBLE_MASK 0x0F
#define __AD7606C16_ADDR_POS 0x8U

#define __AD7606C16_WRITE_READ_POS 14U
#define __AD7606C16_READ 0x01U

#define __AD7606C16_WEN_POS 15U
#define __AD7606C16_WEN 0x00U
#define __AD7606C16_OPMODE_POS 0U
#define __AD7606C16_DOUT_MODE_POS 3U
#define __AD7606C16_EXT_OS_CLOCK_POS 5U
#define __AD7606C16_STATUS_HEAD_POS 6U

//@op_mode
#define AD7606C16_MODE_NORMAL 0x00U
#define AD7606C16_MODE_STBY 0x01U
#define AD7606C16_MODE_AUTOSTBY 0x02U
#define AD7606C16_MODE_SHUTDOWN 0x03U
//@dout_mode
#define AD7606C16_DOUTX_1 0x00U
#define AD7606C16_DOUTX_2 0x01U
#define AD7606C16_DOUTX_4 0x02U
#define AD7606C16_DOUTX_8 0x03U
//@ext_os_clk
#define AD7606C16_EXT_OS_CLK_DI RESET
#define AD7606C16_EXT_OS_CLK_EN SET
//@status_header
#define AD7606C16_STATUS_HEAD_DI RESET
#define AD7606C16_STATUS_HEAD_EN SET
//@en_di
#define ENABLE SET
#define DISABLE RESET

#define ERR_SET SET
#define ERR_RESET RESET

#define FLAG_SET SET
#define FLAG_RESET RESET

HAL_StatusTypeDef AD7606C16_ReadReg(AD7606C16_Handle_t *hadc,AD7606C16_RegTypeDef_t reg,uint8_t *pData);
HAL_StatusTypeDef AD7606C16_Init(AD7606C16_Handle_t *hadc);
HAL_StatusTypeDef AD7606C16_Reset_Full(AD7606C16_Handle_t *hadc);
HAL_StatusTypeDef AD7606C16_Set_Ch_Range(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no,AD7606C16_Range_t range);
HAL_StatusTypeDef AD7606C16_Set_Ch_Phase_Calib(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no,uint8_t phase);
HAL_StatusTypeDef AD7606C16_Set_Ch_Offset_Calib(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no,uint8_t offset);
HAL_StatusTypeDef AD7606C16_Set_Ch_Gain_Calib(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no,uint8_t gain);
HAL_StatusTypeDef AD7606_Get_Vltg_Value(AD7606C16_Handle_t *hadc,uint16_t *pData);
HAL_StatusTypeDef AD7606C16_Set_Digital_Diag(AD7606C16_Handle_t *hadc,AD7606C16_Error_t error,uint8_t en_di);
uint8_t AD7606C16_Get_Digital_Diag_Err(AD7606C16_Handle_t *hadc,AD7606C16_Error_t error);
HAL_StatusTypeDef AD7606C16_Set_Open_Detect_En_Di(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no,uint8_t en_di);
uint8_t AD7606C16_Get_Open_Detect(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no);
HAL_StatusTypeDef AD7606C16_Set_Diag_Mux_Ctrl(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t channel_no,AD7606C16_Diag_Mux_t diag_mux);
#endif /* AD7606C16_H_ */
