/*
 * ADC7606C-16.c
 *
 *  Created on: Jul 15, 2026
 *      Author: SRIVATHSJAYAN
 */
/*
 * Over Sampling pins are grounded and as Software Mode is Enabled
 * SPI need to set in 16Bit mode
 * BUSY_Pin - Pull down - PC1 - Input
 * RST_Pin - Pull down - PB0 - Output
 * CONVST_Pin - Pull Down - PC0 - Output
 * CS_Pin - Pull up - PA4 - Output
 * */
//Init Mode for Default Operation
//op_mode = AD7606C16_MODE_NORMAL;
//dout_format = AD7606C16_DOUTX_1;
//ext_os_clk = AD7606C16_EXT_OS_CLK_DI;
//status_header = AD7606C16_STATUS_HEAD_DI;

#include "AD7606C16.h"

static HAL_StatusTypeDef AD7606C16_Write(AD7606C16_Handle_t *hadc, AD7606C16_RegTypeDef_t reg, uint8_t data);
/**
 * @brief  Initializes the AD7606C-16 device.
 * @details Performs a full hardware reset and configures the CONFIG register
 *          according to the parameters specified in the initialization structure.
 *
 * @param[in] hadc Pointer to the AD7606C-16 handle structure.
 *
 * @retval HAL_OK    Initialization completed successfully.
 * @retval HAL_ERROR Invalid handle, invalid configuration, or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Init(AD7606C16_Handle_t *hadc)
{
	if ((hadc == NULL) || (hadc->hspi == NULL))
	{
		return HAL_ERROR;
	}
	if ((hadc->CS_Port == NULL) ||
	    (hadc->RST_Port == NULL) ||
	    (hadc->CONVST_Port == NULL) ||
	    (hadc->BUSY_Port == NULL))
	{
	    return HAL_ERROR;
	}

	if(AD7606C16_Reset_Full(hadc)!= HAL_OK)
	{
		return HAL_ERROR;
	}

	uint8_t reg=0;

	if (hadc->Init.op_mode > AD7606C16_MODE_SHUTDOWN)
	{
		return HAL_ERROR;
	}
	if (hadc->Init.op_mode == AD7606C16_MODE_SHUTDOWN)
	{
		return HAL_ERROR;
	}
	if(hadc->Init.ext_os_clk > AD7606C16_EXT_OS_CLK_EN)
	{
	    return HAL_ERROR;
	}

	if(hadc->Init.status_header > AD7606C16_STATUS_HEAD_EN)
	{
	    return HAL_ERROR;
	}
	// 1. Set the Operating Mode
	reg = (hadc->Init.op_mode << __AD7606C16_OPMODE_POS);

	if (hadc->Init.dout_mode > AD7606C16_DOUTX_8)
	{
		return HAL_ERROR;
	}
	// 2. Set Dout Mode
	reg |= (hadc->Init.dout_mode  << __AD7606C16_DOUT_MODE_POS);

	// 3. Set Ext OC Clk
	if (hadc->Init.ext_os_clk)
	{
		reg |= (1U << __AD7606C16_EXT_OS_CLOCK_POS);
	}
	// 4. Set Status Header
	if (hadc->Init.status_header)
	{
		reg |= (1U << __AD7606C16_STATUS_HEAD_POS);
	}

	return AD7606C16_Write(hadc,AD7606_REG_CONFIG,reg);
}
/**
 * @brief  Writes an 8-bit value to an AD7606C-16 register.
 *
 * @param[in] hadc Pointer to the AD7606C-16 handle structure.
 * @param[in] reg  Register address to be written.
 * @param[in] data 8-bit data to write.
 *
 * @retval HAL_OK    Register written successfully.
 * @retval HAL_ERROR Invalid parameters or SPI communication failure.
 */
