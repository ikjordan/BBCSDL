// sympico.c - Declare functions accessable via SYM
// Automatically generated - do not edit.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

void stub__pio_encode_instr_and_args (void);
void stub__pio_encode_instr_and_src_dest (void);
void stub__pio_encode_irq (void);
void stub__pio_major_instr_bits (void);
void stub_absolute_time_diff_us (void);
void stub_adc_fifo_drain (void);
void stub_adc_fifo_get (void);
void stub_adc_fifo_get_blocking (void);
void stub_adc_fifo_get_level (void);
void stub_adc_fifo_is_empty (void);
void stub_adc_fifo_setup (void);
void stub_adc_get_selected_input (void);
void stub_adc_gpio_init (void);
void adc_init (void);
void stub_adc_irq_set_enabled (void);
void stub_adc_read (void);
void stub_adc_run (void);
void stub_adc_select_input (void);
void stub_adc_set_clkdiv (void);
void stub_adc_set_round_robin (void);
void stub_adc_set_temp_sensor_enabled (void);
void stub_add_alarm_at (void);
void stub_add_alarm_in_ms (void);
void stub_add_alarm_in_us (void);
void stub_add_repeating_timer_ms (void);
void stub_add_repeating_timer_us (void);
void alarm_pool_add_alarm_at (void);
void stub_alarm_pool_add_alarm_in_ms (void);
void stub_alarm_pool_add_alarm_in_us (void);
void stub_alarm_pool_add_repeating_timer_ms (void);
void alarm_pool_add_repeating_timer_us (void);
void alarm_pool_cancel_alarm (void);
void alarm_pool_create (void);
void alarm_pool_destroy (void);
void alarm_pool_get_default (void);
void alarm_pool_hardware_alarm_num (void);
void alarm_pool_init_default (void);
void asctime (void);
void asctime_r (void);
void atoi (void);
void bbcfont (void);
void best_effort_wfe_or_timeout (void);
void busy_wait_ms (void);
void busy_wait_until (void);
void busy_wait_us (void);
void busy_wait_us_32 (void);
void calloc (void);
void stub_cancel_alarm (void);
void cancel_repeating_timer (void);
void stub_channel_config_get_ctrl_value (void);
void stub_channel_config_set_bswap (void);
void stub_channel_config_set_chain_to (void);
void stub_channel_config_set_dreq (void);
void stub_channel_config_set_enable (void);
void stub_channel_config_set_high_priority (void);
void stub_channel_config_set_irq_quiet (void);
void stub_channel_config_set_read_increment (void);
void stub_channel_config_set_ring (void);
void stub_channel_config_set_sniff_enable (void);
void stub_channel_config_set_transfer_data_size (void);
void stub_channel_config_set_write_increment (void);
void stub_check_dma_channel_param (void);
void stub_check_dma_timer_param (void);
void stub_check_gpio_param (void);
void stub_check_hardware_alarm_num_param (void);
void stub_check_irq_param (void);
void stub_check_pio_param (void);
void stub_check_slice_num_param (void);
void stub_check_sm_mask (void);
void stub_check_sm_param (void);
void check_sys_clock_khz (void);
void clock_configure (void);
void clock_configure_gpin (void);
void clock_get_hz (void);
void clock_gpio_init (void);
void clock_set_reported_hz (void);
void clock_stop (void);
void clocks_enable_resus (void);
void clocks_init (void);
void critical_section_deinit (void);
void stub_critical_section_enter_blocking (void);
void stub_critical_section_exit (void);
void critical_section_init (void);
void critical_section_init_with_lock_num (void);
void ctime (void);
void datetime_to_str (void);
void stub_delayed_by_ms (void);
void stub_delayed_by_us (void);
void div_s32s32 (void);
void div_s64s64 (void);
void div_u32u32 (void);
void div_u64u64 (void);
void divmod_s32s32 (void);
void stub_divmod_s32s32_rem (void);
void divmod_s64s64 (void);
void divmod_s64s64_rem (void);
void divmod_u32u32 (void);
void stub_divmod_u32u32_rem (void);
void divmod_u64u64 (void);
void divmod_u64u64_rem (void);
void stub_dma_channel_abort (void);
void stub_dma_channel_acknowledge_irq0 (void);
void stub_dma_channel_acknowledge_irq1 (void);
void dma_channel_claim (void);
void stub_dma_channel_configure (void);
void stub_dma_channel_get_default_config (void);
void stub_dma_channel_get_irq0_status (void);
void stub_dma_channel_get_irq1_status (void);
void dma_channel_is_claimed (void);
void stub_dma_channel_set_config (void);
void stub_dma_channel_set_irq0_enabled (void);
void stub_dma_channel_set_irq1_enabled (void);
void stub_dma_channel_set_read_addr (void);
void stub_dma_channel_set_trans_count (void);
void stub_dma_channel_set_write_addr (void);
void stub_dma_channel_start (void);
void dma_channel_unclaim (void);
void dma_claim_mask (void);
void dma_claim_unused_channel (void);
void dma_claim_unused_timer (void);
void stub_dma_get_channel_config (void);
void stub_dma_get_timer_dreq (void);
void stub_dma_irqn_acknowledge_channel (void);
void stub_dma_irqn_get_channel_status (void);
void stub_dma_irqn_set_channel_enabled (void);
void stub_dma_irqn_set_channel_mask_enabled (void);
void stub_dma_set_irq0_channel_mask_enabled (void);
void stub_dma_set_irq1_channel_mask_enabled (void);
void stub_dma_start_channel_mask (void);
void dma_timer_claim (void);
void dma_timer_is_claimed (void);
void stub_dma_timer_set_fraction (void);
void dma_timer_unclaim (void);
void dma_unclaim_mask (void);
void double2fix (void);
void double2fix64 (void);
void double2int (void);
void double2int64 (void);
void double2int64_z (void);
void double2int_z (void);
void double2ufix (void);
void double2ufix64 (void);
void exception_get_vtable_handler (void);
void exception_restore_handler (void);
void exception_set_exclusive_handler (void);
void exp10 (void);
void exp10f (void);
void fclose (void);
void fflush (void);
void fix2double (void);
void fix2float (void);
void fix642double (void);
void fix642float (void);
void flash_do_cmd (void);
void flash_get_unique_id (void);
void flash_range_erase (void);
void flash_range_program (void);
void float2fix (void);
void float2fix64 (void);
void float2int (void);
void float2int64 (void);
void float2int64_z (void);
void float2int_z (void);
void float2ufix (void);
void float2ufix64 (void);
void fputwc (void);
void fread (void);
void free (void);
void frequency_count_khz (void);
void stub_frequency_count_mhz (void);
void fseek (void);
void fseeko (void);
void fvtab (void);
void fwrite (void);
void stub_get_absolute_time (void);
void getchar_timeout_us (void);
void gmtime_r (void);
void gpio_acknowledge_irq (void);
void stub_gpio_add_raw_irq_handler (void);
void gpio_add_raw_irq_handler_masked (void);
void stub_gpio_add_raw_irq_handler_with_order_priority (void);
void gpio_add_raw_irq_handler_with_order_priority_masked (void);
void stub_gpio_clr_mask (void);
void gpio_debug_pins_init (void);
void gpio_deinit (void);
void stub_gpio_disable_pulls (void);
void stub_gpio_get (void);
void stub_gpio_get_all (void);
void stub_gpio_get_dir (void);
void gpio_get_drive_strength (void);
void gpio_get_function (void);
void stub_gpio_get_irq_event_mask (void);
void stub_gpio_get_out_level (void);
void gpio_get_slew_rate (void);
void gpio_init (void);
void gpio_init_mask (void);
void stub_gpio_is_dir_out (void);
void gpio_is_input_hysteresis_enabled (void);
void stub_gpio_is_pulled_down (void);
void stub_gpio_is_pulled_up (void);
void stub_gpio_pull_down (void);
void stub_gpio_pull_up (void);
void stub_gpio_put (void);
void stub_gpio_put_all (void);
void stub_gpio_put_masked (void);
void stub_gpio_remove_raw_irq_handler (void);
void gpio_remove_raw_irq_handler_masked (void);
void stub_gpio_set_dir (void);
void stub_gpio_set_dir_all_bits (void);
void stub_gpio_set_dir_in_masked (void);
void stub_gpio_set_dir_masked (void);
void stub_gpio_set_dir_out_masked (void);
void gpio_set_dormant_irq_enabled (void);
void gpio_set_drive_strength (void);
void gpio_set_function (void);
void gpio_set_inover (void);
void gpio_set_input_enabled (void);
void gpio_set_input_hysteresis_enabled (void);
void gpio_set_irq_callback (void);
void gpio_set_irq_enabled (void);
void gpio_set_irq_enabled_with_callback (void);
void gpio_set_irqover (void);
void stub_gpio_set_mask (void);
void gpio_set_oeover (void);
void gpio_set_outover (void);
void gpio_set_pulls (void);
void gpio_set_slew_rate (void);
void stub_gpio_xor_mask (void);
void hardware_alarm_cancel (void);
void hardware_alarm_claim (void);
void hardware_alarm_is_claimed (void);
void hardware_alarm_set_callback (void);
void hardware_alarm_set_target (void);
void hardware_alarm_unclaim (void);
void hw_claim_clear (void);
void hw_claim_lock (void);
void hw_claim_or_assert (void);
void hw_claim_unlock (void);
void hw_claim_unused_from_range (void);
void hw_divider_divmod_s32 (void);
void stub_hw_divider_divmod_s32_start (void);
void hw_divider_divmod_u32 (void);
void stub_hw_divider_divmod_u32_start (void);
void stub_hw_divider_pause (void);
void stub_hw_divider_quotient_s32 (void);
void stub_hw_divider_remainder_s32 (void);
void hw_divider_restore_state (void);
void stub_hw_divider_result_nowait (void);
void stub_hw_divider_result_wait (void);
void stub_hw_divider_s32_quotient_inlined (void);
void stub_hw_divider_s32_quotient_wait (void);
void stub_hw_divider_s32_remainder_inlined (void);
void stub_hw_divider_s32_remainder_wait (void);
void hw_divider_save_state (void);
void stub_hw_divider_u32_quotient (void);
void stub_hw_divider_u32_quotient_inlined (void);
void stub_hw_divider_u32_quotient_wait (void);
void stub_hw_divider_u32_remainder (void);
void stub_hw_divider_u32_remainder_inlined (void);
void stub_hw_divider_u32_remainder_wait (void);
void stub_hw_divider_wait_ready (void);
void hw_is_claimed (void);
void i2c_deinit (void);
void stub_i2c_get_dreq (void);
void stub_i2c_get_hw (void);
void stub_i2c_get_read_available (void);
void stub_i2c_get_write_available (void);
void stub_i2c_hw_index (void);
void i2c_init (void);
void i2c_read_blocking (void);
void i2c_read_blocking_until (void);
void stub_i2c_read_raw_blocking (void);
void i2c_read_timeout_per_char_us (void);
void stub_i2c_read_timeout_us (void);
void i2c_set_baudrate (void);
void i2c_set_slave_mode (void);
void i2c_write_blocking (void);
void i2c_write_blocking_until (void);
void stub_i2c_write_raw_blocking (void);
void i2c_write_timeout_per_char_us (void);
void stub_i2c_write_timeout_us (void);
void stub_interp_add_accumulater (void);
void interp_claim_lane (void);
void interp_claim_lane_mask (void);
void stub_interp_config_set_add_raw (void);
void stub_interp_config_set_blend (void);
void stub_interp_config_set_clamp (void);
void stub_interp_config_set_cross_input (void);
void stub_interp_config_set_cross_result (void);
void stub_interp_config_set_force_bits (void);
void stub_interp_config_set_mask (void);
void stub_interp_config_set_shift (void);
void stub_interp_config_set_signed (void);
void stub_interp_default_config (void);
void stub_interp_get_accumulator (void);
void stub_interp_get_base (void);
void stub_interp_get_raw (void);
void stub_interp_index (void);
void interp_lane_is_claimed (void);
void stub_interp_peek_full_result (void);
void stub_interp_peek_lane_result (void);
void stub_interp_pop_full_result (void);
void stub_interp_pop_lane_result (void);
void interp_restore (void);
void interp_save (void);
void stub_interp_set_accumulator (void);
void stub_interp_set_base (void);
void stub_interp_set_base_both (void);
void stub_interp_set_config (void);
void stub_interp_set_force_bits (void);
void interp_unclaim_lane (void);
void interp_unclaim_lane_mask (void);
void irq_add_shared_handler (void);
void stub_irq_clear (void);
void irq_get_exclusive_handler (void);
void irq_get_priority (void);
void irq_get_vtable_handler (void);
void irq_has_shared_handler (void);
void irq_init_priorities (void);
void irq_is_enabled (void);
void irq_remove_handler (void);
void irq_set_enabled (void);
void irq_set_exclusive_handler (void);
void irq_set_mask_enabled (void);
void irq_set_pending (void);
void irq_set_priority (void);
void stub_is_at_the_end_of_time (void);
void stub_is_nil_time (void);
void iswspace (void);
void iswspace_l (void);
void localeconv (void);
void localtime (void);
void localtime_r (void);
void lock_init (void);
void longjmp (void);
void stub_make_timeout_time_ms (void);
void stub_make_timeout_time_us (void);
void malloc (void);
void mbrtowc (void);
void memchr (void);
void memcmp (void);
void memmove (void);
void stub_multicore_fifo_clear_irq (void);
void stub_multicore_fifo_drain (void);
void stub_multicore_fifo_get_status (void);
void multicore_fifo_pop_blocking (void);
void multicore_fifo_pop_timeout_us (void);
void multicore_fifo_push_blocking (void);
void multicore_fifo_push_timeout_us (void);
void stub_multicore_fifo_rvalid (void);
void stub_multicore_fifo_wready (void);
void multicore_launch_core1 (void);
void multicore_launch_core1_raw (void);
void multicore_launch_core1_with_stack (void);
void multicore_lockout_end_blocking (void);
void multicore_lockout_end_timeout_us (void);
void multicore_lockout_start_blocking (void);
void multicore_lockout_start_timeout_us (void);
void multicore_lockout_victim_init (void);
void multicore_reset_core1 (void);
void mutex_enter_block_until (void);
void mutex_enter_blocking (void);
void mutex_enter_timeout_ms (void);
void mutex_enter_timeout_us (void);
void mutex_exit (void);
void mutex_init (void);
void stub_mutex_is_initialized (void);
void mutex_try_enter (void);
void nan (void);
void nanf (void);
void next_striped_spin_lock_num (void);
void ph_clear (void);
void stub_ph_contains_node (void);
void ph_create (void);
void ph_destroy (void);
void ph_dump (void);
void stub_ph_free_node (void);
void stub_ph_get_node (void);
void stub_ph_insert_node (void);
void stub_ph_new_node (void);
void stub_ph_peek_head (void);
void ph_post_alloc_init (void);
void stub_ph_remove_and_free_head (void);
void ph_remove_and_free_node (void);
void ph_remove_head (void);
void pico_get_unique_board_id (void);
void pico_get_unique_board_id_string (void);
void pio_add_program (void);
void pio_add_program_at_offset (void);
void stub_pio_calculate_clkdiv_from_float (void);
void pio_can_add_program (void);
void pio_can_add_program_at_offset (void);
void pio_claim_sm_mask (void);
void pio_claim_unused_sm (void);
void pio_clear_instruction_memory (void);
void stub_pio_clkdiv_restart_sm_mask (void);
void stub_pio_enable_sm_mask_in_sync (void);
void stub_pio_encode_delay (void);
void stub_pio_encode_in (void);
void stub_pio_encode_irq_clear (void);
void stub_pio_encode_irq_set (void);
void stub_pio_encode_irq_wait (void);
void stub_pio_encode_jmp (void);
void stub_pio_encode_jmp_not_osre (void);
void stub_pio_encode_jmp_not_x (void);
void stub_pio_encode_jmp_not_y (void);
void stub_pio_encode_jmp_pin (void);
void stub_pio_encode_jmp_x_dec (void);
void stub_pio_encode_jmp_x_ne_y (void);
void stub_pio_encode_jmp_y_dec (void);
void stub_pio_encode_mov (void);
void stub_pio_encode_mov_not (void);
void stub_pio_encode_mov_reverse (void);
void stub_pio_encode_nop (void);
void stub_pio_encode_out (void);
void stub_pio_encode_pull (void);
void stub_pio_encode_push (void);
void stub_pio_encode_set (void);
void stub_pio_encode_sideset (void);
void stub_pio_encode_sideset_opt (void);
void stub_pio_encode_wait_gpio (void);
void stub_pio_encode_wait_irq (void);
void stub_pio_encode_wait_pin (void);
void stub_pio_get_default_sm_config (void);
void stub_pio_get_dreq (void);
void stub_pio_get_index (void);
void stub_pio_gpio_init (void);
void stub_pio_interrupt_clear (void);
void stub_pio_interrupt_get (void);
void pio_remove_program (void);
void stub_pio_restart_sm_mask (void);
void stub_pio_set_irq0_source_enabled (void);
void stub_pio_set_irq0_source_mask_enabled (void);
void stub_pio_set_irq1_source_enabled (void);
void stub_pio_set_irq1_source_mask_enabled (void);
void stub_pio_set_irqn_source_enabled (void);
void stub_pio_set_irqn_source_mask_enabled (void);
void stub_pio_set_sm_mask_enabled (void);
void pio_sm_claim (void);
void stub_pio_sm_clear_fifos (void);
void stub_pio_sm_clkdiv_restart (void);
void pio_sm_drain_tx_fifo (void);
void stub_pio_sm_exec_wait_blocking (void);
void stub_pio_sm_get (void);
void stub_pio_sm_get_blocking (void);
void stub_pio_sm_get_pc (void);
void stub_pio_sm_get_rx_fifo_level (void);
void stub_pio_sm_get_tx_fifo_level (void);
void pio_sm_init (void);
void pio_sm_is_claimed (void);
void stub_pio_sm_is_exec_stalled (void);
void stub_pio_sm_is_rx_fifo_empty (void);
void stub_pio_sm_is_rx_fifo_full (void);
void stub_pio_sm_is_tx_fifo_empty (void);
void stub_pio_sm_is_tx_fifo_full (void);
void stub_pio_sm_put (void);
void stub_pio_sm_put_blocking (void);
void stub_pio_sm_restart (void);
void stub_pio_sm_set_clkdiv (void);
void stub_pio_sm_set_clkdiv_int_frac (void);
void stub_pio_sm_set_config (void);
void pio_sm_set_consecutive_pindirs (void);
void stub_pio_sm_set_enabled (void);
void stub_pio_sm_set_in_pins (void);
void stub_pio_sm_set_out_pins (void);
void pio_sm_set_pindirs_with_mask (void);
void pio_sm_set_pins (void);
void pio_sm_set_pins_with_mask (void);
void stub_pio_sm_set_set_pins (void);
void stub_pio_sm_set_sideset_pins (void);
void stub_pio_sm_set_wrap (void);
void pio_sm_unclaim (void);
void pll_deinit (void);
void pll_init (void);
void powint (void);
void powintf (void);
void putchar_raw (void);
void puts_raw (void);
void stub_pwm_advance_count (void);
void stub_pwm_clear_irq (void);
void stub_pwm_config_set_clkdiv (void);
void stub_pwm_config_set_clkdiv_int (void);
void stub_pwm_config_set_clkdiv_int_frac (void);
void stub_pwm_config_set_clkdiv_mode (void);
void stub_pwm_config_set_output_polarity (void);
void stub_pwm_config_set_phase_correct (void);
void stub_pwm_config_set_wrap (void);
void stub_pwm_force_irq (void);
void stub_pwm_get_counter (void);
void stub_pwm_get_default_config (void);
void stub_pwm_get_dreq (void);
void stub_pwm_get_irq_status_mask (void);
void stub_pwm_gpio_to_channel (void);
void stub_pwm_gpio_to_slice_num (void);
void stub_pwm_init (void);
void stub_pwm_retard_count (void);
void stub_pwm_set_both_levels (void);
void stub_pwm_set_chan_level (void);
void stub_pwm_set_clkdiv (void);
void stub_pwm_set_clkdiv_int_frac (void);
void stub_pwm_set_clkdiv_mode (void);
void stub_pwm_set_counter (void);
void stub_pwm_set_enabled (void);
void stub_pwm_set_gpio_level (void);
void stub_pwm_set_irq_enabled (void);
void stub_pwm_set_irq_mask_enabled (void);
void stub_pwm_set_mask_enabled (void);
void stub_pwm_set_output_polarity (void);
void stub_pwm_set_phase_correct (void);
void stub_pwm_set_wrap (void);
void queue_add_blocking (void);
void queue_free (void);
void stub_queue_get_level (void);
void stub_queue_init (void);
void queue_init_with_spinlock (void);
void stub_queue_is_empty (void);
void stub_queue_is_full (void);
void queue_peek_blocking (void);
void queue_remove_blocking (void);
void queue_try_add (void);
void queue_try_peek (void);
void queue_try_remove (void);
void recursive_mutex_enter_block_until (void);
void recursive_mutex_enter_blocking (void);
void recursive_mutex_enter_timeout_ms (void);
void recursive_mutex_enter_timeout_us (void);
void recursive_mutex_exit (void);
void recursive_mutex_init (void);
void stub_recursive_mutex_is_initialized (void);
void recursive_mutex_try_enter (void);
void stub_reset_block (void);
void rtc_disable_alarm (void);
void rtc_enable_alarm (void);
void rtc_get_datetime (void);
void rtc_init (void);
void rtc_running (void);
void rtc_set_alarm (void);
void rtc_set_datetime (void);
void sem_acquire_block_until (void);
void sem_acquire_blocking (void);
void sem_acquire_timeout_ms (void);
void sem_acquire_timeout_us (void);
void sem_available (void);
void sem_init (void);
void sem_release (void);
void sem_reset (void);
void sem_try_acquire (void);
void set_sys_clock_48mhz (void);
void stub_set_sys_clock_khz (void);
void set_sys_clock_pll (void);
void setjmp (void);
void setlocale (void);
void setup_default_uart (void);
void sincos (void);
void sincosf (void);
void siprintf (void);
void siscanf (void);
void sleep_ms (void);
void sleep_until (void);
void sleep_us (void);
void stub_sm_config_set_clkdiv (void);
void stub_sm_config_set_clkdiv_int_frac (void);
void stub_sm_config_set_fifo_join (void);
void stub_sm_config_set_in_pins (void);
void stub_sm_config_set_in_shift (void);
void stub_sm_config_set_jmp_pin (void);
void stub_sm_config_set_mov_status (void);
void stub_sm_config_set_out_pins (void);
void stub_sm_config_set_out_shift (void);
void stub_sm_config_set_out_special (void);
void stub_sm_config_set_set_pins (void);
void stub_sm_config_set_sideset (void);
void stub_sm_config_set_sideset_pins (void);
void stub_sm_config_set_wrap (void);
void snprintf (void);
void spi_deinit (void);
void spi_get_baudrate (void);
void stub_spi_get_const_hw (void);
void stub_spi_get_dreq (void);
void stub_spi_get_hw (void);
void stub_spi_get_index (void);
void spi_init (void);
void stub_spi_is_busy (void);
void stub_spi_is_readable (void);
void stub_spi_is_writable (void);
void spi_read16_blocking (void);
void spi_read_blocking (void);
void spi_set_baudrate (void);
void stub_spi_set_format (void);
void stub_spi_set_slave (void);
void spi_write16_blocking (void);
void spi_write16_read16_blocking (void);
void spi_write_blocking (void);
void spi_write_read_blocking (void);
void spin_lock_claim (void);
void spin_lock_claim_mask (void);
void spin_lock_claim_unused (void);
void spin_lock_init (void);
void spin_lock_is_claimed (void);
void spin_lock_unclaim (void);
void spin_locks_reset (void);
void sprintf (void);
void sscanf (void);
void stdin_uart_init (void);
void stdio_filter_driver (void);
void stdio_flush (void);
void stdio_init_all (void);
void stdio_set_driver_enabled (void);
void stdio_set_translate_crlf (void);
void stdio_uart_init (void);
void stdio_uart_init_full (void);
void stdout_uart_init (void);
void strcat (void);
void strchr (void);
void strcpy (void);
void strcspn (void);
void strlen (void);
void strncasecmp (void);
void strncat (void);
void strncmp (void);
void strncpy (void);
void strnlen (void);
void strrchr (void);
void strspn (void);
void strstr (void);
void strtod (void);
void strtod_l (void);
void strtof (void);
void strtof_l (void);
void strtol (void);
void strtol_l (void);
void strtoll (void);
void strtoll_l (void);
void strtoul (void);
void strtoul_l (void);
void strtoull (void);
void strtoull_l (void);
void system (void);
void time (void);
void stub_time_reached (void);
void stub_time_us_32 (void);
void time_us_64 (void);
void stub_to_ms_since_boot (void);
void stub_to_us_since_boot (void);
void truncl (void);
void ttxtfont (void);
void tzset (void);
void stub_uart_default_tx_wait_blocking (void);
void uart_deinit (void);
void stub_uart_get_dreq (void);
void stub_uart_get_hw (void);
void stub_uart_get_index (void);
void stub_uart_get_instance (void);
void stub_uart_getc (void);
void uart_init (void);
void stub_uart_is_enabled (void);
void stub_uart_is_readable (void);
void uart_is_readable_within_us (void);
void stub_uart_is_writable (void);
void stub_uart_putc (void);
void stub_uart_putc_raw (void);
void stub_uart_puts (void);
void stub_uart_read_blocking (void);
void uart_set_baudrate (void);
void stub_uart_set_break (void);
void stub_uart_set_fifo_enabled (void);
void stub_uart_set_format (void);
void stub_uart_set_hw_flow (void);
void stub_uart_set_irq_enables (void);
void uart_set_translate_crlf (void);
void stub_uart_tx_wait_blocking (void);
void stub_uart_write_blocking (void);
void ufix2double (void);
void ufix2float (void);
void ufix642double (void);
void ufix642float (void);
void ungetc (void);
void stub_unreset_block (void);
void stub_unreset_block_wait (void);
void stub_update_us_since_boot (void);
void stub_us_to_ms (void);
void user_irq_claim (void);
void user_irq_claim_unused (void);
void user_irq_is_claimed (void);
void user_irq_unclaim (void);
void vfiprintf (void);
void vfprintf (void);
void vprintf (void);
void vreg_set_voltage (void);
void vsprintf (void);
void watchdog_caused_reboot (void);
void watchdog_enable (void);
void watchdog_enable_caused_reboot (void);
void watchdog_get_count (void);
void watchdog_reboot (void);
void watchdog_start_tick (void);
void watchdog_update (void);
void wcrtomb (void);
void stub_xip_alias_check_addr (void);
void xosc_disable (void);
void xosc_dormant (void);
void xosc_init (void);

