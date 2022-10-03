/* @ISongwut */
/* Call Libraly */
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <SevenSegmentTM1637.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

// RTC Module
ThreeWire myWire(8, 7, 9); // SDA_Pin, SCL_Pin, RST_Pin
RtcDS1302<ThreeWire> Rtc(myWire);

// Seven Segment Module
const byte PIN_CLK = A5;
const byte PIN_DIO = A4;
SevenSegmentTM1637 display(PIN_CLK, PIN_DIO);

void setup()
{
  SerialInit();
  DisplayInit();
  RTCInit();
}

void loop()
{
  RtcDateTime now = Rtc.GetDateTime();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(now);
  if (compiled > now)
  {
    Rtc.SetDateTime(compiled);
    printDateTime(compiled);
  }
  else
  {
    printDateTime(now);
  }

  (!now.IsValid() || !compiled.IsValid()) ? Serial.println("RTC lost confidence in the DateTime!"):NULL;
}

void printDateTime(const RtcDateTime &dt)
{
  char datestring[10], printSec[10];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u%02u"),
             dt.Hour(),
             dt.Minute());

  snprintf_P(printSec,
             countof(datestring),
             PSTR("%02u"),
             dt.Second());

  display.print(datestring);
  Serial.println(printSec);
}

void SerialInit()
{
  Serial.begin(9600);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
}

void DisplayInit()
{
  display.begin();           // Initializes the display
  display.setBacklight(100); // Set the brightness to 100 %
  display.setColonOn(1);     // Show the semi-colon on the display
}

void RTCInit()
{
  Rtc.Begin(); // Initializes the RTC

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); // Get compiled date-time
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) // Check the date-time is valid or not
  {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }
}
