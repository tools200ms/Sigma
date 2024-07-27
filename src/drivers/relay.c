
/**
 * https://www.waveshare.com/wiki/RPi_Relay_Board
 *
 */

#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

#include <wiringPi.h>

#include "../lib/messages.h"

#include "relay.h"


#define RELAY_ONE_CHANEL 1
#define RELAY_TWO_CHANEL 2
#define RELAY_THREE_CHANEL 3
#define RELAY_FOUR_CHANEL 3


static void _mark_channel_status_open( unsigned long long *status, const int ch_no );
static void _mark_channel_status_closed( unsigned long long *status, const int ch_no );


RelayDev *relay1ch_create( int ch_pin ) {
	return relay_create( DEV_RELAY_MODE_NORMALLY_OPEN, RELAY_ONE_CHANEL, ch_pin );
}

RelayDev *relay2ch_create( int ch1_pin, int ch2_pin ) {
	return relay_create( DEV_RELAY_MODE_NORMALLY_OPEN, RELAY_TWO_CHANEL, ch1_pin, ch2_pin );
}

RelayDev *relay3ch_create( int ch1_pin, int ch2_pin, int ch3_pin ) {
	return relay_create( DEV_RELAY_MODE_NORMALLY_OPEN, RELAY_THREE_CHANEL, ch1_pin, ch2_pin, ch3_pin );
}

RelayDev *relay4ch_init( int ch1_pin, int ch2_pin, int ch3_pin, int ch4_pin ) {
	return relay_create( DEV_RELAY_MODE_NORMALLY_OPEN, RELAY_FOUR_CHANEL, ch1_pin, ch2_pin, ch3_pin, ch4_pin );
}

RelayDev *relayXch_create( const short mode, int chanel_cnt, int pins[] ) {

	RelayDev *relay_dev = NULL;

	if( chanel_cnt == 0 || chanel_cnt > DEV_RELAY_MAX_CHANEL_CNT ) {
		MSG_ERROR_( "Illegal chanel count provided: %d\n", chanel_cnt );
		return NULL;
	} else if( chanel_cnt < 0 ) {
		for( int idx = 0; idx < DEV_RELAY_MAX_CHANEL_CNT; ++idx ) {

			if( pins[idx] < 0 ) {
				chanel_cnt = idx;
				break;
			}
		}

		if( chanel_cnt < 0 ) {
			MSG_ERROR( "Can not find chanel cnt." );
			return NULL;
		}
	}

	switch( chanel_cnt ) {
		case 1:
			relay_dev = relay_create( mode, chanel_cnt, pins[0] );
		break;

		case 2:
			relay_dev = relay_create( mode, chanel_cnt, pins[0], pins[1] );
		break;

		case 3:
			relay_dev = relay_create( mode, chanel_cnt, pins[0], pins[1], pins[2] );
		break;

		default:
			MSG_ERROR("Function 'relayXch_create()' NOT FULLY IMPLEMENTED !!!");
	}

	return relay_dev;
}

RelayDev *relay_create( const short mode, const int chanel_cnt, ... ) {
	va_list pin_list;
	RelayDev *relay_dev;
	int *relay_chanel_pins;

	// TODO: validate mode

	if( chanel_cnt < 1 || chanel_cnt > DEV_RELAY_MAX_CHANEL_CNT ) {
		MSG_ERROR_( "Illegal chanel count provided: %d\n", chanel_cnt );
		return NULL;
	}

	if( (relay_dev = malloc(sizeof(RelayDev))) == NULL ||
		(relay_dev->chanel_pins = relay_chanel_pins = malloc(chanel_cnt * sizeof(int))) == NULL ) {

		MSG_ERROR_MALLOC();
	}

	if( (mode & (DEV_RELAY_MODE_NULL)) != (DEV_RELAY_MODE_NULL) ) {
		MSG_ERROR( "Incorrect relay mode flag provided" );
		return NULL;
	}

	char *mode_str;

	if( (mode & (DEV_RELAY_MODE_NORMALLY_OPEN)) == (DEV_RELAY_MODE_NORMALLY_OPEN) ) {
		relay_dev->OPEN_FLAG  = LOW;
		relay_dev->CLOSE_FLAG = HIGH;
		mode_str = "open";
	} else if( (mode & (DEV_RELAY_MODE_NORMALLY_CLOSE)) == (DEV_RELAY_MODE_NORMALLY_CLOSE) ) {
		relay_dev->OPEN_FLAG  = HIGH;
		relay_dev->CLOSE_FLAG = LOW;
		mode_str = "close";
	} else {
		MSG_ERROR( "Incorrect relay mode flag provided" );
		return NULL;
	}

	MSGBEGIN_INFO_( "Adding %d chanel relay (normally %s mode): \n", chanel_cnt, mode_str );

	relay_dev->mode = mode;
	relay_dev->chanel_cnt = chanel_cnt;
	relay_dev->status = 0ULL;

	va_start(pin_list, chanel_cnt);

	/* iterate over pins provided as arguments */
	for( int idx = 0, ch = 1, pin; idx < chanel_cnt; ++idx, ++ch ) {
		pin = va_arg( pin_list, int );

		// set GPIO PIN:
		//pinMode( pin, OUTPUT );

		relay_chanel_pins[idx] = pin;

		//MSGBEGIN_INFO_( "\tch. #%d at pin %d", ch, pin );

		// read pin, if open (that indicates that program crashed
		// or other unexpected thing happen) notify
		//if( digitalRead( pin ) != relay_dev->OPEN_FLAG ) {
		//	MSGBEGIN_INFO( "\n" );
		//}
		//else {
		//	MSGBEGIN_INFO( " - !pin already in HIGH state!\n" );
		//	_mark_channel_status_open( &(relay_dev->status), ch );
		//}
	}

	/* clean memory used by variable arguments */
	va_end(pin_list);

	MSGEND_INFO( "Relay OK" );
	return relay_dev;
}

