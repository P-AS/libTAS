/*
 * Stub header for xcb/xkb.h - macOS compatibility layer
 * Provides stub XKB protocol types
 */

#ifndef __xkb_h_
#define __xkb_h_

#include <stdint.h>

/* XKB types */
typedef uint32_t xcb_xkb_device_spec_t;
typedef uint32_t xcb_xkb_type_id_t;
typedef uint32_t xcb_xkb_type_len_t;
typedef uint32_t xcb_xkb_count_t;
typedef uint32_t xcb_xkb_str_size_t;
typedef uint32_t xcb_xkb_names_index_t;
typedef uint32_t xcb_xkb_n_names_t;
typedef uint32_t xcb_xkb_n_types_t;
typedef uint32_t xcb_xkb_n_indicators_t;
typedef uint32_t xcb_xkb_indicator_map_len_t;
typedef uint32_t xcb_xkb_map_index_t;
typedef uint32_t xcb_xkb_map_n_sections_t;
typedef uint32_t xcb_xkb_n_key_types_t;
typedef uint32_t xcb_xkb_map_section_types_len_t;
typedef uint32_t xcb_xkb_key_codes_t;
typedef uint32_t xcb_xkb_virtual_mods_t;
typedef uint32_t xcb_xkb_first_key_t;
typedef uint32_t xcb_xkb_last_key_t;
typedef uint32_t xcb_xkb_keys_n_syms_t;
typedef uint32_t xcb_xkb_n_groups_t;
typedef uint32_t xcb_xkb_num_keys_t;
typedef uint32_t xcb_xkb_key_sym_index_t;
typedef uint32_t xcb_xkb_sym_index_t;
typedef uint32_t xcb_xkb_key_group_index_t;
typedef uint32_t xcb_xkb_group_width_t;
typedef uint32_t xcb_xkb_key_sym_translate_index_t;
typedef uint32_t xcb_xkb_sym_translate_index_t;
typedef uint32_t xcb_xkb_sym_translate_group_width_t;
typedef uint32_t xcb_xkb_action_index_t;
typedef uint32_t xcb_xkb_actions_len_t;
typedef uint32_t xcb_xkb_action_type_t;
typedef uint32_t xcb_xkb_action_flags_t;
typedef uint32_t xcb_xkb_action_key_t;
typedef uint32_t xcb_xkb_action_group_t;
typedef uint32_t xcb_xkb_action_mods_t;
typedef uint32_t xcb_xkb_action_mods_mask_t;
typedef uint32_t xcb_xkb_action_t;
typedef uint32_t xcb_xkb_indicator_index_t;
typedef uint32_t xcb_xkb_indicator_mask_t;
typedef uint32_t xcb_xkb_indicator_state_t;
typedef uint32_t xcb_xkb_controls_t;
typedef uint32_t xcb_xkb_controls_mask_t;
typedef uint32_t xcb_xkb_led_group_t;
typedef uint32_t xcb_xkb_led_keys_t;
typedef uint32_t xcb_xkb_btn_codes_t;
typedef uint32_t xcb_xkb_btn_indices_t;
typedef uint32_t xcb_xkb_n_buttons_t;
typedef uint32_t xcb_xkb_btn_action_t;
typedef uint32_t xcb_xkb_btn_action_group_t;
typedef uint32_t xcb_xkb_btn_action_mods_t;
typedef uint32_t xcb_xkb_btn_action_mods_mask_t;

/* XKB constants */
#define XCB_XKB_USE_CORE_KBD 0

#endif /* __xkb_h_ */
