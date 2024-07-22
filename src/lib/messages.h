#ifndef _SIGMA_200MS_NET__LIB_MESSAGES_H_
#define _SIGMA_200MS_NET__LIB_MESSAGES_H_

#include <stdio.h>
#include <stdbool.h>

/**
* Message Types
*
* Info messages:
*         - print sensors/devices initialisation and de-initialisation messages
*
* Verbose messages:
*      - print messages regarding status change of a certain devices, e.g. signal recived
*
* Debuging messages:
*      - pritn detailed informatio including values read from sensors
*/

/**
	deffinitions of silent, verbose and debugging mode functions and macros
*/

void msgSetSilentMode();
void msgSetVerboseMode();

void msgSetDebuggingMode();


bool msgIsSilentModeSet();
bool msgIsSilentModeUnSet();
bool msgIsVerboseModeSet();

bool msgIsDebuggingModeSet();

/**
	definitions of debug macros, it can be enabled with -DDEBUG gcc option or
	make DEBUG=y
 */

#ifdef DEBUG

#define MSG_DEBUG( desc ) \
	if( msgIsDebuggingModeSet() ) fprintf( stdout, __FILE__ ": %d, %s\n", __LINE__, desc );

#define MSG_DEBUG_( desc, format, val ) \
	if( msgIsDebuggingModeSet() ) fprintf( stdout, __FILE__ ": %d, " desc " is '" format "'\n", __LINE__, val );

#define MSG_DEBUG_DEC( desc, val ) \
	if( msgIsDebuggingModeSet() ) fprintf( stdout, __FILE__ ": %d, " desc " is '%d'\n", __LINE__, val );

#define MSG_DEBUG_STR( desc, val ) \
	if( msgIsDebuggingModeSet() ) fprintf( stdout, __FILE__ ": %d, " desc " is '%s'\n", __LINE__, val );

#define MSG_DEBUG_BR \
	if( msgIsDebuggingModeSet() ) fprintf( stdout, "==============================================\n" );
#else

/**
	just put nothing when DEBUG is not defined
*/
#define DEBUG(__info,...)

#define MSG_DEBUG( desc )
#define MSG_DEBUG_( desc, format, val )
#define MSG_DEBUG_DEC( desc, val )
#define MSG_DEBUG_STR( desc, val )
#define MSG_DEBUG_BR

#endif // DEBUG

/**
* Define standard messages (should be used sparingly)
*/
#define MSG_INFO( msg ) \
	if( msgIsSilentModeSet() == false ) fprintf( stdout, msg "\n" );

#define MSG_INFO_( format, msgs... ) \
	if( msgIsSilentModeSet() == false ) fprintf( stdout, format "\n", msgs );


#define MSGBEGIN_INFO( msg ) \
	if( msgIsSilentModeSet() == false ) fprintf( stdout, msg );

#define MSGBEGIN_INFO_( format, msgs... ) \
	if( msgIsSilentModeSet() == false ) fprintf( stdout, format, msgs );

#define MSGEND_INFO( msg ) \
	if( msgIsSilentModeSet() == false ) fprintf( stdout, msg "\n" );


#define MSG_INFO_FUN( print_fun ) \
	if( msgIsSilentModeSet() == false ) print_fun( stdout );

#define MSG_INFO_FUN_ARGS( print_fun, args... ) \
	if( msgIsSilentModeSet() == false ) print_fun( stdout, args );

/**
* Define warning messages (should be used sparingly)
*/
#define MSG_WARN( msg ) \
if( msgIsSilentModeSet() == false ) fprintf( stdout, msg "\n" );

#define MSG_WARN_( format, msgs... ) \
if( msgIsSilentModeSet() == false ) fprintf( stdout, format "\n", msgs );

// Verbose mode

#define MSG_VERBOSE( msg ) \
	if( msgIsVerboseModeSet() ) fprintf( stdout, msg "\n" );

#define MSG_VERBOSE_( format, msgs... ) \
	if( msgIsVerboseModeSet() ) fprintf( stdout, format "\n", msgs );

#define MSG_VERBOSE_FUN( print_fun ) \
	if( msgIsVerboseModeSet() ) print_fun( stdout );

#define MSG_VERBOSE_FUN_ARGS( print_fun, args... ) \
	if( msgIsVerboseModeSet() ) print_fun( stdout, args );


#define MSG_ERROR( msg ) \
	{ fprintf( stderr, "ERROR: %s\n", msg ); }

#define MSG_ERROR_( format, msgs... ) \
	{ fprintf( stderr, "ERROR: " format, msgs ); }

#define MSG_ERROR_AND_EXIT( msg ) \
	{ fprintf( stderr, "ERROR: %s\n", msg ); exit(1); }


#endif // _SIGMA_200MS_NET__LIB_MESSAGES_H_
