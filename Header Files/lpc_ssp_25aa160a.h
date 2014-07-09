/******************************************************************//**
* @file		lpc_ssp_25aa160a.h
* @brief	Contains all SSP based E2PROM 25aa160a Initialization code
* @version	1.0
* @date		16. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup EEPROM
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SSP_25AA160A_H_
#define LPC_SSP_25AA160A_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros -------------------------------------------------------------- */
/** @defgroup EEPROM_Public_Macros
 * @{
 */

#define EEP_WRITE  0x02;  /* Write bit of IR     */
#define EEP_READ   0x03;  /* Read bit of IR      */
#define EEP_WREN   0x06;  /* Write Enable Latch  */
#define EEP_WRDI   0x04;  /* Write Disable Latch */
#define EEP_RDSR   0x05;  /* Read Status Reg     */
#define EEP_WRSR   0x01;  /* Write Status Reg    */

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup EEPROM_Public_Functions EEPROM Public Functions
 * @{
 */

void print_status_reg(void);
void Read_Eeprom_Status (LPC_SSP_TypeDef *SSPx);
uint8_t Ssp_Eeprom_Read_Status_Reg (LPC_SSP_TypeDef *SSPx);
uchar Ssp_Eeprom_Write_Status_Reg (LPC_SSP_TypeDef *SSPx, uint8_t status_reg);
uchar Ssp_Eeprom_Write_Byte (LPC_SSP_TypeDef *SSPx, uint16 eep_address, uint8_t byte_data);
uchar Ssp_Eeprom_Write (LPC_SSP_TypeDef *SSPx, uint16_t eep_address, uint8_t *data_start, uint8_t length);
uint8_t Ssp_Eeprom_Read_Byte (LPC_SSP_TypeDef *SSPx, uint16 eep_address);
uchar Ssp_Eeprom_Read (LPC_SSP_TypeDef *SSPx, uint16_t eep_address, uint8_t *dest_addr, uint8_t length);
void Display_Eeprom_Array (uint8_t *string, uint8_t length);
void Display_Eeprom_Loc (LPC_SSP_TypeDef *SSPx, uint16 mem_start_address, uint16 mem_end_address);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SSP_25AA160A_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
