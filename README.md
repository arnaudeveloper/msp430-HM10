# msp430-HM10
Basic library for msp430F5529LP and HM-10 Bluetooth module

In this repository you will find the code to discover other HM-10 modules, connect to them and cut off the communication to return initial configuration.

I use the following AT commands
1. AT => OK,OK+LOST
2. AT+RENEW => OK+RENEW
3. AT+RESET => OK+RESET
4. AT+NOTI1 => OK+Set:1
5. AT+ADDR? => OK+ADDR:[MAC]
6. AT+DISC? => OK+DISCS, OK+DISCE, OK+DIS0:[MAC] 
7. AT+CO => OK+CONN
8. AT+CON (commented)

Thease commands are basic to initialize the module, do a scan, and connect to a discovered module or static MAC.

In one hand, there are the differents functions to send via UART. On the other hand, there are the RX UART reception and analysis.

The TX it's tedious but easy, you only must to pick the correct letters and send via UART. The tedious part is because the AT commands format, that it isn't coherent and tricky to use in c.

On the other hand RX has been the most difficult part for the same reason of the AT commands format.Therefore, almost all code is "bare metal".


