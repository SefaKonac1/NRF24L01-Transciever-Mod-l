/*                          */
/*  Author:Sefa Konac       */
/*  Design Half Dublex      */
/*  Communication           */


#include "SPI.h"
#include "nRF24L01.h" 
#include "RF24.h" 

int behavior = 1;

/*Pin ayarlari, gonderilecek data deposu*/
const int xpin = 0;   /*analog pin*/
const int ypin = 1;   /*analog pin*/
const int swpin = 2;  /*switch pin digital pin*/

int sendData[3] = {0,0,0};    /*Bu dizi x ve y kordinatlarini ve davranış degerini tutacak*/
int takeData[3] = {0,0,0};    /*Bu dizi gelen sicaklik ve nem degerlerini tutacak*/

RF24 radio(9,10);            /*NRF24L01 icin sectigim pinler*/
  
void setup() {

    pinMode(swpin,INPUT);
    Serial.begin(115200);           /*saniyedeki bit sayisi bilgisayar-arduino haberlesmesi*/
    digitalWrite(swpin,HIGH);

    radio.begin();                  /*arduino-arduino haberlesmesi*/

    radio.setDataRate( RF24_250KBPS );
   
   
    //radio.startListening(); /*mesajlari dinle*/ 
}

void loop() {

    bool control;

    sendData[0] = behavior;
    sendData[1] = analogRead(xpin); /*xpindeki degeri oku ve depola*/
    sendData[2] = analogRead(ypin); /*ypindeki degeri oku ve depola*/

    /*master davranisinda olup olmadigini kontrol et*/
    if(behavior == 1){

        radio.stopListening(); /*gelen mesajlara karsi dinlemeyi durdur.*/
        radio.openWritingPipe(0xF0F0F0F0);
       
 
        control = radio.write(&sendData,sizeof(sendData)); 
        if(control == true){
          Serial.println("Data sending...\n"); 
        }
        else{
                     
          Serial.println("Writing data on pipe operation was crashed !");
          
        } 
    }
    if(behavior == 0){

        //digitalWrite(swpin,LOW); /*Konsolun data üretmesini engelle.*/
        radio.openReadingPipe(1,0xFAFAFAFA);
        radio.startListening();
        
        
        /*veri alana kadar kontrol et.*/
        if(radio.available()){
          do{
                     
            radio.read(&takeData,sizeof(takeData)); /*gelen datayi oku*/
  
            behavior = takeData[0]; /*cihazi master konumuna getir.*/
          
          }while(!radio.available());
  
           printReceivedData();
  
           radio.stopListening(); 
        }                
    }
   
   
    delay(500);
   
}

void printReceivedData(){

    Serial.print("\n-------------------------------------------------\n");

    Serial.print("Temparature: ");
    Serial.print(takeData[1]);

    Serial.println("      ");

    Serial.print("Humidity: ");
    Serial.print(takeData[2]);


}



