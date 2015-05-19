#ifndef __ARDUINOAPI_ARDUINOSERIAL_H__
#define __ARDUINOAPI_ARDUINOSERIAL_H__

#include "Print.h"
#include "WString.h"

typedef enum {
    ARDUINOSERIAL_BUFFER_SIZE = 512,
} ArduinoSerialConstant;

typedef struct {
    unsigned char buffer[ARDUINOSERIAL_BUFFER_SIZE];
    volatile unsigned int head;
    volatile unsigned int tail;
} ArduinoSerialRingBuffer;

class ArduinoSerial;

typedef struct ArduinoSerialLinkedNode{
    ArduinoSerial* data;
    struct ArduinoSerialLinkedNode *next;
} ArduinoSerialLinkedNode;

class ArduinoSerial: public Serial, public Print
{
public:
    ArduinoSerial(PinName tx, PinName rx);
    
    virtual ~ArduinoSerial(void);
    
    virtual size_t write(uint8_t data);
    
    void begin(int baud_rate);
    
    int available(void);

    void flush(void);
    
    char readChr(void);
    
    void setTimeout(unsigned long millisecond);
    
    bool find(const char *str);
    
private:
    static void uartIrqCallback(void);
    static ArduinoSerialLinkedNode *initHeadNode(ArduinoSerialLinkedNode *head);
    static ArduinoSerialLinkedNode *addNode(ArduinoSerialLinkedNode *head, ArduinoSerial* data);
    static ArduinoSerialLinkedNode *delNode(ArduinoSerialLinkedNode *head, ArduinoSerial* data);
    static ArduinoSerialLinkedNode *findNode(ArduinoSerialLinkedNode *head, ArduinoSerial* data);
    
    static ArduinoSerialLinkedNode ms_list_head;
    static unsigned int ms_instance_counter;
    
    void writeChr(unsigned char c);
 
    ArduinoSerialRingBuffer m_rx_buffer;
    unsigned long m_find_timeout;
};

#endif /* #ifndef __ARDUINOAPI_ARDUINOSERIAL_H__ */