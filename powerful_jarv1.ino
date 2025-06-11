int botao = 7;




const int mq135 = A0;//no esp 32 = 34
const int buzzer = 12;
int contador = 0;

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


void alarme_dois_tons() {
int freqAlta = 2000;
int freqBaixa = 800;
int duracaoTom = 1000;

  tone(buzzer, freqAlta, duracaoTom);
delay(duracaoTom);
tone(buzzer, freqBaixa, duracaoTom);
delay(duracaoTom);

}//fim do ale


void TocarCampainha(){
int estado = digitalRead(botao);

  if (estado == HIGH) {
    tone(buzzer, 1000);  // Frequência de 1000Hz
  } else {
    noTone(buzzer);
  }
  
 
}//Fim da campainha



void setup()
{
 Serial.begin(9600);
  pinMode(mq135, INPUT); 
pinMode(buzzer, OUTPUT);
   pinMode(botao, INPUT);
}

void loop()
{
  verificarVazamentoDeGas();
   
  TocarCampainha();
}