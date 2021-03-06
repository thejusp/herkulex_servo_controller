//
// Created by lukasz on 31.10.16.
//

#ifndef HERKULEX_SERVO_CONTROLLER_HERKULEXCONTROLLER_H
#define HERKULEX_SERVO_CONTROLLER_HERKULEXCONTROLLER_H


#define EEP_WRITE (u_char) 0x01 // command for writing to EEP
#define EEP_READ (u_char) 0x02// command for reading from EEP
#define RAM_WRITE (u_char) 0x03 // command for writing to RAM
#define RAM_READ (u_char) 0x04 // command for reading from RAM
#define I_JOG (u_char) 0x05 // command for servo movement control
#define S_JOG (u_char) 0x06 // command for servo movement control
#define STAT (u_char) 0x07 // command for getting servo status
#define ROLLBACK (u_char) 0x08 // command for restoring factory defaults
#define REBOOT (u_char) 0x09 // command for rebooting servo

#define ACK_POLICY_RAM_ADDR (u_char) 0x01
#define ACK_POLICY_EEP_ADDR (u_char) 0x07

#define ACK_POLICY_NO_REPLY (u_char) 0x00
#define ACK_POLICY_REPLY_TO_READ (u_char) 0x01
#define ACK_POLICY_REPLY_TO_ALL (u_char) 0x02

#define TORQUE_CONTROL_RAM_ADDR (u_char) 52
#define TORQUE_CONTROL_TORQUE_FREE (u_char) 0x00 // servo manually movable, operation command (I_JOG, S_JOG) not possible
#define TORQUE_CONTROL_BREAK_ON (u_char) 0x40 // servo stopped, operation command (I_JOG, S_JOG) not possible
#define TORQUE_CONTROL_TORQUE_ON (u_char) 0x60 // operation possible

#define LED_CONTROL_RAM_ADDR (u_char) 53
#define LED_CONTROL_OFF (u_char) 0x00
#define LED_CONTROL_GREEN (u_char) 0x01
#define LED_CONTROL_BLUE (u_char) 0x02
#define LED_CONTROL_RED (u_char) 0x04

#define STATUS_ERROR_RAM_ADDR (u_char) 48

#define MIN_POSITION_RAM_ADDR (u_char) 20
#define MIN_POSITION_EEP_ADDR (u_char) 26
#define MAX_POSITION_RAM_ADDR (u_char) 22
#define MAX_POSITION_EEP_ADDR (u_char) 28

#define ABSOLUTE_POSITION_RAM_ADDR (u_char) 60

#define SERVO_TYPE_UNKNOWN 0
#define SERVO_TYPE_DRS_0101 1
#define SERVO_TYPE_DRS_0602 2

#define DRS_0101_MIN_POS 0 // steps
#define DRS_0101_MAX_POS 1023 // steps
#define DRS_0101_RESOLUTION 0.325 // degrees/step
#define DRS_0101_ZERO_POS 512 // steps

#define DRS_0602_MIN_POS 10381 // steps
#define DRS_0602_MAX_POS  22129 // steps
#define DRS_0602_RESOLUTION 0.02778 // degrees/step
#define DRS_0602_ZERO_POS 16384 // steps


#include <string>
#include <vector>
#include <iostream>
#include "serial/serial.h"
#include <math.h>

class HerkulexController {

    // constructors & destructors
public:
    /**
     * Left motor is to be connected to M1 output on Roboclaw board
     */
    //HerkulexController(int port_number);
    HerkulexController(std::string port, int servo_id, uint8_t servo_type, uint16_t timeout);

    ~HerkulexController();

    // fields
private:
    serial::Serial *my_serial;
    // functions
public:
    bool has_acces_to_ComPort();

    void read_version();

    void i_jog_control(u_char servo_id, u_int16_t position);

    void i_jog_control(u_char servo_id, double position);

    u_int16_t get_absolute_position(u_char servo_id);

    double get_absolute_position_deg(u_char servo_id);

    double get_absolute_position_rad(u_char servo_id);

private:
    bool comPort_opened;
    bool servo_detected;
    u_char response_len;
    u_char internal_servo_id;
    uint8_t internal_servo_type;
    uint16_t internal_min_pos;
    uint16_t internal_max_pos;
    double internal_res;
    uint16_t internal_zero;

    std::vector<u_char> make_command_packet(u_char servo_id, u_char command,
                                            std::vector<u_char> data);

    void set_ack_policy(u_char servo_id, u_char ack_policy);

    void set_led(u_char servo_id, u_char led_state);

    u_char get_led(u_char servo_id);

    void set_min_position(u_char servo_id, u_int16_t min_pos);

    u_int16_t get_min_position(u_char servo_id);

    void set_max_position(u_char servo_id, u_int16_t max_pos);

    u_int16_t get_max_position(u_char servo_id);

    void set_torque_control(u_char servo_id, u_char torque_control_mode);

    u_char get_torque_control(u_char servo_id);

    u_char get_status_error(u_char servo_id);

    void calculate_checksum(u_char *checksum_1, u_char *checksum_2, u_char *packet_size, u_char
    servo_id, u_char command, std::vector<u_char> data);

    std::vector<u_char> ram_read(u_char servo_id, std::vector<u_char> data);

    void ram_write(u_char servo_id, std::vector<u_char> data);


};


#endif //HERKULEX_SERVO_CONTROLLER_HERKULEXCONTROLLER_H
