#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include "serial.h"

#define DEFAULT_MAPPING {'J', 'K', '\r', '\b', 'A', 'D', 'W', 'S'} 

enum Buttons
{
    BUTTON_A = 0x01,
    BUTTON_B = 0x02,
    BUTTON_START = 0x04,
    BUTTON_SELECT = 0x08,
    DPAD_L = 0x10,
    DPAD_R = 0x20,
    DPAD_U = 0x40,
    DPAD_D = 0x80
};

static inline bool parseMapping(const char* str, uint8_t* mapping)
{
    int len = strlen(str);
    int strIdx = 0;
    uint8_t read = 0;
    for(int i = 0; i < 8; ++i)
    {
        if(strIdx >= len) return false;
        read = (uint8_t)str[strIdx++];
        
        if(read == '\\')
        {
            if(strIdx >= len) return false;
            read = (uint8_t)str[strIdx++];
            switch(read)
            {
            case 'b':
            case 'B':
                read = '\b';
                break;
            case 'r':
            case 'R':
            case 'n':
            case 'N':
                read = '\r';
                break;
            case 't':
            case 'T':
                read = '\t';
                break;
            case 's':
            case 'S':
                read = ' ';
                break;
            case '\\':
                read = '\\';
                break;
            case '\'':
                read = '\'';
                break;
            default:
                return false;
                break;
            }
        }
        else if(read >= 97 && read < 123)
        {
            read -= 32;
        }
        else if(read == '\''){}
        else if(read == ';'){}
        else if(read == '='){}
        else if(read == '`'){}
        else if(read >= 44 && read < 58){}
        else if(read >= 65 && read < 94){}
        else return false;
        
        mapping[i] = read;
    }
    
    return true;
}

static inline void printMaping(uint8_t* mapping)
{
    puts("Mapping: ");
    for(int i = 0; i < 8; ++i)
    {
        printf("%x ", (int)mapping[i]);
    }
    puts("\n");
}

static inline int addEvent(INPUT* inputs, int index, bool current, bool last, int code)
{
    if(current && !last)
    {
        inputs[index].ki.dwFlags = 0;
        inputs[index++].ki.wVk = code;
    }
    else if(!current && last)
    {
        inputs[index].ki.dwFlags = KEYEVENTF_KEYUP;
        inputs[index++].ki.wVk = code;
    }
    
    return index;
}

int main(int argc, char** argv)
{
    SerialPort port;
    uint8_t current = 0;
    uint8_t last = 0;
    uint8_t mapping[8] = DEFAULT_MAPPING;
    int32_t read = 0;
    int index;
    INPUT inputs[8] = {};
    
    if(argc <  2)
    {
        fputs("Invalid usage: no serial port given.\nValid usage:\nserialConroller <PORT NAME>\n", stderr);
        return -1;
    }
    
    if(argc > 2)
    {
        if(!parseMapping(argv[2], mapping))
        {
            fputs("Invalid mapping given.\n", stderr);
        }
    }
    
    printMaping(mapping);
    
    for(int i = 0; i < 8; ++i)
    {
        inputs[i].type = INPUT_KEYBOARD;
    }
    
    port = serialInit(argv[1], 9600);
    
    while(true)
    {
        index = 0;
        
        read = serialRead(port, &current, 1);
        
        if(read < 0)
        {
            break;
        }
        
        index = addEvent(inputs, index, (current & BUTTON_A) != 0, (last & BUTTON_A) != 0, mapping[0]);
        index = addEvent(inputs, index, (current & BUTTON_B) != 0, (last & BUTTON_B) != 0, mapping[1]);
        index = addEvent(inputs, index, (current & BUTTON_START) != 0, (last & BUTTON_START) != 0, mapping[2]);
        index = addEvent(inputs, index, (current & BUTTON_SELECT) != 0, (last & BUTTON_SELECT) != 0, mapping[3]);
        index = addEvent(inputs, index, (current & DPAD_L) != 0, (last & DPAD_L) != 0, mapping[4]);
        index = addEvent(inputs, index, (current & DPAD_R) != 0, (last & DPAD_R) != 0, mapping[5]);
        index = addEvent(inputs, index, (current & DPAD_U) != 0, (last & DPAD_U) != 0, mapping[6]);
        index = addEvent(inputs, index, (current & DPAD_D) != 0, (last & DPAD_D) != 0, mapping[7]);
        
        SendInput(index, inputs, sizeof(INPUT));
        
        last = current;
    }
    
    serialClose(port);
    return 0;
}
