#include <SPI.h>
#include <RH_RF95.h>

// Pin definitions for the receiver
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// if using the Moteino board, initilize it as an arduino uno

// Create instance of the radio
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);
  
  // Initialize the RFM95W radio
  if (!rf95.init()) {
    Serial.println("RFM95W Radio init failed!");
    while (1);
  }

  // Set frequency for the receiver
  rf95.setFrequency(915.0); // Set to the same frequency as the transmitter (MHz)
  rf95.setTxPower(23, false); // Optional, not used for receiving but keeps settings consistent

  Serial.println("LoRa Receiver setup complete.");
}

void loop() {
  // Buffer to store the received data
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  // Check if a packet is available
  if (rf95.available()) {
    // Receive the message
    if (rf95.recv(buf, &len)) {
      // Convert buffer to string
      buf[len] = '\0'; // Null terminate the string
      String receivedData = String((char*)buf);

      // Print the received data
      Serial.print("Received: ");
      Serial.print(receivedData);

      // Parse the received data (assuming it's in the format temperature,humidity,pressure,x,y,z)
      float temperature, humidity, pressure, x, y, z;
      sscanf(receivedData.c_str(), "%f,%f,%f,%f,%f,%f", &temperature, &humidity, &pressure, &x, &y, &z);

   // Parse the received data using strtok and atof
      char* token = strtok((char*)buf, ",");
      if (token != nullptr) {
        float temperature = atof(token);

        token = strtok(nullptr, ",");
        float humidity = token ? atof(token) : 0.0;

        token = strtok(nullptr, ",");
        float pressure = token ? atof(token) : 0.0;

        token = strtok(nullptr, ",");
        float x = token ? atof(token) : 0.0;

        token = strtok(nullptr, ",");
        float y = token ? atof(token) : 0.0;

        token = strtok(nullptr, ",");
        float z = token ? atof(token) : 0.0;

        // Print the parsed values
        Serial.print("Temperature: ");
        Serial.println(temperature);

        Serial.print("Humidity: ");
        Serial.println(humidity);

        Serial.print("Pressure: ");
        Serial.println(pressure);

        Serial.print("Acceleration X: ");
        Serial.println(x);

        Serial.print("Acceleration Y: ");
        Serial.println(y);

        Serial.print("Acceleration Z: ");
        Serial.println(z);
        Serial.println();
      }
    } else {
      Serial.println("Receive failed");
    }
  }
}

