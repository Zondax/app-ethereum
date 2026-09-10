#pragma once

#ifdef HAVE_NBGL
#include "nbgl_use_case.h"
#else
// BAGL devices ship no NBGL, but the EIP-712 UI logic is written against the
// NBGL tag/value model. Provide the layout-agnostic subset of those types so
// that logic, and the allocator below, stay shared between both UI stacks.
// Only the fields the shared code actually touches are declared.
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    const char *item;
    const char *value;
    uint8_t forcePageStart : 1;
    uint8_t centeredInfo : 1;
    uint8_t aliasValue : 1;
} nbgl_contentTagValue_t;

typedef struct {
    const nbgl_contentTagValue_t *pairs;
    uint8_t nbPairs;
    uint8_t startIndex;
    bool wrapping;
} nbgl_contentTagValueList_t;
#endif  // HAVE_NBGL

extern nbgl_contentTagValue_t *g_pairs;
extern nbgl_contentTagValueList_t *g_pairsList;

extern char *g_titleMsg;
extern char *g_subTitleMsg;
extern char *g_finishMsg;

void ui_all_cleanup(void);

bool ui_pairs_init(uint8_t nbPairs);
void ui_pairs_cleanup(void);

bool ui_buffers_init(uint8_t title_len, uint8_t subtitle_len, uint8_t finish_len);
void ui_buffers_cleanup(void);
