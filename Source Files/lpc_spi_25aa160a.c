/******************************************************************//**
* @file		lpc_spi_25aa160a.c
* @brief	Contains all functions support for Spi based E2PROM 25aa160a
*           library on LPC17xx
* @version	1.0
* @date		09. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup EEPROM
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_spi_25aa160a.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

const char *status_reg[4]={"   STATUS REGISTER:\r\n\n",
                           "W/R                    W/R  W/R  R    R\r\n",
                           "D7   D6   D5   D4      D3   D2   D1   D0\r\n",
                           "WPEN X    X    X   --  BP1  BP0  WEL  WIP",
};


void print_status_reg(void)
{
	uint8_t count;

	clr_scr_rst_cur(LPC_UART0);

	for(count = 0; count<4; ++count)
	{
		printf(LPC_UART0,status_reg[count]);
	}
}


/** @addtogroup EEPROM_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    Get the status about locked or unlocked memory blocks.
 * @param[in]	None
 * @return 		status byte
 **********************************************************************/
uint8_t Spi_Eeprom_Read_Status_Reg (void)
{
	SPI_DATA_SETUP_Type xferConfig;

	Tx_Buf[0] = EEP_RDSR;                      /* Read Status 8bit msb    */
	CS_Force (DISABLE);                        /* Select device           */

	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);
	xferConfig.tx_data = NULL;               /* Send Instruction Byte    */
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(Rx_Buf[0])
	{
		CS_Force (ENABLE);                     /* Select device           */
		return(Rx_Buf[0]);                    /* Return value            */
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Reads status register value
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Read_Eeprom_Status (void)
{
	uint8_t wpen=0, bp1=0, bp0=0, wel=0, wip=0;
	uint8_t dat;

	clr_scr_rst_cur(LPC_UART0);
	dat = Spi_Eeprom_Read_Status_Reg();
	if(dat&0x80){wpen=1;}else{wpen=0;}
	if(dat&0x08){bp1 =1;}else{bp1 =0;}
	if(dat&0x04){bp0 =1;}else{bp0 =0;}
	if(dat&0x02){wel =1;}else{wel =0;}
	if(dat&0x01){wip =1;}else{wip =0;}
	print_status_reg();
	printf(LPC_UART0,"\n\r%d01    X    X    X       %d01    %d01    %d01    %d01\n\r",wpen,bp1,bp0,wel,wip);

	getche(LPC_UART0);
	EscFlag = 0;
}


/*********************************************************************//**
 * @brief	    To lock or unlock data sectors
 * @param[in]	status_reg     Write Status Value
 * @return 		status byte
 **********************************************************************/
uchar Spi_Eeprom_Write_Status_Reg (uint8_t status_reg)
{
	SPI_DATA_SETUP_Type xferConfig;

	uint8_t WriteData[1],WriteStatus =0;

	Tx_Buf[0] = EEP_WRSR;                 /* Write Status 8bit msb                     */
	Tx_Buf[1] = status_reg;               /* STATUS REGISTER                           */
	                                       /* W/R                    W/R  W/R  R    R   */
                                           /* D7   D6   D5   D4      D3   D2   D1   D0  */
                                           /* WPEN X    X    X   --  BP1  BP0  WEL  WIP */

	WriteData[0] = EEP_WREN;

	CS_Force (DISABLE);                            /* Select device           */

	xferConfig.tx_data = WriteData;            /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);  /* Write Enable Latch      */
	CS_Force (ENABLE);                          /* CS high inactive        */

	CS_Force (DISABLE);                            /* Select device           */
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.length = 2;
	WriteStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force (ENABLE);                          /* CS high inactive        */
		delay_ms(4);
		return(1);
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Write Byte value at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @param[in]   byte_data      byte data to be written
 * @return 		status byte
 **********************************************************************/
uchar Spi_Eeprom_Write_Byte (uint16 eep_address, uint8_t byte_data)
{
	SPI_DATA_SETUP_Type xferConfig;
	uint8_t WriteData[1],WriteStatus =0;

	Tx_Buf[0] = EEP_WRITE;                  /* WRITE IR 8bit msb       */
	Tx_Buf[1] = (uchar)(eep_address>>8);    // 1st byte extract
	Tx_Buf[2] = (uchar) eep_address;        // 2nd byte extract
	Tx_Buf[3] = byte_data;                  /* byte data lsb           */

	WriteData[0] = EEP_WREN;

	CS_Force (DISABLE);
	xferConfig.tx_data = WriteData;            /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);  /* Write Enable Latch      */
	CS_Force (ENABLE);

	CS_Force (DISABLE);                              /* CS low active           */
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.length = 4;
	WriteStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force (ENABLE);                            /* CS high inactive        */
		delay_ms(4);
		return(1);
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Write value array at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @param[in]   *dest_start    buffer address
 * @param[in]   length         size of buffer
 * @return 		write status
 **********************************************************************/
uchar Spi_Eeprom_Write (uint16_t eep_address, uint8_t *data_start, uint8_t length)
{
	SPI_DATA_SETUP_Type xferConfig;
	uint8_t WriteData[1],WriteStatus =0,ip_len,new_length;
	uint16 new_address;

	WriteData[0] = EEP_WREN;

	Tx_Buf[0] = EEP_WRITE;                  // WRITE IR 8bit msb
	Tx_Buf[1] = (uchar)(eep_address>>8);    // 1st byte extract
	Tx_Buf[2] = (uchar)eep_address;         // 2nd byte extract

	/* Intern page length(ip_len) gives length from address that can be occupied in page */
	ip_len = 0x10 - (eep_address % 0x10);
	new_address = eep_address + ip_len;

	if(length > ip_len)
	{
		new_length = length - ip_len;

		CS_Force (DISABLE);
		xferConfig.tx_data = WriteData;            /* Send Instruction Byte    */
		xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
		xferConfig.length = 1;
		SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);  /* Write Enable Latch      */
		CS_Force (ENABLE);

		CS_Force (DISABLE);                            // CS low active
		xferConfig.tx_data = Tx_Buf;                   /* Send Instruction Byte    */
		xferConfig.length = 3;
		SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

		xferConfig.tx_data = data_start;               // Pass value
		xferConfig.length = ip_len;
		WriteStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

		if(WriteStatus)
		{
			CS_Force (ENABLE);                            /* CS high inactive        */
		}
		else
			return(0);
		// Recursive function
		delay_ms(4);
		Spi_Eeprom_Write(new_address,(data_start + ip_len),new_length);
	}

	if(length <= ip_len+1)
	{
		CS_Force (DISABLE);
		xferConfig.tx_data = WriteData;            /* Send Instruction Byte    */
		xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
		xferConfig.length = 1;
		SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);  /* Write Enable Latch      */
		CS_Force (ENABLE);

		CS_Force (DISABLE);                            // CS low
		xferConfig.tx_data = Tx_Buf;                   /* Send Instruction Byte    */
		xferConfig.length = 3;
		SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

		xferConfig.tx_data = data_start;               // Pass value
		xferConfig.length = length;
		WriteStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);
		if(WriteStatus)
		{
			CS_Force (ENABLE);                            /* CS high inactive        */
		}
		else
			return(0);
	}
	return(1);
}


