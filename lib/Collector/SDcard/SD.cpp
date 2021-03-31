#include <SD.h>
#include <SPI.h>

File myFile;

int pinCS;

void setup(){   //Only want to run one so no need for loop
    Serial.begin(9600);
    pinMode(pinCS, OUTPUT);

    //SD Card Initialization
    if(SD.begin())
    {
        Serial.println("SD card is ready to use");
    }
    else
    {
    Serial.println("SD card intialization has failed :( ");
    return;
    }


//Creating an Opening a File
 myFile = SD.open("text.txt", FILE_WRITE);

//Testing if the file opened correctly

if(myFile) {
    Serial.println("Writing to file...");
    //Write to file
    myFile.println("Testing text 1,2,3...");
    myFile.close();
    Serial.println("Done!");
}
//If file didn't open
else {
    Serial.println("error opening test.txt");
}

}




