// #include <Arduino.h>
// #include <SPI.h>

// #include "Handlers/RTOSHandler.h"

// void setup() {
//   Serial.begin(115200);

//   init();

// }

// void loop() {


// }




/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */

// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN         9          // Configurable, see typical pin layout above
// #define SS_PIN          5         // Configurable, see typical pin layout above

// MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// void setup() {
// 	Serial.begin(115200);		// Initialize serial communications with the PC
// 	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
// 	SPI.begin();			// Init SPI bus
// 	mfrc522.PCD_Init();		// Init MFRC522
// 	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
// 	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
// 	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
// }

// void loop() {
// 	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
// 	if ( ! mfrc522.PICC_IsNewCardPresent()) {
// 		return;
// 	}

// 	// Select one of the cards
// 	if ( ! mfrc522.PICC_ReadCardSerial()) {
// 		return;
// 	}

// 	// Dump debug info about the card; PICC_HaltA() is automatically called
// 	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
// }


/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */

// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 5
// #define RST_PIN 9
 
// MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

// MFRC522::MIFARE_Key key; 

// // Init array that will store new NUID 
// byte nuidPICC[4];

// /**
//  * Helper routine to dump a byte array as hex values to Serial. 
//  */
// void printHex(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], HEX);
//   }
// }

// /**
//  * Helper routine to dump a byte array as dec values to Serial.
//  */
// void printDec(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(' ');
//     Serial.print(buffer[i], DEC);
//   }
// }

// void setup() { 
//   Serial.begin(115200);
//   SPI.begin(); // Init SPI bus
//   rfid.PCD_Init(); // Init MFRC522 

//   for (byte i = 0; i < 6; i++) {
//     key.keyByte[i] = 0xFF;
//   }

//   Serial.println(F("This code scan the MIFARE Classsic NUID."));
//   Serial.print(F("Using the following key:"));
//   printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
// }
 
// void loop() {

//   // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
//   if ( ! rfid.PICC_IsNewCardPresent())
//     return;

//   // Verify if the NUID has been readed
//   if ( ! rfid.PICC_ReadCardSerial())
//     return;

//   Serial.print(F("PICC type: "));
//   MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//   Serial.println(rfid.PICC_GetTypeName(piccType));

//   // Check is the PICC of Classic MIFARE type
//   if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
//     piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
//     piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//     Serial.println(F("Your tag is not of type MIFARE Classic."));
//     return;
//   }

//   if (rfid.uid.uidByte[0] != nuidPICC[0] || 
//     rfid.uid.uidByte[1] != nuidPICC[1] || 
//     rfid.uid.uidByte[2] != nuidPICC[2] || 
//     rfid.uid.uidByte[3] != nuidPICC[3] ) {
//     Serial.println(F("A new card has been detected."));

//     // Store NUID into nuidPICC array
//     for (byte i = 0; i < 4; i++) {
//       nuidPICC[i] = rfid.uid.uidByte[i];
//     }
   
//     Serial.println(F("The NUID tag is:"));
//     Serial.print(F("In hex: "));
//     printHex(rfid.uid.uidByte, rfid.uid.size);
//     Serial.println();
//     Serial.print(F("In dec: "));
//     printDec(rfid.uid.uidByte, rfid.uid.size);
//     Serial.println();
//   }
//   else Serial.println(F("Card read previously."));

//   // Halt PICC
//   rfid.PICC_HaltA();

//   // Stop encryption on PCD
//   rfid.PCD_StopCrypto1();
// }





// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 5
// #define RST_PIN 9

// MFRC522 mfrc522(SS_PIN, RST_PIN);
// MFRC522::MIFARE_Key key;

// int block = 2;
// byte blockcontent[16] = {0}; // Initialize with zeros

// void clearBlock(int blockNumber) {
//  // Authentication of the desired block for access
//  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(mfrc522.uid));

