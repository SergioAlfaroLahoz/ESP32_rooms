#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

//#define RST_PIN         32  //Reset (Not used)  
#define SS_PIN          18  //Comunicación SPI, selecciona y habilita al esclavo   
#define RF_CLK          19  //Serial Clock, señal de reloj para sincronizar los dispositivos
#define RF_MISO         35  //Master In Slave Out, bits del maestro al esclavo
#define RF_MOSI         4  //Master Out Slave In, bits del esclavo al maestro
#define IRQ_PIN         32 //Interrupción presencia de tarjeta RFID

#define USERLENGTH  4

#define RFIDTAG "RFID"

class RFID{
    private: 
        //Create MFRC522 instance.
        MFRC522 mfrc522; //NULL value in RST pin (not used)
        MFRC522::MIFARE_Key key;
        byte regVal = 0x7F;

        byte user[USERLENGTH];
        byte userAnt[USERLENGTH];

        //Claves validas
        byte bookingValidKey[4] = {0xAB, 0x26, 0xF7, 0x20};
        byte validKey1[4] = {0xAB, 0x26, 0xF7, 0x20};  //Valid key example
        byte validKey2[4] = {0x7B, 0xC4, 0xC6, 0x21};
        byte validKey3[4] = {0xCB, 0x0D, 0x11, 0x21};
        byte validKey4[4] = {0x57, 0x3D, 0x13, 0x3E};
        byte validKey5[4] = {0xA7, 0x95, 0x98, 0x40};
        
    public:
        RFID(){;}
        void init();
        bool readCard();
        byte * getUser() {return user;}
        void activateRec(); //The function sending to the MFRC522 the needed commands to activate the reception
        void clearInt(); //The function to clear the pending interrupt bits after interrupt serving routine
        void printArray(byte *buffer, byte bufferSize); //Helper routine to dump a byte array as hex values to Serial
        void readUID(); //Reads the RFID tag and identify user
        bool isEqualArray(byte* arrayA, byte* arrayB, int length); //Compare two arrays, return true if equals
        bool validKey();
        bool userPresent();
        bool cmpUsers();
        void setUser(byte* newUser);
        void setvalidKey(uint8_t usr);
};

#endif