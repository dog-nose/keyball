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

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Base
  [0] = LAYOUT_universal(
    KC_Q             , KC_W                        , KC_E    , KC_R            , KC_T          ,                            KC_Y           , KC_U    , KC_I    , KC_O    , KC_P             ,
    LCTL_T(KC_A)     , KC_S                        , KC_D    , KC_F            , KC_G          ,                            KC_H           , KC_J    , KC_K    , KC_L    , RCTL_T(KC_ENT)   ,
    LSFT_T(KC_Z)     , KC_X                        , KC_C    , KC_V            , KC_B          ,                            KC_N           , KC_M    , KC_COMM , KC_DOT  , RSFT_T(KC_SLSH)  ,
    LCTL_T(KC_TAB)   , MT(MOD_LGUI|MOD_LALT,KC_BSLS), KC_LNG1, LGUI_T(KC_LNG2), LT(1,KC_SPC) , MO(2) , LGUI_T(KC_BSPC) , LT(3,KC_SPC)  , KC_NO   , KC_NO   , KC_NO   , LT(4,KC_LNG1)
  ),

  // Layer 1: Symbol / Navigation
  [1] = LAYOUT_universal(
    S(KC_GRV)        , KC_UP   , S(KC_LBRC) , S(KC_RBRC) , S(KC_BSLS) ,                            KC_TRNS    , KC_TRNS , KC_LBRC    , KC_RBRC    , KC_BSLS    ,
    LCTL_T(KC_LEFT)  , KC_DOWN , KC_RGHT    , KC_TRNS    , KC_MINS    ,                            KC_EQL     , KC_TRNS , S(KC_SCLN) , S(KC_QUOT) , KC_TRNS    ,
    LSFT_T(KC_P1)    , KC_P2   , KC_P3      , KC_P4      , KC_P5      ,                            KC_P6      , KC_P7   , KC_P8      , KC_P9      , KC_P0      ,
    KC_TRNS          , KC_TRNS , KC_TRNS    , KC_TRNS    , KC_TRNS    , KC_TRNS , KC_TRNS         , KC_TRNS   , KC_TRNS , KC_TRNS    , KC_TRNS    , KC_TRNS
  ),

  // Layer 2: Symbol / Number (LM(2))
  [2] = LAYOUT_universal(
    S(KC_1)          , S(KC_2) , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                            S(KC_6)    , S(KC_7) , S(KC_8)  , S(KC_9)  , S(KC_0)        ,
    LCTL_T(KC_COMM)  , KC_2    , KC_3     , KC_4     , KC_5     ,                            S(KC_EQL)  , KC_SCLN , KC_QUOT  , KC_GRV   , KC_TRNS        ,
    LSFT_T(KC_1)     , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  ,                            KC_6       , KC_7    , KC_8     , KC_9     , RSFT_T(KC_0)   ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  , KC_TRNS , KC_TRNS         , KC_TRNS  , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS
  ),

  // Layer 3: Numpad / Screenshot
  [3] = LAYOUT_universal(
    KC_P1            , KC_P2   , KC_P3    , KC_P4    , KC_P5    ,                            KC_P6      , KC_P7   , KC_P8                  , KC_P9                   , KC_P0   ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  ,                            KC_TRNS    , KC_TRNS , KC_TRNS                 , KC_TRNS                 , KC_TRNS ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  ,                            KC_TRNS    , KC_TRNS , LSFT(LGUI(KC_4))        , LCTL(LSFT(LGUI(KC_4))) , KC_TRNS ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  , KC_TRNS , KC_TRNS         , KC_TRNS  , KC_TRNS , KC_TRNS                 , KC_TRNS                 , KC_TRNS
  ),

  // Layer 4: Mouse
  [4] = LAYOUT_universal(
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  ,                            KC_TRNS    , KC_LPAD        , KC_MS_WH_DOWN  , KC_TRNS        , KC_TRNS ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  ,                            KC_MS_BTN4 , KC_MS_BTN1     , KC_MCTL        , KC_MS_BTN2     , KC_TRNS ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  ,                            KC_MS_BTN5 , KC_MS_WH_LEFT  , KC_MS_WH_UP    , KC_MS_WH_RIGHT , KC_TRNS ,
    KC_TRNS          , KC_TRNS , KC_TRNS  , KC_TRNS  , KC_TRNS  , TG(4)   , TG(4)           , KC_TRNS  , KC_TRNS , KC_TRNS          , KC_TRNS        , KC_TRNS
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3 (numpad)
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
