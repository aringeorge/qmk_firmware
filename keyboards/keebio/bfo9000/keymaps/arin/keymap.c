// Copyright 2026 Arin George (@ageorge)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "transactions.h"

uint16_t last_pressed = 0;

#define _QWERTY     0
#define _COLEMAK    1
#define _DVORAK     2
#define _PROG       3
#define _GAME       4



// Handler executed on the slave when the master sends data
void slave_keycode_rpc_handler(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer)
{
    if (is_keyboard_master())
    {
        return;
    }
    if (initiator2target_buffer == NULL)
    {
        return;
    }
    if (initiator2target_buffer_size != sizeof(uint16_t))
    {
        return;
    }
    uint16_t* keycode_buffer = (uint16_t*)initiator2target_buffer;
    last_pressed = *keycode_buffer;
}

void keyboard_post_init_user(void)
{
    if (is_keyboard_master())
    {
        return;
    }

    transaction_register_rpc(RPC_ID_LAST_KEY, slave_keycode_rpc_handler);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    if (record->event.pressed)
    {
        last_pressed = keycode;

        if (is_keyboard_master())
        {
            transaction_rpc_send(RPC_ID_LAST_KEY, sizeof(keycode), &keycode);
        }
    }
    return true;
}

bool oled_task_user(void)
{
    static char szBuff[32];
    static uint8_t current_layer = 0;
    static uint8_t mod_mask = 0;
    static bool is_master = false;
    static led_t led_state;

    is_master = is_keyboard_master();
    current_layer = get_highest_layer(layer_state);
    mod_mask = get_mods();
    led_state = host_keyboard_led_state();

    sprintf(szBuff, "Key:     %d\n\n", (int)last_pressed);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "Shift:   %d\n\n",(mod_mask & MOD_MASK_SHIFT) ? 1 : 0);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "Control: %d\n\n",(mod_mask & MOD_MASK_CTRL) ? 1 : 0);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "Alt:     %d\n\n",(mod_mask & MOD_MASK_ALT) ? 1 : 0);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "GUI:     %d\n\n",(mod_mask & MOD_MASK_GUI) ? 1 : 0);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "Caps:    %d\n\n", led_state.caps_lock ? 1 : 0);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "Layer:   %d\n\n", (int)current_layer);
    oled_write_P(PSTR(szBuff), false);
    sprintf(szBuff, "Master:  %d", is_master ? 1 : 0);
    oled_write_P(PSTR(szBuff), false);

    return false;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [_QWERTY] = LAYOUT(
        MO(_PROG),      KC_NO,      KC_ESC,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,      KC_F11,      KC_F12,      KC_DEL,      KC_INS,      MO(_PROG),
        TO(_QWERTY),    KC_NO,      KC_GRV,      KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,        KC_MINS,     KC_EQL,      KC_BSPC,     KC_HOME,     KC_END,
        TO(_COLEMAK),   KC_NO,      KC_NO,       KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,        KC_P,        KC_LBRC,     KC_RBRC,     KC_BSLS,     KC_PGUP,
        TO(_DVORAK),    KC_NO,      KC_NO,       KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,        KC_SCLN,     KC_QUOT,     KC_ENT,      KC_NO,       KC_PGDN,
        KC_NO,          KC_NO,      KC_NO,       KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,      KC_SLSH,     KC_RSFT,     KC_NO,       KC_UP,       KC_NO,
        TO(_GAME),      KC_NO,      KC_NO,       KC_NO,      KC_LCTL,    KC_LALT,    KC_LGUI,    KC_APP,     KC_SPC,     KC_SPC,     KC_APP,     KC_RGUI,    KC_RALT,     KC_RCTL,     KC_NO,       KC_LEFT,     KC_DOWN,     KC_RGHT
    ),
    [_COLEMAK] = LAYOUT(
        MO(_PROG),      KC_NO,      KC_ESC,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,      KC_F11,      KC_F12,      KC_DEL,      KC_INS,      MO(_PROG),
        TO(_QWERTY),    KC_NO,      KC_GRV,      KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,        KC_MINS,     KC_EQL,      KC_BSPC,     KC_HOME,     KC_END,
        TO(_COLEMAK),   KC_NO,      KC_NO,       KC_TAB,     KC_Q,       KC_W,       KC_F,       KC_P,       KC_G,       KC_J,       KC_L,       KC_U,       KC_Y,        KC_SCLN,     KC_LBRC,     KC_RBRC,     KC_BSLS,     KC_PGUP,
        TO(_DVORAK),    KC_NO,      KC_NO,       KC_CAPS,    KC_A,       KC_R,       KC_S,       KC_T,       KC_D,       KC_H,       KC_N,       KC_E,       KC_I,        KC_O,        KC_QUOT,     KC_ENT,      KC_NO,       KC_PGDN,
        KC_NO,          KC_NO,      KC_NO,       KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_K,       KC_M,       KC_COMM,    KC_DOT,      KC_SLSH,     KC_RSFT,     KC_NO,       KC_UP,       KC_NO,
        TO(_GAME),      KC_NO,      KC_NO,       KC_NO,      KC_LCTL,    KC_LALT,    KC_LGUI,    KC_APP,     KC_SPC,     KC_SPC,     KC_APP,     KC_RGUI,    KC_RALT,     KC_RCTL,     KC_NO,       KC_LEFT,     KC_DOWN,     KC_RGHT
    ),
    [_DVORAK] = LAYOUT(
        MO(_PROG),      KC_NO,      KC_ESC,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,      KC_F11,      KC_F12,      KC_DEL,      KC_INS,      MO(_PROG),
        TO(_QWERTY),    KC_NO,      KC_GRV,      KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,        KC_LBRC,     KC_RBRC,     KC_BSPC,     KC_HOME,     KC_END,
        TO(_COLEMAK),   KC_NO,      KC_NO,       KC_TAB,     KC_QUOT,    KC_COMM,    KC_DOT,     KC_P,       KC_Y,       KC_F,       KC_G,       KC_C,       KC_R,        KC_L,        KC_SLSH,     KC_EQL,      KC_BSLS,     KC_PGUP,
        TO(_DVORAK),    KC_NO,      KC_NO,       KC_CAPS,    KC_A,       KC_O,       KC_E,       KC_U,       KC_I,       KC_D,       KC_H,       KC_T,       KC_N,        KC_S,        KC_MINS,     KC_ENT,      KC_NO,       KC_PGDN,
        KC_NO,          KC_NO,      KC_NO,       KC_LSFT,    KC_SCLN,    KC_Q,       KC_J,       KC_K,       KC_X,       KC_B,       KC_M,       KC_W,       KC_V,        KC_Z,        KC_RSFT,     KC_NO,       KC_UP,       KC_NO,
        TO(_GAME),      KC_NO,      KC_NO,       KC_NO,      KC_LCTL,    KC_LALT,    KC_LGUI,    KC_APP,     KC_SPC,     KC_SPC,     KC_APP,     KC_RGUI,    KC_RALT,     KC_RCTL,     KC_NO,       KC_LEFT,     KC_DOWN,     KC_RGHT
    ),
    [_PROG] = LAYOUT(
        KC_TRNS,    KC_NO,      KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,       KC_NO,       KC_NO,       KC_NO,       KC_NO,       KC_TRNS,
        KC_LCBR,    KC_RCBR,    KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_GRV,     KC_QUOT,    KC_DQUO,    KC_SCLN,    KC_COLN,    KC_QUES,    KC_BSLS,     KC_NO,       KC_NO,       KC_NO,       KC_NO,       KC_NO,
        KC_LPRN,    KC_RPRN,    KC_NO,       KC_NO,      KC_NO,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_7,       KC_8,       KC_9,       KC_PSLS,     KC_NO,       KC_NO,       KC_NO,       KC_NO,       KC_NO,
        KC_LBRC,    KC_RBRC,    KC_NO,       KC_NO,      KC_NO,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_4,       KC_5,       KC_6,       KC_PAST,     KC_NO,       KC_NO,       KC_NO,       KC_AT,       KC_UNDS,
        KC_LT,      KC_GT,      KC_NO,       KC_NO,      KC_NO,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_1,       KC_2,       KC_3,       KC_PMNS,     KC_NO,       KC_NO,       KC_NO,       KC_HASH,     KC_DLR,
        KC_NO,      KC_NO,      KC_NO,       KC_AMPR,    KC_PIPE,    KC_EXLM,    KC_TILD,    KC_CIRC,    KC_PERC,    KC_0,       KC_DOT,     KC_PEQL,    KC_PPLS,     KC_NO,       KC_NO,       KC_NO,       KC_COMM,     KC_DOT
    ),
    [_GAME] = LAYOUT(
        KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_6,           LCTL(KC_1),     LCTL(KC_2),      LCTL(KC_3),    KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      TO(_QWERTY),
        KC_7,           KC_8,           KC_9,           KC_0,           KC_MINS,        KC_EQL,         LCTL(KC_4),     LCTL(KC_5),      LCTL(KC_6),    KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      TO(_COLEMAK),
        LSFT(KC_1),     LSFT(KC_2),     LSFT(KC_3),     LSFT(KC_4),     LSFT(KC_5),     LSFT(KC_6),     LCTL(KC_7),     LCTL(KC_8),      LCTL(KC_9),    KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      TO(_DVORAK),
        LSFT(KC_7),     LSFT(KC_8),     LSFT(KC_9),     LSFT(KC_0),     LSFT(KC_MINS),  LSFT(KC_EQL),   LCTL(KC_0),     LCTL(KC_MINS),   LCTL(KC_EQL),  KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        LALT(KC_1),     LALT(KC_2),     LALT(KC_3),     LALT(KC_4),     LALT(KC_5),     LALT(KC_6),     KC_NO,          KC_NO,           KC_NO,         KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        LALT(KC_7),     LALT(KC_8),     LALT(KC_9),     LALT(KC_0),     LALT(KC_MINS),  LALT(KC_EQL),   KC_NO,          KC_NO,           KC_NO,         KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO
    )
};
