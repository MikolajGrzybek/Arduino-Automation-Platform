#define enA 9
#define in1 2
#define in2 3   
#define in3 4
#define in4 5
#define enB 10

class Motor{

int enablePin;
int directionPin1;
int directionPin2;
public:
 
//Method to define the motor pins
Motor(int ENPin,int dPin1,int dPin2){
enablePin = ENPin;
directionPin1 = dPin1;
directionPin2 = dPin2;
};
 
//Method to drive the motor 0~255 driving forward. -1~-255 driving backward
Drive(int speed){
if(speed>=0){
digitalWrite(directionPin1, LOW);
digitalWrite(directionPin2, HIGH);
}
else{
digitalWrite(directionPin1, HIGH);
digitalWrite(directionPin2, LOW);
speed = - speed;
}
analogWrite(enablePin, speed);
}
};
Motor leftMotor = Motor(enA, in1, in2);
Motor rightMotor = Motor(enB, in3, in4);
void setup()
{
pinMode(enA, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
// Set initial direction and speed
digitalWrite(enA, LOW);
digitalWrite(enB, LOW);
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}



void loop()
{
  int pomiarp = analogRead(A0);
  int pomiarl = analogRead(A1);

    int roznica = pomiarp - pomiarl;
    
    if (roznica < -100)
    {
        leftMotor.Drive(100);
        rightMotor.Drive(-100);
     }
    else if (roznica > 100)
    {
        leftMotor.Drive(-100);
        rightMotor.Drive(100);      
    }
    else if (-200 < roznica < 200 && pomiarp > 50 && pomiarl > 50)
    {
        leftMotor.Drive(100);
        rightMotor.Drive(100);
    }
    else 
    {
        leftMotor.Drive(0);
        rightMotor.Drive(0);
    }
}
