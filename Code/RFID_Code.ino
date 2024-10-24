#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

int lock = 6;     // Pin for the lock
int serrure = 3;  // Pin for another lock or motor

char st[20];      // Buffer for card's serial number

void setup() {
    pinMode(lock, OUTPUT);
    pinMode(serrure, OUTPUT);

    Serial.begin(9600);
    SPI.begin();            // Initialize SPI bus
    mfrc522.PCD_Init();     // Initialize MFRC522

    Serial.println("Access test via RFID tag");
    Serial.println();
}

void loop() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Dump UID
    Serial.print("Tag :");
    String tag = "";
    byte caractere;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    tag.toUpperCase();

    // Check if the card matches a known tag
    if (tag.substring(1) == "33 B1 2E 98") {
        digitalWrite(lock, HIGH);
        Serial.println("TAG checked - Access allowed!");
        delay(3000);
        digitalWrite(lock, LOW);
        digitalWrite(serrure, LOW);
    } 
    else if (tag.substring(1) == "21 D6 10 1A") {
        digitalWrite(lock, HIGH);
        Serial.println("TAG checked - Access allowed!");
        delay(3000);
        digitalWrite(lock, LOW);
        digitalWrite(serrure, LOW);
    } 
    else {
        Serial.println("Unknown tag - Access refused!");
        for (int i = 1; i < 5; i++) {
            digitalWrite(lock, HIGH);
            delay(200);
            digitalWrite(lock, LOW);
            delay(200);
        }
    }

    delay(1000);
}
