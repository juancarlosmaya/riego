/*
  Firmware para prueba en ESP32 usando Bloutooth Low Energy de aplicacion móvil ELECTROESTIMULADOR
  
  Se crea el siguente servicio BLE
  SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

  Con las siguentes características:
  #define CHARACTERISTIC_UUID         "beb5483e-36e1-4688-b7f5-ea07361b26a8"   ---> para recibir la información de la aplicación móvil (frecuencias y tiempos)
  #define CHARACTERISTIC_STATUS_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a9"    ---> para notificar a la aplicaión movil del estado del eslectroestimulador ('enEspera', 'electroestimulando' y 'fin')

  El dispositio espera la llegada de la información en CHARACTERISTIC_UUID, para determinar :
  frecuencia0    ---> la frecuencia en hz de la primera terapia
  duracion0      ---> la duración en segundos de la primera terapia
  frecuencia1    ---> la frecuencia en hz de la segunda terapia
  duracion1      ---> la duración en segundos de la segunda terapia

  recibida la información se ejecuta 
  electroestimular()    ---> generación de seña de onda cuadrada visible en el led incorporado en módulo ESP32

  La aplicación móvil recibe la notificacion de la modificación del estado del electroestimulador CHARACTERISTIC_STATUS_UUID
  status_pCharacteristic->setValue("electroestimulando");
  
  La aplicación móvil recibe la notificacion de la modificación del estado del electroestimulador CHARACTERISTIC_STATUS_UUID
  status_pCharacteristic->setValue("fin");

  Notas adicionales:
  The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

  REF:

    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara
*/


#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* status_pCharacteristic = NULL;

uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID         "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_STATUS_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a9"

// Dato enviados por aplicación Electroestimulador
bool llegoDato=false;
int frecuencia0=0;
int duracion0=0;
int frecuencia1=0;
int duracion1=0;
int frecuencia2=0;
int duracion2=0;
int frecuencia3=0;
int duracion3=0;
int frecuencia4=0;
int duracion4=0;
int frecuencia5=0;
int duracion5=0;
int frecuencia6=0;
int duracion6=0;
int frecuencia7=0;
int duracion7=0;
int frecuencia8=0;
int duracion8=0;
int frecuencia9=0;
int duracion9=0;

// Simulación de electroestimulación según duraciones y frecuencias leidas
void electroestimular(){
  int tiempo=millis();
  while ((millis()-tiempo)<1000*duracion0){
    digitalWrite(2,HIGH);
    delay(500/frecuencia0);
    digitalWrite(2,LOW);
    delay(500/frecuencia0);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion1){
    digitalWrite(2,HIGH);
    delay(500/frecuencia1);
    digitalWrite(2,LOW);
    delay(500/frecuencia1);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion2){
    digitalWrite(2,HIGH);
    delay(500/frecuencia2);
    digitalWrite(2,LOW);
    delay(500/frecuencia2);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion3){
    digitalWrite(2,HIGH);
    delay(500/frecuencia3);
    digitalWrite(2,LOW);
    delay(500/frecuencia3);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion4){
    digitalWrite(2,HIGH);
    delay(500/frecuencia4);
    digitalWrite(2,LOW);
    delay(500/frecuencia4);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion5){
    digitalWrite(2,HIGH);
    delay(500/frecuencia5);
    digitalWrite(2,LOW);
    delay(500/frecuencia5);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion6){
    digitalWrite(2,HIGH);
    delay(500/frecuencia6);
    digitalWrite(2,LOW);
    delay(500/frecuencia6);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion7){
    digitalWrite(2,HIGH);
    delay(500/frecuencia7);
    digitalWrite(2,LOW);
    delay(500/frecuencia7);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion8){
    digitalWrite(2,HIGH);
    delay(500/frecuencia8);
    digitalWrite(2,LOW);
    delay(500/frecuencia8);
  }
  tiempo=millis();
  while ((millis()-tiempo)<1000*duracion9){
    digitalWrite(2,HIGH);
    delay(500/frecuencia9);
    digitalWrite(2,LOW);
    delay(500/frecuencia9);
  }

}



