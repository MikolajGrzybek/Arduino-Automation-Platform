/*
    Zrobił to Grzybek żeby nie oblać z pneumatyki bo nie chciało mu się rysować tych schematów...
    A.D 2019
*/

//załączenie niezbędnych bibliotek
#include <SoftwareSerial.h>
#include <NewPing.h>
#include <DHT.h>

//Zadeklarrowanie zmiennej wymaganej przez bibliotekę DHT.h
#define DHT11_PIN 6

//Zdefiniowanie pinów RX i TX
SoftwareSerial mySerial(1, 0); // RX, TX

//cewka K1...K8, której sterowanie wpięte jest do wyjścia cyfrowego arduino nr.: 3...10
const int K1=3; //lampa
const int K2=4; //klimatyzacja
const int K3=5; 
//const int K4=6;
//const int K5=7;
//const int K6=8;
const int K7=9; //brama
const int K8=10; //brama

//czujniki cyfrowe S1...S4 wpięte do wejść cryfowycj arduino nr.:2, 11...13
const int DHT11_PIN=2; //Czujnik temperatury i wilgotności DHT11
const int Trig=12; //Czujnik ultradźwiękowy HC-SR04 pin trig
const int Echo=13; //Czujnik ultradźwiękowy HC-SR04 pin echo
    
//czujniki analogowe SA1...SA6 wpiętę do wejść analogowych arduino nr.: A0...A5const int SA1=A0; //Czujnik natężenia światła PT550
const int SA1=A0; //Czujnik światła (fotorezystor)
  
//zdefiniowanie zmiennej 'krańcówki' w której jest przetrzymowana informacja o tym czy brama otworzyła się do końca
int S1; //Krańcówka z prawej
int S2; //Krańcówka z lewej

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

//zmienna logiczna opisująca wybrany tryb sterowania
boolean sterowanie;

