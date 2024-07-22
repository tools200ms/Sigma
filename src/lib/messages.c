
#include "messages.h"

// default settings:
static bool suspend_messages = false,
			verbose_messages = false,
			debugging_messages = false;

void msgSetSilentMode() {
    suspend_messages = true;
    // cancel previous possible settings:
    verbose_messages = false;
}

void msgSetVerboseMode() {
    verbose_messages = true;
    // cancel previous possible settings:
    suspend_messages = false;
}

void msgSetDebuggingMode()	{
    debugging_messages = true;
    // cancel previous possible settings:
    suspend_messages = false;
}

inline bool msgIsSilentModeSet()   { return suspend_messages; }
inline bool msgIsSilentModeUnSet()   { return suspend_messages == false; }
inline bool msgIsVerboseModeSet()   { return verbose_messages; }

inline bool msgIsDebuggingModeSet()   { return debugging_messages; }
