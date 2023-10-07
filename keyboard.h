
#define KEY_UP 12
#define KEY_DOWN 8
#define KEY_LEFT 13
#define KEY_RIGHT 10
#define KEY_MIDDLE 9

#define KEY_BOTTOM_1 100
#define KEY_BOTTOM_2 11
#define KEY_BOTTOM_3 101

#define JOY_UP 7
#define JOY_DOWN 4
#define JOY_LEFT 3
#define JOY_RIGHT 5
#define JOY_MIDDLE 6

void _init_pin(uint8_t pin) {
  if (pin < 99) {
    pinMode(pin, INPUT);
  }
}

void init_key_pins() {
  _init_pin(KEY_UP);
  _init_pin(KEY_DOWN);
  _init_pin(KEY_LEFT);
  _init_pin(KEY_RIGHT);
  _init_pin(KEY_MIDDLE);

  _init_pin(KEY_BOTTOM_1);
  _init_pin(KEY_BOTTOM_2);
  _init_pin(KEY_BOTTOM_3);

  _init_pin(JOY_UP);
  _init_pin(JOY_DOWN);
  _init_pin(JOY_LEFT);
  _init_pin(JOY_RIGHT);
  _init_pin(JOY_MIDDLE);
}

int inline _read_key_pin(uint8_t pin) {
  if (pin < 99) {
    return digitalRead(pin);
  } else {
    return analogRead(pin - 100);
  }
}

uint16_t inline _get_key_status_in_pos(uint8_t key_pos, int key_value) {
  if (key_pos >= 100) {
    key_pos = key_pos - 100;
    key_value = key_value > 512 ? 0 : 1; 
  } else {
    key_value = key_value == 0 ? 1 : 0;
  }
  
  return ((uint16_t)key_value) << key_pos;
}

uint16_t inline _get_key_status_mask(uint8_t key_pos) {
  return _get_key_status_in_pos(key_pos, 0);
}

uint16_t read_all_key_status() {
  uint16_t status = 0;
  status |= _get_key_status_in_pos(KEY_UP,     _read_key_pin(KEY_UP));
  status |= _get_key_status_in_pos(KEY_DOWN,   _read_key_pin(KEY_DOWN));
  status |= _get_key_status_in_pos(KEY_LEFT,   _read_key_pin(KEY_LEFT));
  status |= _get_key_status_in_pos(KEY_RIGHT,  _read_key_pin(KEY_RIGHT));
  status |= _get_key_status_in_pos(KEY_MIDDLE, _read_key_pin(KEY_MIDDLE));

  status |= _get_key_status_in_pos(KEY_BOTTOM_1, _read_key_pin(KEY_BOTTOM_1));
  status |= _get_key_status_in_pos(KEY_BOTTOM_2, _read_key_pin(KEY_BOTTOM_2));
  status |= _get_key_status_in_pos(KEY_BOTTOM_3, _read_key_pin(KEY_BOTTOM_3));

  status |= _get_key_status_in_pos(JOY_UP,     _read_key_pin(JOY_UP));
  status |= _get_key_status_in_pos(JOY_DOWN,   _read_key_pin(JOY_DOWN));
  status |= _get_key_status_in_pos(JOY_LEFT,   _read_key_pin(JOY_LEFT));
  status |= _get_key_status_in_pos(JOY_RIGHT,  _read_key_pin(JOY_RIGHT));
  status |= _get_key_status_in_pos(JOY_MIDDLE, _read_key_pin(JOY_MIDDLE));

  return status;
}

uint8_t is_key_pressed(uint8_t key, uint16_t key_status) {
  uint16_t mask = _get_key_status_mask(key);
  // Serial.println(" Status: " + String(key_status, HEX) + " + MASK: " + String(mask, HEX) + " === " + String(key_status & mask));
  return (key_status & mask) == 0 ? 0 : 1;
}
