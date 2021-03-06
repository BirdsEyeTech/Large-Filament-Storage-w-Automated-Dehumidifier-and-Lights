//LED Control
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 74 // Popular NeoPixel ring size (20 on each short strand and 34 on long strand)

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

//Colors
uint32_t red = strip.Color(255, 0, 0);
uint32_t white = strip.Color(255, 255, 255);
uint32_t orange = strip.Color(255, 140, 0);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//OLED Screen Stuff
#include <Adafruit_SSD1306.h> 
#include <splash.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//DHT22 Sensor Stuff
#include <dht.h>
dht DHT;
#define DHT22_PIN 2
float hum;
float temp;

//Relay Control Pin
int HumidOut = 10;
int TempOut = 11;
boolean humidState = 0;
//boolean tempState = 0;

void setup ()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  pinMode(HumidOut, OUTPUT);
 // pinMode(TempOut, OUTPUT);

 strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 strip.show();
}

void loop ()
{
  
  int chk = DHT.read22(DHT22_PIN);
  hum = DHT.humidity;
  temp = DHT.temperature;
  delay(1000);

  //Relay Trigger Settings TEMP SAFETY and WARNING
    if (temp >= 100) {
    digitalWrite(HumidOut, LOW);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("TEMP HIGH!");
    display.display();
    strip.fill(red, 0, 15);
    strip.show();
    delay(10000);
  }

  //Relay Trigger Settings Humidity
  if ((hum >= 20) && (temp <= 100)) {
    digitalWrite(HumidOut, HIGH);
    strip.fill(white, 0, 19);
    strip.fill(orange, 20, 53);
    strip.fill(white, 54, 73);
    strip.show();
  }
  else {
    digitalWrite(HumidOut, LOW);
    strip.fill(white, 0, 73);
    strip.show();
  }

  //OLED Display Settings
  
  String humid = String(hum, 1);
  String temper = String(temp, 1);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //Dehumidifier Status
  humidState = digitalRead(HumidOut);
  display.println("STATUS:");
  display.setCursor(0, 16);
  if (humidState == 0) {
    display.println("OFF");
  }
  else {
    display.println("ON");
  }
  display.setCursor(30, 16);
  display.display();
  display.clearDisplay();
  delay(2000);

  //Humidity Level
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Humidity:");
  display.setCursor(0, 16);
  display.println(humid);
  display.setCursor(50, 16);
  display.println("%");
  display.display();
  delay(2000);

  //Temp Level
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temp:");
  display.setCursor(0, 16);
  display.println(temper);
  display.setCursor(50, 16);
  display.println("C");
  display.display();
  display.clearDisplay();
  delay(2000);


}