static HAL_StatusTypeDef AD7606C16_Write(AD7606C16_Handle_t *hadc, AD7606C16_RegTypeDef_t reg, uint8_t data)
{
	if ((hadc == NULL) || (hadc->hspi == NULL)||(hadc->CS_Port == NULL))
	{
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status;

	uint16_t tx;
	tx = (((uint16_t)__AD7606C16_WEN << __AD7606C16_WEN_POS) |
		  ((uint16_t)__AD7606C16_WRITE << __AD7606C16_WRITE_READ_POS) |
		  ((uint16_t)reg << __AD7606C16_ADDR_POS) |
		  ((uint16_t)data));

	hadc->CS_Port->BSRR = ((uint32_t)hadc->CS_Pin<<16);

	status = HAL_SPI_Transmit(hadc->hspi,(uint8_t*)&tx,1,HAL_MAX_DELAY);

	hadc->CS_Port->BSRR = (uint32_t)hadc->CS_Pin;

	return status;
}
/**
 * @brief  Reads an 8-bit register from the AD7606C-16.
 * @details The register read operation requires two SPI transfers.
 *          The first transfer sends the read command and the second
 *          retrieves the register contents.
 *
 * @param[in]  hadc Pointer to the AD7606C-16 handle structure.
 * @param[in]  reg  Register address to read.
 * @param[out] rx   Pointer to store the received register value.
 *
 * @retval HAL_OK    Register read successfully.
 * @retval HAL_ERROR Invalid parameters or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Read_Reg(AD7606C16_Handle_t *hadc,AD7606C16_RegTypeDef_t ad7606_reg,uint8_t *pData)
{
	if ((hadc == NULL) || (hadc->hspi == NULL)||(hadc->CS_Port == NULL)||(pData == NULL))
	{
		return HAL_ERROR;
	}
	uint16_t tx = 0;
	uint16_t tx2 = 0x0000;
	uint16_t rx_buff = 0;
	HAL_StatusTypeDef status;
	tx = (((uint16_t)__AD7606C16_WEN << __AD7606C16_WEN_POS) |
			((uint16_t)__AD7606C16_READ << __AD7606C16_WRITE_READ_POS) |
			((uint16_t)ad7606_reg << __AD7606C16_ADDR_POS));
	hadc->CS_Port->BSRR = hadc->CS_Pin<<16;
	status = HAL_SPI_TransmitReceive(hadc->hspi,(uint8_t*)&tx,(uint8_t*)&rx_buff, 1, HAL_MAX_DELAY);
	if(status != HAL_OK)
	{
	    return status;
	}
	status = HAL_SPI_TransmitReceive(hadc->hspi,(uint8_t*)&tx2,(uint8_t*)&rx_buff, 1, HAL_MAX_DELAY);
	hadc->CS_Port->BSRR = hadc->CS_Pin;
	*pData = (uint8_t)rx_buff;
	return status;
}
/**
 * @brief  Performs a hardware reset of the AD7606C-16.
 * @details Generates a reset pulse on the RESET pin. All user
 *          registers are restored to their default values.
 *
 * @param[in] hadc Pointer to the AD7606C-16 handle structure.
 *
 * @retval None
 */
HAL_StatusTypeDef AD7606C16_Reset_Full(AD7606C16_Handle_t *hadc)
{
	if ((hadc == NULL) || (hadc->RST_Port == NULL))
	{
		return HAL_ERROR;
	}

	hadc->RST_Port->BSRR = (uint32_t)hadc->RST_Pin;
	HAL_Delay(1);
	hadc->RST_Port->BSRR = ((uint32_t)hadc->RST_Pin<<16);
	return HAL_OK;
}
/**
 * @brief  Configures the input voltage range of a specified channel.
 * @details Performs a read-modify-write operation on the corresponding
 *          channel range register to preserve the paired channel settings.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel to configure.
 * @param[in] range      Input voltage range selection.
 *
 * @retval HAL_OK    Range updated successfully.
 * @retval HAL_ERROR Invalid parameters or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Ch_Range(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no,AD7606C16_Range_t ad7606c_range)
{
	if(hadc == NULL || hadc->hspi == NULL)
	{
	    return HAL_ERROR;
	}

	if(ad7606c_channel_no > AD7606C_CHANNEL_NO_8)
	{
	    return HAL_ERROR;
	}

	if(ad7606c_range > AD7606C_RANGE_D20V)
	{
	    return HAL_ERROR;
	}

	HAL_StatusTypeDef status;
	uint16_t reg=0;
	status = AD7606C16_Read_Reg(hadc,(AD7606_REG_RANGE_CH1_CH2 + (ad7606c_channel_no/2)),(uint8_t*)&reg);
	if(status != HAL_OK)
	{
		return status;
	}

	if(ad7606c_range % 2 == 0)       // CH1, CH3, CH5, CH7
	{
	    reg &= __UPPER_NIBBLE_MASK;
	    reg |= ad7606c_range;
	}
	else                           // CH2, CH4, CH6, CH8
	{
	    reg &= __LOWER_NIBBLE_MASK;
	    reg |= (ad7606c_range << 4);
	}

	status = AD7606C16_Write(hadc,(AD7606_REG_RANGE_CH1_CH2 + (ad7606c_channel_no/2)),reg);
	return status;
}
/**
 * @brief  Configures the phase calibration value of a specified channel.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel to configure.
 * @param[in] phase      Phase calibration value (0x00 to 0xFF).
 *
 * @retval HAL_OK    Phase calibration updated successfully.
 * @retval HAL_ERROR Invalid parameters or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Ch_Phase_Calib(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no,uint8_t phase)
{
	if(hadc == NULL || hadc->hspi == NULL || hadc->CS_Port == NULL)
	{
	    return HAL_ERROR;
	}

	if(ad7606c_channel_no > AD7606C_CHANNEL_NO_8)
	{
	    return HAL_ERROR;
	}

	return AD7606C16_Write(hadc,(AD7606C16_RegTypeDef_t)(AD7606_REG_CH1_PHASE + ad7606c_channel_no), phase);
}
/**
 * @brief  Configures the offset calibration value of a specified channel.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel to configure.
 * @param[in] offset     Offset calibration value.
 *
 * @retval HAL_OK    Offset calibration updated successfully.
 * @retval HAL_ERROR Invalid parameters or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Ch_Offset_Calib(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no,uint8_t offset)
{
	if(hadc == NULL || hadc->hspi == NULL || hadc->CS_Port == NULL)
	{
	    return HAL_ERROR;
	}

	if(ad7606c_channel_no > AD7606C_CHANNEL_NO_8)
	{
	    return HAL_ERROR;
	}

	return AD7606C16_Write(hadc,(AD7606C16_RegTypeDef_t)(AD7606_REG_CH1_OFFSET + ad7606c_channel_no), offset);
}
/**
 * @brief  Configures the gain calibration value of a specified channel.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel to configure.
 * @param[in] gain       Gain calibration value (0x00 to 0x3F).
 *
 * @retval HAL_OK    Gain calibration updated successfully.
 * @retval HAL_ERROR Invalid parameters or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Ch_Gain_Calib(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no,uint8_t gain)
{
	if(hadc == NULL || hadc->hspi == NULL || hadc->CS_Port == NULL)
	{
	    return HAL_ERROR;
	}

	if(ad7606c_channel_no > AD7606C_CHANNEL_NO_8)
	{
	    return HAL_ERROR;
	}

	if(gain > 0x3F)
	{
		return HAL_ERROR;
	}

	return AD7606C16_Write(hadc,(AD7606C16_RegTypeDef_t)(AD7606_REG_CH1_GAIN + ad7606c_channel_no), gain);
}

HAL_StatusTypeDef AD7606_Get_Vltg_Value(AD7606C16_Handle_t *hadc,uint16_t *pData)
{
	if(hadc == NULL || hadc->hspi == NULL || hadc->CS_Port == NULL)
	{
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status;

	hadc->CONVST_Port->BSRR = (uint32_t)hadc->CONVST_Pin;
	hadc->CONVST_Port->BSRR = (uint32_t)hadc->CONVST_Pin<<16;

	while(hadc->BUSY_Port->IDR & hadc->BUSY_Pin);
	HAL_Delay(1);
	hadc->CS_Port->BSRR = ((uint32_t)hadc->CS_Pin << 16);

	status = HAL_SPI_Receive(hadc->hspi,(uint8_t*)pData,8,HAL_MAX_DELAY);

	hadc->CS_Port->BSRR = hadc->CS_Pin;

	return status;
}
/**
 * @brief  Enables or disables a digital diagnostic function of the AD7606C-16.
 * @details Reads the DIGITAL_DIAG_ENABLE register, modifies the bit corresponding
 *          to the selected diagnostic error, and writes the updated value back.
 *
 * @param[in] hadc   Pointer to the AD7606C-16 handle structure.
 * @param[in] error  Digital diagnostic error to configure.
 * @param[in] en_di  Enable/disable control for the selected diagnostic.
 *
 * @retval HAL_OK    Digital diagnostic configuration updated successfully.
 * @retval HAL_ERROR Invalid parameter or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Digital_Diag(AD7606C16_Handle_t *hadc,AD7606C16_Error_t ad7606c_err,uint8_t en_di)
{
	if(hadc == NULL)
	{
		return HAL_ERROR;
	}
	if(en_di > ENABLE)
	{
		return HAL_ERROR;
	}
	if(ad7606c_err > AD7606C_ERR_BUSY_STUCK_HIGH)
	{
		return HAL_ERROR;
	}

	HAL_StatusTypeDef status;
	uint8_t rx;
	status = AD7606C16_Read_Reg(hadc, AD7606_REG_DIGITAL_DIAG_ENABLE,&rx);
	if(status != HAL_OK)
	{
		return status;
	}

	if(en_di == 0x01)
	{
		rx |= (1 << (uint8_t)ad7606c_err);
	}
	else
	{
		rx &= ~(1 << (uint8_t)ad7606c_err);
	}
	status = AD7606C16_Write(hadc, AD7606_REG_DIGITAL_DIAG_ENABLE, rx);

	return status;
}
/**
 * @brief  Gets the status of a digital diagnostic error of the AD7606C-16.
 * @details Reads the DIGITAL_DIAG_ERR register and checks the bit corresponding
 *          to the specified diagnostic error.
 *
 * @param[in] hadc   Pointer to the AD7606C-16 handle structure.
 * @param[in] error  Digital diagnostic error to check.
 *
 * @retval ERR_SET    The specified diagnostic error is set.
 * @retval ERR_RESET  The specified diagnostic error is not set.
 * @retval 0x99       SPI communication failure.
 */
uint8_t AD7606C16_Get_Digital_Diag_Err(AD7606C16_Handle_t *hadc,AD7606C16_Error_t ad7606c_err)
{
	HAL_StatusTypeDef status;
	uint8_t rx;
	status = AD7606C16_Read_Reg(hadc,AD7606_REG_DIGITAL_DIAG_ERR, &rx);
	if(status != HAL_OK)
	{
		return 0x99;
	}
	if(rx & (1 << ad7606c_err))
	{
		return ERR_SET;
	}
	return ERR_RESET;
}
/**
 * @brief  Enables or disables the open-detect function for a specified channel.
 * @details Reads the OPEN_DETECT_ENABLE register, modifies the bit corresponding
 *          to the selected channel, and writes the updated value back.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel for which open detection is configured.
 * @param[in] en_di      Enable/disable control for the selected channel.
 *
 * @retval HAL_OK    Open-detect configuration updated successfully.
 * @retval HAL_ERROR Invalid parameter or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Open_Detect_En_Di(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no,uint8_t en_di)
{
	if(hadc == NULL)
	{
		return HAL_ERROR;
	}
	if(en_di > ENABLE)
	{
		return HAL_ERROR;
	}
	if(ad7606c_channel_no > AD7606C_CHANNEL_NO_8)
	{
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status;
	uint8_t rx;

	status = AD7606C16_Read_Reg(hadc, AD7606_REG_OPEN_DETECT_ENABLE,&rx);
	if(status != HAL_OK)
	{
		return status;
	}
	if(en_di == 0x01)
	{
		rx |= (1 << (uint8_t)ad7606c_channel_no);
	}
	else
	{
		rx &= ~(1 << (uint8_t)ad7606c_channel_no);
	}

	status = AD7606C16_Write(hadc, AD7606_REG_OPEN_DETECT_ENABLE,rx);

	return status;
}
/**
 * @brief  Gets the open-detect status of a specified channel.
 * @details Reads the OPEN_DETECTED register and checks the bit corresponding
 *          to the selected channel.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel for which the open-detect status is checked.
 *
 * @retval FLAG_SET    Open detection is indicated for the specified channel.
 * @retval FLAG_RESET  No open detection is indicated for the specified channel.
 * @retval 0x99        SPI communication failure.
 */
uint8_t AD7606C16_Get_Open_Detect(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no)
{
	HAL_StatusTypeDef status;
	uint8_t rx;
	status = AD7606C16_Read_Reg(hadc,AD7606_REG_OPEN_DETECTED, &rx);
	if(status != HAL_OK)
	{
		return 0x99;
	}
	if(rx & (1 << ad7606c_channel_no))
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
/**
 * @brief  Configures the diagnostic multiplexer for a specified channel.
 * @details Writes the selected diagnostic multiplexer control value to the
 *          corresponding diagnostic multiplexer register for the channel pair.
 *
 * @param[in] hadc       Pointer to the AD7606C-16 handle structure.
 * @param[in] channel_no ADC channel for which the diagnostic multiplexer is configured.
 * @param[in] diag_mux   Diagnostic multiplexer selection.
 *
 * @retval HAL_OK    Diagnostic multiplexer configuration updated successfully.
 * @retval HAL_ERROR Invalid parameter or SPI communication failure.
 */
HAL_StatusTypeDef AD7606C16_Set_Diag_Mux_Ctrl(AD7606C16_Handle_t *hadc,AD7606C16_Channel_t ad7606c_channel_no,AD7606C16_Diag_Mux_t ad7606c_diag_mux)
{
	if(hadc == NULL)
	{
		return HAL_ERROR;
	}
	if(ad7606c_channel_no > AD7606C_CHANNEL_NO_8 || ad7606c_diag_mux > AD7606C_DIAG_MUX_AVCCC)
	{
		return HAL_ERROR;
	}
	return AD7606C16_Write(hadc, (AD7606_REG_DIAG_MUX_CH1_CH2 + (ad7606c_channel_no/2)), ad7606c_diag_mux);
}
