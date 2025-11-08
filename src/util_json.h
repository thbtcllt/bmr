/* SPDX-License-Identifier: AGPL-3.0-or-later */

#pragma once

#include <jansson.h>
#include <stdint.h>

void json_print_or_pretty(json_t * o, int pretty);

void rd_block_string(int fd, uint8_t cmd, const char *key, json_t *root);
