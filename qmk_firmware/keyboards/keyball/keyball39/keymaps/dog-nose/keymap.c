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

// カスタムキーコード定義
enum custom_keycodes {
    EMAIL_ADDR_0 = QK_KB_0,  // User 0: y.hiro4823gta@gmail.com
    EMAIL_ADDR_1 = QK_KB_1,  // User 1: dog.nose.rc@gmail.com
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LGUI  , KC_LALT  ,LSFT_T(KC_LNG2),LT(1,KC_SPC),LT(3,KC_LNG1),KC_BSPC,LT(2,KC_ENT),LSFT_T(KC_LNG2),KC_RALT,KC_RGUI, KC_RSFT
  ),

  [1] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_RBRC  ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_F5    , KC_EXLM  , S(KC_6)  ,S(KC_INT3), S(KC_8)  ,                           S(KC_INT1), KC_BTN1  , KC_PGUP  , KC_BTN2  , KC_SCLN  ,
    S(KC_EQL),S(KC_LBRC),S(KC_7)   , S(KC_2)  ,S(KC_RBRC),                            KC_LBRC  , KC_DLR   , KC_PGDN  , KC_BTN3  , KC_F11   ,
    KC_INT1  , KC_EQL   , S(KC_3)  , _______  , _______  , _______  ,      TO(2)    , TO(0)    , _______  , KC_RALT  , KC_RGUI  , KC_F12
  ),

  [2] = LAYOUT_universal(
    KC_TAB   , KC_7     , KC_8     , KC_9     , KC_MINS  ,                            KC_NUHS  , _______  , KC_BTN3  , _______  , KC_BSPC  ,
   S(KC_QUOT), KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                            S(KC_9)  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_QUOT  ,
    KC_SLSH  , KC_1     , KC_2     , KC_3     ,S(KC_MINS),                           S(KC_NUHS), KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  ,
    KC_ESC   , KC_0     , KC_DOT   , KC_DEL   , KC_ENT   , KC_BSPC  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            _______  , _______  , _______  , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),

  [4] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LGUI  , KC_LALT  ,LSFT_T(KC_LNG2),LT(1,KC_SPC),LT(3,KC_LNG1),KC_BSPC,LT(2,KC_ENT),LSFT_T(KC_LNG2),KC_RALT,KC_RGUI, KC_RSFT
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
  // Auto enable scroll mode when the highest layer is 3
  // keyball_set_scroll_mode(get_highest_layer(state) == 3);
  return state;
}

// カスタムキーコード処理
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

// ============================================================================
// カスタムトラックボール制御
// - レイヤー0,2,3（キーボード専用）ではマウス移動を無効化
// - トラックボールを左→右にジェスチャーしたらレイヤー1（マウスレイヤー）に自動切り替え
//   - 左への移動：25以上（-25以下）
//   - 右への移動：15以上（15以上）
//   - 左から右への遷移時間：200ms以内
// ============================================================================

// ヘルパー関数（keyball.cのstatic関数を再定義）
static inline int8_t clip2int8(int16_t v) {
    return (v) < -127 ? -127 : (v) > 127 ? 127 : (int8_t)v;
}

// ジェスチャー検出用の変数
static bool left_motion_detected = false;
static uint32_t left_motion_time = 0;

#define MOUSE_LAYER 1
#define LEFT_MOTION_THRESHOLD -25   // 左への移動量（負の値で25以上）
#define RIGHT_MOTION_THRESHOLD 15   // 右への移動量（正の値で15以上）
#define MOTION_TIME_WINDOW 200      // 200ms以内

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    uint8_t current_layer = get_highest_layer(layer_state);

    // モーションの有無をチェック（モーション値をクリアする前に）
    bool has_motion = (m->x != 0 || m->y != 0);

    if (current_layer == MOUSE_LAYER) {
        // マウスレイヤーでは通常のマウス動作
        r->x = clip2int8(m->y);
        r->y = clip2int8(m->x);
        if (is_left) {
            r->x = -r->x;
            r->y = -r->y;
        }
        // ジェスチャー状態をリセット
        left_motion_detected = false;
    } else {
        // キーボード専用レイヤー（0, 2, 3）ではマウス移動を無効化
        // r->x, r->y は設定しない

        // ジェスチャー検出：左→右の動きでマウスレイヤーに切り替え
        if (has_motion) {
            uint32_t now = timer_read32();

            // マウスカーソルのX方向の動き（m->yがマウスのX方向に対応）
            int16_t mouse_x_motion = m->y;
            if (is_left) {
                mouse_x_motion = -mouse_x_motion;  // 左手用の場合は反転
            }

            // 左への動きを検出（負の値で閾値以下）
            if (mouse_x_motion <= LEFT_MOTION_THRESHOLD) {
                left_motion_detected = true;
                left_motion_time = now;
            }
            // 右への動きを検出（正の値で閾値以上）
            else if (left_motion_detected && mouse_x_motion >= RIGHT_MOTION_THRESHOLD) {
                // 左動作から200ms以内かチェック
                if (TIMER_DIFF_32(now, left_motion_time) <= MOTION_TIME_WINDOW) {
                    // マウスレイヤーに切り替え
                    layer_move(MOUSE_LAYER);
                }
                // ジェスチャー完了、状態リセット
                left_motion_detected = false;
            }
            // 時間切れチェック：左動作から200ms以上経過したらリセット
            else if (left_motion_detected && TIMER_DIFF_32(now, left_motion_time) > MOTION_TIME_WINDOW) {
                left_motion_detected = false;
            }
        }
    }

    // モーション値をクリア（必須）
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
