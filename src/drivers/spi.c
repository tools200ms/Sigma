
/**
 * Code based on demo: https://www.waveshare.com/wiki/1.54inch_LCD_Module#Demo_Download
 */

#include "spi.h"

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
void spi_send_command( UBYTE cmd ) {
    SPI_LCD_DC_0;

    wiringPiSPIDataRW( 0, &cmd, 1 );
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
void spi_send_data_8bit( UBYTE data ) {
    SPI_LCD_DC_1;

    wiringPiSPIDataRW( 0, &data, 1 );
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
void spi_send_data_16bit( UWORD data ) {
    unsigned char byte;

    SPI_LCD_DC_1;

    byte = (data >> 8) & 0xFF;
    wiringPiSPIDataRW( 0, &byte, 1 );
    byte = data & 0xFF;
    wiringPiSPIDataRW( 0, &byte, 1 );
}


inline void spi_write_byte( uint8_t value ) {
    wiringPiSPIDataRW( 0, &value, 1 );
}

inline void spi_write_bytes( uint8_t *data, int len ) {
    wiringPiSPIDataRW( 0, (unsigned char *)data, len );
}

void spi_set_backlight( UWORD value ) {
    pwmWrite( LCD_BL, value );
}
