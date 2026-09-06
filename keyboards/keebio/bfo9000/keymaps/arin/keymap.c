// Copyright 2026 Arin George (@ageorge)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "transactions.h"

#define _BASE         0

uint16_t last_pressed = 0;

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


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_BASE] = LAYOUT(
    TO(_BASE),       KC_NO,           KC_ESC,          KC_F1,            KC_F2,           KC_F3,           KC_F4,           KC_F5,           KC_F6,           KC_F7,            KC_F8,           KC_F9,           KC_F10,          KC_F11,           KC_F12,           KC_DELETE,        KC_NO,             KC_INSERT,
    KC_NO,           KC_NO,           KC_GRAVE,        KC_1,             KC_2,            KC_3,            KC_4,            KC_5,            KC_6,            KC_7,             KC_8,            KC_9,            KC_0,            KC_MINUS,         KC_EQUAL,         KC_BSPC,          KC_HOME,           KC_END,
    KC_NO,           KC_NO,           KC_NO,           KC_TAB,           KC_Q,            KC_W,            KC_E,            KC_R,            KC_T,            KC_Y,             KC_U,            KC_I,            KC_O,            KC_P,             KC_LBRC,          KC_RBRC,          KC_BSLS,           KC_PGUP,
    KC_NO,           KC_NO,           KC_NO,           KC_CAPS,          KC_A,            KC_S,            KC_D,            KC_F,            KC_G,            KC_H,             KC_J,            KC_K,            KC_L,            KC_SCLN,          KC_QUOT,          KC_ENT,           KC_NO,             KC_PGDN,
    KC_NO,           KC_NO,           KC_NO,           KC_LSFT,          KC_Z,            KC_X,            KC_C,            KC_V,            KC_B,            KC_N,             KC_M,            KC_COMM,         KC_DOT,          KC_SLSH,          KC_RSFT,          KC_NO,            KC_UP,             KC_NO,
    KC_NO,           KC_NO,           KC_NO,           KC_NO,            KC_LCTL,         KC_LALT,         KC_LGUI,         KC_APPLICATION,  KC_SPC,          KC_SPC,           KC_APP,          KC_RGUI,         KC_RALT,         KC_RCTL,          KC_NO,            KC_LEFT,          KC_DOWN,           KC_RIGHT
)
};