//  if (status != MFRC522::STATUS_OK) {
//     Serial.print("PCD_Authenticate() failed: ");
//     Serial.println(mfrc522.GetStatusCodeName(status));
//     return;
//  }

//  // Clearing the block
//  status = mfrc522.MIFARE_Write(blockNumber, blockcontent, 16);

//  if (status != MFRC522::STATUS_OK) {
//     Serial.print("MIFARE_Write() failed: ");
//     Serial.println(mfrc522.GetStatusCodeName(status));
//     return;
//  }

//  Serial.println("Block " + (String)blockNumber + " was cleared");
// }


// void setup() {
//  Serial.begin(115200);
//  SPI.begin();
//  mfrc522.PCD_Init();

//  for (byte i = 0; i < 6; i++) {
//     key.keyByte[i] = 0xFF;
//  }
// }

// void loop() {
//  if (!mfrc522.PICC_IsNewCardPresent()) {
//     return;
//  }

//  if (!mfrc522.PICC_ReadCardSerial()) {
//     return;
//  }

// //  Serial.println("Card selected");
//  clearBlock(block);
// //  clearBlock(1);
// //  clearBlock(0);
// //  for (block = 1; block < 32; block++){
// //   clearBlock(block);
// //   delay(2000);
// //  }
// }



// #include <SPI.h>
// #include <MFRC522.h>

// constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
// constexpr uint8_t SS_PIN = 5;     // Configurable, see typical pin layout above

// MFRC522 rfid(SS_PIN, RST_PIN);     // Create MFRC522 instance

// void setup() {
//   Serial.begin(115200);        // Initialize serial communications with the PC
//   while (!Serial);           // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
//   SPI.begin();               // Init SPI bus
//   rfid.PCD_Init();           // Init MFRC522
// }

// void loop() {
//   // Look for new cards
//   if ( ! rfid.PICC_IsNewCardPresent())
//     return;

//   // Verify if the NUID has been readed
//   if ( ! rfid.PICC_ReadCardSerial())
//     return;

//   MFRC522::MIFARE_Key key;
//   for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

//   byte block;
//   byte len;
//   MFRC522::StatusCode status;

//   len = 32;
//   block = 10;
//   byte buffer1[len];
//   byte buffer2[len];

//   // Authenticate using key A
//   status = (MFRC522::StatusCode) rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid));

//   if (status != MFRC522::STATUS_OK) {
//     Serial.print(F("Authentication failed: "));
//     Serial.println(rfid.GetStatusCodeName(status));
//     return;
//   }

//   // Read block
//   status = (MFRC522::StatusCode) rfid.MIFARE_Read(block, buffer1, &len);

//   if (status != MFRC522::STATUS_OK) {
//     Serial.print(F("Reading failed: "));
//     Serial.println(rfid.GetStatusCodeName(status));
//     return;
//   }

//   // Write empty data to block
//   for (byte i = 0; i < 16; i++) buffer1[i] = 0;

//   status = (MFRC522::StatusCode) rfid.MIFARE_Write(block, buffer1, 16);

//   if (status != MFRC522::STATUS_OK) {
//     Serial.print(F("Writing failed: "));
//     Serial.println(rfid.GetStatusCodeName(status));
//     return;
//   }

//   // Halt PICC
//   rfid.PICC_HaltA();
//   // Stop encryption on PCD
//   rfid.PCD_StopCrypto1();
// }



/**
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 *
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 *
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * This sample shows how to read and write data blocks on a MIFARE Classic PICC
 * (= card/tag).
 *
 * BEWARE: Data will be written to the PICC, in sector #1 (blocks #4 to #7).
 *
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 *
 */

// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN         9           // Configurable, see typical pin layout above
// #define SS_PIN          5          // Configurable, see typical pin layout above

// MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// MFRC522::MIFARE_Key key;

