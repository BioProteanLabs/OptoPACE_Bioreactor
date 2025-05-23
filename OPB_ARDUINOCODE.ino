// Import libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#include <SPI.h>
#include <SD.h>

#define ENCA 2
#define ENCB 3
#define DEBOUNCE_DELAY 50

// Mid log Set point Variables
// int setpoint = x; <-Replace x with the midlog value you find, it should be in the range of 0-1023
int setpoint = 800;                         // This variable is set to the value of the photoresistor when the bacteria should hit mid log growth. When the photoresistor reading matches this set point the pump will turn on and move media.
bool setpointhit = false;                   // This is a boolean that tells the code when the set point has been hit we set this to false when the code starts
unsigned long previousintervalsensor = 0;   // Similar to the variable for the pump function, this variable is the time when the sensor was last read, and is initally set to 0.
unsigned long setpointdelay = 5000;         // This variable determines the time between when the program checks if the sensor value is equivelent to the set point. The time is in milliseconds so here we have a 10 second delay.

// LCD Screen Menus
enum MenuState {MAIN_MENU, SUBMENUSTARTBACK, SUBRUNPACESD, SUBMENUTIME, SUBSETTIME, SUBPACEOFF, SUBMENUCHANGELEDS, SUBCHANGELEDS, SUBCALIBRATION, SUBSTOPCLEANING, SUBSTOPCLEANINGPROMPT};
MenuState currentMenu = MAIN_MENU;
LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile int time = 0;

// Rotary encoder variables
volatile int counter = 0;
int buttonState = 0;
int lastButtonState = 0;
const int buttonPin = 4;
unsigned long lastDebounceTime = 0;
bool buttonPressed = false;

const int stir1 = 5;  // Assigns the stirmotors to pin 5 on the microcontroller

// Variables for Peristaltic Pump function
const int pump = 9;                         // Assigns the pump to pin 9
unsigned long previousintervalpump = 0;     // This variable is set to 0 in our loop function it is set to time when the pump was last turned on                                    
unsigned long pumponinterval = 1000;        // Controls how long the runs once it is turned on. The time is in milliseconds in our case the pump is on for 1 second or 1000 milliseconds
unsigned long currenttime = 0;              // This variable represents the current time of the system we set it to 0 to start

// Variables for Optical Density Measurements
const int sensor1pin = A0;                 // This sets the pin for the photoresistor as A0 on the microcotroller  
int sensor1;                               // This initilizes a variable to store the value of the photoresistor reading
unsigned long sensordelay = 1000;          // This variable determines the time between when the voltage across the photoresistor is read. ie the time between sensor readings

// Variables for Averaging Function        // To combat potnetial noise in the system, our code averages the sensor values it reads
int counterSensor = 0;                     // This variable counts the amount of times a sensor reading is taken
int sum = 0;                               // This variable stores the sum of the sensor reading values
int average = 0;                           // This variable stores the computed average

// Variables for NeoPixel LEDs
const int LEDpin = 6;
int numleds = 8;
int numledsuv = 12;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numleds,LEDpin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsuv = Adafruit_NeoPixel(numledsuv, LEDpin, NEO_GRB + NEO_KHZ800);
uint32_t WHITE = pixels.Color(255,255,255);
uint32_t RED = pixels.Color(255,0,0);
uint32_t GREEN = pixels.Color(0,255,0);

// Variables for determining states for the menu values
bool runsystem = false;
bool runsystemsd = false;
bool settime = false;
bool paceoff = false;
bool runcalibration = false;
bool turnred = false;
bool turngreen = false;
bool turnuv = false;
bool turnblack = false;
bool stopcleaning = true;

// Variables for the cleaning function
unsigned long currenttimeclean = 0;
unsigned long timewhenclicked = 0;

//Function Declarations                   // Instead of including all of our code in one void loop function we seperated it into multiple different functions to allow for easier interpretation and editing.
void stirmotors();                        // This function runs the stir motors
void runsensors();                        // This function tells the arduino to collect data from the photoresistor
void checksetpoint();                     // This function checks to see if the sensor value has hit the set point
void LEDs();
void turnPaceOff();


