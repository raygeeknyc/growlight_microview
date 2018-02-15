 /**
 * GrowLight 24 hour cycle using Common Cathode RGB LEDs
 *
 * This file and supporting library files can be found at:
 * http://www.insanegiantrobots.com/arduino/RgbLed.zip
 *
 * @author("Raymond Blum" <raymond@insanegiantrobots.com>) 
 **/

#include "RTClib.h"
#include <MicroView.h>
#include <RgbLed.h>

RTC_DS3231 rtc;

#define LED1_PIN_RED  0
#define LED1_PIN_BLUE  2
#define LED1_PIN_GREEN 1

#define LED2_PIN_RED 3
#define LED2_PIN_GREEN  4
#define LED2_PIN_BLUE 5

#define DURATION_BLINK 300
unsigned long DURATION_HOUR = 1000L*60L*60L;  // avoid delay() overflow errors
//#define DURATION_HOUR 1000L*10L

// create objects of the appropriate types for the connected LED
RgbLedCommonAnode led_rgb1(LED1_PIN_RED, LED1_PIN_GREEN, LED1_PIN_BLUE);
RgbLedCommonAnode led_rgb2(LED2_PIN_RED, LED2_PIN_GREEN, LED2_PIN_BLUE);

void setup() {
   #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("<Setup>");
  #endif

  uView.begin();    // begin of MicroView
  uView.clear(PAGE);  // erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
  uView.display();  // display the content in the buffer memory, by default it is the MicroView logo

  if (! rtc.begin()) {
  }
  
  if (rtc.lostPower()) {
  }
  
  led_rgb1.setColor(Color::RED);
  delay(2000);
  led_rgb1.setColor(Color::GREEN);
  delay(2000);
  led_rgb1.setColor(Color::BLUE);
  delay(2000);

  led_rgb1.test();
  led_rgb1.setColor(Color::NONE);
  
  #ifdef DEBUG
    Serial.println("</SerSetup>");
  #endif

}

void quickComplements() {
  #ifdef DEBUG
    Serial.print("changing at ");
    Serial.println(millis());
  #endif
  
  led_rgb1.setColor(Color::RED );
  delay(DURATION_BLINK);    

  led_rgb1.setColor(Color::BLUE);
  delay(DURATION_BLINK);
   
   led_rgb1.setColor(Color::GREEN);
  delay(DURATION_BLINK);

  led_rgb1.setColor(Color::CYAN);
  delay(DURATION_BLINK);
   
  led_rgb1.setColor(Color::YELLOW);
  delay(DURATION_BLINK);

  led_rgb1.setColor(Color::MAGENTA);
  delay(DURATION_BLINK);    
 
  led_rgb1.setColor(Color::RGB_WHITE);
  delay(DURATION_BLINK);       
}

void waitUntil(RTC_DS3231 clock, int hours, int minutes) {
    DateTime now = clock.now();

    #ifdef DEBUG
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    #endif
}

void loop() {
  // Dawn  at 06:00
  waitUntil(rtc, 6, 00);
  quickComplements();
  led_rgb1.setColor(Color::NONE);
  led_rgb2.setColor(Color::RED);
 
  // Morning light at 06:30
  waitUntil(rtc, 6, 30);
  quickComplements();
  led_rgb1.setColor(Color::RED);
  led_rgb2.setColor(Color::BLUE);

  // Shift light at 08:00
  waitUntil(rtc, 8, 00);
  quickComplements();
  led_rgb1.setColor(Color::BLUE);
  led_rgb2.setColor(Color::CYAN);

  // Bright light at 10:00
  waitUntil(rtc, 10, 00);
  quickComplements();
  led_rgb1.setColor(Color::RGB_WHITE);
  led_rgb2.setColor(Color::CYAN);
   
  // Full light at 10:30
  waitUntil(rtc, 10, 30);
  quickComplements();
  led_rgb1.setColor(Color::RGB_WHITE);
  led_rgb2.setColor(Color::RGB_WHITE);
   
  // Reduced light at 14:00
  waitUntil(rtc, 14, 30);
  quickComplements();
  led_rgb1.setColor(Color::RGB_WHITE);
  led_rgb2.setColor(Color::YELLOW);
  
  // Afternoon light at 14:30
  waitUntil(rtc, 14, 30);
  quickComplements();
  led_rgb1.setColor(Color::MAGENTA);
  led_rgb2.setColor(Color::YELLOW);

  // Evening light at 17:30
  waitUntil(rtc, 17, 30);
  quickComplements();
  led_rgb1.setColor(Color::MAGENTA);
  led_rgb2.setColor(Color::MAGENTA);

  // Dusk light at 19:00
  waitUntil(rtc, 19, 00);
  quickComplements();
  led_rgb1.setColor(Color::MAGENTA);
  led_rgb2.setColor(Color::RED);

  // Twilight light at 20:00
  waitUntil(rtc, 20, 00);
  quickComplements();
  led_rgb1.setColor(Color::RED);
  led_rgb2.setColor(Color::RED);

  // Off at 20:30
  waitUntil(rtc, 20, 30);
  quickComplements();
  led_rgb1.setColor(Color::NONE); 
  led_rgb2.setColor(Color::NONE); 
}
