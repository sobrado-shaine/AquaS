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
#define RELAY_PIN 7

// Define constants for timing
const unsigned long MILLISECONDS_PER_HOUR = 3600000UL;
const unsigned long START_TIME = 5 * MILLISECONDS_PER_HOUR;  // 5 AM
const unsigned long END_TIME = 19 * MILLISECONDS_PER_HOUR;   // 7 PM
const unsigned long IRRIGATION_DURATION = 60000UL;           // 1 minute in milliseconds
const unsigned long SIX_AM = 6 * MILLISECONDS_PER_HOUR;      // 6 AM
const unsigned long FIVE_PM = 17 * MILLISECONDS_PER_HOUR;    // 5 PM

unsigned long previousMillis = 0;

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
  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis % MILLISECONDS_PER_HOUR;

  // Read humidity and temperature values from DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read the soil moisture level
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float soilMoisturePercent = map(soilMoistureValue, 620, 215, 0, 100);

  // Print the values to the Serial Monitor
Serial.print("Humidity: ");
Serial.print(humidity);
Serial.println(" %");
Serial.print("Temperature: ");
Serial.print(temperature);
Serial.println(" °C");
Serial.print("Soil Moisture: ");
Serial.print(soilMoisturePercent);
Serial.println(" %");
Serial.print("Time: ");
Serial.print((currentMillis / MILLISECONDS_PER_HOUR) % 24); // Approximate hour
Serial.print(":");
Serial.println((elapsedMillis / 60000) % 60); // Approximate minute


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

  // Control the relay based on the approximate time and sensor data
  unsigned long hourMillis = currentMillis % (24 * MILLISECONDS_PER_HOUR);
  if (hourMillis >= START_TIME && hourMillis < END_TIME) {
    if ((hourMillis >= SIX_AM && hourMillis < SIX_AM + IRRIGATION_DURATION) ||
        (hourMillis >= FIVE_PM && hourMillis < FIVE_PM + IRRIGATION_DURATION)) {
      // Turn on the relay to start irrigation
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      // Turn off the relay
      digitalWrite(RELAY_PIN, LOW);
    }
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(1000); // Wait for 1 second before the next loop
}
