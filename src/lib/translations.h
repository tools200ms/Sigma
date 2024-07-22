#ifndef _SIGMA_200MS_NET__LIB_TRANSLATIONS_H_
#define _SIGMA_200MS_NET__LIB_TRANSLATIONS_H_

#include "../sigma.h"

#ifndef SIGMA_LANG_VERSION
    #include "translations_en.h"
#elif SIGMA_LANG_VERSION == "en"
    #include "translations_en.h"
#else
    #error "Unsupported LANG"
#endif

#endif // _SIGMA_200MS_NET__LIB_TRANSLATIONS_H_
