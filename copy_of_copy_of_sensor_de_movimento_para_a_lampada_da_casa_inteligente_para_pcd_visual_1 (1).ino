 #include <Servo.h>
Servo motor;



//Variaveis Globais Detecção de gás
const int mq135 = A0;
const int buzzer = 12;
int contador = 0;

const int ledVermelho = 13; //pino do led
const int PIR = 2; // pino do PIR


int buttonPin = 7;
int ledPin = 10;
int estadoButton = 0;



void acenderLEDAoDetectarPresenca(){
int estadoPIR = digitalRead(PIR);

  
  if(estadoPIR == HIGH){
    
    digitalWrite(ledVermelho, HIGH);
    Serial.println();
      Serial.println("Movimento detectado! LED aceso.");
    delay(2000);
}
else {
   digitalWrite(ledVermelho, LOW);
  Serial.println("------------");
Serial.println("Sem movimento. LED apagado.");
  delay(3000);
  }

}//Fim do led


void alarme_dois_tons() {
int freqAlta = 2000;
int freqBaixa = 800;
int duracaoTom = 1000;

  tone(buzzer, freqAlta, duracaoTom);
delay(duracaoTom);
tone(buzzer, freqBaixa, duracaoTom);
delay(duracaoTom);

}//fim do ale



void verificarVazamentoDeGas(){

  int estadoMQ135 = analogRead(mq135);
  
  if (estadoMQ135 > 700) {
while (contador == 0) {
  Serial.println();
Serial.println("Detectamos que ha vazamento de gas, iremosacionar o alarme em:");
 Serial.println("3");
delay(1000);
 Serial.println("2");
delay(1000);
 Serial.println("1");
delay(1000);
 
  
  contador = 1;
}
  alarme_dois_tons();
}
else {
if (contador == 1) {
contador = 0;

  noTone(buzzer); // Para o som
delay(1000);
}
  Serial.println();
Serial.println("Ufa!!! Nao ha detecçao de gas no ambiente.");
  delay(4000);
}


}//fim do veri


void abrirporta(){
 estadoButton = digitalRead (buttonPin); 
  if ( estadoButton == HIGH ) {
    digitalWrite(ledPin , HIGH);
    motor.write(90);
    delay(1000);
  }
  else {
    digitalWrite (ledPin , LOW );
    motor.write(0);
    delay(1000);
  }

}//Fim do porta




void setup() {
Serial.begin(9600);
  
pinMode(ledVermelho, OUTPUT);
pinMode(PIR, INPUT);

pinMode(mq135, INPUT); 
pinMode(buzzer, OUTPUT);  
  
 motor.attach(11);
 motor.write(0);
 
pinMode(ledPin , OUTPUT);
  pinMode (buttonPin , INPUT);  

Serial.println("Calibrando o sensor PIR...");
Serial.println("Por favor, nao se mova na frente do sensor.");
delay(2000);
Serial.println("Sensor PIR calibrado e pronto!");  
  
  
}//fim do stp


void loop() {
  
  abrirporta();
  acenderLEDAoDetectarPresenca();
  verificarVazamentoDeGas();
}//fim do loop 
  