void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(ENCA, INPUT_PULLUP);
  pinMode(ENCB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCA), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB), read_encoder, CHANGE);
  pinMode(buttonPin, INPUT_PULLUP);

  MenuDisplay();  // Display the initial menu correctly

  //Stirmotor Function
  pinMode(stir1, OUTPUT);                // Sets the stirmotor pin to an output, so the microcontroller can control when and how fast the stir motors spin.
 
  //Pump Function
  pinMode(pump, OUTPUT);                // Sets the pump pin as an output, this allows the microcontroller to control for how long and how fast the pump is moving liquid.
 
  pixelsuv.begin();
  pixelsuv.clear();
  pixelsuv.show();
  pixels.begin();
  pixels.clear();
  pixels.show();
 
  // Set up for LEDs
 
  Serial.begin(115200);
}

void loop() {

  // Run PACE
  if (runsystem == true) {
    currenttime = millis();             // This uses the millis(); function to set the variable currenttime to the currenttime
    stirmotors();                       // Calls the function stirmotors
    runsensors();                       // Calls the function runsensors
    checksetpoint();                    // Calls the function checksetpoint    
    LEDs();
  }

  // Turn off the system
  if (runsystem == false && runcalibration == false && settime == false && stopcleaning == true) {
    analogWrite(stir1,0);
    analogWrite(pump,0);
  }
  
  // Run the set time function for cleaning
  if (settime == true) {
    runpumptime();
  }

  // Run the calibration function 
  if (runcalibration == true) {
    currenttime = millis();
    stirmotors();
    runsensors();
  }

  // Turn the RGB LEDs red
  if (turnred == true) {
    red();
  }

  // Turn the RGB LEDs green
  if (turngreen == true) {
    green();
  }

  // Turn only the UV LEDs on
  if (turnuv == true) {
    uv();
  }
 
  // Turn off the LEDs
  if (turnblack == true) {
    black();
  }

  // Update menu display
  MenuDisplay();

  // Handle button press
  if (buttonPressed) {
    handleButtonPress();
    buttonPressed = false;  // Reset the button press flag
  }

  // Rotary encoder output
  static int lastCounter = 0;
  if(counter != lastCounter) {
    //Serial.println(counter);
    lastCounter = counter;
  }
 
  // Debounce button press
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        buttonPressed = true;
      }
    }
  }
  lastButtonState = reading;
}

// Pump Function
void runpump(){
 analogWrite(pump, 255);     // This is the maximum output of an analog pin and essentially tells the pump to run at full speed. We decided to keep the analog function here instead of using digital write to allow others to easily change the pump speed.        
}

// Pump function for cleaning as certain amount of time in minutes
void runpumptime(){ 
 currenttimeclean = millis();
 if ((currenttimeclean - timewhenclicked < (time*60000)))  {
 analogWrite(pump, 255);
 }
 else if (currenttimeclean - timewhenclicked > (time*60000)){
 analogWrite(pump, 0);

 settime = false;  
 }            
 stopcleaning = true;
 
}

// Stop running the pump
void stopclean(){
  analogWrite(pump,0);
}

