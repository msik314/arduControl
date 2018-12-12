Simple Serial
=============

Simple serial is a simple synchronous serial port reader for Win32.

Functions
--------
```c
SerialPort serialInit(const char* port, int32_t baud);
```
Creates the serial connection.  Call this function before using any of the other functions.

```c
int32_t serialClose(SerialPort port);
```
Closes the serial connection.  Call this function when done with the serial port.

```c
int32_t serialRead(SerialPort port, uint8_t* data, int32_t size);
```
Read data from the serial port.  Puts size bytes from the serial port in the array given by data.

```c
int32_t serialWrite(SerialPort port, uint8_t* data, int32_t size);
```
Write data to the serial port.  Write size bytes from array given by data to the serial port.

Notes
-----
```serialInit()``` waits for 2 seconds after creating the connection.  This appears to be necessary for the connection to function.

*Simple Serial has only been tested on Windows using MinGW 8.1.  It should work with Clang or MSVC, but it is untested with those compilers.*
