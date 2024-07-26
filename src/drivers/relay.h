
#ifndef _SIGMA_200MS_NET__DRIVERS_RELAY_H_
#define _SIGMA_200MS_NET__DRIVERS_RELAY_H_


#define DEV_RELAY_MAX_CHANEL_CNT 32

// Mode:
#define DEV_RELAY_MODE_NULL           0x80
#define DEV_RELAY_MODE_NORMALLY_OPEN  0x01 | DEV_RELAY_MODE_NULL // normally default settings for relay
#define DEV_RELAY_MODE_NORMALLY_CLOSE 0x02 | DEV_RELAY_MODE_NULL

#define DEV_RELAY_MODE_RESET_AT_INIT  0x04 | DEV_RELAY_MODE_NULL


#define DEV_RELAY_CH_NOWOPEN 0x04
#define DEV_RELAY_CH_NOWCLOSED 0x07


typedef struct relay_dev {

	unsigned short mode;
	int OPEN_FLAG, CLOSE_FLAG;

    int chanel_cnt;
	int *chanel_pins;
	unsigned long long status;

} RelayDev;

RelayDev *relay1ch_init( int ch_pin );
RelayDev *relay2ch_init( int ch1_pin, int ch2_pin );
RelayDev *relay3ch_init( int ch1_pin, int ch2_pin, int ch3_pin );
RelayDev *relay4ch_init( int ch1_pin, int ch2_pin, int ch3_pin, int ch4_pin );

// Initialise relay by providing pins as int [] table,
// chanel_cnt is chanel count, if set to '-1' function will
// read table until first negative value is encountered.
RelayDev *relayXch_init( const short mode, int chanel_cnt, int pins[] );

RelayDev *relay_init( const short mode, const int chanel_cnt, ... );

void relay_test( RelayDev *r_dev );
void relay_test_chanel( RelayDev *r_dev, int ch_no );

short relay_status( const RelayDev *r_dev, const int ch_no );

void relay_opencontact( RelayDev *r_dev, const int ch_no );
void relay_closecontact( RelayDev *r_dev, const int ch_no );


#endif // _GARDEN_200MS_NET__DRIVERS_RELAY_H_
