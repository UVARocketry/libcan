/* 
 * File:   can.h
 * Author: henry
 *
 * Created on August 3, 2021, 5:49 PM
 */

#ifndef CAN_H
#define	CAN_H

//used to store RCU ID for the RCU this firmware will run on
//use "const uint8_t RCU_ID_LOCAL = RCU_ID_xxxx_RCU" in the main.c file
extern const uint8_t RCU_ID_LOCAL;

#define CAN_TXQ_LEN (uint8_t)8

struct can_msg_t {
    // Standard Identifier - 11 bits long
    uint16_t id;
    // How many bytes are used in data
    uint8_t len;
    // the data you want to transmit
    uint8_t data[8];
};

void (*can_rx_callback)(const struct can_msg_t *msg);

void can_init(void);

uint8_t can_txq_push(const uint16_t id, const uint8_t len, const uint8_t *data);

#endif	/* CAN_H */

