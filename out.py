#Auto-generated file; do not edit.
from typing import NamedTuple
from typing import Any

class PyroControlData(NamedTuple):
	fire_drogue: Any
	fire_ignitor: Any
	fire_main: Any

class ValveControlData(NamedTuple):
	aux_1_energize: Any
	aux_2_energize: Any
	engine_vent_valve_close: Any
	fuel_pres_valve_goal_pos: Any
	main_fuel_valve_open: Any
	main_ox_valve_goal_pos: Any

class EngineSensorsAData(NamedTuple):
	chamber_press_raw: Any
	fuel_inj_press_raw: Any
	fuel_tank_press_raw: Any
	ox_inj_press_raw: Any

class EngineSensorsBData(NamedTuple):
	aux_1_raw: Any
	aux_2_raw: Any
	aux_3_raw: Any
	thrust_raw: Any

class MotorStatusData(NamedTuple):
	encoder_error: Any
	has_homed: Any
	homing_timeout: Any
	limit_switch: Any
	limit_switch_stuck: Any
	motor_slip_encoder_drift: Any
	motor_stuck: Any
	pos: Any
	speed: Any

class MotorStatusData(NamedTuple):
	encoder_error: Any
	has_homed: Any
	homing_timeout: Any
	limit_switch: Any
	limit_switch_stuck: Any
	motor_slip_encoder_drift: Any
	motor_stuck: Any
	pos: Any
	speed: Any

class PyroStatusData(NamedTuple):
	drogue_cont: Any
	ignitor_cont: Any
	main_cont: Any

class PowerStatusData(NamedTuple):
	current_12V_mA: Any
	current_5V_mA: Any
	external_power_connected: Any
	overcurrent_12V: Any
	overcurrent_5V: Any
	overvoltage: Any
	shutdown_req: Any
	undervoltage: Any
	voltage_12V_mV: Any

class PowerControlData(NamedTuple):
	shutdown_request: Any

class HeartbeatData(NamedTuple):
	health: Any
	uptime_s: Any

class ErrorData(NamedTuple):
	error_data: Any
	error_no: Any

class DebugPrintData(NamedTuple):
	text: Any

class RestartData(NamedTuple):
	target_rcu_id: Any