// set point function
void checksetpoint() {
if(average <= setpoint && !setpointhit) {     // If the sensor value is less then or equal to the setpoint and the boolean setpointhit then execute the code within the {}
  setpointhit = true;                         // Sets the boolean named setpointhit to true, telling the code that the optical density of mid log growth has been achieved
  previousintervalpump = currenttime;         // Resets the interval variable to ensure that it represents the time when the pump was last turned on  
  Serial.println(F("Pump is on"));
  }
  if(setpointhit) {                                                     // If the setpointhit boolean is set to true then execute the code in the {}
    if(currenttime - previousintervalpump < pumponinterval){            // If the current time minus the time when the pump was last on is less then the interval run the pump
     runpump();                                                         // This calls the run pump function
    } else if (currenttime - previousintervalpump >= pumponinterval){   // Otherwise if the current time minus the last time the pump was on is greater then or equal to the amount of time the pump should be on then execute what is in the {}
     analogWrite(pump, 0);                                              // This is the lowest possible value of the analogWrite function and subsequently turns the pump off
    }
 
    if(currenttime - previousintervalpump >= setpointdelay) {           // Asks if the delay or the time between when the program should check the sensor value has been met if that time has been met then execute what is in the {}
      setpointhit = false;                                              // If the time has elapsed, in this case 10 seconds then we no longer know if the setpoint has been hit so we set the variable to false so the program checks the to see if it has been hit again
    }
  } else {
    analogWrite(pump,0);                                                
  }
}




// Stirmotor Function
void stirmotors(){
 analogWrite(stir1, 70);                                     // Powers the stir motors with the specified voltage, ensuring the subsequent vortex does not interfere with OD measurements
}




// OD Measurement Function  
void runsensors(){
if(currenttime - previousintervalsensor >= sensordelay) {    // If the current time minus the last time the sensor value was read is greater then the sensor delay in this case 3 seconds then excecute what is in the {}
   previousintervalsensor = currenttime;                     // Since the interval has been met or exceeded the sensor value is going to be read this means we need to set the previous interval variable to the current time.
   sensor1 = analogRead(sensor1pin);                         // This uses the analog read command to allow us to read the voltage from the sensor pin this voltage is then converted to a range by the microcontroller from 0-1023


   Serial.print(sensor1);                                    // This prints out the sensor's reading to the serial monitor  

 
    //Averaging Function
    sum = sum + sensor1;                                     // Sets the variable sum to the sum of all the readings of the sensor's readings
    counterSensor = counterSensor + 1;                       // Sets the variable counter to what ever the previous value of the variable was plus 1
 
    if (counterSensor == 10) {                               // If the sensor has been read is this case 10 times then the rest of the code executes
      average = (sum/10);                                    // The average variable is set to the sum divided by the number of readings taken
      Serial.print(F(","));                                  // Prints a comma to the serial monitor to make the data in CSV format for easier interpreting later on
      Serial.print(average);                                 // Prints the average variable to the serial monitor
      sum = 0;                                               // Resets the sum
      counterSensor = 0;                                     // Resets the counter
}
   Serial.println();
   }
}
 
// NeoPixel LEDs
void LEDs() {
  pixels.show();
}

// Function to turn the RGB LEDs red
void red() {
  pixels.begin();
  for (int i = 0; i < numleds; i++){
    pixels.setPixelColor(i, RED);
  }
  pixels.show();
}

// Function to turn the RGB LEDs green
void green() {
  pixels.begin();
  for (int i = 0; i < numleds; i++){
    pixels.setPixelColor(i, GREEN);
  }
  pixels.show();
}

// Function to turn the UV LEDs on
void uv() {
    for (int f = numleds; f < numledsuv; f++) {
    pixelsuv.setPixelColor(f,pixelsuv.Color(255,255,255));
    }
  pixelsuv.show();                   
}

// Function to turn the LEDs off
void black(){
pixelsuv.clear();
pixelsuv.show();
pixels.clear();
pixels.show();  

}

