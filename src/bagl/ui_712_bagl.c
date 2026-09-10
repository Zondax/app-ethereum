/*******************************************************************************
 *   BAGL rendering for the EIP-712 review flows.
 *
 *   The shared EIP-712 logic materialises the whole review as a tag/value pair
 *   array (g_pairsList) before anything is displayed, so this file only has to
 *   page through it. That is a different contract from the pre-NBGL BAGL flow,
 *   which pulled one field at a time through ui_712_next_field(); that function
 *   no longer exists.
 *
 *   Paging is forward-only, matching the behaviour of the streaming flow it
 *   replaces.
 ******************************************************************************/

#ifndef HAVE_NBGL

#include <string.h>

#include "os.h"
#include "ux.h"
#include "glyphs.h"

#include "common_ui.h"
#include "common_712.h"
#include "eip712_v1_ui_logic.h"
#include "proxy_info.h"
#include "shared_context.h"
#include "ui_utils.h"

#ifdef HAVE_EIP712_FULL_SUPPORT

// Index of the pair currently on screen.
static uint8_t s_pair_idx;

// bnnn_paging binds its title and text to fixed addresses at declaration, so
// the pair being shown has to be copied into buffers rather than pointed at.
// The value reuses the shared scratch buffer; the tag needs one of its own,
// since strDataTmp_t no longer carries a second field.
#define PAIR_TITLE_SIZE 64
static char s_pair_title[PAIR_TITLE_SIZE];

/**
 * Copy the current pair into the buffers the paging step reads.
 */
static void prepare_pair(void) {
    const nbgl_contentTagValue_t *pair;

    strings.tmp.tmp[0] = '\0';
    s_pair_title[0] = '\0';
    if ((g_pairsList == NULL) || (g_pairsList->pairs == NULL) ||
        (s_pair_idx >= g_pairsList->nbPairs)) {
        return;
    }
    pair = &g_pairsList->pairs[s_pair_idx];
    if (pair->item != NULL) {
        strlcpy(s_pair_title, pair->item, sizeof(s_pair_title));
    }
    if (pair->value != NULL) {
        strlcpy(strings.tmp.tmp, pair->value, sizeof(strings.tmp.tmp));
    }
}

static void review_done(void) {
    ui_all_cleanup();
    proxy_cleanup();
    ui_idle();
}

static void approve(void) {
    ui_712_approve_cb();
    review_done();
}

static void reject(void) {
    ui_712_reject_cb();
    review_done();
}

static void next_pair(void);

// clang-format off
UX_STEP_NOCB(
    ux_712_step_review,
    pnn,
    {
      &C_icon_eye,
      "Review",
      "typed message",
    });
UX_STEP_NOCB_INIT(
    ux_712_step_pair,
    bnnn_paging,
    prepare_pair(),
    {
      .title = s_pair_title,
      .text = strings.tmp.tmp,
    });
UX_STEP_INIT(
    ux_712_step_advance,
    NULL,
    NULL,
    {
      next_pair();
    });
UX_STEP_CB(
    ux_712_step_approve,
    pbb,
    approve(),
    {
      &C_icon_validate_14,
      "Sign",
      "message",
    });
UX_STEP_CB(
    ux_712_step_reject,
    pbb,
    reject(),
    {
      &C_icon_crossmark,
      "Cancel",
      "signature",
    });
// clang-format on

UX_FLOW(ux_712_flow,
        &ux_712_step_review,
        &ux_712_step_pair,
        &ux_712_step_advance,
        &ux_712_step_approve,
        &ux_712_step_reject);

/**
 * Move to the next pair, or fall through to the signing steps once the list is
 * exhausted.
 */
static void next_pair(void) {
    s_pair_idx++;
    if ((g_pairsList != NULL) && (s_pair_idx < g_pairsList->nbPairs)) {
        ux_flow_init(0, ux_712_flow, &ux_712_step_pair);
    } else {
        ux_flow_next();
    }
}

static void start_review(void) {
    s_pair_idx = 0;
    ux_flow_init(0, ux_712_flow, NULL);
}

bool ui_sign_712_v1(e_eip712_filtering_mode filtering_mode) {
    UNUSED(filtering_mode);

    if (!ui_712_push_pairs()) {
        return false;
    }
    start_review();
    return true;
}

bool ui_sign_712_v0(void) {
    if (!ui_712_start(EIP712_FILTERING_BASIC)) {
        return false;
    }
    // Domain hash and message hash.
    if (!ui_pairs_init(2)) {
        return false;
    }
    eip712_format_hash(0);

    start_review();
    return true;
}

#endif  // HAVE_EIP712_FULL_SUPPORT

#endif  // !HAVE_NBGL
