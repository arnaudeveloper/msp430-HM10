# msp430-HM10
Basic library for msp430F5529LP and HM-10 Bluetooth module

## Table of Contents  
1. [Introduction](#1)

2. [How to establish connection between two or more dispositives](#2)

3. [How to create your own protocol](#3)
      
      3.1 [Find the master](#3.1)
      
4. [How to add more AT commands](#3)

<a name="1"/>

## 1. Introduction
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

<a name="2"/>

## 2. How to establish connection between two or more dispositives

> _The connection always will be point-to-point, so you could connect to every module, but not at the same time or send a broadcast meassage._

<a name="3"/>

## 3. How to create your own protocol

In this part we will explain how (and where) to create your own protocol.

For this protocol I use __`#`__ to indicate the start of the my own data. There is a __`if`__ used to detect this symbol and start the capturing of the data.

```c  
      /* Used to detect the owner protocol */
      if(answer[j]=='#')
      {
          answer[0]=answer[j];
          j=0;                  // Start to build the array. At the end j will increase

      }    
 ```
Once __`#`__ has been captured and stored in position 0 of the `answer[]` array, the following data will be analayzed.

```c
      if(answer[0]=='#')    // Symbol to start the protocol
      {
          if(answer[1]=='?')    // Question
          {
              if(answer[2]=='M')    // Are you a master?
              {
                  /* Change "estat" to 4, for answer the question */
                  estat = 4;
              }
              //Code...
          }
          
          if(answer[1]=='!')    // Answer
          {
              match=TRUE;

              if(answer[2]=='M')    // It is a master
              {
                  /* It is a master, so we must to save the MAC address */
                  master_detected = TRUE;

                  i=j=k=0;      //Reset
                  memset(&answer,0, sizeof answer);     // Reset of the answer variable
                  memset(&word_cap,0,sizeof word_cap);  // Reset of the word_cap variable
                  __bic_SR_register_on_exit(LPM3_bits); // Exit LPM

              }
              //Code...
          }
          //Code...
      }// End of owner protocol
      
 ```
<a name="3.1"/>

### 3.1 Find the master

> _TIP: To establish a connection the module that need to establish this connection must change his role to master. That doesn't mean that the module it's a master. In other words, we differentiate the module that it's a master because it needs take this role, of the "real master" of the net because we have decided on the code_

Find the master is the "game" used for `config_DISC()` to scan around and discoverd an HM-10 master module.

I will use this function to explain how it works "my own protocol".

If you launch `config_DISC()` after a connection has been established sender will send __#?m[MAC]__ like "You're a master?", and the MAC of the master sender, all of this with `SEND()`function.
```c
        while(match==0)
        {
            SEND();                             // Send ACK
            TA0CCTL0 = CCIE;                    // Start Timer
            __bis_SR_register(LPM3_bits);
            if(x>10)match=1;                   // Used for bad communications
            x++;
        }
 ```
The reciver will recive this data, and after the detection in the folowing lines:
      
```c

      if(answer[0]=='#')    // Symbol to start the protocol
      {
          if(answer[1]=='?')    // Question
          {
              if(answer[2]=='M')    // Are you a master?
              {
                  /* Change "estat" to 4, for answer the question */
                  estat = 4;
              }
              //Code...
          }
 ```
If the receiver is an HM-10 module and also master, will send the answer, in that case `#!M`.
 
 ```c
           case 4:
              /*In that case, sends the answer after analyze the question*/
              /*Analyze the 2nd position of the answer array*/
              switch (answer[2])
              {
                  case 'M': //master?

                      send_rol();       //Answer
                      break;
              }
  ```
  Once the original sender have recived the answer, will be analyzed in the same way.
 
 ```c
          if(answer[1]=='!')    // Answer
          {
              match=TRUE;

              if(answer[2]=='M')    // It is a master
              {
                  /* It is a master, so we must to save the MAC address */
                  master_detected = TRUE;

                  i=j=k=0;      //Reset
                  memset(&answer,0, sizeof answer);     // Reset of the answer variable
                  memset(&word_cap,0,sizeof word_cap);  // Reset of the word_cap variable
                  __bic_SR_register_on_exit(LPM3_bits); // Exit LPM

              }
              //Code...
          }
          //Code...
      }// End of owner protocol      
 ```
 In that case we use global variables to change the state. In that case case `master_dectected` will change to `TRUE`, and the MAC addres will be stored.



<a name="4"/>

## 4. How to add more AT commands





