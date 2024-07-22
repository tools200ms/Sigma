
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

#define RELAY_MAX_CHANEL_CNT 32

#define RELAY_ONE_CHANEL 1
#define RELAY_TWO_CHANEL 2
#define RELAY_THREE_CHANEL 3
#define RELAY_FOUR_CHANEL 3


static void _mark_channel_status_open( unsigned long long *status, const int ch_no );
static void _mark_channel_status_closed( unsigned long long *status, const int ch_no );


RelayDev *relay1ch_init( int ch_pin ) {
	return relay_init( RELAY_MODE_NORMALLY_OPEN, RELAY_ONE_CHANEL, ch_pin );
}

RelayDev *relay2ch_init( int ch1_pin, int ch2_pin ) {
	return relay_init( RELAY_MODE_NORMALLY_OPEN, RELAY_TWO_CHANEL, ch1_pin, ch2_pin );
}

RelayDev *relay3ch_init( int ch1_pin, int ch2_pin, int ch3_pin ) {
	return relay_init( RELAY_MODE_NORMALLY_OPEN, RELAY_THREE_CHANEL, ch1_pin, ch2_pin, ch3_pin );
}

RelayDev *relay4ch_init( int ch1_pin, int ch2_pin, int ch3_pin, int ch4_pin ) {
	return relay_init(RELAY_MODE_NORMALLY_OPEN, RELAY_FOUR_CHANEL, ch1_pin, ch2_pin, ch3_pin, ch4_pin );
}


RelayDev *relay_init(const short mode, const int chanel_cnt, ...) {
	va_list pin_list;
	RelayDev *relay_dev;
	int *relay_chanel_pins;

	// TODO: validate mode

	if( chanel_cnt < 1 || chanel_cnt > RELAY_MAX_CHANEL_CNT ) {
		MSG_ERROR_( "Illegal chanel count provided: %d\n", chanel_cnt );
		return NULL;
	}

	if( (relay_dev = malloc(sizeof(RelayDev))) == NULL ||
		(relay_dev->chanel_pins = relay_chanel_pins = malloc(chanel_cnt * sizeof(int))) == NULL ) {

		MSG_ERROR( "malloc() has failed" );
		return NULL;
	}

	if( (mode & (RELAY_MODE_NULL)) != (RELAY_MODE_NULL) ) {
		MSG_ERROR( "Incorrect relay mode flag provided" );
		return NULL;
	}

	char *mode_str;

	if( (mode & (RELAY_MODE_NORMALLY_OPEN)) == (RELAY_MODE_NORMALLY_OPEN) ) {
		relay_dev->OPEN_FLAG  = LOW;
		relay_dev->CLOSE_FLAG = HIGH;
		mode_str = "open";
	} else if( (mode & (RELAY_MODE_NORMALLY_CLOSE)) == (RELAY_MODE_NORMALLY_CLOSE) ) {
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
		pinMode( pin, OUTPUT );
		relay_chanel_pins[idx] = pin;

		MSGBEGIN_INFO_( "\tch. #%d at pin %d", ch, pin );

		// read pin, if open (that indicates that program crashed
		// or other unexpected thing happen) notify
		if( digitalRead( pin ) != relay_dev->OPEN_FLAG ) {
			MSGBEGIN_INFO( "\n" );
		}
		else {
			MSGBEGIN_INFO( " - !pin already in HIGH state!\n" );
			_mark_channel_status_open( &(relay_dev->status), ch );
		}
	}

	/* clean memory used by variable arguments */
	va_end(pin_list);

	MSGEND_INFO( "Relay OK" );
	return relay_dev;
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

	return ((r_dev->status) & status_mask) != 0 ? RELAY_CH_NOWOPEN : RELAY_CH_NOWCLOSED;
}

// End of Relay status updates/reads

void relay_opencontact( RelayDev *r_dev, const int ch_no ) {
	//unsigned long long status_mask;

	if( ch_no < 1 || ch_no > r_dev->chanel_cnt ) {
		printf("Invalid chanel for relay");
	}

	//status_mask = 1ULL << (ch_no - 1);

	if( relay_status( r_dev, ch_no ) == RELAY_CH_NOWOPEN ) {
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

	if( relay_status( r_dev, ch_no ) == RELAY_CH_NOWCLOSED ) {
		// relay already closed!
		MSG_WARN("Ralay contact already closed!");

		// intentionally no 'return;'
	}

	digitalWrite( r_dev->chanel_pins[ch_no - 1], r_dev->CLOSE_FLAG );

	_mark_channel_status_closed( &(r_dev->status), ch_no );
	//(r_dev->status) &= (~status_mask);
}

