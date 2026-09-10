#pragma once

#include "shared_context.h"

#ifdef HAVE_NBGL
#include "nbgl_use_case.h"

// Global Warning struct for NBGL review flows
extern nbgl_warning_t warning;
#endif  // HAVE_NBGL

void ui_idle(void);
void ui_settings(void);
