#include<SPI.h>
#include<MFRC522.h> //Descargar la Libreria con anterioridad
#include<Wire.h>
#include<Adafruit_GFX.h>//Descargar Libreria con anterioridad
#include<Adafruit_SSD1306.h>//Descargar Libreria con anterioridad
//Lector
#define RTS_PIN 9 
#define SS_PIN 10 
//Pantalla OLED
#define ANCHO 128
#define ALTO 64
#define OLED_RESET 4
Adafruit_SSD1306 oled(ANCHO, ALTO,&Wire,OLED_RESET);

MFRC522 mfrc522(SS_PIN,RTS_PIN);

byte LecturaUID[4];
byte Usuario1[4]={0x90, 0x0E, 0xE4, 0xA4}; //Numero UID Tarjeta Ejemplo
byte Usuario2[4]={0x06, 0x76, 0x25, 0xD9}; //Numero UID Llavero Ejemplo

void setup() {
  Serial.begin(9600);
  SPI.begin();
  Wire.begin();
  mfrc522.PCD_Init();
  oled.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  Serial.println("Configuracion lista");

}

void loop() {
  if(!mfrc522.PICC_IsNewCardPresent()){
      return;
    }
  if(!mfrc522.PICC_ReadCardSerial()){
      return;
    }
  
  Serial.println("UID:");
  for(byte i = 0; i < mfrc522.uid.size;i++){
    if (mfrc522.uid.uidByte[i] < 0x10){
        Serial.println(" 0");
      }
      else{
        Serial.println(" ");
      }
      Serial.println(mfrc522.uid.uidByte[i],HEX);
      LecturaUID[i]=mfrc522.uid.uidByte[i];
      }
      Serial.println("\t");
      //Hacemos la comparacion de la lectura con los datos en nuestra base 
      if(comparaUID(LecturaUID,Usuario1)){
        //Ejecutamos el bloque de codigo correspondiente al usuario
        Serial.print("Bienvenido Usuario 1");
        mostrarMensaje("Confirmado");}
      else if(comparaUID(LecturaUID,Usuario2)){
        //Ejecutamos el bloque de codigo correspondiente al usuario
        Serial.print("Bienvenido Usuario 2");
        mostrarMensaje("Confirmado");}
      else{
        Serial.print("Usuario Incorrecto");
        mostrarMensaje("Denegado");}
      mfrc522.PICC_HaltA();
    }

//Funcion para comparar los datos de la tarjeta RFDI
boolean comparaUID(byte lectura[],byte usuario[]){
    for (byte i=0; i <mfrc522.uid.size; i++){
      if(lectura[i] != usuario[i]){
        return(false);
      }else{
        return(true);
      }
    }
}


//Funcion para mostrar el mensaje en la pantalla OLED
void mostrarMensaje(String mensaje){
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setCursor(0,0);
  oled.setTextSize(1);
  oled.print("Acceso");
  oled.setCursor(10,30);
  oled.setTextSize(2);
  oled.print(mensaje);
  oled.display();

}
