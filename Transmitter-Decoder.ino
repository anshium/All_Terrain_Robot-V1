//Joystick analog to 4 bit encoder code
//(Transmitter Code)
//8th March, 2023
//The goal is to write code that would take the analog inputs of both the coordinates of two joystick and convert it into a 4-bit signal. 
//One joystick would control the forward and the backward motion, that is four center wheels and the other would control whether or not to
//rotate the back and front servos.

/*Macros to define joystick inputs
//First Joystick (Only Y would be used)#redundancy but easier to control with this control design
#define JS1X A0 //First Joystick X-Coordinate
#define JS1Y A1 //First Joystick Y-Coordinate

//Second Joystick (Only X would be used)
#define JS2X A2 //Second Joystick X-Coordinate
#define JS2Y A3 //Second Joystick Y-Coordinate
*/

//rules
/** LSB->FWD(0)/BKW(1), LSB + 1-> Motion(1)/No-Motion(0), LSB + 2-> Left Turning(0)/Right Turning(1), LSB + 3-> Turning(1)/No-Turning(0)
 * Don't acre states are treated as 0
 * FWD No-Turning: 0010
 * BKW No-Turning: 0011
 * FWD Turning Left: 1010
 * FWD Turning Right: 1110
 * BKW Turning Left: 1011
 * BKW Turning Right: 1111
 * Turning Left No Motion: 1000
 * Turning Right No Motion: 1100
 * **/

//Global center value holding identifiers
float JS1Y_init_val;
float JS2X_init_val;

//Global Variables holding pin numbers of output signals
int out1 = 5;
int out2 = 6;
int out3 = 7;
int out4 = 8;

//Global Variables holding bit values of output signals (4-bit) (zero or one)
int aus1;
int aus2;
int aus3;
int aus4;

//Global buffer value, after which the signal should be high
float buff = 100;

//Normalised Values of Joystick inputs
float nor_JS1Y;
float nor_JS2X;

//driver code
void setup() {
  //initialise the pins of Joystick as inputs
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  //Initialise output pins
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  
  //initialise the center-most values of the joystick. This code assumes that the joysticks were not pulled during initialisation.
  //Initialisation happens everytime Arduino is reset.
  JS1Y_init_val = analogRead(A0);
  JS2X_init_val = analogRead(A2);

  //Serial.begin()
  Serial.begin(9600);
}

void loop() {
  //Serial.println(analogRead(A0) - JS1Y_init_val);
  //normalized values of joystick inputs
  nor_JS1Y = analogRead(A0) - JS1Y_init_val;
  nor_JS2X = analogRead(A2) - JS2X_init_val;

  //LSB + 1 (Motion or No-Motion Handling)
  if(abs(nor_JS1Y) >= buff)
  {
    aus2 = 1;
  } else{ //no motion
    aus2 = 0;
    aus1 = 0; //It is actually a don't-care state, so used 0 as a convention
  }

  //LSB + 3 (Turning or No-Turning Handling)
  if(abs(nor_JS2X) >= buff)
  {
    aus4 = 1;
  } else{ //no motion
    aus4 = 0;
    aus3 = 0; //It is actually a don't-care state, so used 0 as a convention
  }

  if(nor_JS1Y >= buff){
    aus1 = 0; //FWD
  } else if(nor_JS1Y <= -buff){
    aus1 = 1; //BWD
  }
  
  if(nor_JS2X >= buff){
    aus3 = 0; //Left
  } else if(nor_JS2X <= -buff){
    aus3 = 1; //Right
  }

  //writing them
  digitalWrite(out1, aus1);
  digitalWrite(out2, aus2);
  digitalWrite(out3, aus3);
  digitalWrite(out4, aus4);

  Serial.print(aus4);
  Serial.print(aus3);
  Serial.print(aus2);
  Serial.print(aus1);
  Serial.println();
}
