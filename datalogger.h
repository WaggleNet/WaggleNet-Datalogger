#include <Arduino.h>
#include <Wire.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RtcDS1307.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"


#define BTN1 9
#define BTN2 5
#define SD_CS 10

enum StateMachine {IDLE, ARMED, RECORDING, CLEANUP} state = ARMED;
uint32_t lastRecord;
uint16_t interval = 5;
uint32_t recCounter = 0;
uint32_t lastDisplay;

void (*collector)(void);

RtcDS1307<TwoWire> rtc(Wire);
SSD1306AsciiAvrI2c display;
File dataFile;
String filename = "";

void logStart() {
    RtcDateTime now = rtc.GetDateTime();
    dataFile.print(now.Epoch32Time());
}

void logEnd() {
    dataFile.println();
}

#define logItem(X) dataFile.print(','); dataFile.print(X);

void setTimeFromSerial() {
    Serial.setTimeout(5000);
    Serial.println(F("Please enter the Epoch_32 date time"));
    Serial.println(F("You can enter \"date +\%s\" to obtain this"));
    uint32_t epoch = Serial.parseInt();
    if (!epoch) {
        Serial.println(F("You entered a wrong value. Try again!"));
        setTimeFromSerial();
        return;
    }
    RtcDateTime timestamp(0);
    timestamp.InitWithEpoch32Time(epoch);
    rtc.SetDateTime(timestamp);
}

void initRTC () {
    rtc.Begin();
    if (!rtc.GetIsRunning()){
        Serial.println(F("RTC was not actively running, starting now"));
        rtc.SetIsRunning(true);
    }
    rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
}

void initSD () {
    if (!SD.begin(SD_CS)) {
        Serial.println(F("SD Card initialization failed!"));
        display.println(F("NO SD CARD"));
        display.println(F("ME WANTS CARD!!!"));
        display.println(":)");
        while(true);
    }
}

void beginDatalogger() {
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);
    display.begin(&Adafruit128x64, 0x3C);
    display.setFont(System5x7);
    initRTC();
    Serial.println(F("Datalogger start!"));
    display.clear();
    initSD();

    // Check for time reset
    if (!rtc.IsDateTimeValid()) {
        display.println(F("RTC Time Invalid!"));
    }
    display.println(F("BTN1 = reset time"));
    auto cur_time = millis();
    while (!rtc.IsDateTimeValid() || millis() <= cur_time + 3000) {
        if (!digitalRead(BTN1)) {
            setTimeFromSerial();
            break;
        }
    }
    state = IDLE;
}

void setCollector(void(*func)(void)) {
    // Must be called beforehand
    collector = func;
}

void displayTime() {
    char datestring[20];
    RtcDateTime dt = rtc.GetDateTime();
    snprintf_P(datestring, 
            sizeof(datestring),
            PSTR("%02u%02u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Hour(),
            dt.Minute(),
            dt.Second());
    display.println(datestring);
}

void displayIdleScreen() {
    // Writes interval info on the screen
    display.clear();
    display.println(F("Interval:"));
    // display.setTextSize(2);
    display.print(interval);
    display.println(F(" secs"));
    display.println();
    // display.setTextSize(1);
    display.println(F("BTN1 = Change"));
    display.println(F("BTN2 = Record"));
    display.println();
    displayTime();
    lastDisplay = millis();
}

void displayRecordScreen() {
    display.clear();
    display.print(F("FILE: "));
    display.println(filename);
    displayTime();
    display.println();
    display.println(F("> Recording"));
    display.println(F("BTN1 = LCD OFF"));
    display.println(F("BTN2 = STOP"));
    display.print(recCounter);
    display.print(F(" records in"));
    lastDisplay = millis();
}

void changeInterval() {
    switch(interval) {
        case 1: interval = 2; break;
        case 2: interval = 5; break;
        case 5: interval = 10; break;
        case 10: interval = 15; break;
        case 15: interval = 30; break;
        case 30: interval = 60; break;
        case 60: interval = 1; break;
        default: interval = 5;
    }
    displayIdleScreen();
}

void stateIdle() {
    if ((long)(millis() - lastDisplay) >= 1000)
        displayIdleScreen();
    if (!digitalRead(BTN1)) {
        changeInterval();
        delay(100);
    }
    if (!digitalRead(BTN2)) {
        state = ARMED;
    }
}

void stateArmed() {
    display.clear();
    display.println(F("Please wait.."));

    // Settle the filename
    RtcDateTime now = rtc.GetDateTime();
    char fnameSeg[9];
    snprintf_P(fnameSeg, 
        sizeof(fnameSeg),
        PSTR("%02u%02uR"),
        now.Month(),
        now.Day());
    uint16_t counter = 0;
    bool found = false;
    char* fname_ptr;
    while (!found) {
        counter ++;
        filename = fnameSeg;
        filename += counter;
        filename += ".csv";
        fname_ptr = filename.c_str();
        if (!SD.exists(fname_ptr)) found = true;
    }

    // Create the file
    dataFile = SD.open(fname_ptr, FILE_WRITE);

    state = RECORDING;
    recCounter = 0;
}

void stateRecording() {
    if ((long)(millis() - lastDisplay) >= 1000)
        displayRecordScreen();
    if ((long)(millis() - lastRecord) >= interval * 1000) {
        logStart();
        collector();
        logEnd();
        recCounter ++;
        lastRecord = millis();
    }
    if (!digitalRead(BTN2)) state = CLEANUP;
}

void stateCleanup() {
    display.clear();
    display.println(F("PLEASE WAIT..."));
    dataFile.flush();
    dataFile.close();
    delay(200);
    state = IDLE;
}

void loopDatalogger() {
    switch(state) {
        case IDLE: stateIdle(); break;
        case ARMED: stateArmed(); break;
        case RECORDING: stateRecording(); break;
        case CLEANUP: stateCleanup(); break;
    }
}
