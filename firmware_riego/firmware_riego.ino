
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define valvula1 14
#define valvula2 32
#define valvula3 33
#define valvula4 25

float minutosEncendidoValvula1 = 0;
float minutosApagadoValvula1 = 0;
float minutosEncendidoValvula2 = 0;
float minutosApagadoValvula2 = 0;
float minutosEncendidoValvula3 = 0;
float minutosApagadoValvula3 = 0;
float minutosEncendidoValvula4 = 0;
float minutosApagadoValvula4 = 0;

float minutosHoraActual = 0;


const int maxNumbers = 100; // Define el tamaño máximo del array
int numbersIndex = 0;
String currentNumberString = "";
bool newDataAvailable = false;

float milisegundosInicio = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_Serial");
  Serial.println("Bluetooth Serial started ok");
  pinMode(2,OUTPUT);
  pinMode(valvula1,OUTPUT);
  pinMode(valvula2,OUTPUT);
  pinMode(valvula3,OUTPUT);
  pinMode(valvula4,OUTPUT);
  delay(1000);
  digitalWrite(valvula1,HIGH);
  digitalWrite(valvula2,HIGH);
  digitalWrite(valvula3,HIGH);
  digitalWrite(valvula4,HIGH);

  milisegundosInicio = millis();

  
}

void loop() {
  // Recibir datos desde el Bluetooth y almacenar los números en el array
  while (SerialBT.available()) {
    char receivedChar = (char)SerialBT.read();
    currentNumberString += receivedChar;
    if (receivedChar == '\n') {
        processReceivedNumbers();
        currentNumberString ="";
        break;
     }
  }
  delay(20);

  encenderApagarValvulas();
}


/* Dependiendo de los minutos transcurridos desde la hora cero del día se activan o se apagan las válvulas */
void encenderApagarValvulas() {
  float minutosTranscurridos = ((millis()-milisegundosInicio)/1000)/60;

  Serial.println(minutosHoraActual+minutosTranscurridos);
  Serial.println(minutosEncendidoValvula1);
  Serial.println(minutosApagadoValvula1);

  
  if ((minutosEncendidoValvula1< (minutosHoraActual+minutosTranscurridos)) && ((minutosHoraActual+minutosTranscurridos) < minutosApagadoValvula1)) {
    digitalWrite(2,HIGH);
    digitalWrite(valvula1,LOW);
  }else{
    digitalWrite(2,LOW);
    digitalWrite(valvula1,HIGH);
  }

  if ((minutosEncendidoValvula2< (minutosHoraActual+minutosTranscurridos)) && ((minutosHoraActual+minutosTranscurridos) < minutosApagadoValvula2)) {
    digitalWrite(valvula2,LOW);
  }else{
    digitalWrite(valvula2,HIGH);
  }

  if ((minutosEncendidoValvula3< (minutosHoraActual+minutosTranscurridos)) && ((minutosHoraActual+minutosTranscurridos) < minutosApagadoValvula3)) {
    digitalWrite(valvula3,LOW);
  }else{
    digitalWrite(valvula3,HIGH);
  }

  if ((minutosEncendidoValvula4< (minutosHoraActual+minutosTranscurridos)) && ((minutosHoraActual+minutosTranscurridos) < minutosApagadoValvula4)) {
    digitalWrite(valvula4,LOW);
  }else{
    digitalWrite(valvula4,HIGH);
  }

  if ((minutosHoraActual+minutosTranscurridos)>(24*60)){
    minutosHoraActual = 0;
    milisegundosInicio = millis();
  }
}


/* A partir de una cadena de texto del tipo numero1,numero2,numero3,numero4,numero5,....,numero9,numero10/n se extraen los minutos a esperar desde las cero horas para encender
cada valvula y para apagarla, tambien se extraen los minutos de la hora actual desde las cero horas */

void processReceivedNumbers() {
  Serial.println("Números recibidos:");
  Serial.println(currentNumberString);
  
  float recortes[20];
  int size = 0;
  int lastIndex = 0;
  int index = 0;

  while (index >= 0) {
    index = currentNumberString.indexOf(',', lastIndex);
    String value = (index == -1) ? currentNumberString.substring(lastIndex) : currentNumberString.substring(lastIndex, index);
    recortes[size++] = value.toInt();
    lastIndex = index + 1;
  }

  Serial.println("Array resultante:");
  for (int i = 0; i < size; i++) {
    Serial.print(recortes[i]);
    Serial.print(" ");
  }
  Serial.println("");
  
  minutosEncendidoValvula1 = recortes[0]*60 + recortes[1];
  minutosApagadoValvula1 = recortes[2]*60 + recortes[3];
  minutosEncendidoValvula2 = recortes[4]*60 + recortes[5];
  minutosApagadoValvula2 = recortes[6]*60 + recortes[7];
  minutosEncendidoValvula3 = recortes[8]*60 + recortes[9];
  minutosApagadoValvula3 = recortes[10]*60 + recortes[11];
  minutosEncendidoValvula4 = recortes[12]*60 + recortes[13];
  minutosApagadoValvula4 = recortes[14]*60 + recortes[15];

  minutosHoraActual = recortes[16]*60 + recortes[17];
        
  Serial.print("los minutos totales desde las 0 horas para prender la valvula 1 son:");
  Serial.println(minutosEncendidoValvula1);
  Serial.print("los minutos totales desde las 0 horas para apagar la valvula 1 son:");
  Serial.println(minutosApagadoValvula1);
  
  Serial.print("los minutos totales desde las 0 horas para prender la valvula 2 son:");
  Serial.println(minutosEncendidoValvula2);
  Serial.print("los minutos totales desde las 0 horas para apagar la valvula 2 son:");
  Serial.println(minutosApagadoValvula2);
  
  Serial.print("los minutos totales desde las 0 horas para prender la valvula 3 son:");
  Serial.println(minutosEncendidoValvula3);
  Serial.print("los minutos totales desde las 0 horas para apagar la valvula 3 son:");
  Serial.println(minutosApagadoValvula3);
  
  Serial.print("los minutos totales desde las 0 horas para prender la valvula 4 son:");
  Serial.println(minutosEncendidoValvula4);
  Serial.print("los minutos totales desde las 0 horas para apagar la valvula 4 son:");
  Serial.println(minutosApagadoValvula4);
  
  Serial.print("los minutos totales desde las 0 horas del dia de hoy son:");
  Serial.println(minutosHoraActual);

  milisegundosInicio = millis();
}
