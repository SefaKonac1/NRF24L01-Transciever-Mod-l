/*                          */
/*  Author:Sefa Konac       */
/*  Design Half             */
/*                          */



#include "DHT.h"
#include "SPI.h"
#include "nRF24L01.h" 
#include "RF24.h" 


int behavior = 0;        /*master yada slave oldugunu belirlemek icin olusturuldu.*/

DHT dht(0,DHT11);         /*sensor objesi 0-> analog pin DHT11 ->mod*/
int sendData[3] = {0,0,0};  /*sicaklik ve nemi depoluyor*/
int takeData[3] = {0,0,0};  /*x ve y kordinatlarını alacak*/



RF24  radio(9,10);           /*NRF24L01 icin sectigim pinler.*/

void setup() {

  Serial.begin(115200);           /*saniyedeki bit sayisi*/
  dht.begin();

  
  radio.begin();

  radio.setDataRate( RF24_250KBPS ); 
}
void loop() {

  bool control=0;

  sendData[0] = behavior;
  sendData[1] = dht.readHumidity();
  sendData[2] = dht.readTemperature(true);
   
  if(behavior == 0){

      radio.startListening();
      radio.openReadingPipe(1,0xF0F0F0F0);

      if(!radio.available()){
        
        do{
          
          control = radio.read(&takeData,sizeof(takeData)); /*gelen datayi oku*/
         // behavior = takeData[0]; /*cihazi master konumuna getir.*/

          Serial.println(control);
          delay(500);
          if(control == false){
  
              Serial.println("Reading from pipe operation was crashed !");
          }
  
          
        }while(!radio.available());

      printReceivedData();
      
      radio.stopListening();
      } 
  }

  else{

       radio.stopListening(); /*gelen mesajlara karsi dinlemeyi durdur.*/
       radio.openWritingPipe(0xFAFAFAFA);
       control = radio.write(&sendData,sizeof(sendData));
       behavior = 0; /*Cihazi slave konumuna getir.*/

       if(control == true){
          
          Serial.println("Data sending...\n");
          
       } 
       else{
                 
         Serial.println("Writing data on pipe operation was crashed !");   
       }
  }
     
  delay(500);
 
}

void printReceivedData(){

    Serial.print("\n-------------------------------------------------\n");

    
    Serial.print("X-axis: ");
    Serial.print(takeData[0]);

    Serial.println("      ");

    Serial.print("Y-axis: ");
    Serial.print(takeData[1]);


}




