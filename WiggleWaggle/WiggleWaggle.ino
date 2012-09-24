/*
*/

#include <IRremote.h>

int L1 = 10;
int L2 = 11;
int L3 = 5;
int L4 = 3;
int FULL = 255;
int ZERO = 0;
int SLOW = 127;
int FWD_INCR = 5;
int fwdSpeed = SLOW;
int bwdSpeed = SLOW;

int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;


// the setup routine runs once when you press reset:
void setup()  { 
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    stop();
    delay(2000);  // delay to reduce inertia before changing directions
    switch (results.value) {
    case 0xFDBD8C7D:
    case 0x09005059:
       Serial.println("play");
       forward(); 
       break;       
    case 0x3C0A67AF:
    case 0x09000009:
       Serial.println("stop");
       stop(); 
       break;
    case 0x512C81BF:
    case 0x0900C0C9:
      faster();
      Serial.println(fwdSpeed);
      break;
    case 0x99C67A57:
    case 0x09004049:
      slower();
      Serial.println(fwdSpeed);
      break;
    case 0x09000801:
      Serial.println("wiggle");
      wiggle();
      break;
    case 0x09008881:
      Serial.println("waddle");
      waddle();
      break;
    case 0x09004841:
      Serial.println("spin left");
      spinLeft();
      break;
    case 0x0900C8C1:
      Serial.println("spin right");
      spinRight();
      break;      
    default:
      break;
    }
    irrecv.resume(); // Receive the next value
  }
}

void forward() {
  fwdLeft();
  fwdRight();
}

void slower() {
  if (fwdSpeed < 255) {
    fwdSpeed -= FWD_INCR;
    forward();
  }
}

void faster() {
  if (fwdSpeed > 0) {
    fwdSpeed += FWD_INCR;
    forward();
  }
}

void wiggle() {
  fwdSpeed = random(FULL, FULL-20);  
  bwdSpeed = random(FULL, FULL-20);  
//  Serial.println(fwdSpeed);
  for (int i=0; i < 10; i++) {
  stop(); 
  delay(random(150,220)); 
  spinLeft(); 
  delay(random(150,220)); 
  stop();
  delay(random(150,220));
  spinRight();
  delay(random(150,220));
  }
  stop();
}

void spinLeft() {
  fwdRight();
  bwdLeft();
}

void spinRight() {
  fwdLeft();
  bwdRight();
}

void waddle() {
  fwdSpeed = FULL;
  for (int i=0; i < 10; i++) {
    waddleLeft();
    delay(1000);
    waddleRight();
    delay(1000);
  }
}

void waddleLeft() {
  stopRight();
  fwdLeft();
  delay(300);
  stopLeft();
  fwdRight();
  delay(300);
  stopRight();
  fwdLeft();
  delay(300);
  stopLeft();
  fwdRight();
  delay(300);
  stopRight();
}

void waddleRight() {
  stopLeft();
  fwdRight();
  delay(300);
  stopRight();
  fwdLeft();
  delay(300);
  stopLeft();
  fwdRight();
  delay(300);
  stopRight();
  fwdLeft();
  delay(300);
  stopLeft();
}

void fwdLeft() {
  analogWrite(L3, FULL-fwdSpeed);    
  digitalWrite(L4, HIGH);    // H=fwd, L=bwd
}

void fwdRight() {
  analogWrite(L1, fwdSpeed);    
  digitalWrite(L2, LOW);   // H=fwd, L=bwd
}

void bwdLeft() {
  analogWrite(L3, fwdSpeed);    
  digitalWrite(L4, LOW);    // H=fwd, L=bwd
}

void bwdRight() {
  analogWrite(L1, FULL-fwdSpeed);    
  digitalWrite(L2, HIGH);   // H=fwd, L=bwd
}

void stopLeft() {
  analogWrite(L3, 0);    
  digitalWrite(L4, LOW);
}

void stopRight() {
  analogWrite(L1, 0);    
  digitalWrite(L2, LOW);
}

void stop() {
  analogWrite(L3, 0);    
  digitalWrite(L4, LOW);
  analogWrite(L1, 0);    
  digitalWrite(L2, LOW);
}


