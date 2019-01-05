//Dołączone biblioteki. JEŚLI KOMPILATOR MÓWI, ŻE NIE MOŻE ICH ZNALEŹĆ, POBIERZ JE!!! LINKI I QR KODY WRAZ Z INSTRUKJCĄ INSTALACJI BIBLIOTEK ZNAJDZIESZ W DOKUMENTACJI!!!
    #include <NewPing.h>
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
    const int S1=2;  
    const int S2=11;
    const int Trig=12; //Czujnik ultradźwiękowy HC-SR04 pin trig
    const int Echo=13; //Czujnik ultradźwiękowy HC-SR04 pin echo

//czujniki analogowe SA1...SA6 wpiętę do wejść analogowych arduino nr.: A0...A5const int SA1=A0; //Czujnik natężenia światła PT550
    const int SA2=A1;
    const int SA3=A2;
    const int SA4=A3;
    const int SA5=A4;
    const int SA6=A5;
    
//zdefiniowanie zmiennej data czyli danych przychodzących 
 char data='X';
//zdefiniowanie sposobu sterowania
 boolean automatic_control=true;
//zdefiniowanie zmiennej 'light_intensity', w której przetrzymywana jest informacja o natężeniu światła
    int light_intensity=0;
//Konfiguracja czujnika ultradźwiękowego zgodnie z biblioteką NewPing.h Zdefiniowanie zmiennej 'distance', w której przetrzymywana jest informacja o odległości ciała od czujnika
    #define Max_Distance 400
    NewPing sonar(Trig, Echo, Max_Distance); 
    int distance=0;
void setup() {
  pinMode(K1, OUTPUT); //zdefiniowanie I/O 3..10 cyfrowych jako wyjścia
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  pinMode(K4, OUTPUT);
  pinMode(K5, OUTPUT);
  pinMode(K6, OUTPUT);
  pinMode(K7, OUTPUT);
  pinMode(K8, OUTPUT);
  
  pinMode(S1, INPUT); //zdefiniowanie I/O cyfrowych 2, 11...13 jako wejść 
  pinMode(S2, INPUT);
  pinMode(Trig, INPUT);
  pinMode(Echo, INPUT);
  
  Serial.begin(9600); //Zainicjowanie komunikacja poprzez port szeregowy 
  
  pinMode(0, INPUT); //Zdefiniowanie złącz TX i RX
  pinMode(1, OUTPUT);
    
}
// Zmienne opisujące stan przkaźnika. Jest to potrzebne do późniejszych warunków logicznych (linie: ******************* ) WPISZ TUTAJ !!!!!!!!!!!!!
    int p1=0; // '0' oznacza, że może zostać rozłączone, a '1', że są załączone
    int p2=0; 
    int p3=0;
    int p4=0;
    int p5=0;
    int p6=0;
    int p7=0;
    int p8=0;
  
void loop()   
{

    //Dokonanie odczytu z czujników i zapis wartości do zmiennych
    light_intensity = analogRead(0); //Odczyt natężenia światła z czujnika PT550 wpiętego do A0

    unsigned int uS = sonar.ping(); //Odczyt z czujnika ultradźwiękowego HC-SR04
    distance = uS / US_ROUNDTRIP_CM; //Przetworzenie dostarczonego sygnały na odległość w [cm]

if(Serial.available()>0)
 {    
    data=Serial.read();

    switch(data)
    {
        case '1': automatic_control=true; //Gdy zostanie wysłany sygnał '1' z aplikacji to zostanie włączone automatyczne sterowanie

        case '0': automatic_control=false; //Gdy zostanie wysłany sygnał '0' z aplikacji to zostanie włączone sterowanie ręczne

        default: break;
    }

    //W zależności od tego czy kontrola automatyczna jest włączona czy nie zostaną zrealizowane różne funkcje. W sterowaniu ręcznym mamy pełną kontrolę lecz w automatycznym musimy liczyć się ze wskaźnikami z czujników
    if(automatic_control==false)
    {
        switch(data)
        {
        case 'a': p1=1;   // W zależności od sygnału wysłanego z aplikacji, wartość zmiennej się zmieni
        case 'b': p1=0;    
        
        case 'c': p2=1;   
        case 'd': p2=0;   
        
        case 'e': p3=1;  
        case 'f': p3=0;  
        
        case 'g': p4=1;  
        case 'h': p4=0;  
                
        default: break;
        }

        
        //Załączanie cewek przekaźników, uwzględniając stany czujników
        if(p1==1) 
        {
            digitalWrite(K1, HIGH);
        
        }
        else 
        {
            digitalWrite(K1, LOW);
        }
       
        if(p2==1)
        {
         digitalWrite(K2, HIGH);
        
        }
        else
        {
            digitalWrite(K2, LOW);
        }

        if(p3==1)
        {
            digitalWrite(K3, HIGH);
        }
        else
        {
            digitalWrite(K3, LOW);
        }

        if(p4==1)
        {
         digitalWrite(K4, HIGH);
        
        }
        else
        {
            digitalWrite(K4, LOW);
        }
     }
     
    else //automatic_control==true
     {
         
     }
 }

else
{
    return;
}
}
