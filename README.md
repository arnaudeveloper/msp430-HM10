# msp430-HM10
Basic library for msp430F5529LP and HM-10 Bluetooth module
![imagen](https://user-images.githubusercontent.com/38794634/54521270-de830400-496a-11e9-90ee-7a19d0b0e352.png)
![imagen](https://user-images.githubusercontent.com/38794634/54521841-15a5e500-496c-11e9-94d9-9d324cdc18e1.png)

## Table of Contents
1. [Introduction](#1)

2. [How to establish connection between two or more dispositives](#2)

      2.1 [Possibles reasons to unable establish a connection](#2.1)

3. [How to create your own protocol](#3)

      3.1 [Find the master](#3.1)

4. [How to add more AT commands](#3)

<a name="1"/>

## 1. Introduction
In this repository you will find the code to discover other HM-10 modules, connect to them, send and recive data, and cut off the communication to return initial configuration.

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

The TX code it's tedious but easy, you only must to pick the correct letters and send via UART. The tedious part is because the AT commands format, that it isn't coherent and tricky to use in c.

On the other hand RX has been the most difficult part for the same reason of the AT commands format.Therefore, almost all code is "bare metal".

<a name="2"/>

## 2. How to establish connection between two or more dispositives

> _The connection always will be point-to-point, so you could connect to every module, but not at the same time or send a broadcast meassage._

To establish a connection first you need to configure the module in master rol to start a communication with other modules.
For this objective you can use  `config_DISC()`.
This function works in the folowing way:
1. Initializing the HM-10 module as master

  Setting AT+IMME1 and AT+ROLE1.

2. Discovering the MAC addresses around of Bluetooth modules

  With `AT+DISC?` command the microcontroler can discover up to 3 dispositves*

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
(*)As you can see in the image you only need to add more else if, and others global variables.

After launch `AT+DISC?` command you will see the captured MAC address in the following global varaibles

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

4. Succesful connection

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
  If the the reciver answer correctly the answer, the connection is done and if it is a master we will save the MAC address.

  ![imagen](https://user-images.githubusercontent.com/38794634/54521806-fd35ca80-496b-11e9-97b4-410812b62095.png)





<a name="2.1"/>

  ### 2.1 Possibles reasons to unable establish a connection

  1. Be sure that the other device are in slave role

     If the module is in master role it couldn't be possible to estalish connection with this module. By default the module return to slave role after send data.

  2. Be sure that you are trying to connect to another HM-10 module

     Theoretically it could be possible to connect with differents Bluetooth modules of that HM-10, but it has not been studied in this work and is out of the scope.


<a name="3"/>

 ## 3. How to create your own protocol

In this part we will explain how (and where) to create your own protocol. The main idea is that when we have been established a connection, start a communication between each module.


For this protocol I use __`#`__ to indicate the start of the my own data. There is a __`if`__ used to detect this symbol and start the capturing of the data.

> The below code is part of the UART reception interrupt.

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

 So, if you want to add some code, I recommend adding it below indicated lines.
 And be sure to reset all variables after ending data streming.
 ```c
 i=j=k=0;      //Reset
 memset(&answer,0, sizeof answer);     // Reset of the answer variable
 memset(&word_cap,0,sizeof word_cap);  // Reset of the word_cap variable
 __bic_SR_register_on_exit(LPM3_bits); // Exit LPM
 ```

<a name="3.1"/>

### 3.1 Find the master

> _To establish a connection the module that need to establish this connection must change his role to master. That doesn't mean that the module it's a master. In other words, we differentiate the module that it's a master because it needs take this role, of the "real master" of the net because we have decided on the code_

Find the master is the "game" used for `config_DISC()` to scan around and discoverd an HM-10 master module.

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
In that case case `master_dectected` will change to `TRUE`, and the MAC addres will be stored.



<a name="4"/>

## 4. How to add more AT commands
