//bibliotecas
#include <DHT.h>
#include <DHT_U.h>
#include <DHT11.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Atalho de indentação(organizar): alt + shift + f
//Variáveis acender led

const int PIR = 2;
const int Led = 13;
//variáveis verificar vazamentos
const int MQ135 = A0;
const int buzzer = 12;
// variaveis sensor temperatura e umidade
#define DHTPINO A1
#define DHTTIPO DHT11

DHT dht(DHTPINO, DHTTIPO);
LiquidCrystal_I2C lcd(0x27, 20, 4);

//
Servo motor; //variavel do tipo servo

void acenderLEDaodetectarpresenca() {
  int estadoPIR = digitalRead(PIR);
  if (estadoPIR == HIGH) {
    digitalWrite(Led, HIGH);
    Serial.println("Presença detectada;D");

  } else {
    digitalWrite(Led, LOW);
    Serial.println("Ninguém por perto:(");
  }
}

void verificarvazamentosdegas (){
int estadoMQ135 = analogRead(MQ135);
//Serial.println(estadoMQ135);
if(estadoMQ135 >= 200){
  alarme_dois_tons();
}
else {
  noTone(buzzer);
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

void verificartemperaturaeumidade(){
  float temperatura = dht.readTemperature(); //shift+alt+ seta para baixo(copia)
  float umidade = dht.readHumidity();

  lcd.clear();//limpa o lcd
  lcd.setCursor(0, 0);//0 - Primeira linha
  lcd.print("Temperatura: " + String(temperatura) + "c");
  lcd.setCursor(0, 1);//1 - Segunda linha
  lcd.print("Umidade: " + String(umidade) + "%");

  //Serial.println("Temperatura: " + String(temperatura) + "C");
 // Serial.println("Umidade: " + String(umidade) + "%");
  delay(5000);
}



void abrirportacommotor(){
  motor.write(180);
  delay(1500);
}

void fecharportacommotor(){
  motor.write(0);
    delay(1500);
}

void setup() {
  Serial.begin(9600);

  dht.begin();//inicializando o DHT11
  lcd.init(); //lcd.begin();

  lcd.backlight();

  pinMode(Led, OUTPUT);
  pinMode(MQ135, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(DHT11, INPUT);
  motor.attach(3);

  //
motor.write(0); //por enquanto sera angulo 0

  Serial.println("Sensores sendo calibrados, aguarde um pouco...");
  delay(10000);
  Serial.println("Sensores calibrados! Obrigada por esperar!!:D");

  lcd.setCursor(0, 0);//primeira linha (do inicio até o final)
  lcd.print("Iniciando leituras"); 
  lcd.setCursor(0, 1);//segunda linha
  lcd.print("Obrigada! :D"); 
}

void loop() {
 // acenderLEDaodetectarpresenca();
 // verificarvazamentosdegas ();
// verificartemperaturaeumidade();
  abrirportacommotor();
  fecharportacommotor();

  
}