/*********************************************************************//**
 * @brief	    Read Byte value at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @return 		Read Byte is returned
 **********************************************************************/
uint8_t Spi_Eeprom_Read_Byte (uint16 eep_address)
{
	SPI_DATA_SETUP_Type xferConfig;

	Tx_Buf[0] = EEP_READ;                 /* READ IR 8bit msb        */
	Tx_Buf[1] = (uchar)(eep_address>>8);  // 1st byte extract
	Tx_Buf[2] = (uchar)eep_address;       // 2nd byte extract

	CS_Force (DISABLE);
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 3;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);
	xferConfig.tx_data = NULL;
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(Rx_Buf[0])
	{
		CS_Force (ENABLE);
		return(Rx_Buf[0]);                    /* Return value            */
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Read Page value at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @param[in]   *dest_addr     buffer address
 * @param[in]   length         size of buffer
 * @return 		read status
 **********************************************************************/
uchar Spi_Eeprom_Read (uint16_t eep_address, uint8_t *dest_addr, uint8_t length)
{
	SPI_DATA_SETUP_Type xferConfig;
	uchar ReadStatus =0;

	Tx_Buf[0] = EEP_READ;                      // READ IR 8bit msb
	Tx_Buf[1] = (uchar)(eep_address>>8);       // 1st byte extract
	Tx_Buf[2] = (uchar)eep_address;            // 2nd byte extract

	CS_Force (DISABLE);
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 3;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);
	xferConfig.tx_data = NULL;
	xferConfig.rx_data = dest_addr;               /* Store byte  */
	xferConfig.length = length;
	ReadStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(ReadStatus)
	{
		CS_Force (ENABLE);
		delay_ms(4);
		return(1);                                // Return value
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Display Read data stored in array
 * @param[in]   *dest_addr     buffer address
 * @param[in]   length         size of buffer
 * @return 		None
 **********************************************************************/
void Display_Eeprom_Array (uint8_t *string, uint8_t length)
{
	while(length)
	{
		printf(LPC_UART0,"%x02  ",*string++);
		length--;
	}
}


/*********************************************************************//**
 * @brief	    Ask for Address Range you want to display
 * @param[in]   *dest_addr     buffer address
 * @param[in]   length         size of buffer
 * @return 		None
 **********************************************************************/
void Display_Eeprom_Loc (uint16 mem_start_address, uint16 mem_end_address)
{
	uint8_t line=0,count=0;
	uint16_t dat;
	uint16_t addr;

	printf(LPC_UART0,"EEPROM Range = 0x000 - 0x7FF \r\n");

	clr_scr_rst_cur(LPC_UART0);
	printf(LPC_UART0,"Start: %x03   End: %x03 \r\n",mem_start_address,mem_end_address);

	for(addr=mem_start_address; addr<mem_end_address+1; addr++)
	{
		if(count == 0) printf(LPC_UART0,"%x03   ",addr);

		dat = Spi_Eeprom_Read_Byte(addr);   /* read byte from address                  */

		printf(LPC_UART0,"%x02  ",dat);
		count++;

		if(count == 16)                    /* check for last digit entered            */
		{
			line++;
			count = 0;
			printf(LPC_UART0,"\r\n");
		}

		if(line == 20 || addr == mem_end_address)
		{
			printf(LPC_UART0,"\x1b[24;01HPress any key to continue.");
			line = 0;
			getche(LPC_UART0);

			clr_scr_rst_cur(LPC_UART0);
		}
	}
}




/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

