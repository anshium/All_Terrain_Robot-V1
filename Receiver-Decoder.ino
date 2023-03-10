//Receiver Decoder. Input 4-bit signal to moto control
//(Receiver Code)
//8th March, 2023
//The goal is to write code that would control the 8 motors based on the command received
//The 8 motors consist of 6 DC motors that would be affected only by forward or backward commands.
//The remaining two servos would rotate depending on the input signal received.

//Including the Servo library
#include <Servo.h>

//Defining servo objects
Servo forward_servo;
Servo backward_servo;

//Servo default position (degrees)
float servo_default_angle = 90;

//Maximum and minimum angle servos can rotate (degrees)
int max_servo_angle = 145;
int min_servo_angle = 180 - max_servo_angle;

//current angle of servo motors
float forward_servo_angle = 90;
float backward_servo_angle = 90;

//Pins for DC motors
int dcm_pin1 = 3;
int dcm_pin2 = 4;

//Defining pins for input signals from receiver
int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 11;

//Defining the input signals from receiver
int data1;
int data2;
int data3;
int data4;

//time
int t = 0;
int t_scaled;
//driver code
void setup() {
  //The input pins should ben defined as input
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  
  //Servos to be attached to two pwm pins
  forward_servo.attach(5);
  backward_servo.attach(6);

  //The output pins for DC motors
  pinMode(dcm_pin1, OUTPUT);
  pinMode(dcm_pin2, OUTPUT);
  
  //On start or reset, set the Servo motors to default position
  forward_servo.write(servo_default_angle);
  backward_servo.write(servo_default_angle);

  //also stop all DC motors
  digitalWrite(dcm_pin1, LOW);
  digitalWrite(dcm_pin2, LOW);

  Serial.begin(9600);
}

void loop() {
  //reading the received signal as input
  data1 = digitalRead(in1);
  data2 = digitalRead(in2);
  data3 = digitalRead(in3);
  data4 = digitalRead(in4);

  if(data1 == 0 && data2 == 0 && data3 == 0 && data4 == 0){
    //put all 6 DC motos to rest
    //On start or reset, set the Servo motors to default position
    digitalWrite(dcm_pin1, LOW);
    digitalWrite(dcm_pin2, LOW);
  } else{
    //first giving commands to the 6 motors
    if(data2 == 1){ //if motion
      if(data1 == 0){
        //forward
        digitalWrite(dcm_pin1, HIGH);
        digitalWrite(dcm_pin2, LOW);
      } else if(data1 == 1){
        //backward
        digitalWrite(dcm_pin1, LOW);
        digitalWrite(dcm_pin2, HIGH);
      }
    } else{ //else shut these motors off
      digitalWrite(dcm_pin1, LOW);
      digitalWrite(dcm_pin2, LOW);
    }

    //now for turning commands
    if(data4 == 1){
      if(data3 == 0){
        //left
        if((servo_default_angle + t_scaled <= max_servo_angle) && (servo_default_angle - t_scaled >= min_servo_angle)){
          forward_servo.write(servo_default_angle + t_scaled);
          backward_servo.write(servo_default_angle - t_scaled);
        }
      } else if(data4 == 1){
        //right
        if((servo_default_angle + t_scaled <= max_servo_angle) && (servo_default_angle - t_scaled >= min_servo_angle)){
          forward_servo.write(servo_default_angle - t_scaled);
          backward_servo.write(servo_default_angle + t_scaled);
        }
      }
    } else{
      //else just relax B)
    }
  }

  //serial monitor print
  Serial.print(data4);
  Serial.print(data3);
  Serial.print(data2);
  Serial.print(data1);
  Serial.println();
  
  t++;
  t_scaled = t/60;
}
