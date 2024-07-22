
/**
 * Code based on demo: https://www.waveshare.com/wiki/1.54inch_LCD_Module#Demo_Download
 */

#ifndef _SIGMA_200MS_NET__DRIVERS_SPI_H_
#define _SIGMA_200MS_NET__DRIVERS_SPI_H_

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "../lib/system.h"


#define LCD_CS   8  // SPI0 CE0 - (SPI Chip Select 0
#define LCD_RST  27 // SD0 DAT3 - not in https://pinout.xyz/pinout/spi as SPI pin!
#define LCD_DC   25 // SD0 DAT1 - not in https://pinout.xyz/pinout/spi as SPI pin!
#define LCD_BL   18 // PCM CLK  - SPI LCD BL

//SPI LCD
#define SPI_LCD_CS_0		digitalWrite( LCD_CS, 0 )
#define SPI_LCD_CS_1		digitalWrite( LCD_CS, 1 )

#define SPI_LCD_RST_0		digitalWrite( LCD_RST, 0 )
#define SPI_LCD_RST_1		digitalWrite( LCD_RST, 1 )

#define SPI_LCD_DC_0		digitalWrite( LCD_DC, 0 )
#define SPI_LCD_DC_1		digitalWrite( LCD_DC, 1 )

#define SPI_LCD_BL_0		digitalWrite( LCD_BL, 0 )
#define SPI_LCD_BL_1		digitalWrite( LCD_BL, 1 )

void spi_send_command( UBYTE cmd );
void spi_send_data_8bit( UBYTE data );
void spi_send_data_16bit( UWORD data );


void spi_write_byte( uint8_t value );
void spi_write_bytes( uint8_t *data, int len );

void spi_set_backlight( UWORD value );

#endif // _SIGMA_200MS_NET__DRIVERS_SPI_H_
