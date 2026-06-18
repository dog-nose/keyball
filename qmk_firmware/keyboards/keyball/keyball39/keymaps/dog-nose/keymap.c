/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

enum custom_keycodes {
    EMAIL_ADDR_0 = KEYBALL_SAFE_RANGE,
    EMAIL_ADDR_1,
};

#define MOUSE_LAYER 3
#define LEFT_MOTION_THRESHOLD  -25
#define RIGHT_MOTION_THRESHOLD  15
#define MOTION_TIME_WINDOW     200

// Layer 4 trackball gestures (macOS).
//   swipe left / right -> move one Space left / right (Ctrl+Left / Ctrl+Right)
//   swipe down         -> Launchpad
//   swipe up           -> Mission Control
// Requires "Move left/right a space" shortcuts enabled in
// System Settings > Keyboard > Keyboard Shortcuts > Mission Control.
#define GESTURE_LAYER        4
// Layer 5 trackball gestures (browser / page navigation).
//   swipe left  -> back        (Mouse Button 4)
//   swipe right -> forward      (Mouse Button 5)
//   swipe up    -> scroll up    (Mouse Wheel Up)
//   swipe down  -> scroll down  (Mouse Wheel Down)
#define GESTURE_LAYER_PAGE   5
#define GESTURE_THRESHOLD    50   // accumulated motion counts needed to fire
#define GESTURE_IDLE_MS      120  // a pause longer than this starts a fresh gesture
#define GESTURE_COOLDOWN_MS  400  // minimum time between two gesture fires

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Base
  [0] = LAYOUT_universal(
    KC_Q              , KC_W                          , KC_E    , KC_R             , KC_T         ,                             KC_Y            , KC_U    , KC_I    , KC_O    , KC_P             ,
    LCTL_T(KC_A)      , KC_S                          , KC_D    , KC_F             , KC_G         ,                             KC_H            , KC_J    , KC_K    , KC_L    , RCTL_T(KC_ENT)   ,
    LSFT_T(KC_Z)      , KC_X                          , KC_C    , KC_V             , KC_B         ,                             KC_N            , KC_M    , KC_COMM , KC_DOT  , RSFT_T(KC_SLSH)  ,
    LCTL_T(KC_TAB)    , MT(MOD_LGUI|MOD_LALT,KC_BSLS) , KC_LNG1 , LGUI_T(KC_LNG2) , LT(1,KC_SPC) , MO(2)  , LGUI_T(KC_BSPC) , LT(4,KC_SPC)  , KC_NO   , KC_NO   , KC_NO   , KC_LNG1
  ),

  // Layer 1: Symbol / Navigation
  [1] = LAYOUT_universal(
    S(KC_GRV)         , KC_UP   , S(KC_LBRC) , S(KC_RBRC) , S(KC_BSLS) ,                             _______     , _______  , KC_LBRC    , KC_RBRC    , KC_BSLS    ,
    LCTL_T(KC_LEFT)   , KC_DOWN , KC_RGHT    , _______    , KC_MINS    ,                             KC_EQL      , _______  , S(KC_SCLN) , S(KC_QUOT) , _______    ,
    LSFT_T(KC_P1)     , KC_P2   , KC_P3      , KC_P4      , KC_P5      ,                             KC_P6       , KC_P7    , KC_P8      , KC_P9      , KC_P0      ,
    _______           , _______  , _______   , _______    , _______    , _______  , _______          , _______    , _______  , _______    , _______    , _______
  ),

  // Layer 2: Symbol / Number (MO(2))
  [2] = LAYOUT_universal(
    S(KC_1)           , S(KC_2) , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                             S(KC_6)     , S(KC_7)  , S(KC_8)  , S(KC_9)  , S(KC_0)        ,
    LCTL_T(KC_COMM)   , KC_2    , KC_3     , KC_4     , KC_5     ,                             S(KC_EQL)   , KC_SCLN  , KC_QUOT  , KC_GRV   , _______        ,
    LSFT_T(KC_1)      , _______  , _______  , _______  , _______  ,                             KC_6        , KC_7     , KC_8     , KC_9     , RSFT_T(KC_0)   ,
    _______           , _______  , _______  , _______  , _______  , _______  , _______          , _______   , _______  , _______  , _______  , _______
  ),

  // Layer 3: Mouse (auto-switch target). Enter pos -> MO(4) gesture, P pos -> MO(5) gesture.
  [3] = LAYOUT_universal(
    _______           , _______  , _______  , _______  , _______  ,                             _______     , KC_LPAD        , KC_MS_WH_DOWN  , _______        , MO(5)    ,
    _______           , _______  , _______  , _______  , _______  ,                             KC_MS_BTN4  , KC_MS_BTN1     , KC_MCTL        , KC_MS_BTN2     , MO(4)    ,
    _______           , _______  , _______  , _______  , _______  ,                             KC_MS_BTN5  , KC_MS_WH_LEFT  , KC_MS_WH_UP    , KC_MS_WH_RIGHT , _______  ,
    _______           , _______  , _______  , _______  , _______  , TG(3)    , TG(3)            , _______   , _______  , _______          , _______        , _______
  ),

  // Layer 4: Gesture A (macOS Spaces / Mission Control / Launchpad) + Numpad / Screenshot
  [4] = LAYOUT_universal(
    KC_P1             , KC_P2   , KC_P3    , KC_P4    , KC_P5    ,                             KC_P6       , KC_P7    , KC_P8                  , KC_P9                   , KC_P0    ,
    _______           , _______  , _______  , _______  , _______  ,                             _______     , _______  , _______                 , _______                 , _______  ,
    _______           , _______  , _______  , _______  , _______  ,                             _______     , _______  , LSFT(LGUI(KC_4))        , LCTL(LSFT(LGUI(KC_4))) , _______  ,
    _______           , _______  , _______  , _______  , _______  , _______  , _______          , _______   , _______  , _______                 , _______                 , _______
  ),

  // Layer 5: Gesture B (browser page navigation: left -> Btn4 back, right -> Btn5 forward, up/down -> page scroll)
  [5] = LAYOUT_universal(
    _______           , _______  , _______  , _______  , _______  ,                             _______     , _______  , _______  , _______  , _______  ,
    _______           , _______  , _______  , _______  , _______  ,                             _______     , _______  , _______  , _______  , _______  ,
    _______           , _______  , _______  , _______  , _______  ,                             _______     , _______  , _______  , _______  , _______  ,
    _______           , _______  , _______  , _______  , _______  , _______  , _______          , _______   , _______  , _______  , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Trackball gestures are driven from motion in
    // keyball_on_apply_motion_to_mouse_move (layers 4 and 5), so scroll
    // mode stays off.
    keyball_set_scroll_mode(false);
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case EMAIL_ADDR_0:
            if (record->event.pressed) {
                SEND_STRING("y.hiro4823gta@gmail.com");
            }
            return false;
        case EMAIL_ADDR_1:
            if (record->event.pressed) {
                SEND_STRING("dog.nose.rc@gmail.com");
            }
            return false;
    }
    return true;
}

