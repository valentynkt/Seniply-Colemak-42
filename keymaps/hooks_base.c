void hooks_housekeeping_task_user(void);


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

	#ifdef CONSOLE_ENABLE
		uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
	#endif 

  return true;
}

void hooks_housekeeping_task_user() {
    #ifdef RGB_MATRIX_ENABLE
        int val = rgb_matrix_get_val();
        if (val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
            rgb_matrix_decrease_val();
        }
    #endif
}