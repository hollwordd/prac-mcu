/*******************************************************************
* File Name          : spi_flash.h
* Author             : www.armjishu.com
* Version            : V1.0
* Library            : Using STM32F10X_STDPERIPH_VERSION V3.3.0
* Date               : 
* Description        : Header for spi_flash.c file.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "usart.h"
#include "lcd.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/**
  * @brief  SPI_FLASH SPI Interface pins
  */

#define SPI_FLASH_SPI                           SPI1
#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN                        GPIO_Pin_9                  /* PB.09 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOB                       /* GPIOD */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOB


/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(SPI_FLASH_CS_GPIO_PORT, SPI_FLASH_CS_PIN)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(SPI_FLASH_CS_GPIO_PORT, SPI_FLASH_CS_PIN)

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

/*----- Users Define-----*/
void SPI_FLASH_WriteHzku(u32 num);
void SPI_FLASH_ReadHzbuf(u16 x,u16 y,u16 fcolor,u16 bcolor,u8 *Hzbuf);
#endif /* __SPI_FLASH_H */

