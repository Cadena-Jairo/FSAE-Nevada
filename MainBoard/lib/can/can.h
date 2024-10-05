#pragma once

// Most of this comes from the CANMessage class
struct Message {
    unsigned long message_id;
    // These are all different ways to access the same 64 bits of data
    union {
        unsigned long long  data_u64;
        unsigned long       data_u32[2];
        unsigned short      data_u16[4];
        unsigned char       data_u8 [8];
        long long           data_s64;
        long                data_s32[2];
        short               data_s16[4];
        char                data_s8 [8];
        float               data_float[2];
    };
    unsigned char length;
    bool extended;
    bool remote_frame;
};

struct CommandMessage {
    unsigned char   direction_command;
    unsigned char   inverter_discharge;
    unsigned char   rolling_counter;
    unsigned short  speed_command;
    unsigned char   speed_mode_enable;
    unsigned short  torque_command;
    unsigned short  torque_limit_command;
};

void can_init();
unsigned long long can_rx();
bool can_tx(unsigned long long message_data);