// /**
//  * Helper routine to dump a byte array as hex values to Serial.
//  */
// void dump_byte_array(byte *buffer, byte bufferSize) {
//     for (byte i = 0; i < bufferSize; i++) {
//         Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//         Serial.print(buffer[i], HEX);
//     }
// }


// /**
//  * Initialize.
//  */
// void setup() {
//     Serial.begin(115200); // Initialize serial communications with the PC
//     while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
//     SPI.begin();        // Init SPI bus
//     mfrc522.PCD_Init(); // Init MFRC522 card

//     // Prepare the key (used both as key A and as key B)
//     // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
//     for (byte i = 0; i < 6; i++) {
//         key.keyByte[i] = 0;
//     }

//     Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
//     Serial.print(F("Using key (for A and B):"));
//     dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
//     Serial.println();

//     Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
// }

// /**
//  * Main loop.
//  */
// void loop() {
//     // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
//     if ( ! mfrc522.PICC_IsNewCardPresent())
//         return;

//     // Select one of the cards
//     if ( ! mfrc522.PICC_ReadCardSerial())
//         return;

//     // Show some details of the PICC (that is: the tag/card)
//     Serial.print(F("Card UID:"));
//     dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
//     Serial.println();
//     Serial.print(F("PICC type: "));
//     MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
//     Serial.println(mfrc522.PICC_GetTypeName(piccType));

//     // Check for compatibility
//     if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
//         &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
//         &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//         Serial.println(F("This sample only works with MIFARE Classic cards."));
//         return;
//     }

//     // In this sample we use the second sector,
//     // that is: sector #1, covering block #4 up to and including block #7
//     byte sector         = 1;
//     byte blockAddr      = 3;
//     byte dataBlock[]    = {
//         0x00, 0x00, 0x00, 0x00, //  1,  2,   3,  4,
//         0x00, 0x00, 0xFF, 0x07, //  5,  6,   7,  8,
//         0x80, 0x69, 0xFF, 0xFF, //  9, 10, 255, 11,
//         0xFF, 0xFF, 0xFF, 0xFF  // 12, 13, 14, 15
//     };
//     // byte trailerBlock   = 7;
//     MFRC522::StatusCode status;
//     byte buffer[18];
//     byte size = sizeof(buffer);

//     // Authenticate using key A
//     Serial.println(F("Authenticating using key A..."));
//     status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid));
//     if (status != MFRC522::STATUS_OK) {
//         Serial.print(F("PCD_Authenticate() failed: "));
//         Serial.println(mfrc522.GetStatusCodeName(status));
//         return;
//     }

    // // Show the whole sector as it currently is
    // Serial.println(F("Current data in sector:"));
    // mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    // Serial.println();

    // // Read data from the block
    // Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    // Serial.println(F(" ..."));
    // status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    // if (status != MFRC522::STATUS_OK) {
    //     Serial.print(F("MIFARE_Read() failed: "));
    //     Serial.println(mfrc522.GetStatusCodeName(status));
    // }
    // Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    // dump_byte_array(buffer, 16); Serial.println();
    // Serial.println();

    // // Authenticate using key B
    // Serial.println(F("Authenticating again using key B..."));
    // status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    // if (status != MFRC522::STATUS_OK) {
    //     Serial.print(F("PCD_Authenticate() failed: "));
    //     Serial.println(mfrc522.GetStatusCodeName(status));
    //     return;
    // }

    // Write data to the block
    // Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    // Serial.println(F(" ..."));
    // dump_byte_array(dataBlock, 16); Serial.println();
    // status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    // if (status != MFRC522::STATUS_OK) {
    //     Serial.print(F("MIFARE_Write() failed: "));
    //     Serial.println(mfrc522.GetStatusCodeName(status));
    // }
    // Serial.println();

    // // Read data from the block (again, should now be what we have written)
    // Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    // Serial.println(F(" ..."));
    // status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    // if (status != MFRC522::STATUS_OK) {
    //     Serial.print(F("MIFARE_Read() failed: "));
    //     Serial.println(mfrc522.GetStatusCodeName(status));
    // }
    // Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    // dump_byte_array(buffer, 16); Serial.println();

    // Check that data in block is what we have written
    // by counting the number of bytes that are equal
    // Serial.println(F("Checking result..."));
    // byte count = 0;
    // for (byte i = 0; i < 16; i++) {
    //     // Compare buffer (= what we've read) with dataBlock (= what we've written)
    //     if (buffer[i] == dataBlock[i])
    //         count++;
    // }
    // Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    // if (count == 16) {
    //     Serial.println(F("Success :-)"));
    // } else {
    //     Serial.println(F("Failure, no match :-("));
    //     Serial.println(F("  perhaps the write didn't work properly..."));
    // }
    // Serial.println();

    // // Dump the sector data
    // Serial.println(F("Current data in sector:"));
    // mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    // Serial.println();

    // Halt PICC
    // mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    // mfrc522.PCD_StopCrypto1();