#pragma GCC diagnostic pop


typedef struct st_symbols
    {
    const char *s;
    void *p;
    } symbols;

const symbols sdkfuncs[] = {
    {"_pio_encode_instr_and_args", stub__pio_encode_instr_and_args},
    {"_pio_encode_instr_and_src_dest", stub__pio_encode_instr_and_src_dest},
    {"_pio_encode_irq", stub__pio_encode_irq},
    {"_pio_major_instr_bits", stub__pio_major_instr_bits},
    {"absolute_time_diff_us", stub_absolute_time_diff_us},
    {"adc_fifo_drain", stub_adc_fifo_drain},
    {"adc_fifo_get", stub_adc_fifo_get},
    {"adc_fifo_get_blocking", stub_adc_fifo_get_blocking},
    {"adc_fifo_get_level", stub_adc_fifo_get_level},
    {"adc_fifo_is_empty", stub_adc_fifo_is_empty},
    {"adc_fifo_setup", stub_adc_fifo_setup},
    {"adc_get_selected_input", stub_adc_get_selected_input},
    {"adc_gpio_init", stub_adc_gpio_init},
    {"adc_init", adc_init},
    {"adc_irq_set_enabled", stub_adc_irq_set_enabled},
    {"adc_read", stub_adc_read},
    {"adc_run", stub_adc_run},
    {"adc_select_input", stub_adc_select_input},
    {"adc_set_clkdiv", stub_adc_set_clkdiv},
    {"adc_set_round_robin", stub_adc_set_round_robin},
    {"adc_set_temp_sensor_enabled", stub_adc_set_temp_sensor_enabled},
    {"add_alarm_at", stub_add_alarm_at},
    {"add_alarm_in_ms", stub_add_alarm_in_ms},
    {"add_alarm_in_us", stub_add_alarm_in_us},
    {"add_repeating_timer_ms", stub_add_repeating_timer_ms},
    {"add_repeating_timer_us", stub_add_repeating_timer_us},
    {"alarm_pool_add_alarm_at", alarm_pool_add_alarm_at},
    {"alarm_pool_add_alarm_in_ms", stub_alarm_pool_add_alarm_in_ms},
    {"alarm_pool_add_alarm_in_us", stub_alarm_pool_add_alarm_in_us},
    {"alarm_pool_add_repeating_timer_ms", stub_alarm_pool_add_repeating_timer_ms},
    {"alarm_pool_add_repeating_timer_us", alarm_pool_add_repeating_timer_us},
    {"alarm_pool_cancel_alarm", alarm_pool_cancel_alarm},
    {"alarm_pool_create", alarm_pool_create},
    {"alarm_pool_destroy", alarm_pool_destroy},
    {"alarm_pool_get_default", alarm_pool_get_default},
    {"alarm_pool_hardware_alarm_num", alarm_pool_hardware_alarm_num},
    {"alarm_pool_init_default", alarm_pool_init_default},
    {"asctime", asctime},
    {"asctime_r", asctime_r},
    {"atoi", atoi},
    {"bbcfont", bbcfont},
    {"best_effort_wfe_or_timeout", best_effort_wfe_or_timeout},
    {"busy_wait_ms", busy_wait_ms},
    {"busy_wait_until", busy_wait_until},
    {"busy_wait_us", busy_wait_us},
    {"busy_wait_us_32", busy_wait_us_32},
    {"calloc", calloc},
    {"cancel_alarm", stub_cancel_alarm},
    {"cancel_repeating_timer", cancel_repeating_timer},
    {"channel_config_get_ctrl_value", stub_channel_config_get_ctrl_value},
    {"channel_config_set_bswap", stub_channel_config_set_bswap},
    {"channel_config_set_chain_to", stub_channel_config_set_chain_to},
    {"channel_config_set_dreq", stub_channel_config_set_dreq},
    {"channel_config_set_enable", stub_channel_config_set_enable},
    {"channel_config_set_high_priority", stub_channel_config_set_high_priority},
    {"channel_config_set_irq_quiet", stub_channel_config_set_irq_quiet},
    {"channel_config_set_read_increment", stub_channel_config_set_read_increment},
    {"channel_config_set_ring", stub_channel_config_set_ring},
    {"channel_config_set_sniff_enable", stub_channel_config_set_sniff_enable},
    {"channel_config_set_transfer_data_size", stub_channel_config_set_transfer_data_size},
    {"channel_config_set_write_increment", stub_channel_config_set_write_increment},
    {"check_dma_channel_param", stub_check_dma_channel_param},
    {"check_dma_timer_param", stub_check_dma_timer_param},
    {"check_gpio_param", stub_check_gpio_param},
    {"check_hardware_alarm_num_param", stub_check_hardware_alarm_num_param},
    {"check_irq_param", stub_check_irq_param},
    {"check_pio_param", stub_check_pio_param},
    {"check_slice_num_param", stub_check_slice_num_param},
    {"check_sm_mask", stub_check_sm_mask},
    {"check_sm_param", stub_check_sm_param},
    {"check_sys_clock_khz", check_sys_clock_khz},
    {"clock_configure", clock_configure},
    {"clock_configure_gpin", clock_configure_gpin},
    {"clock_get_hz", clock_get_hz},
    {"clock_gpio_init", clock_gpio_init},
    {"clock_set_reported_hz", clock_set_reported_hz},
    {"clock_stop", clock_stop},
    {"clocks_enable_resus", clocks_enable_resus},
    {"clocks_init", clocks_init},
    {"critical_section_deinit", critical_section_deinit},
    {"critical_section_enter_blocking", stub_critical_section_enter_blocking},
    {"critical_section_exit", stub_critical_section_exit},
    {"critical_section_init", critical_section_init},
    {"critical_section_init_with_lock_num", critical_section_init_with_lock_num},
    {"ctime", ctime},
    {"datetime_to_str", datetime_to_str},
    {"delayed_by_ms", stub_delayed_by_ms},
    {"delayed_by_us", stub_delayed_by_us},
    {"div_s32s32", div_s32s32},
    {"div_s64s64", div_s64s64},
    {"div_u32u32", div_u32u32},
    {"div_u64u64", div_u64u64},
    {"divmod_s32s32", divmod_s32s32},
    {"divmod_s32s32_rem", stub_divmod_s32s32_rem},
    {"divmod_s64s64", divmod_s64s64},
    {"divmod_s64s64_rem", divmod_s64s64_rem},
    {"divmod_u32u32", divmod_u32u32},
    {"divmod_u32u32_rem", stub_divmod_u32u32_rem},
    {"divmod_u64u64", divmod_u64u64},
    {"divmod_u64u64_rem", divmod_u64u64_rem},
    {"dma_channel_abort", stub_dma_channel_abort},
    {"dma_channel_acknowledge_irq0", stub_dma_channel_acknowledge_irq0},
    {"dma_channel_acknowledge_irq1", stub_dma_channel_acknowledge_irq1},
    {"dma_channel_claim", dma_channel_claim},
    {"dma_channel_configure", stub_dma_channel_configure},
    {"dma_channel_get_default_config", stub_dma_channel_get_default_config},
    {"dma_channel_get_irq0_status", stub_dma_channel_get_irq0_status},
    {"dma_channel_get_irq1_status", stub_dma_channel_get_irq1_status},
    {"dma_channel_is_claimed", dma_channel_is_claimed},
    {"dma_channel_set_config", stub_dma_channel_set_config},
    {"dma_channel_set_irq0_enabled", stub_dma_channel_set_irq0_enabled},
    {"dma_channel_set_irq1_enabled", stub_dma_channel_set_irq1_enabled},
    {"dma_channel_set_read_addr", stub_dma_channel_set_read_addr},
    {"dma_channel_set_trans_count", stub_dma_channel_set_trans_count},
    {"dma_channel_set_write_addr", stub_dma_channel_set_write_addr},
    {"dma_channel_start", stub_dma_channel_start},
    {"dma_channel_unclaim", dma_channel_unclaim},
    {"dma_claim_mask", dma_claim_mask},
    {"dma_claim_unused_channel", dma_claim_unused_channel},
    {"dma_claim_unused_timer", dma_claim_unused_timer},
    {"dma_get_channel_config", stub_dma_get_channel_config},
    {"dma_get_timer_dreq", stub_dma_get_timer_dreq},
    {"dma_irqn_acknowledge_channel", stub_dma_irqn_acknowledge_channel},
    {"dma_irqn_get_channel_status", stub_dma_irqn_get_channel_status},
    {"dma_irqn_set_channel_enabled", stub_dma_irqn_set_channel_enabled},
    {"dma_irqn_set_channel_mask_enabled", stub_dma_irqn_set_channel_mask_enabled},
    {"dma_set_irq0_channel_mask_enabled", stub_dma_set_irq0_channel_mask_enabled},
    {"dma_set_irq1_channel_mask_enabled", stub_dma_set_irq1_channel_mask_enabled},
    {"dma_start_channel_mask", stub_dma_start_channel_mask},
    {"dma_timer_claim", dma_timer_claim},
    {"dma_timer_is_claimed", dma_timer_is_claimed},
    {"dma_timer_set_fraction", stub_dma_timer_set_fraction},
    {"dma_timer_unclaim", dma_timer_unclaim},
    {"dma_unclaim_mask", dma_unclaim_mask},
    {"double2fix", double2fix},
    {"double2fix64", double2fix64},
    {"double2int", double2int},
    {"double2int64", double2int64},
    {"double2int64_z", double2int64_z},
    {"double2int_z", double2int_z},
    {"double2ufix", double2ufix},
    {"double2ufix64", double2ufix64},
    {"exception_get_vtable_handler", exception_get_vtable_handler},
    {"exception_restore_handler", exception_restore_handler},
    {"exception_set_exclusive_handler", exception_set_exclusive_handler},
    {"exp10", exp10},
    {"exp10f", exp10f},
    {"fclose", fclose},
    {"fflush", fflush},
    {"fix2double", fix2double},
    {"fix2float", fix2float},
    {"fix642double", fix642double},
    {"fix642float", fix642float},
    {"flash_do_cmd", flash_do_cmd},
    {"flash_get_unique_id", flash_get_unique_id},
    {"flash_range_erase", flash_range_erase},
    {"flash_range_program", flash_range_program},
    {"float2fix", float2fix},
    {"float2fix64", float2fix64},
    {"float2int", float2int},
    {"float2int64", float2int64},
    {"float2int64_z", float2int64_z},
    {"float2int_z", float2int_z},
    {"float2ufix", float2ufix},
    {"float2ufix64", float2ufix64},
    {"fputwc", fputwc},
    {"fread", fread},
    {"free", free},
    {"frequency_count_khz", frequency_count_khz},
    {"frequency_count_mhz", stub_frequency_count_mhz},
    {"fseek", fseek},
    {"fseeko", fseeko},
    {"fvtab", fvtab},
    {"fwrite", fwrite},
    {"get_absolute_time", stub_get_absolute_time},
    {"getchar_timeout_us", getchar_timeout_us},
    {"gmtime_r", gmtime_r},
    {"gpio_acknowledge_irq", gpio_acknowledge_irq},
    {"gpio_add_raw_irq_handler", stub_gpio_add_raw_irq_handler},
    {"gpio_add_raw_irq_handler_masked", gpio_add_raw_irq_handler_masked},
    {"gpio_add_raw_irq_handler_with_order_priority", stub_gpio_add_raw_irq_handler_with_order_priority},
    {"gpio_add_raw_irq_handler_with_order_priority_masked", gpio_add_raw_irq_handler_with_order_priority_masked},
    {"gpio_clr_mask", stub_gpio_clr_mask},
    {"gpio_debug_pins_init", gpio_debug_pins_init},
    {"gpio_deinit", gpio_deinit},
    {"gpio_disable_pulls", stub_gpio_disable_pulls},
    {"gpio_get", stub_gpio_get},
    {"gpio_get_all", stub_gpio_get_all},
    {"gpio_get_dir", stub_gpio_get_dir},
    {"gpio_get_drive_strength", gpio_get_drive_strength},
    {"gpio_get_function", gpio_get_function},
    {"gpio_get_irq_event_mask", stub_gpio_get_irq_event_mask},
    {"gpio_get_out_level", stub_gpio_get_out_level},
    {"gpio_get_slew_rate", gpio_get_slew_rate},
    {"gpio_init", gpio_init},
    {"gpio_init_mask", gpio_init_mask},
    {"gpio_is_dir_out", stub_gpio_is_dir_out},
    {"gpio_is_input_hysteresis_enabled", gpio_is_input_hysteresis_enabled},
    {"gpio_is_pulled_down", stub_gpio_is_pulled_down},
    {"gpio_is_pulled_up", stub_gpio_is_pulled_up},
    {"gpio_pull_down", stub_gpio_pull_down},
    {"gpio_pull_up", stub_gpio_pull_up},
    {"gpio_put", stub_gpio_put},
    {"gpio_put_all", stub_gpio_put_all},
    {"gpio_put_masked", stub_gpio_put_masked},
    {"gpio_remove_raw_irq_handler", stub_gpio_remove_raw_irq_handler},
    {"gpio_remove_raw_irq_handler_masked", gpio_remove_raw_irq_handler_masked},
    {"gpio_set_dir", stub_gpio_set_dir},
    {"gpio_set_dir_all_bits", stub_gpio_set_dir_all_bits},
    {"gpio_set_dir_in_masked", stub_gpio_set_dir_in_masked},
    {"gpio_set_dir_masked", stub_gpio_set_dir_masked},
    {"gpio_set_dir_out_masked", stub_gpio_set_dir_out_masked},
    {"gpio_set_dormant_irq_enabled", gpio_set_dormant_irq_enabled},
    {"gpio_set_drive_strength", gpio_set_drive_strength},
    {"gpio_set_function", gpio_set_function},
    {"gpio_set_inover", gpio_set_inover},
    {"gpio_set_input_enabled", gpio_set_input_enabled},
    {"gpio_set_input_hysteresis_enabled", gpio_set_input_hysteresis_enabled},
    {"gpio_set_irq_callback", gpio_set_irq_callback},
    {"gpio_set_irq_enabled", gpio_set_irq_enabled},
    {"gpio_set_irq_enabled_with_callback", gpio_set_irq_enabled_with_callback},
    {"gpio_set_irqover", gpio_set_irqover},
    {"gpio_set_mask", stub_gpio_set_mask},
    {"gpio_set_oeover", gpio_set_oeover},
    {"gpio_set_outover", gpio_set_outover},
    {"gpio_set_pulls", gpio_set_pulls},
    {"gpio_set_slew_rate", gpio_set_slew_rate},
    {"gpio_xor_mask", stub_gpio_xor_mask},
    {"hardware_alarm_cancel", hardware_alarm_cancel},
    {"hardware_alarm_claim", hardware_alarm_claim},
    {"hardware_alarm_is_claimed", hardware_alarm_is_claimed},
    {"hardware_alarm_set_callback", hardware_alarm_set_callback},
    {"hardware_alarm_set_target", hardware_alarm_set_target},
    {"hardware_alarm_unclaim", hardware_alarm_unclaim},
    {"hw_claim_clear", hw_claim_clear},
    {"hw_claim_lock", hw_claim_lock},
    {"hw_claim_or_assert", hw_claim_or_assert},
    {"hw_claim_unlock", hw_claim_unlock},
    {"hw_claim_unused_from_range", hw_claim_unused_from_range},
    {"hw_divider_divmod_s32", hw_divider_divmod_s32},
    {"hw_divider_divmod_s32_start", stub_hw_divider_divmod_s32_start},
    {"hw_divider_divmod_u32", hw_divider_divmod_u32},
    {"hw_divider_divmod_u32_start", stub_hw_divider_divmod_u32_start},
    {"hw_divider_pause", stub_hw_divider_pause},
    {"hw_divider_quotient_s32", stub_hw_divider_quotient_s32},
    {"hw_divider_remainder_s32", stub_hw_divider_remainder_s32},
    {"hw_divider_restore_state", hw_divider_restore_state},
    {"hw_divider_result_nowait", stub_hw_divider_result_nowait},
    {"hw_divider_result_wait", stub_hw_divider_result_wait},
    {"hw_divider_s32_quotient_inlined", stub_hw_divider_s32_quotient_inlined},
    {"hw_divider_s32_quotient_wait", stub_hw_divider_s32_quotient_wait},
    {"hw_divider_s32_remainder_inlined", stub_hw_divider_s32_remainder_inlined},
    {"hw_divider_s32_remainder_wait", stub_hw_divider_s32_remainder_wait},
    {"hw_divider_save_state", hw_divider_save_state},
    {"hw_divider_u32_quotient", stub_hw_divider_u32_quotient},
    {"hw_divider_u32_quotient_inlined", stub_hw_divider_u32_quotient_inlined},
    {"hw_divider_u32_quotient_wait", stub_hw_divider_u32_quotient_wait},
    {"hw_divider_u32_remainder", stub_hw_divider_u32_remainder},
    {"hw_divider_u32_remainder_inlined", stub_hw_divider_u32_remainder_inlined},
    {"hw_divider_u32_remainder_wait", stub_hw_divider_u32_remainder_wait},
    {"hw_divider_wait_ready", stub_hw_divider_wait_ready},
    {"hw_is_claimed", hw_is_claimed},
    {"i2c_deinit", i2c_deinit},
    {"i2c_get_dreq", stub_i2c_get_dreq},
    {"i2c_get_hw", stub_i2c_get_hw},
    {"i2c_get_read_available", stub_i2c_get_read_available},
    {"i2c_get_write_available", stub_i2c_get_write_available},
    {"i2c_hw_index", stub_i2c_hw_index},
    {"i2c_init", i2c_init},
    {"i2c_read_blocking", i2c_read_blocking},
    {"i2c_read_blocking_until", i2c_read_blocking_until},
    {"i2c_read_raw_blocking", stub_i2c_read_raw_blocking},
    {"i2c_read_timeout_per_char_us", i2c_read_timeout_per_char_us},
    {"i2c_read_timeout_us", stub_i2c_read_timeout_us},
    {"i2c_set_baudrate", i2c_set_baudrate},
    {"i2c_set_slave_mode", i2c_set_slave_mode},
    {"i2c_write_blocking", i2c_write_blocking},
    {"i2c_write_blocking_until", i2c_write_blocking_until},
    {"i2c_write_raw_blocking", stub_i2c_write_raw_blocking},
    {"i2c_write_timeout_per_char_us", i2c_write_timeout_per_char_us},
    {"i2c_write_timeout_us", stub_i2c_write_timeout_us},
    {"interp_add_accumulater", stub_interp_add_accumulater},
    {"interp_claim_lane", interp_claim_lane},
    {"interp_claim_lane_mask", interp_claim_lane_mask},
    {"interp_config_set_add_raw", stub_interp_config_set_add_raw},
    {"interp_config_set_blend", stub_interp_config_set_blend},
    {"interp_config_set_clamp", stub_interp_config_set_clamp},
    {"interp_config_set_cross_input", stub_interp_config_set_cross_input},
    {"interp_config_set_cross_result", stub_interp_config_set_cross_result},
    {"interp_config_set_force_bits", stub_interp_config_set_force_bits},
    {"interp_config_set_mask", stub_interp_config_set_mask},
    {"interp_config_set_shift", stub_interp_config_set_shift},
    {"interp_config_set_signed", stub_interp_config_set_signed},
    {"interp_default_config", stub_interp_default_config},
    {"interp_get_accumulator", stub_interp_get_accumulator},
    {"interp_get_base", stub_interp_get_base},
    {"interp_get_raw", stub_interp_get_raw},
    {"interp_index", stub_interp_index},
    {"interp_lane_is_claimed", interp_lane_is_claimed},
    {"interp_peek_full_result", stub_interp_peek_full_result},
    {"interp_peek_lane_result", stub_interp_peek_lane_result},
    {"interp_pop_full_result", stub_interp_pop_full_result},
    {"interp_pop_lane_result", stub_interp_pop_lane_result},
    {"interp_restore", interp_restore},
    {"interp_save", interp_save},
    {"interp_set_accumulator", stub_interp_set_accumulator},
    {"interp_set_base", stub_interp_set_base},
    {"interp_set_base_both", stub_interp_set_base_both},
    {"interp_set_config", stub_interp_set_config},
    {"interp_set_force_bits", stub_interp_set_force_bits},
    {"interp_unclaim_lane", interp_unclaim_lane},
    {"interp_unclaim_lane_mask", interp_unclaim_lane_mask},
    {"irq_add_shared_handler", irq_add_shared_handler},
    {"irq_clear", stub_irq_clear},
    {"irq_get_exclusive_handler", irq_get_exclusive_handler},
    {"irq_get_priority", irq_get_priority},
    {"irq_get_vtable_handler", irq_get_vtable_handler},
    {"irq_has_shared_handler", irq_has_shared_handler},
    {"irq_init_priorities", irq_init_priorities},
    {"irq_is_enabled", irq_is_enabled},
    {"irq_remove_handler", irq_remove_handler},
    {"irq_set_enabled", irq_set_enabled},
    {"irq_set_exclusive_handler", irq_set_exclusive_handler},
    {"irq_set_mask_enabled", irq_set_mask_enabled},
    {"irq_set_pending", irq_set_pending},
    {"irq_set_priority", irq_set_priority},
    {"is_at_the_end_of_time", stub_is_at_the_end_of_time},
    {"is_nil_time", stub_is_nil_time},
    {"iswspace", iswspace},
    {"iswspace_l", iswspace_l},
    {"localeconv", localeconv},
    {"localtime", localtime},
    {"localtime_r", localtime_r},
    {"lock_init", lock_init},
    {"longjmp", longjmp},
    {"make_timeout_time_ms", stub_make_timeout_time_ms},
    {"make_timeout_time_us", stub_make_timeout_time_us},
    {"malloc", malloc},
    {"mbrtowc", mbrtowc},
    {"memchr", memchr},
    {"memcmp", memcmp},
    {"memmove", memmove},
    {"multicore_fifo_clear_irq", stub_multicore_fifo_clear_irq},
    {"multicore_fifo_drain", stub_multicore_fifo_drain},
    {"multicore_fifo_get_status", stub_multicore_fifo_get_status},
    {"multicore_fifo_pop_blocking", multicore_fifo_pop_blocking},
    {"multicore_fifo_pop_timeout_us", multicore_fifo_pop_timeout_us},
    {"multicore_fifo_push_blocking", multicore_fifo_push_blocking},
    {"multicore_fifo_push_timeout_us", multicore_fifo_push_timeout_us},
    {"multicore_fifo_rvalid", stub_multicore_fifo_rvalid},
    {"multicore_fifo_wready", stub_multicore_fifo_wready},
    {"multicore_launch_core1", multicore_launch_core1},
    {"multicore_launch_core1_raw", multicore_launch_core1_raw},
    {"multicore_launch_core1_with_stack", multicore_launch_core1_with_stack},
    {"multicore_lockout_end_blocking", multicore_lockout_end_blocking},
    {"multicore_lockout_end_timeout_us", multicore_lockout_end_timeout_us},
    {"multicore_lockout_start_blocking", multicore_lockout_start_blocking},
    {"multicore_lockout_start_timeout_us", multicore_lockout_start_timeout_us},
    {"multicore_lockout_victim_init", multicore_lockout_victim_init},
    {"multicore_reset_core1", multicore_reset_core1},
    {"mutex_enter_block_until", mutex_enter_block_until},
    {"mutex_enter_blocking", mutex_enter_blocking},
    {"mutex_enter_timeout_ms", mutex_enter_timeout_ms},
    {"mutex_enter_timeout_us", mutex_enter_timeout_us},
    {"mutex_exit", mutex_exit},
    {"mutex_init", mutex_init},
    {"mutex_is_initialized", stub_mutex_is_initialized},
    {"mutex_try_enter", mutex_try_enter},
    {"nan", nan},
    {"nanf", nanf},
    {"next_striped_spin_lock_num", next_striped_spin_lock_num},
    {"ph_clear", ph_clear},
    {"ph_contains_node", stub_ph_contains_node},
    {"ph_create", ph_create},
    {"ph_destroy", ph_destroy},
    {"ph_dump", ph_dump},
    {"ph_free_node", stub_ph_free_node},
    {"ph_get_node", stub_ph_get_node},
    {"ph_insert_node", stub_ph_insert_node},
    {"ph_new_node", stub_ph_new_node},
    {"ph_peek_head", stub_ph_peek_head},
    {"ph_post_alloc_init", ph_post_alloc_init},
    {"ph_remove_and_free_head", stub_ph_remove_and_free_head},
    {"ph_remove_and_free_node", ph_remove_and_free_node},
    {"ph_remove_head", ph_remove_head},
    {"pico_get_unique_board_id", pico_get_unique_board_id},
    {"pico_get_unique_board_id_string", pico_get_unique_board_id_string},
    {"pio_add_program", pio_add_program},
    {"pio_add_program_at_offset", pio_add_program_at_offset},
    {"pio_calculate_clkdiv_from_float", stub_pio_calculate_clkdiv_from_float},
    {"pio_can_add_program", pio_can_add_program},
    {"pio_can_add_program_at_offset", pio_can_add_program_at_offset},
    {"pio_claim_sm_mask", pio_claim_sm_mask},
    {"pio_claim_unused_sm", pio_claim_unused_sm},
    {"pio_clear_instruction_memory", pio_clear_instruction_memory},
    {"pio_clkdiv_restart_sm_mask", stub_pio_clkdiv_restart_sm_mask},
    {"pio_enable_sm_mask_in_sync", stub_pio_enable_sm_mask_in_sync},
    {"pio_encode_delay", stub_pio_encode_delay},
    {"pio_encode_in", stub_pio_encode_in},
    {"pio_encode_irq_clear", stub_pio_encode_irq_clear},
    {"pio_encode_irq_set", stub_pio_encode_irq_set},
    {"pio_encode_irq_wait", stub_pio_encode_irq_wait},
    {"pio_encode_jmp", stub_pio_encode_jmp},
    {"pio_encode_jmp_not_osre", stub_pio_encode_jmp_not_osre},
    {"pio_encode_jmp_not_x", stub_pio_encode_jmp_not_x},
    {"pio_encode_jmp_not_y", stub_pio_encode_jmp_not_y},
    {"pio_encode_jmp_pin", stub_pio_encode_jmp_pin},
    {"pio_encode_jmp_x_dec", stub_pio_encode_jmp_x_dec},
    {"pio_encode_jmp_x_ne_y", stub_pio_encode_jmp_x_ne_y},
    {"pio_encode_jmp_y_dec", stub_pio_encode_jmp_y_dec},
    {"pio_encode_mov", stub_pio_encode_mov},
    {"pio_encode_mov_not", stub_pio_encode_mov_not},
    {"pio_encode_mov_reverse", stub_pio_encode_mov_reverse},
    {"pio_encode_nop", stub_pio_encode_nop},
    {"pio_encode_out", stub_pio_encode_out},
    {"pio_encode_pull", stub_pio_encode_pull},
    {"pio_encode_push", stub_pio_encode_push},
    {"pio_encode_set", stub_pio_encode_set},
    {"pio_encode_sideset", stub_pio_encode_sideset},
    {"pio_encode_sideset_opt", stub_pio_encode_sideset_opt},
    {"pio_encode_wait_gpio", stub_pio_encode_wait_gpio},
    {"pio_encode_wait_irq", stub_pio_encode_wait_irq},
    {"pio_encode_wait_pin", stub_pio_encode_wait_pin},
    {"pio_get_default_sm_config", stub_pio_get_default_sm_config},
    {"pio_get_dreq", stub_pio_get_dreq},
    {"pio_get_index", stub_pio_get_index},
    {"pio_gpio_init", stub_pio_gpio_init},
    {"pio_interrupt_clear", stub_pio_interrupt_clear},
    {"pio_interrupt_get", stub_pio_interrupt_get},
    {"pio_remove_program", pio_remove_program},
    {"pio_restart_sm_mask", stub_pio_restart_sm_mask},
    {"pio_set_irq0_source_enabled", stub_pio_set_irq0_source_enabled},
    {"pio_set_irq0_source_mask_enabled", stub_pio_set_irq0_source_mask_enabled},
    {"pio_set_irq1_source_enabled", stub_pio_set_irq1_source_enabled},
    {"pio_set_irq1_source_mask_enabled", stub_pio_set_irq1_source_mask_enabled},
    {"pio_set_irqn_source_enabled", stub_pio_set_irqn_source_enabled},
    {"pio_set_irqn_source_mask_enabled", stub_pio_set_irqn_source_mask_enabled},
    {"pio_set_sm_mask_enabled", stub_pio_set_sm_mask_enabled},
    {"pio_sm_claim", pio_sm_claim},
    {"pio_sm_clear_fifos", stub_pio_sm_clear_fifos},
    {"pio_sm_clkdiv_restart", stub_pio_sm_clkdiv_restart},
    {"pio_sm_drain_tx_fifo", pio_sm_drain_tx_fifo},
    {"pio_sm_exec_wait_blocking", stub_pio_sm_exec_wait_blocking},
    {"pio_sm_get", stub_pio_sm_get},
    {"pio_sm_get_blocking", stub_pio_sm_get_blocking},
    {"pio_sm_get_pc", stub_pio_sm_get_pc},
    {"pio_sm_get_rx_fifo_level", stub_pio_sm_get_rx_fifo_level},
    {"pio_sm_get_tx_fifo_level", stub_pio_sm_get_tx_fifo_level},
    {"pio_sm_init", pio_sm_init},
    {"pio_sm_is_claimed", pio_sm_is_claimed},
    {"pio_sm_is_exec_stalled", stub_pio_sm_is_exec_stalled},
    {"pio_sm_is_rx_fifo_empty", stub_pio_sm_is_rx_fifo_empty},
    {"pio_sm_is_rx_fifo_full", stub_pio_sm_is_rx_fifo_full},
    {"pio_sm_is_tx_fifo_empty", stub_pio_sm_is_tx_fifo_empty},
    {"pio_sm_is_tx_fifo_full", stub_pio_sm_is_tx_fifo_full},
    {"pio_sm_put", stub_pio_sm_put},
    {"pio_sm_put_blocking", stub_pio_sm_put_blocking},
    {"pio_sm_restart", stub_pio_sm_restart},
    {"pio_sm_set_clkdiv", stub_pio_sm_set_clkdiv},
    {"pio_sm_set_clkdiv_int_frac", stub_pio_sm_set_clkdiv_int_frac},
    {"pio_sm_set_config", stub_pio_sm_set_config},
    {"pio_sm_set_consecutive_pindirs", pio_sm_set_consecutive_pindirs},
    {"pio_sm_set_enabled", stub_pio_sm_set_enabled},
    {"pio_sm_set_in_pins", stub_pio_sm_set_in_pins},
    {"pio_sm_set_out_pins", stub_pio_sm_set_out_pins},
    {"pio_sm_set_pindirs_with_mask", pio_sm_set_pindirs_with_mask},
    {"pio_sm_set_pins", pio_sm_set_pins},
    {"pio_sm_set_pins_with_mask", pio_sm_set_pins_with_mask},
    {"pio_sm_set_set_pins", stub_pio_sm_set_set_pins},
    {"pio_sm_set_sideset_pins", stub_pio_sm_set_sideset_pins},
    {"pio_sm_set_wrap", stub_pio_sm_set_wrap},
    {"pio_sm_unclaim", pio_sm_unclaim},
    {"pll_deinit", pll_deinit},
    {"pll_init", pll_init},
    {"powint", powint},
    {"powintf", powintf},
    {"putchar_raw", putchar_raw},
    {"puts_raw", puts_raw},
    {"pwm_advance_count", stub_pwm_advance_count},
    {"pwm_clear_irq", stub_pwm_clear_irq},
    {"pwm_config_set_clkdiv", stub_pwm_config_set_clkdiv},
    {"pwm_config_set_clkdiv_int", stub_pwm_config_set_clkdiv_int},
    {"pwm_config_set_clkdiv_int_frac", stub_pwm_config_set_clkdiv_int_frac},
    {"pwm_config_set_clkdiv_mode", stub_pwm_config_set_clkdiv_mode},
    {"pwm_config_set_output_polarity", stub_pwm_config_set_output_polarity},
    {"pwm_config_set_phase_correct", stub_pwm_config_set_phase_correct},
    {"pwm_config_set_wrap", stub_pwm_config_set_wrap},
    {"pwm_force_irq", stub_pwm_force_irq},
    {"pwm_get_counter", stub_pwm_get_counter},
    {"pwm_get_default_config", stub_pwm_get_default_config},
    {"pwm_get_dreq", stub_pwm_get_dreq},
    {"pwm_get_irq_status_mask", stub_pwm_get_irq_status_mask},
    {"pwm_gpio_to_channel", stub_pwm_gpio_to_channel},
    {"pwm_gpio_to_slice_num", stub_pwm_gpio_to_slice_num},
    {"pwm_init", stub_pwm_init},
    {"pwm_retard_count", stub_pwm_retard_count},
    {"pwm_set_both_levels", stub_pwm_set_both_levels},
    {"pwm_set_chan_level", stub_pwm_set_chan_level},
    {"pwm_set_clkdiv", stub_pwm_set_clkdiv},
    {"pwm_set_clkdiv_int_frac", stub_pwm_set_clkdiv_int_frac},
    {"pwm_set_clkdiv_mode", stub_pwm_set_clkdiv_mode},
    {"pwm_set_counter", stub_pwm_set_counter},
    {"pwm_set_enabled", stub_pwm_set_enabled},
    {"pwm_set_gpio_level", stub_pwm_set_gpio_level},
    {"pwm_set_irq_enabled", stub_pwm_set_irq_enabled},
    {"pwm_set_irq_mask_enabled", stub_pwm_set_irq_mask_enabled},
    {"pwm_set_mask_enabled", stub_pwm_set_mask_enabled},
    {"pwm_set_output_polarity", stub_pwm_set_output_polarity},
    {"pwm_set_phase_correct", stub_pwm_set_phase_correct},
    {"pwm_set_wrap", stub_pwm_set_wrap},
    {"queue_add_blocking", queue_add_blocking},
    {"queue_free", queue_free},
    {"queue_get_level", stub_queue_get_level},
    {"queue_init", stub_queue_init},
    {"queue_init_with_spinlock", queue_init_with_spinlock},
    {"queue_is_empty", stub_queue_is_empty},
    {"queue_is_full", stub_queue_is_full},
    {"queue_peek_blocking", queue_peek_blocking},
    {"queue_remove_blocking", queue_remove_blocking},
    {"queue_try_add", queue_try_add},
    {"queue_try_peek", queue_try_peek},
    {"queue_try_remove", queue_try_remove},
    {"recursive_mutex_enter_block_until", recursive_mutex_enter_block_until},
    {"recursive_mutex_enter_blocking", recursive_mutex_enter_blocking},
    {"recursive_mutex_enter_timeout_ms", recursive_mutex_enter_timeout_ms},
    {"recursive_mutex_enter_timeout_us", recursive_mutex_enter_timeout_us},
    {"recursive_mutex_exit", recursive_mutex_exit},
    {"recursive_mutex_init", recursive_mutex_init},
    {"recursive_mutex_is_initialized", stub_recursive_mutex_is_initialized},
    {"recursive_mutex_try_enter", recursive_mutex_try_enter},
    {"reset_block", stub_reset_block},
    {"rtc_disable_alarm", rtc_disable_alarm},
    {"rtc_enable_alarm", rtc_enable_alarm},
    {"rtc_get_datetime", rtc_get_datetime},
    {"rtc_init", rtc_init},
    {"rtc_running", rtc_running},
    {"rtc_set_alarm", rtc_set_alarm},
    {"rtc_set_datetime", rtc_set_datetime},
    {"sem_acquire_block_until", sem_acquire_block_until},
    {"sem_acquire_blocking", sem_acquire_blocking},
    {"sem_acquire_timeout_ms", sem_acquire_timeout_ms},
    {"sem_acquire_timeout_us", sem_acquire_timeout_us},
    {"sem_available", sem_available},
    {"sem_init", sem_init},
    {"sem_release", sem_release},
    {"sem_reset", sem_reset},
    {"sem_try_acquire", sem_try_acquire},
    {"set_sys_clock_48mhz", set_sys_clock_48mhz},
    {"set_sys_clock_khz", stub_set_sys_clock_khz},
    {"set_sys_clock_pll", set_sys_clock_pll},
    {"setjmp", setjmp},
    {"setlocale", setlocale},
    {"setup_default_uart", setup_default_uart},
    {"sincos", sincos},
    {"sincosf", sincosf},
    {"siprintf", siprintf},
    {"siscanf", siscanf},
    {"sleep_ms", sleep_ms},
    {"sleep_until", sleep_until},
    {"sleep_us", sleep_us},
    {"sm_config_set_clkdiv", stub_sm_config_set_clkdiv},
    {"sm_config_set_clkdiv_int_frac", stub_sm_config_set_clkdiv_int_frac},
    {"sm_config_set_fifo_join", stub_sm_config_set_fifo_join},
    {"sm_config_set_in_pins", stub_sm_config_set_in_pins},
    {"sm_config_set_in_shift", stub_sm_config_set_in_shift},
    {"sm_config_set_jmp_pin", stub_sm_config_set_jmp_pin},
    {"sm_config_set_mov_status", stub_sm_config_set_mov_status},
    {"sm_config_set_out_pins", stub_sm_config_set_out_pins},
    {"sm_config_set_out_shift", stub_sm_config_set_out_shift},
    {"sm_config_set_out_special", stub_sm_config_set_out_special},
    {"sm_config_set_set_pins", stub_sm_config_set_set_pins},
    {"sm_config_set_sideset", stub_sm_config_set_sideset},
    {"sm_config_set_sideset_pins", stub_sm_config_set_sideset_pins},
    {"sm_config_set_wrap", stub_sm_config_set_wrap},
    {"snprintf", snprintf},
    {"spi_deinit", spi_deinit},
    {"spi_get_baudrate", spi_get_baudrate},
    {"spi_get_const_hw", stub_spi_get_const_hw},
    {"spi_get_dreq", stub_spi_get_dreq},
    {"spi_get_hw", stub_spi_get_hw},
    {"spi_get_index", stub_spi_get_index},
    {"spi_init", spi_init},
    {"spi_is_busy", stub_spi_is_busy},
    {"spi_is_readable", stub_spi_is_readable},
    {"spi_is_writable", stub_spi_is_writable},
    {"spi_read16_blocking", spi_read16_blocking},
    {"spi_read_blocking", spi_read_blocking},
    {"spi_set_baudrate", spi_set_baudrate},
    {"spi_set_format", stub_spi_set_format},
    {"spi_set_slave", stub_spi_set_slave},
    {"spi_write16_blocking", spi_write16_blocking},
    {"spi_write16_read16_blocking", spi_write16_read16_blocking},
    {"spi_write_blocking", spi_write_blocking},
    {"spi_write_read_blocking", spi_write_read_blocking},
    {"spin_lock_claim", spin_lock_claim},
    {"spin_lock_claim_mask", spin_lock_claim_mask},
    {"spin_lock_claim_unused", spin_lock_claim_unused},
    {"spin_lock_init", spin_lock_init},
    {"spin_lock_is_claimed", spin_lock_is_claimed},
    {"spin_lock_unclaim", spin_lock_unclaim},
    {"spin_locks_reset", spin_locks_reset},
    {"sprintf", sprintf},
    {"sscanf", sscanf},
    {"stdin_uart_init", stdin_uart_init},
    {"stdio_filter_driver", stdio_filter_driver},
    {"stdio_flush", stdio_flush},
    {"stdio_init_all", stdio_init_all},
    {"stdio_set_driver_enabled", stdio_set_driver_enabled},
    {"stdio_set_translate_crlf", stdio_set_translate_crlf},
    {"stdio_uart_init", stdio_uart_init},
    {"stdio_uart_init_full", stdio_uart_init_full},
    {"stdout_uart_init", stdout_uart_init},
    {"strcat", strcat},
    {"strchr", strchr},
    {"strcmp", strcmp},
    {"strcpy", strcpy},
    {"strcspn", strcspn},
    {"strlen", strlen},
    {"strncasecmp", strncasecmp},
    {"strncat", strncat},
    {"strncmp", strncmp},
    {"strncpy", strncpy},
    {"strnlen", strnlen},
    {"strrchr", strrchr},
    {"strspn", strspn},
    {"strstr", strstr},
    {"strtod", strtod},
    {"strtod_l", strtod_l},
    {"strtof", strtof},
    {"strtof_l", strtof_l},
    {"strtol", strtol},
    {"strtol_l", strtol_l},
    {"strtoll", strtoll},
    {"strtoll_l", strtoll_l},
    {"strtoul", strtoul},
    {"strtoul_l", strtoul_l},
    {"strtoull", strtoull},
    {"strtoull_l", strtoull_l},
    {"system", system},
    {"time", time},
    {"time_reached", stub_time_reached},
    {"time_us_32", stub_time_us_32},
    {"time_us_64", time_us_64},
    {"to_ms_since_boot", stub_to_ms_since_boot},
    {"to_us_since_boot", stub_to_us_since_boot},
    {"truncl", truncl},
    {"ttxtfont", ttxtfont},
    {"tzset", tzset},
    {"uart_default_tx_wait_blocking", stub_uart_default_tx_wait_blocking},
    {"uart_deinit", uart_deinit},
    {"uart_get_dreq", stub_uart_get_dreq},
    {"uart_get_hw", stub_uart_get_hw},
    {"uart_get_index", stub_uart_get_index},
    {"uart_get_instance", stub_uart_get_instance},
    {"uart_getc", stub_uart_getc},
    {"uart_init", uart_init},
    {"uart_is_enabled", stub_uart_is_enabled},
    {"uart_is_readable", stub_uart_is_readable},
    {"uart_is_readable_within_us", uart_is_readable_within_us},
    {"uart_is_writable", stub_uart_is_writable},
    {"uart_putc", stub_uart_putc},
    {"uart_putc_raw", stub_uart_putc_raw},
    {"uart_puts", stub_uart_puts},
    {"uart_read_blocking", stub_uart_read_blocking},
    {"uart_set_baudrate", uart_set_baudrate},
    {"uart_set_break", stub_uart_set_break},
    {"uart_set_fifo_enabled", stub_uart_set_fifo_enabled},
    {"uart_set_format", stub_uart_set_format},
    {"uart_set_hw_flow", stub_uart_set_hw_flow},
    {"uart_set_irq_enables", stub_uart_set_irq_enables},
    {"uart_set_translate_crlf", uart_set_translate_crlf},
    {"uart_tx_wait_blocking", stub_uart_tx_wait_blocking},
    {"uart_write_blocking", stub_uart_write_blocking},
    {"ufix2double", ufix2double},
    {"ufix2float", ufix2float},
    {"ufix642double", ufix642double},
    {"ufix642float", ufix642float},
    {"ungetc", ungetc},
    {"unreset_block", stub_unreset_block},
    {"unreset_block_wait", stub_unreset_block_wait},
    {"update_us_since_boot", stub_update_us_since_boot},
    {"us_to_ms", stub_us_to_ms},
    {"user_irq_claim", user_irq_claim},
    {"user_irq_claim_unused", user_irq_claim_unused},
    {"user_irq_is_claimed", user_irq_is_claimed},
    {"user_irq_unclaim", user_irq_unclaim},
    {"vfiprintf", vfiprintf},
    {"vfprintf", vfprintf},
    {"vprintf", vprintf},
    {"vreg_set_voltage", vreg_set_voltage},
    {"vsprintf", vsprintf},
    {"watchdog_caused_reboot", watchdog_caused_reboot},
    {"watchdog_enable", watchdog_enable},
    {"watchdog_enable_caused_reboot", watchdog_enable_caused_reboot},
    {"watchdog_get_count", watchdog_get_count},
    {"watchdog_reboot", watchdog_reboot},
    {"watchdog_start_tick", watchdog_start_tick},
    {"watchdog_update", watchdog_update},
    {"wcrtomb", wcrtomb},
    {"xip_alias_check_addr", stub_xip_alias_check_addr},
    {"xosc_disable", xosc_disable},
    {"xosc_dormant", xosc_dormant},
    {"xosc_init", xosc_init},
};
