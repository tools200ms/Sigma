/**
 * Code based on demo: https://www.waveshare.com/wiki/1.54inch_LCD_Module#Demo_Download
 */

#include <malloc.h>

#include "../lib/system.h"
#include "../lib/messages.h"

#include "display_spi.h"
#include "spi.h"

static short _display_width = 0, _display_height = 0, _display_pixels = 0;

static void DEV_GPIO_Mode( int pin, int mode ) {
    pinMode( pin, mode );

    if( mode == INPUT ) {
        pullUpDnControl( pin, PUD_UP );
    }
}

static void display_spi_init_gpio() {

    DEV_GPIO_Mode( LCD_CS, OUTPUT );
    DEV_GPIO_Mode( LCD_RST, OUTPUT );
    DEV_GPIO_Mode( LCD_DC, OUTPUT );
    DEV_GPIO_Mode( LCD_BL, OUTPUT );

    /*DEV_GPIO_Mode( KEY_UP_PIN, INPUT );
    DEV_GPIO_Mode( KEY_DOWN_PIN, INPUT );
    DEV_GPIO_Mode( KEY_LEFT_PIN, INPUT );
    DEV_GPIO_Mode( KEY_RIGHT_PIN, INPUT );
    DEV_GPIO_Mode( KEY_PRESS_PIN, INPUT );
    DEV_GPIO_Mode( KEY1_PIN, INPUT );
    DEV_GPIO_Mode( KEY2_PIN, INPUT );
    DEV_GPIO_Mode( KEY3_PIN, INPUT );*/
    SPI_LCD_CS_1;
    SPI_LCD_BL_1;


    wiringPiSPISetup( 0,25000000 );
    pinMode( LCD_BL, PWM_OUTPUT );
    pwmWrite( LCD_BL, 512 );
}

// Hardware reset
static void display_spi_init_reset() {

    SPI_LCD_RST_1;
    delay_ms( 2*100 );
    SPI_LCD_RST_0;
    delay_ms( 2*100 );
    SPI_LCD_RST_1;
    delay_ms( 2*100 );
}

// Set the resolution and scanning method of the screen
static Display * display_spi_init_set_attributes( DisplayModel *model, short display_phy_setup ) {
    Display *displ = NULL;
    UBYTE mem_access_reg = 0x00;

    if( (displ = malloc( sizeof(Display) )) == NULL ) {
        return NULL;
    }

    displ->phy_orientation = display_phy_setup;
    //Get GRAM and LCD width and height
    if(display_phy_setup == DISPLAY_PHY_HORIZONTAL) {
        displ->height	= model->height;
        displ->width  = model->width;
        mem_access_reg = 0X70;
    } else {
        displ->height	= model->width;
        displ->width  = model->height;
        mem_access_reg = 0X00;
    }

    // Set the read / write scan direction of the frame memory
    spi_send_command(0x36); //MX, MY, RGB mode
    spi_send_data_8bit(mem_access_reg);	//0x08 set RGB

    return displ;
}

static void display_spi_init_reg() {
    spi_send_command(0x3A);
    spi_send_data_8bit(0x05);

    spi_send_command(0xB2);
    spi_send_data_8bit(0x0C);
    spi_send_data_8bit(0x0C);
    spi_send_data_8bit(0x00);
    spi_send_data_8bit(0x33);
    spi_send_data_8bit(0x33);

    spi_send_command(0xB7);  //Gate Control
    spi_send_data_8bit(0x35);

    spi_send_command(0xBB);  //VCOM Setting
    spi_send_data_8bit(0x19);

    spi_send_command(0xC0); //LCM Control
    spi_send_data_8bit(0x2C);

    spi_send_command(0xC2);  //VDV and VRH Command Enable
    spi_send_data_8bit(0x01);
    spi_send_command(0xC3);  //VRH Set
    spi_send_data_8bit(0x12);
    spi_send_command(0xC4);  //VDV Set
    spi_send_data_8bit(0x20);

    spi_send_command(0xC6);  //Frame Rate Control in Normal Mode
    spi_send_data_8bit(0x0F);

    spi_send_command(0xD0);  // Power Control 1
    spi_send_data_8bit(0xA4);
    spi_send_data_8bit(0xA1);

    spi_send_command(0xE0);  //Positive Voltage Gamma Control
    spi_send_data_8bit(0xD0);
    spi_send_data_8bit(0x04);
    spi_send_data_8bit(0x0D);
    spi_send_data_8bit(0x11);
    spi_send_data_8bit(0x13);
    spi_send_data_8bit(0x2B);
    spi_send_data_8bit(0x3F);
    spi_send_data_8bit(0x54);
    spi_send_data_8bit(0x4C);
    spi_send_data_8bit(0x18);
    spi_send_data_8bit(0x0D);
    spi_send_data_8bit(0x0B);
    spi_send_data_8bit(0x1F);
    spi_send_data_8bit(0x23);

    spi_send_command(0xE1);  //Negative Voltage Gamma Control
    spi_send_data_8bit(0xD0);
    spi_send_data_8bit(0x04);
    spi_send_data_8bit(0x0C);
    spi_send_data_8bit(0x11);
    spi_send_data_8bit(0x13);
    spi_send_data_8bit(0x2C);
    spi_send_data_8bit(0x3F);
    spi_send_data_8bit(0x44);
    spi_send_data_8bit(0x51);
    spi_send_data_8bit(0x2F);
    spi_send_data_8bit(0x1F);
    spi_send_data_8bit(0x1F);
    spi_send_data_8bit(0x20);
    spi_send_data_8bit(0x23);

    spi_send_command(0x21);  //Display Inversion On

    spi_send_command(0x11);  //Sleep Out

    spi_send_command(0x29);  //Display On
}