// Function to read the rotary encoder 
void read_encoder() {
  static uint8_t oldState = 3;
  static int8_t encoderValue = 0;
  static const int8_t encoderStates[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

  oldState <<= 2;
  if (digitalRead(ENCA)) oldState |= 0x02;
  if (digitalRead(ENCB)) oldState |= 0x01;

  encoderValue += encoderStates[(oldState & 0x0f)];

  if(encoderValue > 3 ) {
    counter += 1;
    encoderValue = 0;
  }
  else if (encoderValue < -3 ) {
    counter -= 1;
    encoderValue = 0;
  }
  //Serial.println(counter);
}

// Function to control the display of the LCD screen
void MenuDisplay() {
    static String lastDisplay = "";  // keeps track of the last text that was displayed
    String currentDisplay = "";

    switch (currentMenu) { 
        // Main menu with the different menu options
        case MAIN_MENU:
            currentDisplay += "Main Menu\n";  // Explicitly add the first line
            if (abs(counter % 5) == 0) {
                currentDisplay += "> Run PACE ";
            } else if (abs(counter % 5) == 1) {
                currentDisplay += "> Change LEDS? ";
            } else if (abs(counter % 5) == 2) {
                currentDisplay += "> Stop System?";
            } else if (abs(counter % 5) == 3) {
                currentDisplay += "> Cleaning";
            } else if (abs(counter % 5) == 4) {
                currentDisplay += "> Calibration";
            }
            break;

        // Main menu for Run PACE
        case SUBMENUSTARTBACK:
            currentDisplay += "Run PACE \n";  // Explicitly add the first line
            if (counter % 2 == 0) {
                currentDisplay += "> Start";
            } else {
                currentDisplay += "> Back";
            }
            break;

        // Main menu for the cleaning function
        case SUBMENUTIME:
            currentDisplay += "Cleaning\n";  // Explicitly add the first line
            if (counter % 2 == 0) {
                currentDisplay += "> Set Time";
            } else {
                currentDisplay += "> Back";
            }
            break;

        // Menu after > Set Time has been selected in the cleaning function main menu    
        case SUBSETTIME:
            if (counter % 2 == 0) {
              if (time > 9) {
                lcd.clear(); }
              lcd.setCursor(0,0);
              lcd.print(F("Time in minutes? "));
              time = abs(counter);
              lcd.setCursor(0,1);
              lcd.print(time);
            }
            else if (abs(counter % 2) == 1) {
              if (time > 9) {
                lcd.clear(); }
              lcd.setCursor(0,0);
              lcd.print(F("Time in minutes?"));
              time = abs(counter);
              lcd.setCursor(0,1);
              lcd.print(time);
            }
            break;

        // Main menu to turn PACE or calibration off
        case SUBPACEOFF:
          lcd.setCursor(0,0);
          lcd.print(F("Stop System?"));
          if (counter % 2 == 0) {
            lcd.setCursor(0,1);
            lcd.print(F("> Yes"));
          }
          else if (abs(counter % 2) == 1) {
            lcd.setCursor(0,1);
            lcd.print(F("> No "));
          }
          break;

        // Main menu to change the LED colors
        case SUBMENUCHANGELEDS:
          if (counter % 2 == 0) {
            lcd.setCursor(0,1);
            lcd.print(F("> Yes"));
          }
          else if (abs(counter % 2) == 1) {
            lcd.setCursor(0,1);
            lcd.print(F("> No "));
          }
          break;
       
        // Menu once > Yes has been clicked in the main menu to change LED colors. Gives color options
        case SUBCHANGELEDS:
          if (counter % 4 == 0) {
            lcd.setCursor(0,1);
            lcd.print(F("> Red              "));
          } else if (abs(counter % 4) == 1) {
            lcd.setCursor(0,1);
            lcd.print(F("> Green           "));
          } else if (abs(counter % 4) == 2) {
            lcd.setCursor(0,1);
            lcd.print(F("> Ultraviolet    "));
          } else if (abs(counter% 4) == 3) {
            lcd.setCursor(0,1);
            lcd.print(F("> Off      "));
          }
          break;
       
        // Main menu to run the calibration
        case SUBCALIBRATION:
          if (counter % 2 == 0) {
            lcd.setCursor(0,0);
            lcd.print(F("Run calibration?"));
            lcd.setCursor(0,1);
            lcd.print(F("> Start  "));
          }
          else if (abs(counter % 2 == 1)) {
            lcd.setCursor(0,0);
            lcd.print(F("Run calibration?"));
            lcd.setCursor(0,1);
            lcd.print(F("> Back    "));            
          }
          break;

    }

    // Only update the display if something has changed
    if (currentDisplay != lastDisplay) {
        lcd.clear();
       
        // Split the display content into two lines manually
        lcd.setCursor(0, 0);  // Move to the first line
        lcd.print(currentDisplay.substring(0, currentDisplay.indexOf('\n')));

        lcd.setCursor(0, 1);  // Move to the second line
        lcd.print(currentDisplay.substring(currentDisplay.indexOf('\n') + 1));
        lastDisplay = currentDisplay;
    }
}

// Function to change the menus once the rotary encoder button has been clicked
void handleButtonPress() {
  switch (currentMenu) {
    case MAIN_MENU:
      if (counter % 5 == 0) {
        currentMenu = SUBMENUSTARTBACK;
      }
        else if(abs(counter % 5) == 1) {
        currentMenu = SUBCHANGELEDS;
      }
        else if(abs(counter % 5) == 2) {
        currentMenu = SUBPACEOFF;
        }
        else if(abs(counter % 5) == 3) {
        currentMenu = SUBMENUTIME;
        }
        else if(abs(counter % 5) == 4) {
        currentMenu = SUBCALIBRATION;
        }
      counter = 0;
      break;
    case SUBMENUSTARTBACK:
      if (counter % 2 == 0) {
     
        runsystem = true;
        currentMenu = MAIN_MENU;
      } else {
        currentMenu = MAIN_MENU;
      }
    break;
    case SUBMENUTIME:
      if (counter % 2 == 0) {
        currentMenu = SUBSETTIME;

      } else if (abs(counter % 2) == 1){
        currentMenu = MAIN_MENU;
      }
      break;
    case SUBPACEOFF:
      if (counter % 2 == 0) {
        runsystem = false;

        runcalibration =false;
        //turnred=false;
        //turngreen=false;
        //turnuv=false;
        //turnblack=true;
        currentMenu = MAIN_MENU;
      } else {
        currentMenu = MAIN_MENU;
      }
      break;
      
    case SUBSETTIME:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Cleaning");
        lcd.setCursor(9,0);
        lcd.print(time);
        lcd.setCursor(10,0);
        lcd.print(F("min..."));

        lcd.setCursor(0,1);
        lcd.print("Click to stop");

      timewhenclicked = millis();
      settime = true;
      stopcleaning = false;
      currentMenu = SUBSTOPCLEANING;
      
      break;

    case SUBMENUCHANGELEDS:
      if (counter % 2 == 0) {
        currentMenu = SUBCHANGELEDS;
      } else if (abs(counter % 2) == 1) {
        currentMenu = MAIN_MENU;
      }
      break;

    case SUBCHANGELEDS:
      if (counter % 4 == 0) {
        turnred = true;
        turnblack=false;
        turngreen=false;
        turnuv=false;
        pixelsuv.clear();
        pixelsuv.show();
        currentMenu = MAIN_MENU;
      } else if (abs(counter % 4) == 1) {
        
        turngreen = true;
        turnred=false;
        turnuv=false;
        turnblack=false;
        currentMenu = MAIN_MENU;
        pixelsuv.clear();
        pixelsuv.show();
      } else if (abs(counter % 4) == 2) {
        turnred=false;
        turngreen=false;
        turnblack=false;
        turnuv= true;
        pixels.clear();
        pixels.show();
        currentMenu = MAIN_MENU;
      } else if (abs(counter % 4) == 3) {
        turnred = false;
        turngreen = false;
        turnuv=false;
        turnblack=true;
        pixelsuv.clear();
        pixelsuv.show();
        currentMenu = MAIN_MENU;
      }
      break;
    case SUBCALIBRATION:
      if (counter % 2 == 0) {
        runcalibration = true;
        runsystem = false;
        currentMenu = MAIN_MENU;
      } else if (abs(counter % 2) == 1) {
        currentMenu = MAIN_MENU;
      }
      break;

    case SUBSTOPCLEANING:
      lcd.clear();
      stopcleaning = true;
      settime = false;
      currentMenu = MAIN_MENU;
      break;
  }
}
