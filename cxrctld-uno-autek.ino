/*
 * cxrctlfw-uno-autek
 * Stanislaw Grams <stanislawgrams@gmail.com>
 *
 * Firmware for AUTEK TYPE 18 (18-D-DIP)
 * coaxial relay hooked to Arduino Uno R3
 *
 * cxrctld-uno-autek.ino
*/
#include <string.h>

#define CXR_DEFAULT_NAME         "AUTEK TYPE 18: 18-D-DIP"
#define CXR_DEFAULT_NAME_MAX_LEN 32
#define CXR_DEFAULT_IPORTS       8
#define CXR_DEFAULT_OPORTS       1

typedef struct {
  char    name[CXR_DEFAULT_NAME_MAX_LEN];
  uint8_t iport_num;
  uint8_t oport_num;
} cxr_info_t;

typedef struct {
  uint8_t iport[CXR_DEFAULT_IPORTS];
  uint8_t oport[CXR_DEFAULT_OPORTS];
} cxr_state_t;

cxr_info_t  m_info;
cxr_state_t m_state;

const void _init_info (cxr_info_t *info) {
  // check input parameters
  if (!info) return;

  // name
  strncpy (info->name, CXR_DEFAULT_NAME, CXR_DEFAULT_NAME_MAX_LEN);

  // in ports
  info->iport_num = (uint8_t)CXR_DEFAULT_IPORTS;

  // out ports
  info->oport_num = (uint8_t)CXR_DEFAULT_OPORTS;
}

const void _init_state (cxr_state_t *state) {
  // check input parameters
  if (!state) return;

  // zero-out whole state
  memset (state, 0, sizeof (*state));
}

int _toggle_iport (cxr_info_t *info, cxr_state_t *state, uint8_t p) {
  // check input parameters
  if (!info || !state) return -1;

  // validate port
  if (p >= info->iport_num) {
    return -2;
  }

  // iport toggle
  state->iport[p] = !digitalRead (p);
  digitalWrite (p, state->iport[p]);

  return 0;
}

int _toggle_oport (cxr_info_t *info, cxr_state_t *state, uint8_t p) {
  // check input parameters
  if (!info || !state) return -1;

  // oport toggle unsupported on AUTEK TYPE 18: 18-D-DIP
  return -2;
}

int _init_si (cxr_info_t *info, cxr_state_t *state) {
  int rv = 0;

  // initialize input ports
  // by default GPIO pins [0 ... iport_num] are used for the coax relay
  for (int i = 0; i < info->iport_num; i++) {
    pinMode (i, OUTPUT);
    digitalWrite (i, LOW); // make sure all initialized to zero
  }

  // set default input port
  rv = _toggle_iport (info, state, 0); // default: 0
  if (rv) return rv;

  // initialize output ports
  // rv = _toggle_oport (0); // default: N/A (single output port for AUTEK TYPE 18: 18-D-DIP)

  return rv;
}

void setup () {
  // initialize info & state
  _init_info (&m_info);
  _init_state (&m_state);

  // initialize silicon
  _init_si (&m_info, &m_state);

  // debug BEGIN
  pinMode(LED_BUILTIN, OUTPUT);
  // debug END
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
