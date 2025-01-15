//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverSeatDetector(D4);
DigitalIn passengerSeatDetector(D5);
DigitalIn driverSeatbelt(D2);
DigitalIn passengerSeatbelt(D3);
DigitalIn startEngineButton(D6);

DigitalOut engineEnabledLed(LED1);
DigitalOut engineStartedLed(LED2);

DigitalInOut carAlarm(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============
bool greenLedState = OFF;
bool checkDriverSeat = ON;
bool checkEngineReady = ON;
bool checkEngineStart = ON;
bool checkBuzzer = ON;
// int numberOfIncorrectCodes = 0;
// int buttonBeingCompared    = 0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void checkForGreenLed();
void checkStartEngine();

// void alarmActivationUpdate();
// void alarmDeactivationUpdate();

// void uartTask();
// void availableCommands();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        checkForGreenLed();
        checkStartEngine();

        // if (startEngineButton && !(driverSeatbelt && passengerSeatbelt && driverSeatDetector && passengerSeatDetector)){
        //     carAlarm.output();                                     
        //     carAlarm = LOW;  
        //     if (checkBuzzer){
        //         if(!driverSeatbelt){
        //             uartUsb.write( "Driver seatbelt not fastened\r\n\r\n", 32);
        //         }
        //         if(!passengerSeatbelt){
        //             uartUsb.write( "Passenger seatbelt not fastened\r\n\r\n", 35);
        //         }
        //         if(!driverSeatDetector){
        //             uartUsb.write( "Driver seat not occupied\r\n\r\n", 30);
        //         }
        //         if(!passengerSeatDetector){
        //             uartUsb.write( "Passenger seat not occupied\r\n\r\n", 33);
        //         }

        //         checkBuzzer = OFF;
        //     }
        // }

        
        
    }
}

//=====[Implementations of public functions]===================================
void wrongCode()
{
    if (startEngineButton && !(driverSeatbelt && passengerSeatbelt && driverSeatDetector && passengerSeatDetector)){
        carAlarm.output();                                     
        carAlarm = LOW;
    }
}

void inputsInit()
{
    driverSeatDetector.mode(PullDown);
    passengerSeatDetector.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
    startEngineButton.mode(PullDown);
    carAlarm.mode(OpenDrain);
    carAlarm.input();
}

void outputsInit()
{
    engineEnabledLed = OFF;
    engineStartedLed = OFF;
}

void checkForGreenLed(){
    if(driverSeatDetector && !(passengerSeatDetector && driverSeatbelt && passengerSeatbelt)){
        if (checkDriverSeat){
                uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n\r\n", 50);

                checkDriverSeat = OFF;
        }
        greenLedState = OFF;
    }
    

    else if(driverSeatDetector && passengerSeatDetector && driverSeatbelt && passengerSeatbelt && !startEngineButton){
        if (checkEngineReady){
                engineEnabledLed = ON;
                engineStartedLed = OFF;

                checkEngineReady = OFF;
        }
        greenLedState = ON;
    }
        
    
}

void checkStartEngine(){
    if (startEngineButton && greenLedState == ON){
        if (checkEngineStart){
                engineEnabledLed = OFF;
                engineStartedLed = ON;                
                uartUsb.write( "Engine started\r\n\r\n", 20);
                checkEngineStart = OFF;
        }
    }
    else if (startEngineButton && greenLedState == OFF){
        carAlarm.output();                                     
        carAlarm = LOW; 
        if(checkBuzzer){
            if(!driverSeatbelt){
            uartUsb.write( "Driver seatbelt not fastened\r\n\r\n", 32);
            }
            if(!passengerSeatbelt){
            uartUsb.write( "Passenger seatbelt not fastened\r\n\r\n", 35);
            }
            if(!driverSeatDetector){
            uartUsb.write( "Driver seat not occupied\r\n\r\n", 30);
            }
            if(!passengerSeatDetector){
            uartUsb.write( "Passenger seat not occupied\r\n\r\n", 33);
            }

            checkBuzzer = OFF;
        }
        
    }
}
// void alarmActivationUpdate()
// {
//     if ( gasDetector || overTempDetector ) {
//         alarmState = ON;
//     }
//     alarmLed = alarmState;
// }

