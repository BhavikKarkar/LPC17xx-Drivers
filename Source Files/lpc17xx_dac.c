/******************************************************************//**
* @file		lpc17xx_dac.c
* @brief	Contains all functions support for DAC firmware library on LPC17xx
* @version	1.0
* @date		07. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup DAC
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_dac.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup DAC_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Initial ADC configuration Config
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @return 		None
 ***********************************************************************/
void DAC_Config (LPC_DAC_TypeDef *DACx)
{
	// Pin configuration for DAC
	PINSEL_CFG_Type PinCfg;

	if(DACx == LPC_DAC)
	{
		// Configure P0.26 as AOUT
		PinCfg.Funcnum = 2;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 26;
		PinCfg.Portnum = 0;
		PINSEL_ConfigPin(&PinCfg);
	}

	/**
	 * init DAC structure to default
	 * Maximum	current is 700 uA
	 * First value to AOUT is 0
	 */
	DAC_Init(DACx);
}


/*********************************************************************//**
 * @brief 		Initial ADC configuration
 * 					- Maximum	current is 700 uA
 * 					- Value to AOUT is 0
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @return 		None
 ***********************************************************************/
void DAC_Init(LPC_DAC_TypeDef *DACx)
{
	CHECK_PARAM(PARAM_DACx(DACx));
	/* Set default clock divider for DAC */
	// CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_DAC, CLKPWR_PCLKSEL_CCLK_DIV_4);
	//Set maximum current output
	DAC_SetBias(LPC_DAC,DAC_MAX_CURRENT_700uA);
}

/*********************************************************************//**
 * @brief 		Update value to DAC
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @param[in] 	dac_value : value 10 bit to be converted to output
 * @return 		None
 ***********************************************************************/
void DAC_UpdateValue (LPC_DAC_TypeDef *DACx,uint32_t dac_value)
{
	uint32_t tmp;
	CHECK_PARAM(PARAM_DACx(DACx));
	tmp = DACx->DACR & DAC_BIAS_EN;
	tmp |= DAC_VALUE(dac_value);
	// Update value
	DACx->DACR = tmp;
}

/*********************************************************************//**
 * @brief 		Set Maximum current for DAC
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @param[in] 	bias : 0 is 700 uA
 * 					   1    350 uA
 * @return 		None
 ***********************************************************************/
void DAC_SetBias (LPC_DAC_TypeDef *DACx,uint32_t bias)
{
	CHECK_PARAM(PARAM_DAC_CURRENT_OPT(bias));
	DACx->DACR &=~DAC_BIAS_EN;
	if (bias  == DAC_MAX_CURRENT_350uA)
	{
		DACx->DACR |= DAC_BIAS_EN;
	}
}

/*********************************************************************//**
 * @brief 		To enable the DMA operation and control DMA timer
 * @param[in]	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @param[in] 	DAC_ConverterConfigStruct pointer to DAC_CONVERTER_CFG_Type
 * 					- DBLBUF_ENA :  enable/disable DACR double buffering feature
 * 					- CNT_ENA    :  enable/disable timer out counter
 * 					- DMA_ENA    :	enable/disable DMA access
 * @return 		None
 ***********************************************************************/
void DAC_ConfigDAConverterControl (LPC_DAC_TypeDef *DACx,DAC_CONVERTER_CFG_Type *DAC_ConverterConfigStruct)
{
	CHECK_PARAM(PARAM_DACx(DACx));
	DACx->DACCTRL &= ~DAC_DACCTRL_MASK;
	if (DAC_ConverterConfigStruct->DBLBUF_ENA)
		DACx->DACCTRL	|= DAC_DBLBUF_ENA;
	if (DAC_ConverterConfigStruct->CNT_ENA)
		DACx->DACCTRL	|= DAC_CNT_ENA;
	if (DAC_ConverterConfigStruct->DMA_ENA)
		DACx->DACCTRL	|= DAC_DMA_ENA;
}

/*********************************************************************//**
 * @brief 		Set reload value for interrupt/DMA counter
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @param[in] 	time_out time out to reload for interrupt/DMA counter
 * @return 		None
 ***********************************************************************/
void DAC_SetDMATimeOut(LPC_DAC_TypeDef *DACx, uint32_t time_out)
{
	CHECK_PARAM(PARAM_DACx(DACx));
	DACx->DACCNTVAL = DAC_CCNT_VALUE(time_out);
}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
