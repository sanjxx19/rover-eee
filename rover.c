#include <AFMotor.h>
#include <Servo.h>

double distanced;
double distancef;
const int ground = 15;
const int obstacle = 15;
const int right = 20;
const int left = 160;
const int front = 90;
const int turn = 2700;
boolean hole;
boolean wall;

const int trigd = A5;
const int echod = A4;
const int trigf = A1;
const int echof = A0;

AF_DCMotor backLeft(1);
AF_DCMotor backRight(2);
AF_DCMotor frontRight(3);
AF_DCMotor frontLeft(4);

Servo servo;

void setup(){
  pinMode(trigd, OUTPUT);
  pinMode(trigf, OUTPUT);
  pinMode(echod, INPUT);
  pinMode(echof, INPUT);

  backLeft.setSpeed(0);
  backRight.setSpeed(0);
  frontRight.setSpeed(0);
  frontLeft.setSpeed(0);

  backLeft.run(RELEASE);
  backRight.run(RELEASE);
  frontRight.run(RELEASE);
  frontLeft.run(RELEASE);

  servo.attach(9);
  servo.write(front);

  Serial.begin(9600);
}

void loop(){

  distancef = measureFront();
  distanced = measureDown();

  if(distanced>ground){
    hole = true;
  }
  else{
    hole = false;
  }

  if(distancef<obstacle){
    wall = true;
  }
  else{
    wall = false;
  }

  if(!hole&&!wall){
    moveForward();
  }
  else{
    moveStop();
    delay(2000);
    moveBackward();
    delay(3000);
    moveStop();

    servo.write(right);
    delay(400);
    double distancer = measureFront();
    delay(400);

    servo.write(left);
    delay(400);
    double distancel = measureFront();
    delay(400);

    servo.write(front);

    if(distancer>distancel){
      moveRight();
      delay(turn);
      moveStop();
    }
    else{
      moveLeft();
      delay(turn);
      moveStop();
    }

  }
  Serial.println(hole + " " + wall);
}

void moveForward(){
  backLeft.setSpeed(150);
  backRight.setSpeed(150);
  frontRight.setSpeed(190);
  frontLeft.setSpeed(190);

  backLeft.run(FORWARD);
  backRight.run(FORWARD);
  frontRight.run(FORWARD);
  frontLeft.run(FORWARD);
}

void moveBackward(){
  backLeft.setSpeed(150);
  backRight.setSpeed(150);
  frontRight.setSpeed(190);
  frontLeft.setSpeed(190);

  backLeft.run(BACKWARD);
  backRight.run(BACKWARD);
  frontRight.run(BACKWARD);
  frontLeft.run(BACKWARD);
}

void moveLeft(){
  backLeft.setSpeed(150);
  backRight.setSpeed(150);
  frontRight.setSpeed(190);
  frontLeft.setSpeed(190);

  backLeft.run(BACKWARD);
  backRight.run(FORWARD);
  frontRight.run(FORWARD);
  frontLeft.run(BACKWARD);
}

void moveRight(){
  backLeft.setSpeed(150);
  backRight.setSpeed(150);
  frontRight.setSpeed(190);
  frontLeft.setSpeed(190);

  backLeft.run(FORWARD);
  backRight.run(BACKWARD);
  frontRight.run(BACKWARD);
  frontLeft.run(FORWARD);
}

void moveStop(){
  backLeft.setSpeed(0);
  backRight.setSpeed(0);
  frontRight.setSpeed(0);
  frontLeft.setSpeed(0);

  backLeft.run(RELEASE);
  backRight.run(RELEASE);
  frontRight.run(RELEASE);
  frontLeft.run(RELEASE);
}

double measureDown(){

  double durationd, sampled[5];

  for(int i=0;i<5;i++){
    digitalWrite(trigd, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigd, LOW);
    durationd = pulseIn(echod, HIGH);
    sampled[i] = 0.017 * durationd;
  }

  for(int i=0;i<4;i++){
    for(int j=i+1;j<5;j++){
      double swapd = sampled[i];
      sampled[i] = sampled[j];
      sampled[j] = swapd;
    }
  }

  return sampled[2];
}

double measureFront(){

  double durationf, samplef[5];

  for(int i=0;i<5;i++){
    digitalWrite(trigf, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigf, LOW);
    durationf = pulseIn(echof, HIGH);
    samplef[i] = 0.017 * durationf;
  }

  for(int i=0;i<4;i++){
    for(int j=i+1;j<5;j++){
      double swapf = samplef[i];
      samplef[i] = samplef[j];
      samplef[j] = swapf;
    }
  }

  return samplef[2];
}