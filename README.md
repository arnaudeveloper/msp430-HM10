# msp430-HM10
Basic library for msp430F5529LP and HM-10 Bluetooth module

## Table of Contents
1. [Introduction](#1)

2. [Connectivity](#2)

      2.1 [How to establish connection between two or more devices](#2.1)

      2.2 [Possible reasons to unable establish a connection](#2.2)

3. [Communication](#3)

      3.1 [How to create your own protocol](#3.1)

      3.2 [Example: "Find the master"](#3.2)

4. [How to add more AT commands](#4)

<a name="1"/>

## 1. Introduction
In this repository you will find the code to discover other HM-10 modules, connect to them, send and receive data, and cut off the communication to return initial configuration.

I use the following AT commands
1. AT => OK,OK+LOST
2. AT+RENEW => OK+RENEW
3. AT+RESET => OK+RESET
4. AT+NOTI1 => OK+Set:1
5. AT+ADDR? => OK+ADDR:[MAC]
6. AT+DISC? => OK+DISCS, OK+DISCE, OK+DIS0:[MAC]
7. AT+CO => OK+CONN
8. AT+CON (commented)

These commands are basic to initialize the module, do a scan, and connect to a discovered module or static MAC.

In one hand, there are the different functions to send via UART. On the other hand, there are the RX UART reception and analysis.

The TX code it's tedious but easy, you only must pick the correct letters and send via UART. The tedious part is because the AT commands format, that it isn't coherent and tricky to use in c.

On the other hand, RX has been the most difficult part for the same reason of the AT commands format. Therefore, almost all code is "bare metal".

<a name="2"/>

### 2 Connectivity

Basic connection.
1. VCC to 3V
2. GND to GND
3. TXD to P3.4/UCA0RXD
4. RXD to P3.3/UCA0TXD

![imagen](https://user-images.githubusercontent.com/38794634/54671257-db684f00-4af5-11e9-85ce-713de54a51ea.png)


<a name="2.1"/>

### 2.1 How to establish connection between two or more devices

> _The connection always will be point-to-point, so you could connect to every module, but not at the same time or send a broadcast message._

To establish a connection first you need to configure the module in master role to start a communication with other modules.
For this objective you can use  `config_DISC()`.
This function works in the following way:

1. Initializing the HM-10 module as master

      Setting AT+IMME1 and AT+ROLE1.

2. Discovering the MAC addresses around of Bluetooth modules

      With `AT+DISC?` command the microcontroller can discover up to 3 devices*

```c
/* Capturing the address */
address[k]=answer[j];
 k++;

 if(k==14)
 {
     /* Address captured */
     get_address=TRUE;

     /* Saving MAC addres in a global variable*/
     if(contador==0)
     {
         memcpy(address2, &address[2], 12);
         contador++;
     }
     else if(contador==1)
     {
         memcpy(address3, &address[2], 12);
         contador++;
     }
     else if(contador==2)
     {
         memcpy(address4, &address[2], 12);
         contador++;
     }
     //Code...
```
(*) As you can see in the image you only need to add more else if, and others global variables.

After launch `AT+DISC?` command you will see the captured MAC address in the following global variables.

![imagen](https://user-images.githubusercontent.com/38794634/54521762-decfcf00-496b-11e9-903a-f1ddfc0da529.png)

In that example:
- address2 = 90E202020A47
- address3 = 341513CE6FF6
- address4 = 606405CFCD4F



3. Try to connect and initialize a dialogue

      Once you have a MAC address, you could try to connect it. To do this you must send `AT+CO` command.
  ```c
/* Set AT_CON*/
while(match==0)                     // Resend the command is the communication fail
{
    n_letters= AT_CON(pointer,address2);    // AT_CON0 command. Connect to address2
    TA0CCTL0 = CCIE;                        // Start Timer
    __bis_SR_register(LPM3_bits);           // Enter LPM0
    if(x>10)match=1;                        // Used for bad communications
    x++;
}
```

4. Successful connection

      If a connection has been established, we need to check if are trying to connect to a HM-10 module, or on the other hand is other kind of Bluetooth device.
```c
/* If connection = TRUE, a connection has been established*/
if(connection)
{
    /* Initiate the protocol to know if we have connected to a correct module */
    x=0;
    match=0;

    while(match==0)
    {
        SEND();                             // Send ACK
        TA0CCTL0 = CCIE;                    // Start Timer
        __bis_SR_register(LPM3_bits);
        if(x>10)match=1;                   // Used for bad communications
        x++;
    }

    /* If we have connected to the correct module x<10 */
    if(x>10)
    {
        /* We have connected to an incorrect module. Therefore, disconnect */
        match=0;
        while(match==0 && lost==0)
        {
            pointer = &word_check[0];
            n_letters=AT_2(pointer);
            TA0CCTL0 = CCIE;
            __bis_SR_register(LPM3_bits);
        }
        /* Sometimes we recive OK before LOST.
         * If we recive OK this indicates that the connection has been cut,
         * but Green LED will be TURN-ON, so TURN-OFF
         */
        P4OUT &= ~BIT7;   //Green LED OFF
    }
    else
    {
        /* If it is a master, save the address*/
        if(master_detected)
        {
            memcpy(address_M, address2,12);
        }

        while(lost==0)                          // Resend the command is the communication fail
        {
            pointer = &word_check[0];
            __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data
            n_letters= AT_2(pointer);           // Used to cut off communication
            TA0CCTL0 = CCIE;                    // Start Timer
            __bis_SR_register(LPM3_bits);       // Enter LPM0
        }
    }
}// End of 1st address
```
  If the receiver answers correctly the answer, the connection is done and if it is a master, we will save the MAC address.

  ![imagen](https://user-images.githubusercontent.com/38794634/54521806-fd35ca80-496b-11e9-97b4-410812b62095.png)





<a name="2.2"/>

  ### 2.2 Possible reasons to unable establish a connection

  1. Be sure that the other device is in slave role

     If the module is in master role it couldn't be possible to establish connection with this module. By default, the module returns  to slave role after send data.

  2. Be sure that you are trying to connect to another HM-10 module

     Theoretically it could be possible to connect with different Bluetooth modules of that HM-10, but it has not been studied in this work and is out of the scope.


<a name="3"/>

## 3. Communication

<a name="3.1"/>

 ### 3.1 How to create your own protocol

In this part we will explain how (and where) to create your own protocol. The main idea is that when we have been established a connection, start a communication between each module.


For this protocol I use __`#`__ to indicate the start of my own data. There is an __`if`__ used to detect this symbol and start the capturing of the data.

> The below code is part of the UART reception interrupt.

```c
      /* Used to detect the owner protocol */
      if(answer[j]=='#')
      {
          answer[0]=answer[j];
          j=0;                  // Start to build the array. At the end j will increase
      }
 ```
Once __`#`__ has been captured and stored in position 0 of the `answer[]` array, the following data will be analysed.

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
                  /* It is a master, so we must save the MAC address */
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

 So, if you want to add some code, I recommend adding it below indicated lines.
 And be sure to reset all variables after ending data streaming.
 ```c
 i=j=k=0;      //Reset
 memset(&answer,0, sizeof answer);     // Reset of the answer variable
 memset(&word_cap,0,sizeof word_cap);  // Reset of the word_cap variable
 __bic_SR_register_on_exit(LPM3_bits); // Exit LPM
 ```

<a name="3.2"/>

### 3.2 Example: "Find the master"

> _To establish a connection the module that need to establish this connection must change his role to master. That doesn't mean that the module it's a master. In other words, we differentiate the module that it's a master because it needs take this role, of the "real master" of the net because we have decided on the code_

Find the master is the "game" used for `config_DISC()` to scan around and discovered an HM-10 master module.

I will use this function to explain how it works "my own protocol".

If you launch `config_DISC()` after a connection has been established sender will send **#?m[MAC]** like "You're a master?", and the MAC of the master sender, all of this with `SEND()`function.
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
The receiver will receive this data, and after the detection in the following lines:

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
  Once the original sender has received the answer, will be analysed in the same way.

 ```c
          if(answer[1]=='!')    // Answer
          {
              match=TRUE;

              if(answer[2]=='M')    // It is a master
              {
                  /* It is a master, so we must save the MAC address */
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
In that case `master_dectected` will change to `TRUE`, and the MAC address will be stored.



<a name="4"/>

## 4. How to add more AT commands

>In order, to add more AT commands or change some existing, I highly recommend follow the same structure that has been created.

In one hand, there is the `AT_Commands.c` file. Here there are all the commands used to configure the module, for example AT+ROLE1.

```c
/*
 * SEND: AT+ROLE1
 * Used to set Role at 1 = master.
 * The pointer global variable is used to save data to word_check[].
 */
int AT_ROLE2(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','O','L','E','1'}; // Set master role.
    byte word[]={'1'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;

}
```

On the other hand, there is the code to capture this answer in `USCI_A0_ISR`.

Continuing with the AT+ROLE1, once we sent this command, the HM-10 device will answer with OK+SET1 if all it's OK.

In the following lines there is the code to capture this answer.

```c
//--Set:---
          /* If word_cap[] is equal to "Set:" we have a coincidence */
    if(word_cap[0]=='S' && word_cap[1]=='e' && word_cap[2]=='t' && word_cap[3]==':')
    {
        /* Just move forward*/
        if(answer[j]==':')
        {
            j++;
            break;
        }

        /* Capturing the parameter
         * In that case all parameters must be 1
         */
        else
        {
            parameter1=answer[j];

            if(parameter1=='1')
            {

                match=TRUE;

                  i=j=k=0;
                  parameter1=0;
                  memset(&answer,0, sizeof answer);
                  memset(&word_cap,0,sizeof word_cap);
                  __bic_SR_register_on_exit(LPM3_bits);
                  break;
            }
            else
            {
                match=FALSE;
            }

           //Reset all parameters
                 i=j=k=0;
                 parameter1=0;
                 memset(&answer,0, sizeof answer);
                 memset(&word_cap,0,sizeof word_cap);
                 __bic_SR_register_on_exit(LPM3_bits);
        }
    }//--End of SET--

```

Finally, in `Functions.c` there are all the functions used.

```c
match=0;                            // Set match = 0

/* Set ROLE=1. See datasheet */
while(match==0)                     // Resend the command if the communication fail
{
n_letters=AT_ROLE2(pointer);        // AT_ROLE command
TA0CCTL0 = CCIE;                    // Start Timer
__bis_SR_register(LPM3_bits);       // Enter LPM0
}
```

As you can see will match = 0 we send the command and enter in sleep mode waiting for the response. If the response doesn't arrive, the timer interrupt it will wake up and resend the command.
