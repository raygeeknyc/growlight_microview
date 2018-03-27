/**
  GrowLight 24 hour cycle using Common Cathode RGB LEDs

  This file and supporting library files can be found at:
  http:/github.com/raygeeknyc/growlightmv

  @author("Raymond Blum" <raymond@insanegiantrobots.com>)
**/

#include "RTClib.h"
#include <MicroView.h>
#include <RgbLed.h>

RTC_DS3231 rtc;  // This is accessed via SDA and SCL which are A4 and A5 respectively

#define LED1_PIN_RED  3
#define LED1_PIN_BLUE  5
#define LED1_PIN_GREEN 6

#define LED2_PIN_RED A0
#define LED2_PIN_GREEN  A1
#define LED2_PIN_BLUE A2

#define DURATION_BLINK 300
unsigned const long DELAY_SECOND = 1000L;
unsigned const long DELAY_MINUTE = 60L * DELAY_SECOND;
unsigned const long DURATION_HOUR = 60L * DELAY_MINUTE;

// create objects of the appropriate types for the connected LED
RgbLedCommonAnode led_rgb1(LED1_PIN_RED, LED1_PIN_GREEN, LED1_PIN_BLUE);
RgbLedCommonAnode led_rgb2(LED2_PIN_RED, LED2_PIN_GREEN, LED2_PIN_BLUE);

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("<Setup>");
#endif

  uView.begin();    // begin of MicroView
  uView.clear(ALL);  // erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
  uView.clear(PAGE);  // erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
  uView.println("Testing LEDs");
  uView.display();  // display the content in the buffer memory, by default it is the MicroView logo

  if (! rtc.begin()) {
  }

  led_rgb1.setColor(Color::RED);
  delay(500);
  led_rgb1.setColor(Color::GREEN);
  delay(500);
  led_rgb1.setColor(Color::BLUE);
  delay(500);

  led_rgb1.test();
  led_rgb1.setColor(Color::NONE);

  led_rgb2.setColor(Color::RED);
  delay(500);
  led_rgb2.setColor(Color::GREEN);
  delay(500);
  led_rgb2.setColor(Color::BLUE);
  delay(500);

  led_rgb2.test();
  led_rgb2.setColor(Color::NONE);

  #ifdef DEBUG
  Serial.println("</Setup>");
  #endif
  uView.println("finding current time");
  uView.display();
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

void displayTime(DateTime now, int at_hour, int at_minute) {
    char display_time[3];
    
    uView.setCursor(0,0);
    sprintf(display_time,"%02d", now.hour());
    uView.print(display_time);
    uView.print(":");
    sprintf(display_time,"%02d", now.minute());
    uView.println(display_time);
    uView.print(" to: ");
    sprintf(display_time,"%02d", at_hour);
    uView.print(display_time);
    uView.print(":");
    sprintf(display_time,"%02d", at_minute);
    uView.print(display_time);
    uView.display();
}

void displayLightLevel(float total_light_level) {
    uView.setCursor(0,20);
    int y = int(uView.getLCDHeight());
    int x = uView.getLCDWidth() / 2;
    y -= 19;
    y /= 2;
    uView.circleFill(x, y+19, y, WHITE, NORM);
    uView.rectFill(0, 19, uView.getLCDWidth(), (y*2)*(1.0 - total_light_level), BLACK, NORM);
    uView.circle(x, y+19, y, WHITE, NORM);
    uView.display();
}

/* 
 *  This function does not cross midnight, so wait for 23:59 in the caller 
 */
void displayUntil(RTC_DS3231 clock, int at_hour, int at_minute, float light_level) {
DateTime now = clock.now();

  uView.clear(PAGE);

  while (now.hour() < at_hour) {
    now = clock.now();
    displayTime(now, at_hour, at_minute);
    displayLightLevel(light_level);
    delay(DELAY_MINUTE);
  }
  if (now.hour() == at_hour) {
    while ((now.minute() < at_minute) || (now.minute() == 0 && at_minute == 0)) {
      now = clock.now();
      displayTime(now, at_hour, at_minute);
      displayLightLevel(light_level);
    }
  }
#ifdef DEBUG
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
#endif
}

void loop() {
  quickComplements();
  led_rgb1.setColor(Color::NONE);
  led_rgb2.setColor(Color::NONE);

  displayUntil(rtc, 6, 0, 0.0);

  // Dawn at 06:00
  led_rgb1.setColor(Color::NONE);
  led_rgb2.setColor(Color::RED);
  
  displayUntil(rtc, 6, 30, (1.0/6));
  // Morning light at 06:30
  led_rgb1.setColor(Color::RED);
  led_rgb2.setColor(Color::RED);

  displayUntil(rtc, 8, 0, (1.0/3));
  // Shift light temp at 08:00
  led_rgb1.setColor(Color::RED);
  led_rgb2.setColor(Color::BLUE);
  
  displayUntil(rtc, 10, 0, (1.0/3));
  // Brighter light at 10:00
  led_rgb1.setColor(Color::BLUE);
  led_rgb2.setColor(Color::CYAN);

  displayUntil(rtc, 10, 30, (1.0/2));
  // Bright light at 10:30
  led_rgb1.setColor(Color::RGB_WHITE);
  led_rgb2.setColor(Color::CYAN);

  displayUntil(rtc, 12, 0, (1.0/1.2));
  // Full light at 12:00
  led_rgb1.setColor(Color::RGB_WHITE);
  led_rgb2.setColor(Color::RGB_WHITE);
  
  displayUntil(rtc, 14, 30, 1.0);
  // Afternoon light at 14:30
  led_rgb1.setColor(Color::RGB_WHITE);
  led_rgb2.setColor(Color::YELLOW);
  
  displayUntil(rtc, 14, 30, (1.0/1.2));
  // Evening light at 17:30
  led_rgb1.setColor(Color::MAGENTA);
  led_rgb2.setColor(Color::YELLOW);  

  displayUntil(rtc, 19, 0, (1.0/1.5));
  // Dusk light at 19:00
  led_rgb1.setColor(Color::MAGENTA);
  led_rgb2.setColor(Color::MAGENTA);

  displayUntil(rtc, 20, 0, (1.0/1.5));
  // Twilight light at 20:00
  led_rgb1.setColor(Color::MAGENTA);
  led_rgb2.setColor(Color::RED);

  // Darker at 20:30
  displayUntil(rtc, 20, 30, (1.0/2));
  led_rgb1.setColor(Color::RED);
  led_rgb2.setColor(Color::RED);

  // Off at 21:00
  displayUntil(rtc, 21, 0, (1.0/3));
  led_rgb1.setColor(Color::NONE);
  led_rgb2.setColor(Color::NONE);

  // Wait until 00:00
  displayUntil(rtc, 23, 59, 0.0);
  delay(DELAY_MINUTE);
  delay(DELAY_SECOND);
}
