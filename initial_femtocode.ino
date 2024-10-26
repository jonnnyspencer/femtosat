#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <RH_RF95.h>


// Pin definitions
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Create instances of the sensors and radio
Adafruit_BME280 bme; 
Adafruit_ADXL345_Unified adxl = Adafruit_ADXL345_Unified(12345);
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);
  
  // Initialize BME280 
  if (!bme.begin(0x77)) { // Change the address if necessary. 0x77 is the default unless jumper from SDO to GND then 0x76
    Serial.println("Could not find BME280 sensor!");
    while (1);
  }

  // Initialize ADXL345
  if (!adxl.begin(0x53)) { //0x53 is default address
    Serial.println("Could not find ADXL345 sensor!");
    while (1);
  }

  // Initialize RFM95W
  if (!rf95.init()) { 
    Serial.println("RFM95W Radio init failed!");
    while (1);
  }
  
  // Set frequency
  rf95.setFrequency(915.0); // Set to your desired frequency in MHz
  rf95.setTxPower(23, false); // Set transmit power (in dBm)

  Serial.println("Setup complete.");
}

void loop() {
  // Read BME280 data
  float temperature = bme.readTemperature(); // Celsius
  float humidity = bme.readHumidity(); // Percentage
  float pressure = bme.readPressure() / 100.0F; // hPa
  
  // Read ADXL345 data
  sensors_event_t event; 
  adxl.getEvent(&event);
  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  // Prepare data to transmit
  String data = String(temperature) + "," + String(humidity) + "," + String(pressure) + "," + String(x) + "," + String(y) + "," + String(z)+"\n";
  
  // Send data
  rf95.send((uint8_t *)data.c_str(), data.length()); // convert data to c_str (Ascii codes terminated with \0). uint8_t * makes sure that we are sending bytes
  rf95.waitPacketSent();

  Serial.print("Sent: ");
  Serial.println(data);

  delay(1000); // Wait for 2 seconds before sending again
}

