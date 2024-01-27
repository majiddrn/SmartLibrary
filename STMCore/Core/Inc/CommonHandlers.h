#ifndef COMMONHANDLERS_H
#define COMMONHANDLERS_H

typedef enum {
    MESSAGE_TYPE_INVALID = -1,
    MESSAGE_TYPE_LED,
    MESSAGE_TYPE_PIR,
    MESSAGE_TYPE_AVOID,
} MessageType;

typedef struct {
	MessageType type;
    uint8_t mNumber;
    char message[15];
} Message;

enum ACK_TYPE{
	ACK_OK,
	ACK_INVALID,
	ACK_ERR
};

#endif
