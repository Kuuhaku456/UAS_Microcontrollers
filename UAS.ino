#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define motorPin1 11
#define motorPinEN1 13
#define motorPin2 12
#define motorPinEN2 10
#define ledR 7
#define ledG 6
#define ledB 8
#define led A1
#define potPin A2

LiquidCrystal_I2C lcd(0x27, 16, 2); 
DHT dht(DHTPIN, DHTTYPE);

Adafruit_NeoPixel pixels(20, 4, NEO_GRB + NEO_KHZ800);

double temp, hum;
int potValue, motorSpeed;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPinEN1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPinEN2, OUTPUT);
  pinMode(potPin, INPUT); 
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(led, OUTPUT);
  pixels.begin();
  digitalWrite(led, 0);
  pixels.clear();
}

void loop() {
  hum =  dht.readHumidity();
  temp =  dht.readTemperature();
  temp = 24.7;
  potValue = analogRead(potPin);
  motorSpeed = map(potValue, 0, 1023, 0, 255);
  Serial.println(motorSpeed);

  if (motorSpeed > 0){
    digitalWrite(motorPinEN2, 1);
    analogWrite(motorPin2, motorSpeed);
  } else {
    digitalWrite(motorPinEN2, 0);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(temp));
  lcd.write(byte(223));
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum : " + String(hum) + "%");

  if(temp < 25){
    digitalWrite(ledR, 1);
    digitalWrite(ledG, 1);
    digitalWrite(ledB, 0);
    ledPixel(true);
  } else if (temp > 27){
    digitalWrite(motorPinEN1, 1);
    analogWrite(motorPin1, 200);
    digitalWrite(ledR, 0);
    digitalWrite(ledG, 1);
    digitalWrite(ledB, 1);
    ledPixel(false);
  } else {
    digitalWrite(motorPinEN1, 0);
    digitalWrite(ledR, 1);
    digitalWrite(ledG, 0);
    digitalWrite(ledB, 1);
    ledPixel(false);
  }

  delay(1000);
}

void ledPixel(bool status){
  if (status) {
    for(int i=0; i<20; i++) {
      pixels.setPixelColor(i, pixels.Color(200, 200, 0));
      pixels.show();
    }
  } else {
    for(int i=0; i<20; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
  }
}
