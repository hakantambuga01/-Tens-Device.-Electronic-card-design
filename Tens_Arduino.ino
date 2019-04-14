#include <OneWire.h>            //Sıcaklık sensörü kütüphanesi
#include <DallasTemperature.h>  //Sıcaklık sensörü kütüphanesi
#define dallas 9               // Sensör pin Numarası
OneWire oneWire(dallas);       //Sensör Tanımlaması
DallasTemperature sensors(&oneWire); //Sensör Tanımlaması
#include <LiquidCrystal.h>         //LCD kütüphanesi
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Lcd pin tanımlamaları
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                  //Lcd pin tanımlamaları

//----------------Arduino pin tanımlamaları
#define Hotpack 8    
#define Titresim 7
#define Buzer 6
#define Tens 10
#define buton5 A5
#define buton4 A4
#define buton3 A3
#define buton2 A2
#define buton1 A1
int btnT1,btnT2,btnAuto;
int btnHotpack,btnDurdur;
int a=1,b=1,c=1,d=1;
byte Tens_Pwm=254;


void setup() 
{
  //--------------------GİRİŞ ÇIKIŞ TANIMLAMALARI 
  
  sensors.begin();
  lcd.begin(16, 2);   // Lcd tanımlaması
  //---------LCD ADRESİ VE BAŞLANGIÇTA YAZAN YAZI
  lcd.setCursor(0,0);
  lcd.print("TENS & HOTPACK");
  lcd.setCursor(0,1);
  lcd.print("TIP MUHENDISLIGI");
  pinMode(Tens,OUTPUT);
  pinMode(Hotpack,OUTPUT);
  pinMode(Titresim,OUTPUT);
  pinMode(Buzer,OUTPUT);
  pinMode(buton1,INPUT);
  pinMode(buton2,INPUT);
  pinMode(buton3,INPUT);
  pinMode(buton4,INPUT);
  pinMode(buton5,INPUT);
  digitalWrite(Hotpack,1);
  digitalWrite(Titresim,1);
  digitalWrite(Buzer,1);
  delay(3000);         // 3 saniye bekle
  lcd.clear();         // Lcd temizle
  
}

void loop() 
{

   
  sensors.requestTemperatures();   // sensör okuma
  lcd.setCursor(0,1);
  lcd.print("SICAKLIK"); 
  lcd.setCursor(9,1); 
  lcd.print(sensors.getTempCByIndex(0)); 
  
  btnDurdur=digitalRead(buton1);
  btnAuto=digitalRead(buton2);
  btnT2=digitalRead(buton3);
  btnT1=digitalRead(buton4);
  btnHotpack=digitalRead(buton5);
   a=1;  b=1; c=1; d=1;
  if(btnHotpack==1){ delay(500); hotpack(); }
  if(btnT1==1)     { delay(500); Mod1(); }
  if(btnT2==1)     { delay(500); Mod2(); } 
  if(btnAuto==1)   { delay(500); AutoMode();}  
  }
///----------------------------------TENS & HOTPACK---------------------
void AutoMode()
{    
  lcd.clear();
  while(b==1)
  {
    sensors.requestTemperatures();  
    lcd.print("TENS & HOTPACK");
    lcd.setCursor(0,1); 
    lcd.print("SICAKLIK"); 
    lcd.setCursor(9,1); 
    lcd.print(sensors.getTempCByIndex(0));
    
      for(int i=0; i<660; i++) // 10 Dakika Hotpack & Titreşim
    {
      
      digitalWrite(Hotpack,0);
      digitalWrite(Titresim,0);
      delay(100);
      sensors.requestTemperatures();  
      lcd.setCursor(0,1); 
      lcd.print("SICAKLIK"); 
      lcd.setCursor(9,1); 
      lcd.print(sensors.getTempCByIndex(0));
      btnDurdur=digitalRead(buton1);
      if(btnDurdur==1) { delay(500); b=0; goto dur; }
    }
   
     digitalWrite(Hotpack,1); // Hotpack & Titreşim Kes
     digitalWrite(Titresim,1); 
     

     for( int j=0; j<106; j++)    // 5 Dakika Tens
    {
      
      analogWrite(Tens,Tens_Pwm);
      delay(5);
      analogWrite(Tens,0);
      delay(2000);
      sensors.requestTemperatures();  
      lcd.setCursor(0,1); 
      lcd.print("SICAKLIK"); 
      lcd.setCursor(9,1); 
      lcd.print(sensors.getTempCByIndex(0));
      btnDurdur=digitalRead(buton1);
      if(btnDurdur==1) { delay(500); b=0; goto dur; }
    }
   
   
    analogWrite(Tens,0); // Tens kes
    digitalWrite(Buzer,0); //Buzer Aktif
    btnDurdur=digitalRead(buton1);
    if(btnDurdur==1) { delay(500); goto dur; }
      b=0;
      lcd.clear();
  }

      dur:
      b=0;   
      digitalWrite(Hotpack,1);
      digitalWrite(Titresim,1);
      digitalWrite(Buzer,1);
      analogWrite(Tens,0);
      lcd.clear(); lcd.print("CIHAZ BOSTA");
}
/////----------------------------------------HOTPACK--------------------------
void hotpack()
{
  lcd.clear();
  while(a==1)
  {
   sensors.requestTemperatures();  
   lcd.setCursor(0,0); 
   lcd.print("HOTPACK ISINIYOR");
   lcd.setCursor(0,1); 
   lcd.print("SICAKLIK"); 
   lcd.setCursor(9,1); 
   lcd.print(sensors.getTempCByIndex(0));
   btnDurdur=digitalRead(buton1);
   digitalWrite(Hotpack,0);
   digitalWrite(Titresim,0);
   if(btnDurdur==1)
   {
    delay(500);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CIHAZ BOSTA");
    a=0;
    digitalWrite(Hotpack,1);
    digitalWrite(Titresim,1);
   }
  }
}
///------------------------- TENS MOD1----------------------------
void Mod1()
{
  lcd.clear();
  while(a==1)
  {
   lcd.setCursor(2,0); 
   lcd.print("TENS MOD1");  
   lcd.setCursor(2,1);
   lcd.print("1 SANIYE ");
   btnDurdur=digitalRead(buton1);
   analogWrite(Tens,Tens_Pwm);
   delay(5);
   analogWrite(Tens,0);
   delay(1000);
   if(btnDurdur==1)
   {
    delay(500);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CIHAZ BOSTA");
    digitalWrite(Tens,0);
    a=0;
   }
  }
}
///------------------------------------TENS MOD2-----------------------------
void Mod2()
{
  lcd.clear();
  while(a==1)
  {
   lcd.setCursor(2,0); 
   lcd.print("TENS MOD2");
   lcd.setCursor(2,1);
   lcd.print("2 SANIYE ");
   btnDurdur=digitalRead(buton1);
   analogWrite(Tens,Tens_Pwm);
   delay(5);
   analogWrite(Tens,0);
   delay(2000);
   if(btnDurdur==1)
   {
    delay(500);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CIHAZ BOSTA");
    digitalWrite(Tens,0);
    a=0;
   }
  }
}







