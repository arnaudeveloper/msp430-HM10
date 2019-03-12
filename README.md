# msp430-HM10
Basic library for msp430 and HM-10 Bluetooth module

Basic library in c with examples to configure in differents ways the HM-10 module.
In this repository you will find the code to discover other HM-10 modules, connect to them and cut off the communication to return intial configuration.

I use the follow AT commands
1. AT
2. AT+RENEW
3. AT+RESET
4. AT+NOTI1
5. AT+ADDR?
6. AT+DISC?
7. AT+CO
8. AT+CON (commented)

This commands are basic to initialize the module, do a scan, and connect to a discovered module or static MAC.
