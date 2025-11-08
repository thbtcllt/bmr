/* SPDX-License-Identifier: AGPL-3.0-or-later */

#include "pmbus_io.h"
#include "util_json.h"
#include <jansson.h>
#include <string.h>
#include <stdio.h>

static void
out_double(const char *k, double v, int pretty) {
  json_t *o = json_object();
  json_object_set_new(o, k, json_real(v));
  json_print_or_pretty(o, pretty);
}

int
cmd_read(int fd, int argc, char *const *argv, int pretty) {
  const char *what = (argc >= 1) ? argv[0] : "all";

  int exp5 = 0;
  pmbus_get_vout_mode_exp(fd, &exp5);

  if (!strcmp(what, "all")) {
    int vin = pmbus_rd_word(fd, PMBUS_READ_VIN);
    int vout = pmbus_rd_word(fd, PMBUS_READ_VOUT);
    int iout = pmbus_rd_word(fd, PMBUS_READ_IOUT);
    int t1 = pmbus_rd_word(fd, PMBUS_READ_TEMPERATURE_1);
    int t2 = pmbus_rd_word(fd, PMBUS_READ_TEMPERATURE_2);
    int duty = pmbus_rd_word(fd, PMBUS_READ_DUTY_CYCLE);
    int freq = pmbus_rd_word(fd, PMBUS_READ_FREQUENCY);

    json_t *o = json_object();
    if (vin >= 0)
      json_object_set_new(o, "vin_V", json_real(pmbus_lin11_to_double((uint16_t) vin)));
    if (vout >= 0)
      json_object_set_new(o, "vout_V", json_real(pmbus_lin16u_to_double((uint16_t) vout, exp5)));
    if (iout >= 0)
      json_object_set_new(o, "iout_A", json_real(pmbus_lin11_to_double((uint16_t) iout)));
    if (t1 >= 0)
      json_object_set_new(o, "temp1_C", json_real(pmbus_lin11_to_double((uint16_t) t1)));
    if (t2 >= 0)
      json_object_set_new(o, "temp2_C", json_real(pmbus_lin11_to_double((uint16_t) t2)));
    if (duty >= 0)
      json_object_set_new(o, "duty_pct", json_real(pmbus_lin11_to_double((uint16_t) duty)));
    if (freq >= 0)
      json_object_set_new(o, "freq_khz_raw", json_integer(freq));

    json_print_or_pretty(o, pretty);

    return 0;
  }

  if (!strcmp(what, "vin")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_VIN);
    if (v < 0) {
      perror("READ_VIN");
      return 1;
    }
    out_double("vin_V", pmbus_lin11_to_double((uint16_t) v), pretty);

    return 0;
  }

  if (!strcmp(what, "vout")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_VOUT);
    if (v < 0) {
      perror("READ_VOUT");
      return 1;
    }
    out_double("vout_V", pmbus_lin16u_to_double((uint16_t) v, exp5), pretty);

    return 0;
  }

  if (!strcmp(what, "iout")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_IOUT);
    if (v < 0) {
      perror("READ_IOUT");
      return 1;
    }
    out_double("iout_A", pmbus_lin11_to_double((uint16_t) v), pretty);

    return 0;
  }

  if (!strcmp(what, "temp1")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_TEMPERATURE_1);
    if (v < 0) {
      perror("READ_TEMPERATURE_1");
      return 1;
    }
    out_double("temp1_C", pmbus_lin11_to_double((uint16_t) v), pretty);

    return 0;
  }

  if (!strcmp(what, "temp2")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_TEMPERATURE_2);
    if (v < 0) {
      perror("READ_TEMPERATURE_2");
      return 1;
    }
    out_double("temp2_C", pmbus_lin11_to_double((uint16_t) v), pretty);

    return 0;
  }

  if (!strcmp(what, "duty")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_DUTY_CYCLE);
    if (v < 0) {
      perror("READ_DUTY_CYCLE");
      return 1;
    }
    out_double("duty_pct", pmbus_lin11_to_double((uint16_t) v), pretty);

    return 0;
  }

  if (!strcmp(what, "freq")) {
    int v = pmbus_rd_word(fd, PMBUS_READ_FREQUENCY);
    if (v < 0) {
      perror("READ_FREQUENCY");
      return 1;
    }
    json_t *o = json_object();
    json_object_set_new(o, "freq_khz_raw", json_integer(v));
    json_print_or_pretty(o, pretty);

    return 0;
  }

  fprintf(stderr, "read [vin|vout|iout|temp1|temp2|duty|freq|all]\n");

  return 2;
}
