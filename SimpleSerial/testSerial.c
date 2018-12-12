/**The Happy Bunny License (Modified MIT License)
 * --------------------------------------------------------------------------------
 * Copyright (c) 2018 Max Sichel
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * Restrictions:
 *  By making use of the Software for military purposes, you choose to make a
 *  Bunny unhappy.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "serial.h"

int main(int argc, char** argv)
{
    SerialPort port;
    int res = 0;
    char ostr[8] = {};
    char istr[8] = {};
    
    if(argc < 2)
    {
        fputs("Invalid usage: no port given\n", stderr);
        return -1;
    }
    
    srand(time(NULL));
    
    port = serialInit(argv[1], 9600);
    
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            ostr[j] = (char)(rand() % 256);
        }
        
        printf("Sending %c%c%c%c%c%c%c%c to serial port\n", ostr[0], ostr[1], ostr[2], ostr[3], ostr[4], ostr[5], ostr[6], ostr[7]);
        serialWrite(port, (uint8_t*)ostr, 8);
    
        printf("Waiting for response...\n");
        res = serialRead(port, (uint8_t*)istr, 8);
        
        if(res < 0)
        {
            fputs("Error: unable to read from serial port\n", stderr);
            break;
        }
        else
        {
            printf("%c%c%c%c%c%c%c%c\n", istr[0], istr[1], istr[2], istr[3], istr[4], istr[5], istr[6], istr[7]);
        }
    }
    
    serialClose(port);
    
    return 0;
}
