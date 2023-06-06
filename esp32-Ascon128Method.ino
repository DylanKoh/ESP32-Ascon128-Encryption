#include <CryptoLW.h>
#include <Crypto.h>
#include <Ascon128.h>
#include <string.h>

Ascon128 ascon; //Initialises Ascon module
char key[] = "thisisakeytoende"; //Initialises Key
uint8_t iv[] = { 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 }; //Initialises IV
uint8_t authdata[] = { 100,101,102,103,104,105,106,107 };
uint8_t tag[] = { 117,201,124,241,110,249,174,225,125,83,102,152,229,245,247,233 };
uint8_t cipherdata[16]; //Declare outside scope so that all methods can see, change size accordingly
void setup()
{
    Serial.begin(115200);
    delay(200);
    
    
    String plainString  = "This is a string";
    int sizeOfString = plainString.length();

    
    Serial.println("The plaintext: ");
    Serial.println(plainString);
  
    Serial.println();Serial.println();

    encryptData(plainString); //Encrypts the string into a Byte Array for transmission, nothing else needs to be done
    //Use the cipherdata created globally to send out for transmission
    
    Serial.println("The Cipher text in string:");
    for(int i=0;i<sizeof(cipherdata);i++){
      Serial.print(cipherdata[i]);
      Serial.print(",");
    }
    Serial.println();Serial.println();

    String decryptedText = decryptData(cipherdata, sizeof(cipherdata)); //Decrypts the Byte Array to a String format
    Serial.println("The decrypted text in string:");
    Serial.println(decryptedText);
    
    
    Serial.println();Serial.println();

    if(ascon.checkTag(tag,sizeof(tag))==true){
        Serial.println("Tag check was successful");
    }
    else{
        Serial.println("Tag check was unsuccessful");   
    }
    ascon.clear();
}

void encryptData (String plainText){
    //ascon.clear();
    unsigned long StartTime = micros();
    int sizeOfString = plainText.length();
    char plainStringArray[32];
    plainText.toCharArray(plainStringArray,sizeOfString+1);    
    ascon.setKey((uint8_t *)key,sizeof(key)); 
    ascon.setIV(iv,sizeof(iv));
    ascon.addAuthData(authdata,sizeof(authdata));
    ascon.encrypt(cipherdata,(uint8_t *)plainStringArray,sizeOfString);
    ascon.computeTag(tag,sizeof(tag));
    unsigned long CurrentTime = micros();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial.print("Time taken for Encryption: ");
    Serial.print(ElapsedTime);
    Serial.print("us");
    Serial.println();
    //ascon.clear();
  
}

String decryptData (uint8_t *cipherData, int sizeOfArray){
    //ascon.clear();
    unsigned long StartTime = micros();
    uint8_t buffer1[32];
    ascon.setKey((uint8_t *)key,sizeof(key)); 
    ascon.setIV(iv,sizeof(iv));
    ascon.addAuthData(authdata,sizeof(authdata));
    ascon.decrypt(buffer1,cipherData,sizeOfArray);
    unsigned long CurrentTime = micros();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial.print("Time taken for Decryption: ");
    Serial.print(ElapsedTime);
    Serial.print("us");
    Serial.println();
    //ascon.clear();
    return (char *)buffer1;
}

void loop()
{
}
