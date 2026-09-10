/*******************************************************************************
 *   BAGL counterparts for the remaining screens upstream only ships for NBGL.
 ******************************************************************************/

#ifndef HAVE_NBGL

#include "os.h"
#include "ux.h"
#include "glyphs.h"

#include "common_ui.h"
#include "network.h"

// clang-format off
UX_STEP_CB(
    ux_blind_signing_step,
    pnn,
    ui_idle(),
    {
      &C_icon_crossmark,
      "Blind signing must",
      "be enabled",
    });
// clang-format on

UX_FLOW(ux_blind_signing_flow, &ux_blind_signing_step);

void ui_error_blind_signing(void) {
    ux_flow_init(0, ux_blind_signing_flow, NULL);
}

/**
 * The generic clear-signing screens are NBGL-only, so there is nothing to tear
 * down here. The symbol still has to exist because the shared cleanup paths
 * call it unconditionally.
 */
void ui_gcs_cleanup(void) {
}

/**
 * Dynamically provided networks are an NBGL-only feature: each entry carries an
 * icon for the review screens. Its owning sources are not built for BAGL, but
 * network.c still walks the list, so anchor it empty here.
 */
network_info_t *g_dynamic_network_list = NULL;

#endif  // !HAVE_NBGL
