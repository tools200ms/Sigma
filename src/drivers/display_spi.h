
/**
 * Connection:
 * https://www.waveshare.com/1.54inch-LCD-Module.htm
*/

/**
 * Code based on demo: https://www.waveshare.com/wiki/1.54inch_LCD_Module#Demo_Download
 */

#ifndef _SIGMA_200MS_NET__DRIVERS_DISPLAY_SPI_H_
#define _SIGMA_200MS_NET__DRIVERS_DISPLAY_SPI_H_

#define DISPLAY_PHY_HORIZONTAL 0
#define DISPLAY_PHY_VERTICAL   1


typedef struct display_model {
    short width, height;
}
DisplayModel;

typedef struct display {
    short width, height;
    short phy_orientation;
}
Display;

// https://www.waveshare.com/1.54inch-lcd-module.htm
// Embdeded driver: ST7789
// DisplayModel display_1in54_ST7789 = {240, 240};

Display *display_spi_init( DisplayModel *model, short display_phy_setup );

void display_spi_clear( UWORD Color );
void display_spi_display( UWORD *Image );

#endif // _GARDEN_200MS_NET__DRIVERS_DISPLAY_SPI_H_

