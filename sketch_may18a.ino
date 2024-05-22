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

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Initialize the DHT sensor
  dht.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
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

  // Print the values to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println("%");

  // Display the values on the LCD
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

  delay(2000);

  lcd.clear();
}