//Fukncja wykonująca się przy załączeniu mikroprocesora
void setup() 
{        
  //zdefiniowanie portów jako wyjścia i wejścia cyfrowe        
  pinMode(K1, OUTPUT); 
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  //pinMode(K4, OUTPUT);
  //pinMode(K5, OUTPUT);
  //pinMode(K6, OUTPUT);
  pinMode(K7, OUTPUT);
  pinMode(K8, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  
  //Zainicjowanie komunikacji poprzez bluetooth na portach RX TX (0,1)
  Serial1.begin(9600);
  
  //Konfiguracja czujnika przy użyciu biblioteki DHT.h
  dht.setup(DHT11_PIN);
 
  //Domyślne ustawienie trybu sterowania na automatyczny
  sterowanie = true;
  }

//WYKONANIE KODU GŁÓWNEGO PRAKTYCZNIE 95% PRACY. Funkcja powtarzająca się w kółko
void loop() 
{
  //Odczyt danych z bluetooth i zapisanie do zmiennej 'state'
  state = Serial1.read();
 
  //Wybór trybu pracy
  if(state == '1') //Jeśli zostanie wysłana '1' urządzenie przejdzie w tryb automatyczny
  {
    sterowanie = true;
  } 
  else if (state == '2') //Jeśli zostanie wysłana '2' urządzenie przejdzie w tryb ręczny
  {
    sterowanie = false;
  }

  //wykonanie funkcji dla odpowiednich trybów pracy
  if(sterowanie == true) //Jeśli został włączony tryb automatyczny ...
  {
    automatyka(); // ... wykonaj funkjcę automatyka();
  }
  else if (sterowanie == false) //Jeśli został włączony tryb ręczny ...
  {
     reczne(); // ... wykonaj funkcję reczne();
  }
}

/*

Poniżej zostały napisane nowe funkcje. Zostało to zrobione dla przejrzystości kodu wyżej

*/

void automatyka()
{
    //Dokonanie odczytu z czujników, zapis wartości do zmiennych 
    
    //Stan krańcówek
    S1 = digitalRead(7);
    S2 = digitalRead(8);
    //Odległość
    unsigned int uS = sonar.ping(); //Odczyt z czujnika ultradźwiękowego HC-SR04  
    distance = uS / US_ROUNDTRIP_CM; //Przetworzenie dostarczonego sygnały na odległość w [cm]
    //Światło
    light_intensity = analogRead(A0); //Odczyt natężenia światła z fotorezystora wpiętego do A0
    //temperatura 
    int wilgotnosc = dht.getHumidity(); //Pobranie informacji o wilgotnosci
    int temperatura = dht.getTemperature(); //Pobranie informacji o temperaturze


    //Automatyczne wykonanie aktywności na podstawie pomiarów z czujników

    //Lampa
    if (light_intensity < 700) //DLACZEGO 700????? Rozdzielczość na pinach analogowych w arduino wynosi 10 bitów, co znaczy że będziemy odbierać na nich liczby w zakresie <0, 1023> gdzie 0=0V a 1023=5V
     {
         digitalWrite(K1, HIGH);
         Serial1.println("Zaświecono lampę");
     }
     else
     {
         digitalWrite(K1, LOW);
         Serial1.println("Zgaszono lampę");   
     }
        
     
    //AC
    if (temperatura > 20)
    {
        digitalWrite(K2, HIGH);
        
    }
  
    //Brama
    if (distance < 5) //Jeśli czujnik ultradźwiękowy wykryje obiekt w odległości mniejszej niż 5cm ...
    {
        while (S2 = LOW) // ... I dopóki krańcówka oznaczająca pełne otwarcie bramy, będzie nie aktywna ...
        {
         lewo(); // ... otwieraj bramę do pełnego jej otwarcia
        }       
    } 

    if(distance > 10) //Jeśli czujnik nie wykryje żadnego obiektu w odległości do 10cm to ...
    {
       delay(5000); // ... Poczekaj 5 sekund dla bezpieczeństwa, w wypadku gdyby ktoś znalazł się w zasięgu bramy ...

       while(S1 = LOW) // ... Dopóki brama nie jest całkowicie zamknięta ...
        {
            prawo(); // ... Zamykaj bramę do całkowitego jej zamknięcia
        }
    }   
}


void reczne()
{
  //Funckja, która na podstawie odebranego sygnału z portów bluetooth, wykona odpowiednie działanie
  switch(state)
  {
    /*
        Na użytym module przekaźników, te załączają się poprzez nadanie im na wejście stanu LOW!!!!!!! Nie HIGH!!!!!!!!!!!
    */
    case 'a': digitalWrite(K1, LOW); break; // Załącz cewkę K1
    case 'b': digitalWrite(K1, HIGH); break; // Wyłącz cewkę K1
    
    case 'c': digitalWrite(K2, LOW); break; // Załącz cewkę K2
    case 'd': digitalWrite(K2, HIGH); break; // Wyłącz cewkę K2
    
    case 'e': digitalWrite(K3, LOW); break; // Załącz cewkę K3
    case 'f': digitalWrite(K3, HIGH); break; // Wyłącz cewkę K3
    
   // case 'g': digitalWrite(K4, LOW); break;
   // case 'h': digitalWrite(K4, HIGH); break;
    
   // case 'i': digitalWrite(K5, LOW); break;
    //case 'j': digitalWrite(K5, HIGH); break;
    
    //case 'k': digitalWrite(K6, LOW); break;
    //case 'l': digitalWrite(K6, HIGH); break;
    
    if (S2 = LOW) //Jeśli brama nie jest całkowicie otwarta ...
    {
        case 'm': digitalWrite(K7, LOW); digitalWrite(K8, HIGH); break; // ... otwórz ją do końca
    }   
    if (S1 = LOW) //Jeśli brama nie jest całkowicie zamknięta ...
    {
        case 'o': digitalWrite(K7, HIGH); digitalWrite(K8, LOW);  break; //o ... zamknij ją do końca
    }
    
    //Gdyby został wysłany inny sygnał niż te zadeklarowane, nie reaguj
    default: break;
  }
 
}

//Funkcja realizująca lewe obroty silnika od bramy
void lewo()
{
//LEWO
    digitalWrite(K7, LOW);
    digitalWrite(K8, HIGH);     
}

//Funkcja realizująca prawe obroty silnika od bramy
void prawo()
{
//PRAWO
    digitalWrite(K7, HIGH);
    digitalWrite(K8, LOW);
}