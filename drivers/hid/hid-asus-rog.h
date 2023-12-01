enum ally_gamepad_cmd {
	ally_gamepad_cmd_set_mode			= 0x01,
	ally_gamepad_cmd_set_map			= 0x02,
	ally_gamepad_cmd_set_js_map			= 0x03,
	ally_gamepad_cmd_set_js_dz			= 0x04, /* deadzones */
	ally_gamepad_cmd_set_tr_dz			= 0x05, /* deadzones */
	ally_gamepad_cmd_set_vibe_intensity = 0x06,
	ally_gamepad_cmd_check_ready		= 0x0A,
	ally_gamepad_cmd_xpad_status		= 0x0C,
	ally_gamepad_cmd_check_for_js_curve = 0x12,
	ally_gamepad_cmd_set_js_curve		= 0x13,
	ally_gamepad_cmd_set_xbox_output	= 0x15,
	ally_gamepad_cmd_check_for_js_adz	= 0x17, // anti-deadzones
	ally_gamepad_cmd_set_js_adz			= 0x18, // anti-deadzones
};

enum ally_gamepad_mode {
	ally_gamepad_mode_game	= 0x01,
	ally_gamepad_mode_wasd	= 0x02,
	ally_gamepad_mode_mouse	= 0x03,
};

enum ally_analogue {
	ally_analogue_stick_left	= 0x01,
	ally_analogue_stick_right	= 0x02,
	ally_analogue_trigger_left	= 0x03,
	ally_analogue_trigger_right	= 0x04,
};

/* ROG Ally has many settings related to the gamepad, all using the same n-key endpoint */
struct asus_rog_ally {
	enum ally_gamepad_mode mode;
	// TODO: custom mapping
	// TODO: joystick mapping
	u8 deadzone_left_stick[4];
	u8 deadzone_right_stick[4];
	/*
	 * index: left-min, left-max, right-min, right-max
	 * min/max: 0-64
	 */
	u8 deadzone_triggers[4];
	/*
	 * index: left, right
	 * max: 64
	 */
	u8 vibration_intensity[2];
	/*
	 * joystick response curves:
	 * - 4 points of 2 bytes each
	 * - byte 0 of pair = stick move %
	 * - byte 1 of pair = stick response %
	 * - min/max: 1-63
	 */
	 bool supports_response_curves;
	u8 response_curve_left_stick[8];
	u8 response_curve_right_stick[8];
	/* left = byte 0, right = byte 1*/
	u8 anti_deadzones[2];
};

enum ally_gamepad_map_group {
	ally_gamepad_map_group_blank	= 0x00,
	ally_gamepad_map_group_xpad		= 0x01,
	ally_gamepad_map_group_keyboard	= 0x02,
	ally_gamepad_map_group_mouse	= 0x03,
	ally_gamepad_map_group_macro	= 0x04,
	ally_gamepad_map_group_media	= 0x05,
};

/* when used for custom mapping the byte preceding must be 0x01 */
enum ally_gamepad_button_code {
	ally_gamepad_button_code_a		= 0x01,
	ally_gamepad_button_code_b		= 0x02,
	ally_gamepad_button_code_x		= 0x03,
	ally_gamepad_button_code_y		= 0x04,
	ally_gamepad_button_code_lb		= 0x05,
	ally_gamepad_button_code_rb		= 0x06,
	ally_gamepad_button_code_ls		= 0x07,
	ally_gamepad_button_code_lr		= 0x08,
	ally_gamepad_button_code_view	= 0x11,
	ally_gamepad_button_code_menu	= 0x12,
	ally_gamepad_button_code_xbox	= 0x13,
};