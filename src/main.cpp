#include <Arduino.h>
#include "LiquidLevelSensor.h"
#include "BatteryMonitory.h"
#include "Feed.h"

// Conversion factor for seconds to microseconds
#define S_TO_US_FACTOR 1000000ULL

const int sense_555_pin = 14;
const int power_555_pin = 32;
const int analog_vbatt_pin = A13;
const int measurement_interval_ms = 1000;
const int deep_sleep_duration_seconds = 15;
int min_awake_time_seconds = 0;

LiquidLevelSensor sensor(sense_555_pin, power_555_pin, 10, 50);
BatteryMonitor batteryMonitor(analog_vbatt_pin, 3.3);
AdafruitIoFeed feed;

bool ProcessDepthSensing();
void EnterDeepSleep(unsigned int secondsToSleep);
void PrintWakeupReason();

// the setup function runs once when you press reset or power the board
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(9600);
    PrintWakeupReason();
    if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
        // stay awake longer to allow for OTA update
        min_awake_time_seconds = 20;
    }
    Serial.println("min_awake_time_seconds=" + String(min_awake_time_seconds));
    sensor.Init();
    batteryMonitor.Init();
    feed.Setup();
}

// the loop function runs over and over again forever
void loop()
{
    feed.Loop();
    if(ProcessDepthSensing())
    {
      feed.PublishDepth(sensor.GetLastDepthMm());
      feed.PublishBattery(batteryMonitor.GetLastBatteryVoltage());

      if(deep_sleep_duration_seconds == 0)
      {
        Serial.println("deep sleep disabled");
      }
      else if(millis() > (min_awake_time_seconds * 1000))
      {
        EnterDeepSleep(deep_sleep_duration_seconds);
      }
    }

}

bool ProcessDepthSensing()
{
  static unsigned long lastMeasurementTimeMs = 0;
  bool takeMeasurement = (millis() - lastMeasurementTimeMs > measurement_interval_ms || lastMeasurementTimeMs == 0);

  if(takeMeasurement)
  {
      lastMeasurementTimeMs = millis();
      // digitalWrite(LED_BUILTIN, HIGH);
      sensor.MeasureDepthMm();
      batteryMonitor.MeasureBatteryVoltage();
      // digitalWrite(LED_BUILTIN, LOW);
  }

  return takeMeasurement;
}

void EnterDeepSleep(unsigned int secondsToSleep)
{
    /*
    First we configure the wake up source
    */
    Serial.println("Sleeping for " + String(secondsToSleep) + " seconds...");
    esp_sleep_enable_timer_wakeup(secondsToSleep * S_TO_US_FACTOR);

    /*
    Next we decide what all peripherals to shut down/keep on
    By default, ESP32 will automatically power down the peripherals
    not needed by the wakeup source, but if you want to be a poweruser
    this is for you. Read in detail at the API docs
    http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
    Left the line commented as an example of how to configure peripherals.
    The line below turns off all RTC peripherals in deep sleep.
    */
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    Serial.println("Configured all RTC Peripherals to be powered down in sleep");
  
    /*
    Now that we have setup a wake cause and if needed setup the
    peripherals state in deep sleep, we can now start going to
    deep sleep.
    In the case that no wake up sources were provided but deep
    sleep was started, it will sleep forever unless hardware
    reset occurs.
    */
    Serial.println("Going to sleep now");
    Serial.flush();
    Serial.end();
    digitalWrite(LED_BUILTIN, LOW);
    esp_deep_sleep_start();
}

void PrintWakeupReason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


 