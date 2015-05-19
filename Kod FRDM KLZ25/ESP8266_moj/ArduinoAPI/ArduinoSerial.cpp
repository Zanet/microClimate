#include "ArduinoSerial.h"

ArduinoSerialLinkedNode ArduinoSerial::ms_list_head = {NULL, NULL};
unsigned int ArduinoSerial::ms_instance_counter = 0;

void ArduinoSerial::uartIrqCallback(void) {
    ArduinoSerialLinkedNode *p;
    for (p = ms_list_head.next; p != &ms_list_head; p = p->next) {
        if (p->data != NULL) {
            while(p->data->readable()) {
                p->data->writeChr(p->data->getc());
            }
        }
    }
}

ArduinoSerial::ArduinoSerial(PinName tx, PinName rx):Serial(tx, rx) {
    ms_instance_counter++;
    if (ms_instance_counter == 1) {
        initHeadNode(&ms_list_head);
    }
    
    if (addNode(&ms_list_head, this) != NULL) {
        this->attach(&uartIrqCallback);   
    } else {
    }
}

ArduinoSerial::~ArduinoSerial(void) {
    ms_instance_counter--;
    if (delNode(&ms_list_head, this) != NULL) {
        this->attach(NULL); 
    } else {
    }
}
    
void ArduinoSerial::begin(int baud_rate) {
    baud(baud_rate);
    flush();
}
    
int ArduinoSerial::available(void) {
    return (unsigned int)(ARDUINOSERIAL_BUFFER_SIZE + m_rx_buffer.head - m_rx_buffer.tail) % ARDUINOSERIAL_BUFFER_SIZE;
}

void ArduinoSerial::flush(void)
{
    memset(&m_rx_buffer, 0, sizeof(m_rx_buffer));
}

char ArduinoSerial::readChr(void) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (m_rx_buffer.head == m_rx_buffer.tail) {
        return (char)-1;
    } else {
        unsigned char c = m_rx_buffer.buffer[m_rx_buffer.tail];
        m_rx_buffer.tail = (unsigned int)(m_rx_buffer.tail + 1) % ARDUINOSERIAL_BUFFER_SIZE;
        return c;
    }
}

void ArduinoSerial::setTimeout(unsigned long millisecond) {
    m_find_timeout = millisecond;
}

bool ArduinoSerial::find(const char *str) {
    bool ret = false;
    String data;
    char c;
    unsigned long i;
    for (i = 0; i < m_find_timeout; i++) {
        while(available() > 0) {
            c = readChr();
            data += c;
        }
        if (data.indexOf(String(str)) != -1) {
            ret = true;
            break;
        }
        wait_ms(1);
    }
    return ret;
}

size_t ArduinoSerial::write(uint8_t data) {
    putc(data);
    return 1;
}

void ArduinoSerial::writeChr(unsigned char c) {
    int i = (unsigned int)(m_rx_buffer.head + 1) % ARDUINOSERIAL_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != m_rx_buffer.tail) {
        m_rx_buffer.buffer[m_rx_buffer.head] = c;
        m_rx_buffer.head = i;
    }
}

ArduinoSerialLinkedNode *ArduinoSerial::initHeadNode(ArduinoSerialLinkedNode *head) {
    if (head == NULL) {
        return NULL;
    }
    head->data = NULL;
    head->next = head;
    return head;
}

ArduinoSerialLinkedNode *ArduinoSerial::addNode(ArduinoSerialLinkedNode *head, ArduinoSerial* data) {
    ArduinoSerialLinkedNode *p;
    ArduinoSerialLinkedNode *node = (ArduinoSerialLinkedNode *)malloc(sizeof(ArduinoSerialLinkedNode));
    if (node == NULL) {
        return NULL;
    }
    /* Setting node */
    node->data = data;
    node->next = head;
    
    /* Add node to tail */
    for(p = head; p->next != head; p = p->next);
    p->next = node;
    
    return head;
}
ArduinoSerialLinkedNode *ArduinoSerial::delNode(ArduinoSerialLinkedNode *head, ArduinoSerial* data) {
    ArduinoSerialLinkedNode *p;
    ArduinoSerialLinkedNode *prev;
    
    if (head == NULL) {
        return NULL;
    }
    
    prev = head, p = head->next; 
    while(p != head) {
        if (p->data == data) {
            prev->next = p->next;
            free(p);
            p = prev->next;
        } else {
            prev = p;
            p = p->next;    
        }
    }
    return head;
}
ArduinoSerialLinkedNode *ArduinoSerial::findNode(ArduinoSerialLinkedNode *head, ArduinoSerial* data) {
    ArduinoSerialLinkedNode *p;
    if (head == NULL) {
        return NULL;
    }
    
    for (p = head->next; p != head; p = p->next) {
        if (p->data == data) {
            return p;
        }
    }
    return NULL;
}