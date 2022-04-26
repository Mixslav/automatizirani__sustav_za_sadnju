// Seminarski rad AUTOMATIZIRANI SUSTAV ZA SADNJU BILJAKA U KONTROLIRANIM UVJETIMA
// Mislav Stipić 2018


// Which pins are connected to which LED
const byte light_relay = 8;     //relay for lights
const int cell_input = 12;      //cellphone input for lights
const byte temp_indicator = 9;   //indicator temp 

//vent
const int vent_input = 10;     //input for vent 
const int mod_1 = 5;       // fastest mod
const int mod_2 = 3;       // fast mod
const int mod_3 = 4;       // slow mod

// Time periods of blinks in milliseconds (1000 to a second).
const unsigned long light_check_interval = 50;
const unsigned long temp_vent_interval = 60;
const unsigned long vent_interval = 1;

int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState1 = 0;         // current state of the button
int lastButtonState1 = 0;     // previous state of the button

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 100;

// Variable holding the timer value so far. One for each "Timer"
unsigned long timer_1;
unsigned long timer_2;
unsigned long timer_3;

void setup () 
  {
  pinMode (light_relay, OUTPUT);  // inputs and outputs
  pinMode (cell_input , INPUT);
  pinMode (vent_input , INPUT);
  pinMode (temp_indicator, OUTPUT);
  pinMode (mod_1, OUTPUT);
  pinMode (mod_2, OUTPUT);
  
  timer_1 = millis (); // timers
  timer_2 = millis ();
  timer_3 = millis ();
  Serial.begin(9600); // baud rate for USB connection
 
  }  // end of setup

void regul_vent()
{

  // read the pushbutton input pin:
  buttonState1 = digitalRead(vent_input);

  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1 == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
    }
    // Delay a little bit to avoid bouncing
    delay(20);
  }

  // save the current state as the last state,
  //for next time through the loop
  lastButtonState1 = buttonState1;


  if(buttonPushCounter == 0)    // vent mod off
    digitalWrite(mod_1, LOW);
    digitalWrite(mod_2, LOW);
    digitalWrite(mod_3, LOW);

  if (buttonPushCounter == 1) { // vent mode speed 1
    digitalWrite(mod_1, HIGH);
    digitalWrite(mod_2, LOW);
    digitalWrite(mod_3, LOW);
  }
    if (buttonPushCounter == 2) { // vent mode speed 2
    digitalWrite(mod_2, HIGH);
    digitalWrite(mod_1, LOW);
    digitalWrite(mod_3, LOW);
  }
    if (buttonPushCounter == 3) { // vent mode speed 3
    digitalWrite(mod_3, HIGH);
    digitalWrite(mod_2, LOW);
    digitalWrite(mod_1, LOW);
  }
   
  if (buttonPushCounter > 3)  // counter reset
  {
    buttonPushCounter=0;
    }


   timer_3 = millis (); // save current time
}


void light_controll () // input for cellphone for light controll
  {
  // read the state of the switch into a local variable:
  int reading = digitalRead(cell_input);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the light state
  digitalWrite(light_relay, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  
  lastButtonState = reading;
  timer_1 = millis ();  
  }  // end of light_controll

void temp_vent () // temperature and ventilation
  {
     int sensorValue = analogRead(A0);
     
       float voltage_t = sensorValue * (5.0 / 1023.0);
       Serial.println(voltage_t);

    if(voltage_t >= 3.37)
    {
      digitalWrite(temp_indicator , HIGH);
      }
       else
       {
         digitalWrite(temp_indicator , LOW);
        } 
       
 
  timer_2 = millis ();  
  }  // end of temp_vent

void loop ()
  {

  // Handling the blink of one LED.
  if ( (millis () - timer_1) >= light_check_interval)
     light_controll();

  // The other LED is controlled the same way. Repeat for more LEDs
  if ( (millis () - timer_2) >= temp_vent_interval) 
    temp_vent();

  if ( (millis () - timer_3) >= vent_interval) 
    regul_vent();



}  // end of loop
