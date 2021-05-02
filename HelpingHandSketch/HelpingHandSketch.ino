 /*
Comments by Adam Cotter:

This sketch broadcasts the XYZ variables of the accelerometer over Bluetooth Low Energy and 
allows for text to be written to the LCD.

Pins have been reassigned to suit the Arduino Nano IoT board:
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 3
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)


*/

// include the library code:
#include <LiquidCrystal.h>
#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// User defined service
 
BLEService accelService("91c39bfd-1921-4111-9670-c03fc51eb856"); 

//Accelerometer Characteristics for XYZ
BLEUnsignedIntCharacteristic accelCharX("2a0019db-2b41-49f7-b3a7-d6deca619810", BLERead| BLENotify);
BLEUnsignedIntCharacteristic accelCharY("4e41b6eb-8acc-44d7-961b-9b2706764b49", BLERead| BLENotify);
BLEUnsignedIntCharacteristic accelCharZ("37855a77-602f-4177-b916-c103a1723f5d", BLERead| BLENotify);


//Accelerometer variables
int accelX = 1;
int accelY = 1;
int accelZ = 1;
float x, y ,z;

void setup() {

  //IMU setup
  IMU.begin();
  if (IMU.begin()) {
    Serial.println("IMU began succesfully");
  } else {
    Serial.println("IMU setup failed");
    IMU.end();
    while (1);
  }

  Serial.begin(9600); //Initialising the serial bus
  //while (!Serial); //Wait for serial initilisation (if this is included, the board seems to wait for the serial monitor to be opened to run.)

  pinMode(LED_BUILTIN, OUTPUT);

  //BLE setup
  if (!BLE.begin()){ // Initialise BLE
    Serial.println("starting BLE failed");
    delay(500);
    while(1);
  }
  
  BLE.setLocalName("HelpingHand"); // Name of connection
  
  BLE.setAdvertisedService(accelService); // Advertise Service
  
  // Adds characteristics to service
  accelService.addCharacteristic(accelCharX);
  accelService.addCharacteristic(accelCharY);
  accelService.addCharacteristic(accelCharZ);
  
  BLE.addService(accelService);// Add the service

  //For monitoring purposes
  Serial.println(accelService);
  Serial.println(accelCharX);
  Serial.println(accelCharY);
  Serial.println(accelCharZ);

  BLE.advertise(); // Begins advertising
  Serial.print("Peripheral device MAC: ");
  Serial.println(BLE.address());
  Serial.println("Waiting for connections...");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Helping Hand");  

}

void loop() {
  
  BLEDevice central = BLE.central(); // Waits for a central to connect

  if (central) {
   Serial.print("Connected to central. MAC Address: ");
   Serial.println(central.address());
   //Lights up onboard LED when connected
   digitalWrite(LED_BUILTIN, HIGH); 
   //Writes a message on connect
   lcd.setCursor(0,1);
   lcd.print("Connected!");

   while (central.connected()){  // loops while connected
    delay(100);
    readAccelerometer();

    accelCharX.writeValue(accelX);
    accelCharY.writeValue(accelY);
    accelCharZ.writeValue(accelZ);

    Serial.print("Accel X: ");
    Serial.print(accelX);
    Serial.print(" Accel Y: ");
    Serial.print(accelY);
    Serial.print(" Accel Z: ");
    Serial.print(accelZ);
    Serial.print("\n");
    
    } 

   // on disconnect, turn off the LED
   digitalWrite(LED_BUILTIN, LOW);
   Serial.print("Disconnected from central. MAC Address: ");
   Serial.println(central.address());
   //Clears the "Connected!" Message from the LCD
   lcd.setCursor(0,1);
   lcd.print("          ");
  } 
  
}

// function for reading the accelerometer and updating the variables associated
void readAccelerometer(){
  if(IMU.accelerationAvailable()){
    IMU.readAcceleration(x, y, z);
    Serial.print(x);
    Serial.print(y);
    Serial.print(z);
    accelX = x*100;
    accelY = y*100;
    accelZ = z*100;
  }
}
