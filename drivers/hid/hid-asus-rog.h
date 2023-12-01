/* required so we can have nested attributes with same name but different functions */
#define ALLY_DEVICE_ATTR_RW(_name, _sysfs_name) \
	struct device_attribute dev_attr_##_name = __ATTR(_sysfs_name, 0644, _name##_show, _name##_store)

#define ALLY_DEVICE_ATTR_RO(_name, _sysfs_name) \
	struct device_attribute dev_attr_##_name = __ATTR(_sysfs_name, 0444, _name##_show, NULL)
	
enum ally_gamepad_cmd {
	ally_gamepad_cmd_set_mode		= 0x01,
	ally_gamepad_cmd_set_js_dz		= 0x04, /* deadzones */
	ally_gamepad_cmd_set_tr_dz		= 0x05, /* deadzones */
	ally_gamepad_cmd_check_ready		= 0x0A,
};

enum ally_gamepad_mode {
	ally_gamepad_mode_game	= 0x01,
	ally_gamepad_mode_wasd	= 0x02,
	ally_gamepad_mode_mouse	= 0x03,
};

enum ally_analogue {
	ally_analogue_joystick_left	= 0x01,
	ally_analogue_joystick_right	= 0x02,
	ally_analogue_trigger_left	= 0x03,
	ally_analogue_trigger_right	= 0x04,
};

/* ROG Ally has many settings related to the gamepad, all using the same n-key endpoint */
struct asus_rog_ally {
	enum ally_gamepad_mode mode;
	/*
	 * index: [joysticks, triggers][left(2 bytes), right(2 bytes)]
	 * joysticks: 2 bytes: inner, outer
	 * triggers: 2 bytes: lower, upper
	 * min/max: 0-64
	*/
	u8 deadzones[2][4];
};
