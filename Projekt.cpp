/*
    Zrobił to Grzybek żeby nie oblać z pneumatyki bo nie chciało mu się rysować tych schematów...
*/
//załączenie niezbędnych bibliotek
#include <SoftwareSerial.h>
#include <NewPing.h>
#include <DHT.h>

//Zdefiniowanie pinów RX i TX
SoftwareSerial mySerial(1, 0); // RX, TX

//cewka K1...K8, której sterowanie wpięte jest do wyjścia cyfrowego arduino nr.: 3...10
    const int K1=3; //lampa
    const int K2=4;
    const int K3=5;
    const int K4=6;
    const int K5=7;
    const int K6=8;
    const int K7=9;
    const int K8=10;

//czujniki cyfrowe S1...S4 wpięte do wejść cryfowycj arduino nr.:2, 11...13
    const int DHT11_PIN=2; //Czujnik temperatury i wilgotności DHT11
    const int S2=11;
    const int Trig=12; //Czujnik ultradźwiękowy HC-SR04 pin trig
    const int Echo=13; //Czujnik ultradźwiękowy HC-SR04 pin echo
    
//czujniki analogowe SA1...SA6 wpiętę do wejść analogowych arduino nr.: A0...A5const int SA1=A0; //Czujnik natężenia światła PT550
    const int SA1=A0; //Czujnik światła (fotorezystor)
    const int SA2=A1;
    const int SA3=A2;
    const int SA4=A3;
    const int SA5=A4;
    const int SA6=A5;
    
//zdefiniowanie zmiennej 'light_intensity', w której przetrzymywana jest informacja o natężeniu światła
    int light_intensity=0;

//Konfiguracja czujnika ultradźwiękowego zgodnie z biblioteką NewPing.h Zdefiniowanie zmiennej 'distance', w której przetrzymywana jest informacja o odległości ciała od czujnika
    #define Max_Distance 400
    NewPing sonar(Trig, Echo, Max_Distance); 
    int distance=0;

//Konfiguracja czujnika temperatury i wilgotności DHT11
DHT dht;

//zdefiniowanie zmiennej, która jest wysyłana poprzez bluetooth
int state;

boolean sterowanie;

void setup() {                
  pinMode(K1, OUTPUT); //zdefiniowanie I/O 3..10 cyfrowych jako wyjścia
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  pinMode(K4, OUTPUT);
  pinMode(K5, OUTPUT);
  pinMode(K6, OUTPUT);
  pinMode(K7, OUTPUT);
  pinMode(K8, OUTPUT);
  
 
  Serial1.begin(9600);

  dht.setup(DHT11_PIN);
  }


void loop() {

 if(sterowanie = true)
 {
    automatyka();
 }
 else if (sterowanie = false)
 {
    reczne();
 }
 
 
  
  //For debugging purpose
  //Serial1.println(state);
}

void automatyka()
{
    //Dokonanie odczytu z czujników, zapis wartości do zmiennych i włączenie odpowiednich cewek

    //Światło
    light_intensity = analogRead(A0); //Odczyt natężenia światła z czujnika PT550 wpiętego do A0

        if (light_intensity >= 700) //rozdzielczość na pinach analogowych w arduino wynosi 10 bitów, co znaczy że będziemy odbierać na nich liczby w zakresie <0, 1023> gdzie 0=0V a 1023=5V
        {
            digitalWrite(K1, HIGH);
        }
        else
        {
            digitalWrite(K1, LOW);
        }

    //Temperatura i Wilgotność
    int wilgotnosc = dht.getHumidity(); //Pobranie informacji o wilgotnosci
    int temperatura = dht.getTemperature(); //Pobranie informacji o temperaturze

    //Odległość
    unsigned int uS = sonar.ping(); //Odczyt z czujnika ultradźwiękowego HC-SR04  
    distance = uS / US_ROUNDTRIP_CM; //Przetworzenie dostarczonego sygnały na odległość w [cm]

}
//Funkcja ręcznego załączania cewek
void reczne()
{
    if(Serial1.available() > 0){     
      state = Serial1.read();}
  
  switch(state)
  {
    case 'a': digitalWrite(K1, LOW); break;
    case 'b': digitalWrite(K1, HIGH); break;
    
    case 'c': digitalWrite(K2, LOW); break;
    case 'd': digitalWrite(K2, HIGH); break;
    
    case 'e': digitalWrite(K3, LOW); break;
    case 'f': digitalWrite(K3, HIGH); break;
    
    case 'g': digitalWrite(K4, LOW); break;
    case 'h': digitalWrite(K4, HIGH); break;
    
    case 'i': digitalWrite(K5, LOW); break;
    case 'j': digitalWrite(K5, HIGH); break;
    
    case 'k': digitalWrite(K6, LOW); break;
    case 'l': digitalWrite(K6, HIGH); break;
    
    case 'm': digitalWrite(K7, LOW); break;
    case 'n': digitalWrite(K7, HIGH); break;
    
    case 'o': digitalWrite(K8, LOW); break;
    case 'p': digitalWrite(K8, HIGH); break;
    
    default: break;
  }
}