Display *display_spi_init( DisplayModel *model, short display_phy_setup ) {
    Display *displ;

    MSGBEGIN_INFO_( "Initialising display: %dx%d - %s orientation: ",
                    model->width, model->height,
                    ( display_phy_setup == DISPLAY_PHY_HORIZONTAL ) ? "horizontal" : "vertical" );

    display_spi_init_gpio();

    //Turn on the backlight
    SPI_LCD_BL_1;

    //Hardware reset, at this point screan turns black
    display_spi_init_reset();

    //Set the resolution and scanning method of the screen
    displ = display_spi_init_set_attributes( model, display_phy_setup );

    //Set the initialization register
    display_spi_init_reg();

    _display_width  = displ->width;
    _display_height = displ->height;
    _display_pixels = _display_width * _display_height;

    MSGEND_INFO("\tOK");
    return displ;
}



/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
        Xstart 	:   X direction Start coordinates
        Ystart  :   Y direction Start coordinates
        Xend    :   X direction end coordinates
        Yend    :   Y direction end coordinates
********************************************************************************/
void display_spi_set_windows( UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend ) {
    //set the X coordinates
    spi_send_command(0x2A);
    spi_send_data_8bit((Xstart >> 8) & 0xFF);
    spi_send_data_8bit(Xstart & 0xFF);
    spi_send_data_8bit(((Xend  - 1) >> 8) & 0xFF);
    spi_send_data_8bit((Xend  - 1) & 0xFF);

    //set the Y coordinates
    spi_send_command(0x2B);
    spi_send_data_8bit((Ystart >> 8) & 0xFF);
    spi_send_data_8bit(Ystart & 0xFF);
    spi_send_data_8bit(((Yend  - 1) >> 8) & 0xFF);
    spi_send_data_8bit((Yend  - 1) & 0xFF);

    spi_send_command(0X2C);
}


/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void display_spi_clear( UWORD Color ) {
    UWORD j;
    UWORD Image[ 240*240 ];

    Color = ((Color<<8)&0xff00)|(Color>>8);

    for (j = 0; j < _display_pixels; j++) {
        Image[j] = Color;
    }

    display_spi_set_windows(0, 0, _display_width, _display_height);
    SPI_LCD_DC_1;
    for(j = 0; j < _display_height; j++){
        spi_write_bytes( (uint8_t *)&Image[ j * _display_width ], _display_width * 2 );
    }
}

/******************************************************************************
function :	Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
void display_spi_display( UWORD *Image ) {
    UWORD j;
    display_spi_set_windows(0, 0, _display_width, _display_height);
    SPI_LCD_DC_1;
    for(j = 0; j < _display_height; j++){
        spi_write_bytes( (uint8_t *)&Image[j*_display_width], _display_width * 2 );
    }
}

void display_spi_display_windows( UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image ) {
    // display
    UDOUBLE Addr = 0;

    UWORD j;
    display_spi_set_windows( Xstart, Ystart, Xend , Yend );
    SPI_LCD_DC_1;

    for (j = Ystart; j < Yend - 1; j++) {
        Addr = Xstart + j * _display_width;
        spi_write_bytes((uint8_t *)&Image[Addr], (Xend-Xstart)*2);
    }
}

void display_spi_display_putpixel(UWORD X, UWORD Y, UWORD Color) {
    display_spi_set_windows(X,Y,X,Y);
    spi_send_data_16bit(Color);
}