int relay_init( RelayDev *r_dev ) {

	int ch_cnt = r_dev->chanel_cnt;
	int *ch_pins = r_dev->chanel_pins;

	for( int idx = 0, ch = 1, pin; idx < ch_cnt; ++idx, ++ch ) {
		pin = ch_pins[idx];

		// set GPIO PIN:
		pinMode( pin, OUTPUT );

		MSGBEGIN_INFO_( "\tch. #%d at pin %d", ch, pin );

		// read pin, if open (that indicates that program crashed
		// or other unexpected thing happen) notify
		if( digitalRead( pin ) != r_dev->OPEN_FLAG ) {
			MSGBEGIN_INFO( "\n" );
		}
		else {
			MSGBEGIN_INFO( " - !pin already in HIGH state!\n" );
			_mark_channel_status_open( &(r_dev->status), ch );
		}
	}

	return 0;
}

void relay_test( RelayDev *r_dev ) {
	relay_test_chanel( r_dev, -1 );
}

void relay_test_chanel( RelayDev *r_dev, int ch_no ) {

	int first_ch = 1;
	int last_ch  = r_dev->chanel_cnt;

	if( ch_no != -1 ) {
		first_ch = ch_no;
		last_ch  = ch_no;
	}

	MSGBEGIN_INFO( "Testing relay chanel: " );

	for( int c = first_ch; c <= last_ch; ++c ) {
		MSGBEGIN_INFO_("%d ", c);
		relay_opencontact( r_dev, c );
		delayMicroseconds( 2 * 1000 * 1000 );
	}

	for( int c = first_ch; c <= last_ch; ++c ) {
		relay_closecontact( r_dev, c );
		delayMicroseconds( 2 * 1000 * 1000 );
	}

	MSGEND_INFO( "\tdone" );
}

// Relay 'status' updates/reads

inline static void _mark_channel_status_open( unsigned long long *status, const int ch_no ) {
	(*status) |= 1ULL << (ch_no - 1);
}

inline static void _mark_channel_status_closed( unsigned long long *status, const int ch_no ) {
	(*status) &= (~(1ULL << (ch_no - 1)));
}

short relay_status( const RelayDev *r_dev, const int ch_no ) {
	unsigned long long status_mask
							= 1ULL << (ch_no - 1);

	return ((r_dev->status) & status_mask) != 0 ? DEV_RELAY_CH_NOWOPEN : DEV_RELAY_CH_NOWCLOSED;
}

// End of Relay status updates/reads

void relay_opencontact( RelayDev *r_dev, const int ch_no ) {

	if( ch_no < 1 || ch_no > r_dev->chanel_cnt ) {
		printf("Invalid chanel for relay");
	}

	//status_mask = 1ULL << (ch_no - 1);

	if( relay_status( r_dev, ch_no ) == DEV_RELAY_CH_NOWOPEN ) {
		// relay already open!
		MSG_WARN( "Ralay contact already open!" );
		return;
	}

	digitalWrite( r_dev->chanel_pins[ch_no - 1], r_dev->OPEN_FLAG );

	_mark_channel_status_open( &(r_dev->status), ch_no );
	//(r_dev->status) |= status_mask;
}

void relay_closecontact( RelayDev *r_dev, const int ch_no ) {
	//unsigned long long status_mask;

	if( ch_no < 1 || ch_no > r_dev->chanel_cnt ) {
		printf("Invalid chanel for relay");
	}

	//status_mask = 1ULL << (ch_no - 1);

	if( relay_status( r_dev, ch_no ) == DEV_RELAY_CH_NOWCLOSED ) {
		// relay already closed!
		MSG_WARN("Ralay contact already closed!");

		// intentionally no 'return;'
	}

	digitalWrite( r_dev->chanel_pins[ch_no - 1], r_dev->CLOSE_FLAG );

	_mark_channel_status_closed( &(r_dev->status), ch_no );
	//(r_dev->status) &= (~status_mask);
}

