#include <SoftwareSerial.h>
SoftwareSerial mySerial(1, 0); // RX, TX

const int lewa = 5;
const int prawa = 4;
void setup()
{
Serial1.begin(9600);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
}



void loop()
{
  int pomiarp = analogRead(A0);
  int pomiarl = analogRead(A1);

    Serial1.println("Prawy:");
    Serial1.println(pomiarp);
    Serial1.println("Lewy:");
    Serial1.println(pomiarl);

    int roznica = pomiarp - pomiarl;
    
    if (roznica < -100)
    {
        digitalWrite(lewa, HIGH);
    }
    else if (roznica > 100)
    {
        digitalWrite(prawa, HIGH);
    }
    else if (-200 < roznica < 200 && pomiarp > 50 && pomiarl > 50)
    {
        digitalWrite(lewa, HIGH);
        digitalWrite(prawa, HIGH);
    }
    else 
    {
        digitalWrite(lewa, LOW);
        digitalWrite(prawa, LOW);
    }
    

    
    delay(300);
}
