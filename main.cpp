//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Global Objects]========================================================

DigitalIn isDriverSeatOccupied(D4);        // Detect if the driver seat is occupied
DigitalIn isPassengerSeatOccupied(D5);     // Detect if the passenger seat is occupied
DigitalIn isDriverSeatbeltFastened(D2);    // Detect if the driver seatbelt is fastened
DigitalIn isPassengerSeatbeltFastened(D3); // Detect if the passenger seatbelt is fastened
DigitalIn isStartEngineButtonPressed(D6);  // Detect if the engine start button is pressed

DigitalOut engineReadyLed(LED1);           // LED to indicate the engine is ready to start
DigitalOut engineRunningLed(LED2);         // LED to indicate the engine is running

DigitalInOut carAlarmSignal(PE_10);        // Alarm system to notify incorrect conditions

UnbufferedSerial uartUsb(USBTX, USBRX, 115200); // USB serial communication

//=====[Global Variables]======================================================

bool isEngineReady = false;                 // Tracks if the engine is ready (green LED on)
bool hasDriverBeenGreeted = true;           // Tracks if the driver has been greeted
bool isEngineReadyCheckPending = true;      // Tracks if engine readiness needs to be checked
bool isEngineStartCheckPending = true;      // Tracks if engine start conditions need to be checked
bool isAlarmCheckPending = true;            // Tracks if alarm conditions need to be checked

//=====[Function Prototypes]===================================================
void initializeInputPins();                 
void initializeOutputPins();               
void updateEngineReadyState();              // Update the state of the engine readiness
void handleEngineStartConditions();         // Check and manage engine start conditions
void activateCarAlarm();                    // Trigger the car alarm with a fixed message

//=====[Main Function]=========================================================

int main()
{
    initializeInputPins();  
    initializeOutputPins(); 

    while (true) {
        updateEngineReadyState();     
        handleEngineStartConditions(); 
    }
}

//=====[Function Implementations]==============================================

void initializeInputPins()
{
    // Configure input pins with pull-down resistors
    isDriverSeatOccupied.mode(PullDown);
    isPassengerSeatOccupied.mode(PullDown);
    isDriverSeatbeltFastened.mode(PullDown);
    isPassengerSeatbeltFastened.mode(PullDown);
    isStartEngineButtonPressed.mode(PullDown);

    // Configure car alarm pin as open-drain input by default
    carAlarmSignal.mode(OpenDrain);
    carAlarmSignal.input();
}

void initializeOutputPins()
{
    // Set initial states of LEDs
    engineReadyLed = OFF;
    engineRunningLed = OFF;
}

void updateEngineReadyState()
{
    // Engine not ready: Driver seat occupied but other conditions not met
    if (isDriverSeatOccupied && !(isPassengerSeatOccupied && isDriverSeatbeltFastened && isPassengerSeatbeltFastened)) {
        if (hasDriverBeenGreeted) {
            // Display a welcome message once
            uartUsb.write("Welcome to enhanced alarm system model 218-W24\r\n\r\n", 50);
            hasDriverBeenGreeted = false;
        }
        isEngineReady = false;
    }
    // Engine ready: All conditions for readiness are met
    else if (isDriverSeatOccupied && isPassengerSeatOccupied && isDriverSeatbeltFastened && isPassengerSeatbeltFastened && !isStartEngineButtonPressed) {
        if (isEngineReadyCheckPending) {
            // Turn on engine ready LED and reset running LED
            engineReadyLed = ON;
            engineRunningLed = OFF;
            isEngineReadyCheckPending = false;
        }
        isEngineReady = true;
    }
}

void handleEngineStartConditions()
{
    // Start engine if button is pressed and engine is ready
    if (isStartEngineButtonPressed && isEngineReady) {
        if (isEngineStartCheckPending) {
            // Update LED states and notify via UART
            engineReadyLed = OFF;
            engineRunningLed = ON;
            uartUsb.write("Engine started\r\n\r\n", 20);
            isEngineStartCheckPending = false;
        }
    }
    // Trigger alarm if button is pressed but engine is not ready
    else if (isStartEngineButtonPressed && !isEngineReady) {
        carAlarmSignal.output();
        carAlarmSignal = LOW; 

        if (isAlarmCheckPending) {
            // Check and report unmet conditions
            if (!isDriverSeatbeltFastened) {
                uartUsb.write("Driver seatbelt not fastened\r\n\r\n", 32);
            }
            if (!isPassengerSeatbeltFastened) {
                uartUsb.write("Passenger seatbelt not fastened\r\n\r\n", 35);
            }
            if (!isDriverSeatOccupied) {
                uartUsb.write("Driver seat not occupied\r\n\r\n", 30);
            }
            if (!isPassengerSeatOccupied) {
                uartUsb.write("Passenger seat not occupied\r\n\r\n", 33);
            }
            isAlarmCheckPending = false;
        }
    }
}
