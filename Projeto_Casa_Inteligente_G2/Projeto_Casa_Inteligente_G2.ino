


//bibliotecas
//#include <Servo.h>
#include <ESP32Servo.h>


//Varaiveis - verificarVazamentoDeGas
const int buzzer = 19;
const int MQ135 = 34;

//rele
const int rele = 26;

//Campainha
int botao = 4;

//
Servo motor;
const int servoMotor = 12;

void Campainha(){
 tone(buzzer, 1000, 300);  // Ding - 1000 Hz por 300 ms
  delay(350);
  tone(buzzer, 700, 300);  // Dong - 700 Hz por 300 ms
  delay(350);

}

void verificarVazamentoDeGas() {
  int estadoMQ135 = analogRead(MQ135);

  Serial.println(estadoMQ135);
  if (estadoMQ135 >= 1800) {
    alarme_dois_tons();
  } else {
    noTone(buzzer);  //para de tocar o "alarme" pelo buzzer
  }
}

void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 250;

  tone(buzzer, freqAlta, duracaoTom);
  delay(duracaoTom);
  tone(buzzer, freqBaixa, duracaoTom);
  delay(duracaoTom);
}

void abrirportaservo() {
  motor.write(180);
  delay(1500);
}

void fecharportaservo() {
  motor.write(0);
  delay(1500);
}

void TrancarPorta() {


  digitalWrite(rele, LOW);
  Serial.println("Trancado");
  delay(3000);
}

void DestrancarPorta() {
  String SenhaDg;

  Serial.println("Por gentileza digite a Senha para Destravar a Porta ");
  while (!Serial.available()) {}
  SenhaDg = Serial.readString();

  if (SenhaDg == "senai@134") {
    digitalWrite(rele, HIGH);
    Serial.println("Porta desrancada...");

    abrirportaservo();
    TrancarPorta();
  } else {
    if (motor.read() != 0)
      fecharportaservo();
  }

  Serial.println("Senha invalida! Na 3 tentetiva ira ser bloqueada e tambem iremos acionar a policia, fica esperto(a).");

  digitalWrite(rele, HIGH);
  Serial.println("Porta destrancada...");
  delay(1500);
}

void tocarCampainha() {
 
  if (digitalRead(botao) == LOW) {  // botão pressionado (nível baixo)
    Campainha();
  } else {
    noTone(buzzer);
  }
}


void setup() {

  Serial.begin(115200);  //inicia a comunicação com o monitor serial

  //fala ao programa que o pino 13 será umam saida/output
  pinMode(buzzer, OUTPUT);
  pinMode(MQ135, INPUT);
  motor.attach(servoMotor);
  pinMode(rele, OUTPUT);
  pinMode(botao, INPUT_PULLUP);

  //Deixa Trancado
  digitalWrite(rele, HIGH);

  //
  motor.write(0);

  Serial.println("Sensores sendo calibrados! aguente firme :P");
  delay(10000);
  Serial.println("Sensores calibrados! obrigado por esperar. ");
}

void loop(){
  verificarVazamentoDeGas();
  tocarCampainha();
 // abrirportaservo();
 // fecharportaservo();
 // DestrancarPorta();

}