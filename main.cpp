//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#define NUM_SAMPLES 10
#define DAY_THRESHOLD 0.26
#define NIGHT_THRESHOLD 0.45

//=====[Global Objects]========================================================

DigitalIn isDriverSeatOccupied(D7);        // Detect if the driver seat is occupied
DigitalIn isPassengerSeatOccupied(D6);     // Detect if the passenger seat is occupied
DigitalIn isDriverSeatbeltFastened(D5);    // Detect if the driver seatbelt is fastened
DigitalIn isPassengerSeatbeltFastened(D4); // Detect if the passenger seatbelt is fastened
DigitalIn isStartEngineButtonPressed(BUTTON1);  // Detect if the engine start button is pressed

DigitalOut engineReadyLed(LED1);           // LED to indicate the engine is ready to start
DigitalOut engineRunningLed(LED2);         // LED to indicate the engine is running
DigitalOut leftLowLamp(D1);
DigitalOut rightLowLamp(D2);

DigitalInOut carAlarmSignal(PE_10);        // Alarm system to notify incorrect conditions

UnbufferedSerial uartUsb(USBTX, USBRX, 115200); // USB serial communication

AnalogIn potentiometer(A0);
AnalogIn lightSensor(A2);

//=====[Global Variables]======================================================

bool isEngineReady = false;                 // Tracks if the engine is ready (green LED on)
bool hasDriverBeenGreeted = true;           // Tracks if the driver has been greeted
bool isEngineReadyCheckPending = true;      // Tracks if engine readiness needs to be checked
bool isEngineStartCheckPending = true;      // Tracks if the engine start conditions need to be checked
bool isAlarmCheckPending = true;            // Tracks if alarm conditions need to be checked
bool systemFailAndRestart= true;            // Tracks if the system is failing and restart

float potRead = 0.0;
int strlength = 0;
char str[100];

static float lightReadings[NUM_SAMPLES]={0};
static int index=0;



//=====[Function Prototypes]===================================================
void initializeInputPins();                 
void initializeOutputPins();               
void updateEngineReadyState();              // Update the state of the engine readiness
void handleEngineStartConditions();         // Check and manage engine start conditions
void activateCarAlarm();                    // Trigger the car alarm with a fixed message
void turnOffEngine();                       // Check to turn off the engine
void chooseLightOption();                   // User choose the light option
void whileEngineRunning();                  // While the engine is running, check if the user wants to turn off the engine, or choose a light option
void lightSensorInit();                 
float lightSensorRead();
void autoLight();                           // Auto light option

//=====[Main Function]=========================================================

int main()
{
    isEngineReady = false;                 // Tracks if the engine is ready (green LED on)
    hasDriverBeenGreeted = true;           // Tracks if the driver has been greeted
    isEngineReadyCheckPending = true;      // Tracks if engine readiness needs to be checked
    isEngineStartCheckPending = true;      // Tracks if the engine start conditions need to be checked
    isAlarmCheckPending = true;            // Tracks if alarm conditions need to be checked
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
    leftLowLamp = OFF;
    rightLowLamp = OFF;
}

void updateEngineReadyState()
{
    // Engine not ready: Driver seat occupied but other conditions not met
    if (isDriverSeatOccupied && !(isPassengerSeatOccupied && isDriverSeatbeltFastened && isPassengerSeatbeltFastened) && check) {
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
            delay(300);
            whileEngineRunning();
        }
    }
    // Trigger alarm if button is pressed but engine is not ready
    else if (isStartEngineButtonPressed && !isEngineReady) {
        carAlarmSignal.output();
        carAlarmSignal = LOW; 

        if (isAlarmCheckPending && check) {
            // Check and report unmet conditions
            if (!isDriverSeatbeltFastened) {
                uartUsb.write("Driver seatbelt not fastened\r\n\r\n", 32);
            }
            if (!isPassengerSeatbeltFastened) {
                uartUsb.write("Passenger seatbelt not fastened\r\n\r\n", 35);
            }
            if (!isDriverSeatOccupied) {
                uartUsb.write("Driver seat not occupied\r\n\r\n", 28);
            }
            if (!isPassengerSeatOccupied) {
                uartUsb.write("Passenger seat not occupied\r\n\r\n", 31);
            }
            isAlarmCheckPending = false;
            systemFailAndRestart= false;
            main(); //call the main() fundtion again to restart the 
        }
    }
}

void whileEngineRunning() {
    turnOffEngine();
    chooseLightOption();
    whileEngineRunning();
}

void turnOffEngine(){
    if (engineRunningLed && isStartEngineButtonPressed){
        delay(100);
        if(!isStartEngineButtonPressed) {
            systemFailAndRestart= true;
            uartUsb.write("Engine stopped \r\n\r\n", 19);
            delay(300);
            main();
        }
    }
}

void chooseLightOption() {
    potRead = potentiometer.read();

    if (potRead < 0.2) {
        leftLowLamp = OFF;
        rightLowLamp = OFF;
    }
    else if (potRead > 0.8) {
        leftLowLamp = ON;
        rightLowLamp = ON;
    }
    else {
        autoLight();
    }
}

void lightSensorInit() {
    for (int i=0; i < NUM_SAMPLES; i++) {
        lightReadings[i]=0;
    }
}

float lightSensorRead() {
    // Take a new sample
    lightReadings[index] = lightSensor.read(); // Read value (0.0 - 1.0)
    index=(index + 1) % NUM_SAMPLES;

    float sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++){
        sum += lightReadings[i];
    }
    return sum/NUM_SAMPLES;
}

void autoLight() {
    float lightLevel = lightSensorRead();

    if (lightLevel > NIGHT_THRESHOLD) {
        if (!(leftLowLamp && rightLowLamp)) {
            delay(2000);
        }
        leftLowLamp = 1;
        rightLowLamp = 1; 

    } else if (lightLevel < DAY_THRESHOLD) {
        if (leftLowLamp && rightLowLamp) {
            delay(1000);
        }
        leftLowLamp = 0;
        rightLowLamp = 0;
    }
}


