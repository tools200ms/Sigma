
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <stdlib.h>		//exit()
#include <signal.h>     //signal()

#include <math.h>

#include "drivers/spi.h"
#include "drivers/display_spi.h"

DisplayModel display_1in54_ST7789 = {240, 240};

#include "lib/messages.h"
#include "lib/canvas.h"

#include "shell/arg_parser.h"
#include "shell/conf_read.h"

#include "core/logic_simple.h"
//#include "drivers/buzzer.h"
#include "drivers/1w_bus.h"
#include "drivers/relay.h"

#include "sigma.h"


UWORD *init_display() {
    // Exception handling:ctrl + c
    signal( SIGINT, Handler_clean );

    /* LCD Init */
	display_spi_init( &display_1in54_ST7789, DISPLAY_PHY_HORIZONTAL );

    //display_spi_clear( RED );

    MSG_DEBUG("Ready to draw");
    spi_set_backlight( 255 );

    UDOUBLE Imagesize = 240 * 240 * 2;
    UWORD *img_buff;

    if((img_buff = (UWORD *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage( img_buff, 240, 240, 0, WHITE, 16 );
    Paint_Clear( BLUE );

    return img_buff;
}


void redraw(UWORD *img_buff, float *temp, short relay_ch1_state, short relay_ch2_state) {
    Paint_Clear(WHITE);

    Paint_DrawFloatNum(16, 16, *temp, 1, &Font50, WHITE, BROWN);

    if(relay_ch1_state == DEV_RELAY_CH_NOWOPEN) {
        Paint_DrawRectangle(16, 120, 16 + 20, 120 + 20, RED, 1, 1);
    } else {
        Paint_DrawRectangle(16, 120, 16 + 20, 120 + 20, GRAY, 1, 1);
    }

    if( relay_ch2_state == DEV_RELAY_CH_NOWOPEN ) {
        Paint_DrawRectangle(40, 120, 40 + 20, 120 + 20, RED, 1, 1);
    } else {
        Paint_DrawRectangle(40, 120, 40 + 20, 120 + 20, GRAY, 1, 1);
    }

    display_spi_display(img_buff);
}


int main( int argc, char *argv[] ) {

    uint16_t *image;
    char *conf_file;
    Config *config;

    conf_file = parse_arguments( argc, argv );
    config = read_conf( conf_file );


    MSG_INFO_( "%s - version: %s", SIGMA_NAME, SIGMA_VERSION );
    wiringPiSetupGpio();
    // Display should be initialized as soon as possible
    image = init_display();


    W1BusDev *temp_sensor_ext = w1_bus_add_device( config->dev.temp );
    RelayDev *relay1_dev;


	relay1_dev = relayXch_init( config->dev.relay_mode, -1, config->dev.relay_pins );


    w1_bus_temp_sensor_load( temp_sensor_ext );
    redraw(image, &(temp_sensor_ext->dev.ds18b20.temp), relay_status( relay1_dev, 1), relay_status( relay1_dev, 2));

    // Main loop
    while( 1 ) {
        w1_bus_temp_sensor_load( temp_sensor_ext );

        if( temp_sensor_ext->dev.ds18b20.has_changed ) {

			// Upper range
            /*if( ext_temp > temp1_upper_limit && (relay_status(relay1_dev, 1) == RELAY_CH_NOWCLOSED) ) {
				relay_opencontact( relay1_dev, 1 );
            } else if( ext_temp < temp1_down_limit && (relay_status(relay1_dev, 1) == RELAY_CH_NOWOPEN) ) {
				relay_closecontact(relay1_dev, 1);
            }

            if( ext_temp > temp2_upper_limit && (relay_status(relay1_dev, 2) == RELAY_CH_NOWCLOSED) ) {
				relay_opencontact(relay1_dev, 2);
            } else if( ext_temp < temp2_down_limit && (relay_status(relay1_dev, 2) == RELAY_CH_NOWOPEN) ) {
				relay_closecontact(relay1_dev, 2);
            }*/

            redraw(image, &(temp_sensor_ext->dev.ds18b20.temp), relay_status( relay1_dev, 1), relay_status( relay1_dev, 2));
        }

        sleep(2);
    }

    return 0;
}


void Handler_clean( int signo ) {
    UWORD *img_buff;
    /* Module Exit */

    // scheduler_softexit();

    // Paint_Clear(RED);
    img_buff = Paint_GetImage();

    spi_set_backlight(127);
    display_spi_display( img_buff );
    // exit SPI

    free(img_buff);

    exit(0);
}
