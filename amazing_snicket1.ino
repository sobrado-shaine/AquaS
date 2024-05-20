#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <TimeLib.h>
#include <DS3231.h>

int DHT11_PIN = 5;
int soilSenPin = A0;
int relayPin = 7;

DHT dht(DHT11_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DS3231 rtc(Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();

  Wire.begin();

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

}

void loop() {
  // Get current time
  time_t t = now();
  int hr = hour(t);
  int min = minute(t);
  
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Read soil moisture
  int soilMoistureValue = analogRead(soilSenPin);
  
  // Calculate thresholds
  float maxTemperature = 50.0; // Adjust based on DHT11 specs
  float maxHumidity = 100.0;   // DHT11 max humidity
  float temperatureThreshold = 0.75 * maxTemperature;
  float humidityThreshold = 0.75 * maxHumidity;
  
  // Display readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  lcd.print(soilMoistureValue);
  delay(2000);

  // Check time conditions
  bool withinOperatingHours = (hr >= 5 && hr < 18);
  bool is6AM = (hr == 6 && min == 0);
  bool is5PM = (hr == 17 && min == 0);

  // Check if the pump should be activated
  bool activatePump = false;
  
  if (withinOperatingHours) {
    if (is6AM || is5PM) {
      activatePump = true;
    }
  }

  if (temperature >= temperatureThreshold || humidity >= humidityThreshold) {
    activatePump = true;
  }

  // Operate pump
  if (activatePump) {
    digitalWrite(relayPin, HIGH); // Turn on pump
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pump ON");
    delay(60000); // Pump runs for 1 minute
    digitalWrite(relayPin, LOW); // Turn off pump
    lcd.setCursor(0, 0);
    lcd.print("Pump OFF");
    delay(2000);
  }
}