// void alarmDeactivationUpdate()
// {
//     if ( numberOfIncorrectCodes < 5 ) {
//         if ( aButton && bButton && cButton && dButton && !enterButton ) {
//             incorrectCodeLed = OFF;
//         }
//         if ( enterButton && !incorrectCodeLed && alarmState ) {
//             if ( aButton && bButton && !cButton && !dButton ) {
//                 alarmState = OFF;
//                 numberOfIncorrectCodes = 0;
//             } else {
//                 incorrectCodeLed = ON;
//                 numberOfIncorrectCodes++;
//             }
//         }
//     } else {
//         systemBlockedLed = ON;
//     }
// }

// void uartTask()
// {
//     char receivedChar = '\0';
//     if( uartUsb.readable() ) {
//         uartUsb.read( &receivedChar, 1 );
//         switch (receivedChar) {
//         case '1':
//             if ( alarmState ) {
//                 uartUsb.write( "The alarm is activated\r\n", 24);
//             } else {
//                 uartUsb.write( "The alarm is not activated\r\n", 28);
//             }
//             break;

//         case '2':
//             if ( gasDetector ) {
//                 uartUsb.write( "Gas is being detected\r\n", 22);
//             } else {
//                 uartUsb.write( "Gas is not being detected\r\n", 27);
//             }
//             break;

//         case '3':
//             if ( overTempDetector ) {
//                 uartUsb.write( "Temperature is above the maximum level\r\n", 40);
//             } else {
//                 uartUsb.write( "Temperature is below the maximum level\r\n", 40);
//             }
//             break;
            
//         case '4':
//             uartUsb.write( "Please enter the code sequence.\r\n", 33 );
//             uartUsb.write( "First enter 'A', then 'B', then 'C', and ", 41 ); 
//             uartUsb.write( "finally 'D' button\r\n", 20 );
//             uartUsb.write( "In each case type 1 for pressed or 0 for ", 41 );
//             uartUsb.write( "not pressed\r\n", 13 );
//             uartUsb.write( "For example, for 'A' = pressed, ", 32 );
//             uartUsb.write( "'B' = pressed, 'C' = not pressed, ", 34);
//             uartUsb.write( "'D' = not pressed, enter '1', then '1', ", 40 );
//             uartUsb.write( "then '0', and finally '0'\r\n\r\n", 29 );

//             incorrectCode = false;

//             for ( buttonBeingCompared = 0; 
//                   buttonBeingCompared < NUMBER_OF_KEYS; 
//                   buttonBeingCompared++) {

//                 uartUsb.read( &receivedChar, 1 );
//                 uartUsb.write( "*", 1 );

//                 if ( receivedChar == '1' ) {
//                     if ( codeSequence[buttonBeingCompared] != 1 ) {
//                         incorrectCode = true;
//                     }
//                 } else if ( receivedChar == '0' ) {
//                     if ( codeSequence[buttonBeingCompared] != 0 ) {
//                         incorrectCode = true;
//                     }
//                 } else {
//                     incorrectCode = true;
//                 }
//             }

//             if ( incorrectCode == false ) {
//                 uartUsb.write( "\r\nThe code is correct\r\n\r\n", 25 );
//                 alarmState = OFF;
//                 incorrectCodeLed = OFF;
//                 numberOfIncorrectCodes = 0;
//             } else {
//                 uartUsb.write( "\r\nThe code is incorrect\r\n\r\n", 27 );
//                 incorrectCodeLed = ON;
//                 numberOfIncorrectCodes++;
//             }                
//             break;

//         default:
//             availableCommands();
//             break;

//         }
//     }
// }

// void availableCommands()
// {
//     uartUsb.write( "Available commands:\r\n", 21 );
//     uartUsb.write( "Press '1' to get the alarm state\r\n", 34 );
//     uartUsb.write( "Press '2' to get the gas detector state\r\n", 41 );
//     uartUsb.write( "Press '3' to get the over temperature detector state\r\n", 54 );
//     uartUsb.write( "Press '4' to enter the code sequence\r\n\r\n", 40 );
// }
