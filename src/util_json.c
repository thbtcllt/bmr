/* SPDX-License-Identifier: AGPL-3.0-or-later */

#include "util_json.h"
#include "pmbus_io.h"

#include <jansson.h>

static void
add_hex_ascii(json_t *dst, const uint8_t *buf, int n) {
  json_object_set_new(dst, "len", json_integer(n));
  json_object_set_new(dst, "ascii", json_stringn((const char *) buf, n));

  char *hex = (char *) malloc((size_t) n * 2 + 1);

  for (int i = 0; i < n; i++)
    sprintf(hex + 2 * i, "%02X", buf[i]);
  hex[n * 2] = '\0';

  json_object_set_new(dst, "hex", json_string(hex));

  free(hex);
}

void
rd_block_string(int fd, uint8_t cmd, const char *key, json_t *root) {
  uint8_t b[64];
  int n = pmbus_rd_block(fd, cmd, b, (int) sizeof b);
  if (n < 0)
    return;
  json_t *o = json_object();
  add_hex_ascii(o, b, n);

  json_object_set_new(root, key, o);
}

void
json_print_or_pretty(json_t *o, int pretty) {
  if (!o)
    return;

  char *s = json_dumps(o, pretty ? JSON_INDENT(2) | JSON_SORT_KEYS : JSON_SORT_KEYS);

  puts(s);
  free(s);

  json_decref(o);
}
