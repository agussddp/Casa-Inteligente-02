


//bibliotecas
//#include <Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// --- WiFi & MQTT ---
const char* ssid = "ProjetosIoT_Esp32";         //sua rede wifi
const char* password = "Sen@i134";     //senha da sua rede wifi
const char* mqtt_server = "172.16.39.118";  //endereço do broker público
const int mqtt_port = 1883;       //porta do broker público, geralmente 1883

//Tópicos
const char* topic_gas = "suportfortechnology/sala/gas";
const char* topic_porta = "suportfortechnology/sala/porta";
const char* topic_trava = "suportfortechnology/sala/trava";

//Varaiveis - verificarVazamentoDeGas
const int buzzer = 19;
const int MQ135 = 34;
//rele
const int rele = 26;
//Campainha
int botao = 4;

//Porta
const int servoMotor = 13;


//Objetos
WiFiClient espClient;
PubSubClient client(espClient);
Servo motor;

//---Funçoes wifi----
void conectarWiFi() {//verifica conexão wifi para somente depois iniciar o sistema
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconectarMQTT() {//verifica e reconecta a conexão com o broker mqtt
  while (!client.connected()) {
    Serial.print("Reconectando MQTT...");
    String clientId = "ESP32Client-"; 
    clientId += String(WiFi.macAddress());
    if (client.connect("ESP32ClientTest")) {
      Serial.println("Conectado!");
      client.subscribe(topic_gas);//conecta ao topico do led assim que estabelecer ligação com o broker
      client.subscribe(topic_porta);//conecta ao topico da porta assim que estabelecer ligação com o broker
    } else {
      Serial.print("Falha: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void tratarMensagem(char* topic, byte* payload, unsigned int length) {//
  String mensagem = "";
  for (int i = 0; i < length; i++) {//concatena todas os char* para se ter o texto completo em String
    mensagem += (char)payload[i];
  }

  Serial.printf("Mensagem recebida [%s]: %s\n", topic, mensagem.c_str());
  
  //Tranca - Entrada
  if (strcmp(topic, topic_trava) == 0) {//tópico atual é o do led?
    if (mensagem == "Abrir") {
      digitalWrite(rele, HIGH);
    } else if (mensagem == "Fechar") {
      digitalWrite(rele, LOW);
    }
  }
  
  //porta
  if (strcmp(topic, topic_porta) == 0) {//tópico atual é o da porta?
    if (mensagem == "abrir") {
      //DestrancarPorta();
      //delay(500);
      abrirportaservo();
    } else if (mensagem == "fechar") {
      fecharportaservo();
      //delay(500);
      //TrancarPorta();
    }
  }
}

//---Funçoes Casa----
void Campainha() {
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
  motor.write(150);
}

void fecharportaservo() {
  motor.write(0);
}

void TrancarPorta() {


  digitalWrite(rele, LOW);
  Serial.println("Trancado");
}

void DestrancarPorta() {
   //String SenhaDg;

   //Serial.println("Por gentileza digite a Senha para Destravar a Porta ");
   //while (!Serial.available()) {}
   //SenhaDg = Serial.readString();

   //if (SenhaDg) {
  digitalWrite(rele, HIGH);
  Serial.println("Porta desTrancada...");

  /* abrirportaservo();
   TrancarPorta();
   } else {
   if (motor.read() != 0)
   fecharportaservo();
   }

   Serial.println("Senha invalida! Na 3 tentetiva ira ser bloqueada e tambem iremos acionar a policia, fica esperto(a).");

  digitalWrite(rele, LOW);
   Serial.println("Porta destrancada...");
*/
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
  pinMode(rele, OUTPUT);
  pinMode(botao, INPUT_PULLUP);
  motor.attach(servoMotor);

  //Deixa Trancado
  digitalWrite(rele, LOW);

  //
  motor.write(0);
 
  conectarWiFi();//conecta no wifi
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(tratarMensagem);

  Serial.println("Sistema iniciado");

  Serial.println("Sensores sendo calibrados! aguente firme :P");
  delay(10000);
  Serial.println("Sensores calibrados! obrigado por esperar. ");
}

void loop() {
/*if (!client.connected()) reconectarMQTT();//se não tem conexão com o broker, tenta reconectar
client.loop(); //mantém a conexão com o broker serve sempre aberta*/

  verificarVazamentoDeGas();

  tocarCampainha();

  DestrancarPorta();
  delay(900);

  abrirportaservo();
  delay(2000);

  fecharportaservo();
  delay(2000);

  TrancarPorta();
  delay(900);
}