/* 
 * File:   can_messages.h
 * Author: henry
 *
 * Created on August 3, 2021, 10:34 AM
 */

#ifndef CAN_MESSAGES_H
#define CAN_MESSAGES_H

#include <stdint.h>

//Message ID numbers (11-bit: 0x000 to 0x7FF)
//Lower numbers get priority if there is a bus contention, so use lower numbers
//for more critical data
//The lower 4 bits are a RCU ID (up to 16 RCUs on bus),
//so that message IDs go in increments of 0x10 or 16
//RCU ID numbers (4-bit: 0x00 to 0x0F, 0 to 15)
#define RCU_ID_MAIN_RCU 0x01
#define RCU_ID_ENGINE_VALVE_RCU 0x02
#define RCU_ID_ENGINE_SENSOR_RCU 0x03
#define RCU_ID_PYRO_RCU 0x04

#define CAN_ID_PyroControl 0x050
struct PyroControl_t
{
    uint8_t fire_ignitor : 1;
    uint8_t fire_main : 1;
    uint8_t fire_drogue : 1;
};

#define CAN_ID_ValveControl 0x100

struct ValveControl_t
{
    uint8_t main_ox_valve_goal_pos;
    uint8_t fuel_press_valve_goal_pos;
    struct SolenoidsControl_t
    {
        uint8_t engine_vent_valve_close : 1;
        uint8_t main_fuel_valve_open : 1;
        uint8_t aux_1_energize : 1;
        uint8_t aux_2_energize : 1;
    } solenoids;
};

#define CAN_ID_EngineSensorsA 0x110
struct EngineSensorsA_t
{
    uint16_t fuel_tank_press_raw;
    uint16_t chamber_press_raw;
    uint16_t fuel_inj_press_raw;
    uint16_t ox_inj_press_raw;
};

#define CAN_ID_EngineSensorsB 0x120
struct EngineSensorsB_t
{
    uint16_t thrust_raw;
    uint16_t aux_1_raw;
    uint16_t aux_2_raw;
    uint16_t aux_3_raw;
};

#define CAN_ID_MotorStatus_OX_MAIN 0x200
#define CAN_ID_MotorStatus_FUEL_PRESS 0x210
struct MotorStatus_t
{
    uint8_t pos;
    int8_t speed;
    struct LimitSwitchStatus_t
    {
        uint8_t limit_switch : 1;
        uint8_t has_homed : 1;
    } homing;
    struct MotorValveFaults_t
    {
        uint8_t motor_stuck : 1;
        uint8_t homing_timeout : 1;
        uint8_t limit_switch_stuck : 1;
        uint8_t motor_slip_encoder_drift : 1;
        uint8_t encoder_error : 1;
    } faults;
};

#define CAN_ID_PyroStatus 0x220
struct PyroStatus_t
{
    uint16_t pyro_voltage_raw;
    uint8_t ignitor_cont : 1;
    uint8_t main_cont : 1;
    uint8_t drogue_cont : 1;
};

#define CAN_ID_PowerStatus 0x230
struct PowerStatus_t
{
    uint16_t voltage_12V_mV;
    uint16_t current_12V_mA;
    uint16_t current_5V_mA;
    struct
    {
        uint8_t external_power_connected : 1;
        uint8_t undervoltage : 1;
        uint8_t overvoltage : 1;
        uint8_t overcurrent_12V : 1;
        uint8_t overcurrent_5V : 1;
        uint8_t shutdown_req : 1;
    } flags;
};

#define CAN_ID_PowerControl 0x230
struct PowerControl_t
{
    uint8_t shutdown_request : 1;
};

#define CAN_ID_Heartbeat 0x400
struct Heartbeat_t
{
    uint8_t health;
    uint16_t uptime_s;
};
#define HEALTH_NOMINAL 0
#define HEALTH_WARNING 1
#define HEALTH_CRITICAL 2

#define CAN_ID_Error 0x410
struct Error_t
{
    uint8_t error_no;
    uint8_t error_data[7];
};

#define CAN_ID_DebugPrint 0x420
struct DebugPrint_t
{
    char text[8];
};

#define CAN_ID_Restart 0x430
struct Restart_t
{
    uint8_t rcu_id; //ID of RCU to be restarted
};

#endif /* CAN_MESSAGES_H */