static inline int8_t clip2int8(int16_t v) {
    return (v) < -127 ? -127 : (v) > 127 ? 127 : (int8_t)v;
}

static bool left_motion_detected = false;
static uint32_t left_motion_time = 0;

// Layer 4/5 gesture accumulators (see GESTURE_* defines above).
static int16_t  gesture_x           = 0;
static int16_t  gesture_y           = 0;
static uint32_t gesture_last_motion = 0;
static uint32_t gesture_last_fire   = 0;

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    uint8_t current_layer = get_highest_layer(layer_state);
    bool has_motion = (m->x != 0 || m->y != 0);

    if (current_layer == MOUSE_LAYER) {
        r->x = clip2int8(m->y);
        r->y = clip2int8(m->x);
        if (is_left) {
            r->x = -r->x;
            r->y = -r->y;
        }
        left_motion_detected = false;
    } else if (current_layer == GESTURE_LAYER) {
        if (has_motion) {
            uint32_t now = timer_read32();

            // A pause longer than the idle window starts a fresh gesture.
            if (TIMER_DIFF_32(now, gesture_last_motion) > GESTURE_IDLE_MS) {
                gesture_x = 0;
                gesture_y = 0;
            }
            gesture_last_motion = now;

            if (TIMER_DIFF_32(now, gesture_last_fire) <= GESTURE_COOLDOWN_MS) {
                // Still cooling down: swallow motion so one swipe fires once.
                gesture_x = 0;
                gesture_y = 0;
            } else {
                int16_t dx = m->y;  // + = ball moved right
                int16_t dy = m->x;  // + = ball moved down
                if (is_left) {
                    dx = -dx;
                    dy = -dy;
                }
                gesture_x += dx;
                gesture_y += dy;

                int16_t ax = gesture_x < 0 ? -gesture_x : gesture_x;
                int16_t ay = gesture_y < 0 ? -gesture_y : gesture_y;

                if (ax >= GESTURE_THRESHOLD || ay >= GESTURE_THRESHOLD) {
                    if (ax >= ay) {
                        // Horizontal: move between Spaces.
                        tap_code16(gesture_x > 0 ? LCTL(KC_RGHT) : LCTL(KC_LEFT));
                    } else {
                        // Vertical: down -> Launchpad, up -> Mission Control.
                        tap_code16(gesture_y > 0 ? KC_LPAD : KC_MCTL);
                    }
                    gesture_x         = 0;
                    gesture_y         = 0;
                    gesture_last_fire = now;
                }
            }
        }
    } else if (current_layer == GESTURE_LAYER_PAGE) {
        if (has_motion) {
            uint32_t now = timer_read32();

            // A pause longer than the idle window starts a fresh gesture.
            if (TIMER_DIFF_32(now, gesture_last_motion) > GESTURE_IDLE_MS) {
                gesture_x = 0;
                gesture_y = 0;
            }
            gesture_last_motion = now;

            if (TIMER_DIFF_32(now, gesture_last_fire) <= GESTURE_COOLDOWN_MS) {
                // Still cooling down: swallow motion so one swipe fires once.
                gesture_x = 0;
                gesture_y = 0;
            } else {
                int16_t dx = m->y;  // + = ball moved right
                int16_t dy = m->x;  // + = ball moved down
                if (is_left) {
                    dx = -dx;
                    dy = -dy;
                }
                gesture_x += dx;
                gesture_y += dy;

                int16_t ax = gesture_x < 0 ? -gesture_x : gesture_x;
                int16_t ay = gesture_y < 0 ? -gesture_y : gesture_y;

                if (ax >= GESTURE_THRESHOLD || ay >= GESTURE_THRESHOLD) {
                    if (ax >= ay) {
                        // Horizontal: right -> forward (Btn5), left -> back (Btn4).
                        tap_code16(gesture_x > 0 ? KC_MS_BTN5 : KC_MS_BTN4);
                    } else {
                        // Vertical: down -> scroll down, up -> scroll up.
                        tap_code16(gesture_y > 0 ? KC_MS_WH_DOWN : KC_MS_WH_UP);
                    }
                    gesture_x         = 0;
                    gesture_y         = 0;
                    gesture_last_fire = now;
                }
            }
        }
    } else {
        if (has_motion) {
            uint32_t now = timer_read32();
            int16_t mouse_x_motion = m->y;
            if (is_left) {
                mouse_x_motion = -mouse_x_motion;
            }

            if (mouse_x_motion <= LEFT_MOTION_THRESHOLD) {
                left_motion_detected = true;
                left_motion_time = now;
            } else if (left_motion_detected && mouse_x_motion >= RIGHT_MOTION_THRESHOLD) {
                if (TIMER_DIFF_32(now, left_motion_time) <= MOTION_TIME_WINDOW) {
                    layer_move(MOUSE_LAYER);
                }
                left_motion_detected = false;
            } else if (left_motion_detected && TIMER_DIFF_32(now, left_motion_time) > MOTION_TIME_WINDOW) {
                left_motion_detected = false;
            }
        }
    }

    m->x = 0;
    m->y = 0;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}
#endif

#ifdef COMBO_ENABLE
enum combos {
  JK_MB1,
  KL_MB2,
  HL_ESC,
};

const uint16_t PROGMEM jk_mb1_combo[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM kl_mb2_combo[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM hl_esc_combo[] = {KC_H, KC_L, COMBO_END};
combo_t key_combos[] = {
    [JK_MB1] = COMBO(jk_mb1_combo, KC_BTN1),
    [KL_MB2] = COMBO(kl_mb2_combo, KC_BTN2),
    [HL_ESC] = COMBO(hl_esc_combo, KC_ESC),
};

#endif