// }


#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 15
MFRC522 rfid(SS_PIN, RST_PIN); 

void setup() {
 Serial.begin(115200);
 SPI.begin();        // Init SPI bus
 rfid.PCD_Init();   // Init MFRC522
}

void loop() {
 // Look for new cards
 if ( ! rfid.PICC_IsNewCardPresent())
    return;

 // Verify if the NUID has been readed
 if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::MIFARE_Key key;

 byte blockNumber = 16; // Change this to the block where you want to write the data
 byte keyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Default key
 byte keyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Default key

  for (int i = 0; i < 6; i++) key.keyByte[i] = keyA[i];

 char asciiData[] = "9812762241";
 byte hexData[strlen(asciiData)]; // Each byte will be represented by two hexadecimal digits
 for(int i=0; i<strlen(asciiData); i++) {
    // sprintf(&hexData[i], "%02x", asciiData[i]);
    hexData[i] = static_cast<byte>(asciiData[i]);
 }


 // Authenticate using key A
 MFRC522::StatusCode status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(rfid.uid));
 if (status != MFRC522::STATUS_OK) {
    Serial.println("Authentication failed!");
    return;
 }

 // Write the HEX data to the block
 byte buffer[16] = {0};

 for (int i = 0; i < strlen(asciiData); i++)
  buffer[i] = hexData[i];
 
//  strncpy((char*)buffer, hexData, sizeof(buffer)-1);
 status = rfid.MIFARE_Write(blockNumber, buffer, 16);
 if (status != MFRC522::STATUS_OK) {
    Serial.println("Write failed!");
    return;
 }

 Serial.println("Write successful");
}
// void loop() {
//   // Look for new cards
//   if ( ! rfid.PICC_IsNewCardPresent())
//     return;

//   // Verify if the NUID has been readed
//   if ( ! rfid.PICC_ReadCardSerial())
//     return;

//   MFRC522::MIFARE_Key key;

//   byte blockNumber = 16; // The block you have written your data
//   byte keyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Default key

//   for (int i = 0; i < 6; i++) key.keyByte[i] = keyA[i];

//   // Authenticate using key A
//   MFRC522::StatusCode status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(rfid.uid));
//   if (status != MFRC522::STATUS_OK) {
//     Serial.println("Authentication failed!");
//     return;
//   }

//   // Read the data from the block
//   byte buffer[18] = {0};
//   byte size = sizeof(buffer);
//   status = rfid.MIFARE_Read(blockNumber, buffer, &size);
//   if (status != MFRC522::STATUS_OK) {
//     Serial.println("Read failed!");
//     return;
//   }

//   // Convert the data from HEX to ASCII
//   char asciiData[18] = {0};
//   for(int i=0; i<sizeof(buffer); i++) {
//     sprintf(&asciiData[i], "%c", buffer[i]);
//   }

//   Serial.println("Read successful");
//   Serial.println("Data in ASCII:");
//   Serial.println(asciiData);
// }

// // // /