// Lee los datos enviados por aplicación, los almacena en variables frecuencia y duración, y llama a la función electroestimular
class MyCallbacks: public BLECharacteristicCallbacks {
  int i=0;
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      char cadena[]="                                                                                                                                                                                                                                                   ";
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++){
          cadena[i]=char(value[i]);
        }
      }
      Serial.print("la cadena de texto leida es:");
      Serial.println(cadena);

      // Extrae valores de la cadena de texto
      float recortes[20];
      char * pch;
      pch = strtok(cadena,",");
      i=0;
      while (pch != NULL)
      {
        recortes[i]=atof(pch);
        i=i+1;
        pch = strtok(NULL,",");
      }
      frecuencia0=recortes[0];
      duracion0=recortes[1];
      frecuencia1=recortes[2];
      duracion1=recortes[3];
      frecuencia2=recortes[4];
      duracion2=recortes[5];
      frecuencia3=recortes[6];
      duracion3=recortes[7];
      frecuencia4=recortes[8];
      duracion4=recortes[9];
      frecuencia5=recortes[10];
      duracion5=recortes[11];
      frecuencia6=recortes[12];
      duracion6=recortes[13];
      frecuencia7=recortes[14];
      duracion7=recortes[15];
      frecuencia8=recortes[16];
      duracion8=recortes[17];
      frecuencia9=recortes[18];
      duracion9=recortes[19];
      
      Serial.print("la frecuencia0 es: ");
      Serial.println(frecuencia0);
      Serial.print("la duración0 es: ");
      Serial.println(duracion0);
      
      Serial.print("la frecuencia1 es: ");
      Serial.println(frecuencia1);
      Serial.print("la duración1 es: ");
      Serial.println(duracion1);
      
      Serial.print("la frecuencia2 es: ");
      Serial.println(frecuencia2);
      Serial.print("la duración2 es: ");
      Serial.println(duracion2);
      
      Serial.print("la frecuencia3 es: ");
      Serial.println(frecuencia3);
      Serial.print("la duración3 es: ");
      Serial.println(duracion3);
      
      Serial.print("la frecuencia4 es: ");
      Serial.println(frecuencia4);
      Serial.print("la duración4 es: ");
      Serial.println(duracion4);
      
      Serial.print("la frecuencia5 es: ");
      Serial.println(frecuencia5);
      Serial.print("la duración5 es: ");
      Serial.println(duracion5);
      
      Serial.print("la frecuencia6 es: ");
      Serial.println(frecuencia6);
      Serial.print("la duración6 es: ");
      Serial.println(duracion6);
      
      Serial.print("la frecuencia7 es: ");
      Serial.println(frecuencia7);
      Serial.print("la duración7 es: ");
      Serial.println(duracion7);
      
      Serial.print("la frecuencia8 es: ");
      Serial.println(frecuencia8);
      Serial.print("la duración8 es: ");
      Serial.println(duracion8);
      
      Serial.print("la frecuencia9 es: ");
      Serial.println(frecuencia9);
      Serial.print("la duración1 es: ");
      Serial.println(duracion9);

      llegoDato=true;                                               // BANDERA PARA INDICAR QUE LLEGARON DATOS 
    }
};




void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  

  // Create the BLE Device
  BLEDevice::init("ELECTROESTIMULADOR");
// Create the BLE Server
  pServer = BLEDevice::createServer();

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // CARACTERÍSTICA PARA RECEPCIÓN DE DATOS
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setValue("Valor por defecto");
  pCharacteristic->setCallbacks(new MyCallbacks());                           // manejador de eventos de característica (recepción de datos)

  // CARACTERÍSTICA PARA ENVÍO DE DATOS (NOTIFICACIONES)
  status_pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_STATUS_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
   status_pCharacteristic->setValue("enEspera");
   status_pCharacteristic->addDescriptor(new BLE2902());                             // Crea un descriptor de la característica tipo 'Client characteristic configuration': descriptor que indica que el cliente esta o no interesado en recibir notificaciones 
  
  // INICIALIZA EL SERVICIO
  pService->start();

  // INICIA PUBLICACIÓN DE DISPOSOITVO BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  //pAdvertising->addServiceUUID(SERVICE_UUID);
  //pAdvertising->setScanResponse(false);
  //pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  //BLEDevice::startAdvertising();
  pAdvertising->start();
  Serial.println("En espera de una conexión con cliente");
}

void loop() {
  // Al llegar datos según MyCallbacks, se realiza electroestimulación y se envían notificaciones a aplicación web sobre los cambios de estado  
  if (llegoDato==true){
    // inica electroestimulación
    status_pCharacteristic->setValue("electroestimulando");
    status_pCharacteristic->notify();
    electroestimular();
    status_pCharacteristic->setValue("fin");
    status_pCharacteristic->notify();
    llegoDato=false;
  }
}
