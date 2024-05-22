#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Define DHT sensor pin and type
#define DHTPIN 2
#define DHTTYPE DHT11

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD (change the address to 0x3F if 0x27 doesn't work)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define soil moisture sensor pin
#define SOIL_MOISTURE_PIN A0

// Define relay pin
#define RELAY_PIN A3

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize the relay pin as an output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Make sure the relay is off initially

  // Print a welcome message
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000); // Wait for 2 seconds
  lcd.clear();
}

void loop() {
  // Read humidity and temperature values from DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read the soil moisture level
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float soilMoisturePercent = map(soilMoistureValue, 1023, 0, 0, 100);

  // Verify if soil moisture reading is valid
  if (soilMoistureValue >= 0 && soilMoistureValue <= 1023) {
    // Print sensor values to Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisturePercent);
    Serial.println(" %");

    // Display sensor values on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");

    delay(2000);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Soil Moist: ");
    lcd.print(soilMoisturePercent);
    lcd.print("%");

    delay(3000);

    lcd.clear();

    // Control the relay based on soil moisture level
    if (soilMoisturePercent < 50) {
      // Soil is dry, turn on the relay (start irrigation)
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      // Soil is moist, turn off the relay (stop irrigation)
      digitalWrite(RELAY_PIN, LOW);
    }
  } else {
    // Invalid soil moisture reading, turn off the relay
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Error: Invalid soil moisture reading");
  }

  delay(1000); // Wait for 1 second before the next loop
}
