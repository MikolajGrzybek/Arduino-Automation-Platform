/*
    Zrobił to Grzybek żeby nie oblać z pneumatyki bo nie chciało mu się rysować tych schematów...
    @2019
*/

//załączenie niezbędnych bibliotek
#include <SoftwareSerial.h>
#include <NewPing.h>
//#include <DHT.h>

//Zdefiniowanie pinów RX i TX
SoftwareSerial mySerial(1, 0); // RX, TX

//cewka K1...K8, której sterowanie wpięte jest do wyjścia cyfrowego arduino nr.: 3...10
    const int K1=3; //lampa
    const int K2=4; //brama
    const int K3=5;
    const int K4=6;
    const int K5=7;
    const int K6=8;
    const int K7=9;
    const int K8=10;

//czujniki cyfrowe S1...S4 wpięte do wejść cryfowycj arduino nr.:2, 11...13
    //const int DHT11_PIN=2; //Czujnik temperatury i wilgotności DHT11
    const int S2=11;
    const int Trig=12; //Czujnik ultradźwiękowy HC-SR04 pin trig
    const int Echo=13; //Czujnik ultradźwiękowy HC-SR04 pin echo
    
//czujniki analogowe SA1...SA6 wpiętę do wejść analogowych arduino nr.: A0...A5const int SA1=A0; //Czujnik natężenia światła PT550
    const int SA1=A0; //Czujnik światła (fotorezystor)
    const int SA2=A1; //Krańcówka
    const int SA3=A2;
    const int SA4=A3;
    const int SA5=A4;
    const int SA6=A5;

//zdefiniowanie zmiennej 'krańcówka' w której jest przetrzymowana informacja o tym czy brama otworzyła się do końca
int limit_sensor=0;
//zdefiniowanie zmiennej 'light_intensity', w której przetrzymywana jest informacja o natężeniu światła
    int light_intensity=0;

//Konfiguracja czujnika ultradźwiękowego zgodnie z biblioteką NewPing.h Zdefiniowanie zmiennej 'distance', w której przetrzymywana jest informacja o odległości ciała od czujnika
    #define Max_Distance 400
    NewPing sonar(Trig, Echo, Max_Distance); 
    int distance=0;

//Konfiguracja czujnika temperatury i wilgotności DHT11
//DHT dht;

//zdefiniowanie zmiennej, która jest wysyłana poprzez bluetooth
int state;

boolean sterowanie;

void setup() 
{                
  pinMode(K1, OUTPUT); //zdefiniowanie I/O 3..10 cyfrowych jako wyjścia
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  pinMode(K4, OUTPUT);
  pinMode(K5, OUTPUT);
  pinMode(K6, OUTPUT);
  pinMode(K7, OUTPUT);
  pinMode(K8, OUTPUT);
  
  Serial1.begin(9600);

 //dht.setup(DHT11_PIN);
 sterowanie = true;
  }


void loop() 
{
  //Odczyt danych z bluetooth
  state = Serial1.read();
 
  //Wybór trybu pracy
  if(state == '1') 
  {
    sterowanie = true;
  }
  
  else if (state == '2')
  {
    sterowanie = false;
  }

//wykonanie funkcji dla odpowiednich trybów pracy
if(sterowanie == true)
{
    automatyka();
}
else if (sterowanie == false)
 {
     reczne();
 }
  //Na potrzeby debugowania
  //Serial1.println(sterowanie);
delay(100);
}

void automatyka()
{
    //Na potrzeby debugowania
    Serial1.println("Dziala automat");
    //Dokonanie odczytu z czujników, zapis wartości do zmiennych i włączenie odpowiednich cewek

    //Lampa
    light_intensity = analogRead(A0); //Odczyt natężenia światła z czujnika PT550 wpiętego do A0

     if (light_intensity < 700) //rozdzielczość na pinach analogowych w arduino wynosi 10 bitów, co znaczy że będziemy odbierać na nich liczby w zakresie <0, 1023> gdzie 0=0V a 1023=5V
     {
         digitalWrite(K1, HIGH);
         Serial1.println("Zaświecono lampę");
     }
     else
     {
         digitalWrite(K1, LOW);
         Serial1.println("Zgaszono lampę");   
     }
        
     
    //Klima
   // int wilgotnosc = dht.getHumidity(); //Pobranie informacji o wilgotnosci
   // int temperatura = dht.getTemperature(); //Pobranie informacji o temperaturze

    //Brama
    unsigned int uS = sonar.ping(); //Odczyt z czujnika ultradźwiękowego HC-SR04  
    distance = uS / US_ROUNDTRIP_CM; //Przetworzenie dostarczonego sygnały na odległość w [cm]
    limit_sensor = analogRead(A1);

     if (distance < 10 && limit_sensor < 1000)
     {
         digitalWrite(K2, HIGH);
         Serial1.println("Trwa otwieranie bramy");
     }
    else if(distance > 15 && limit_sensor > 1003) //Zmiana obrotów silnika!!!!!!!!!!!!!!
     {   
         digitalWrite(K2, LOW);
         Serial1.println("UWAGA! Trwa zamykanie bramy")
         miganie(K3);
     }
}

//Funkcja ręcznego załączania cewek
void reczne()
{
  Serial1.println("Działa reczne");
   
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

//Funkcja załącząjąca u wyłączająca napięcie na wyjściu co 700ms
void miganie(int out) //"out" to parametr funkcji, który opisuje wyjście na którym działanie ma zostać przeprowadzone, np.: miganie(K3); Tak jak wyżej w funkcji automatyka();
{
    digitalWrite(out, HIGH);
    delay(700);
    digitalWrite(out, LOW);
    delay(700);
}
