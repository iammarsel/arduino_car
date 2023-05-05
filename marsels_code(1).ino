#include <Servo.h>//include servo library
Servo servo;//create object
int Spoint = 95; //servo center point
int dis;
//motor 0ne
#define ENA 5
#define IN1 7
#define IN2 8
//motor two
#define ENB 6
#define IN3 10
#define IN4 11
void setup() {
  Serial.begin(9600);//enable serial monitor
  pinMode(3, OUTPUT);//trig pin
  pinMode(4, INPUT);//echo pin
  pinMode(ENA, OUTPUT);//motor pins
  pinMode(ENB, OUTPUT);//motor pins
  pinMode(IN1, OUTPUT);//motor pins
  pinMode(IN2, OUTPUT);//motor pins
  pinMode(IN3, OUTPUT);//motor pins
  pinMode(IN4, OUTPUT);//motor pins
  servo.attach(9);//define servo pin
  servo.write(Spoint);//servo motor start point
  delay(2000);
}
int distance() {
  digitalWrite(3, LOW);
  delayMicroseconds(4);
  digitalWrite(3, HIGH);
  delayMicroseconds(10);
  digitalWrite(3, LOW);
  int t = pulseIn(4, HIGH);//input pulse and save it veriable
  int cm = t / 29 / 2; //time convert distance
  return cm;
}
// motors function to move the car (B for backwards, F for forward, L for left, R for Right)
void motors(char action, int Speed) {
  if (action == 'B') {
    digitalWrite(ENA, Speed);
    digitalWrite(ENB, Speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (action == 'F') {
    digitalWrite(ENA, Speed);
    digitalWrite(ENB, Speed);
    digitalWrite(IN1, HIGH); // both wheels on
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); // both wheels on
    digitalWrite(IN4, LOW);
  } else if (action == 'L') {
    digitalWrite(ENA, Speed);
    digitalWrite(ENB, Speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); // turn wheel backward
    digitalWrite(IN4, LOW);
    servo.write(Spoint);
  } else if (action == 'R') {
    digitalWrite(ENA, Speed);
    digitalWrite(ENB, Speed);
    digitalWrite(IN1, HIGH); // turn wheel forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    servo.write(Spoint);
  } else if (action == 'S') {
    digitalWrite(ENA, Speed);
    digitalWrite(ENB, Speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); // stop all motors
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}
void loop (){
  analogWrite(ENA, 200); // Run in less than full speed
  analogWrite(ENB, 200); // Run in less than full speed (200/255)
  delay(100);
  dis = distance();
  Serial.print("dis value: ");
  Serial.print(dis);
  Serial.print("\n");
  if (dis < 35){
    Serial.print("option 1 \n");
    // stop movement
    motors('S', 0);
    delay(1000);
    // check left and right side for walls
    servo.write(180);
    int leftDistance = distance();
    delay(1000);
    servo.write(0);
    int rightDistance = distance();
    delay(1000);
    servo.write(Spoint);
    // if there is space, turn where there is more room to move
    if (rightDistance > 25 || leftDistance > 25){
      if (rightDistance > leftDistance) {
        Serial.print("right turn \n");
        motors('R', 50);
        delay(350);
        motors('S',0);
        delay(1000);
        //motors('F',200);
      } else {
        Serial.print("right turn \n");
        motors('L', 50);
        delay(350);
        motors('S',0);
        delay(1000);
        //motors('F',200);
      }
    }
  // look for new direction to turn to and rotate there
  } else {
    // keep moving forward
    Serial.print("option 2 \n");
    motors('F', 50);
  }
}