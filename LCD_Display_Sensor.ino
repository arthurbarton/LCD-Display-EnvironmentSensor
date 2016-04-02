/*
 * LCD_Display_Sensor
 * See the pin definitons below
 *  - Grove LCD RBG/Backlit = Pin12
 *  - Temperature sensor = Pin A1
 *  - Sound sensor = Pin A0
 *  - Light sensor = Pin A3
 *  - push button = Pin 8
 */
 
#include <Wire.h>
#include <rgb_lcd.h>          // LCD Interface
// Available from https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight
#include <SoftwareSerial.h>   // Button handler

// Initialize the LCD
rgb_lcd lcd;

// Define the LCD size
const int numRows = 2;
const int numCols = 16;

// Define the Temp Sensor Pin
const int tempSensorPin   = 1;
// Define the Button Pin
const int buttonPin = 8;
// Define the Sound sensor
const int pinSound = A0;
// Define the Light sensor
const int pinLight = A3;

// LCD Colours - background
int colourRed   = 51;
int colourGreen = 0;
int colourBlue  = 0;

void setup() {
  // Set the LCD width and height from the ints above
  lcd.begin(numCols,numRows);

  // Set the button to Input
  pinMode(buttonPin,INPUT);

  // Set the LCD background. This is not persistent
  lcd.setRGB(colourRed, colourGreen, colourBlue);

  // Print an initial message
  lcd.print("Hold button to");
  lcd.setCursor(0,1);
  lcd.print("see sensors");
  delay(2500); 
  // Then clear the lcd & set the position back to home
  lcd.clear();
  lcd.home();
}

/* There is a bug in the library that prevents the 
 *  display powering down.
 *  Work around this by setting all the colours to 0 (Black)
 * TODO: see if this reduces power draw.
*/
void setDisplayDark()
{
  lcd.setPWM(REG_RED, 0);
  lcd.setPWM(REG_GREEN, 0);
  lcd.setPWM(REG_BLUE, 0);
  // From the reference guide this can take a moment
  delayMicroseconds(500);  
}

/* See setDisplayDark. Once we've changed all the colours to
 *  black, we need to tell it the lit colours again
*/
void setDisplayLight()
{
  lcd.setPWM(REG_RED, colourRed);
  lcd.setPWM(REG_GREEN, colourGreen);
  lcd.setPWM(REG_BLUE, colourBlue);
  // From the reference guide this can take a moment
  delayMicroseconds(500);  
}

float getTemperature()
{
  // Your guess is as good as mine - this is reference code from the Sensor
  float temperature = analogRead(tempSensorPin); // Get the voltage reading from the Sensor
  temperature = (float)(1023 - temperature)*10000/temperature;
  temperature = 1/(log(temperature/10000)/3975+1/298.15)-273.15;
  return temperature;
}

void loop() {
  // Clearn and Turn off the display
  lcd.clear();
  setDisplayDark();
  
  if(digitalRead(buttonPin))
  {
    // Turn the display back on
    setDisplayLight();

    // Print sensor headers
    lcd.print("temp|sound|light");

    // Get the sensor output
    float temperature = getTemperature();
    int soundVal = analogRead(pinSound);
    int lightVal = analogRead(pinLight);

    // Position the LCD - second row, first character
    lcd.setCursor(0,1);

    // Temperature
//    lcd.print(temperature,2);     // 2 decimal places = 5 chars
    lcd.print(temperature,1);
    lcd.print("|");

    // Sound
    char SoundOut[5];
    sprintf(SoundOut,"%5d",soundVal);
    lcd.print(SoundOut);
    lcd.print("|");

    // Light
    char LightOut[5];
    sprintf(LightOut,"%5d",lightVal);
    lcd.print(LightOut);

    // Wait, Clear, end loop
    delay(1500);
    lcd.clear();
  } else {
    // Turn the display off
    setDisplayDark();
    delay(1500);
  }
}

