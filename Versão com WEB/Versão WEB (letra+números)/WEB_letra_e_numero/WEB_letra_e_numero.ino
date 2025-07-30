#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(&Wire, 0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(&Wire, 0x41);

#define tempo_letra 4000 //Tempo que a letra fica configurada (em ms)
#define numeroCelulas 4 //Numero de celulas conecta
#define SERVOMIN  205 // Pulso mínimo (~1ms)
#define SERVOMAX  409 // Pulso máximo (~2ms)

// Definições WebServer
const char* ssid = "PET ELÉTRICA UFBA";
const char* password = "12345678";

AsyncWebServer server(80);

// Declarando as funções
void analisarCaractere(String inputMessage);
void testaSistema();
String completarString(String s);  // Declaração da função faltante
String ajustaStringNumero(String entrada);

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  
  pwm1.begin();
  pwm1.setPWMFreq(50);  // Frequência típica para servos (50 Hz)
  pwm2.begin();
  pwm2.setPWMFreq(50);

  testaSistema();

  // Configuração do Wi-Fi
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Ponto de Acesso IP: ");
  Serial.println(IP);

  // Página inicial
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html lang="pt-BR">
      <head>
          <meta charset="UTF-8">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <title>Display Braille</title>
      </head>
      <body>
          <h2>Display Braille PET ELÉTRICA UFBA</h2>
          <form action="/get" method="get">
              <label for="input1">Digite uma palavra ou número:</label>
              <input type="text" id="input1" name="input1" required>
              <button type="submit">Enviar</button>
          </form>
      </body>
      </html>
    )rawliteral";
    request->send(200, "text/html", html);
  });

  // Corrigido: estava faltando um fechamento de bloco aqui
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam("input1")) {
      inputMessage = request->getParam("input1")->value();
      Serial.println(inputMessage);

      String html = "A mensagem enviada para o Display Braille foi: " + inputMessage +
                    "<br><a href=\"/\">Voltar para a Página Inicial</a>";
      request->send(200, "text/html", html);

      // Opcional: Chamar função aqui para ativar os servos, se quiser:
      // inputMessage = completarString(inputMessage);
      // analisarCaractere(inputMessage);
    }
  }); // <<--- este fechamento estava faltando

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  if (Serial.available() > 0) {
    delay(1000);
    String inputMessage = Serial.readString();
    inputMessage = ajustaStringNumero(inputMessage);
    inputMessage = completarString(inputMessage);
    analisarCaractere(inputMessage);
  }
}

//Função que faz a identificação do numero
String ajustaStringNumero(String entrada) {
  String saida = "";
  for (int i = 0; i < entrada.length(); i++) {
    char c = entrada[i];
    
    if (isDigit(c)) {
      // Se o caractere anterior não for um número, adiciona o "L" invertido (símbolo Unicode U+2143)
      if (i == 0 || !isDigit(entrada[i - 1])) {
        saida += "*"; // L invertido
      }
      saida += c;

      // Verifica o próximo caractere
      //if (i < entrada.length() - 1) {
      //  char proximo = entrada[i + 1];
      //  if (!isDigit(proximo) && proximo != ' ') {
      //    saida += ';';
      //  }
      //}
    } else {
      saida += c;
    }
  }
  
  return saida;
}
// Função para completar string
String completarString(String s) {
  int resto = s.length() % numeroCelulas;
  if (resto != 0) {
    int faltando = 4 - resto;
    for (int i = 0; i < faltando; i++) {
      s += ' ';
    }
  }
  return s;
}
// Testa todos os servos
void testaSistema() {
  for (int i = 0; i < 15; i++) {
    pwm1.setPWM(i, 0, SERVOMIN); pwm2.setPWM(i, 0, SERVOMIN);
    delay(2);
  }
  delay(1500);
  for (int i = 0; i < 15; i++) {
    pwm1.setPWM(i, 0, SERVOMAX); pwm2.setPWM(i, 0, SERVOMAX);
    delay(2);
  }
  delay(1500);
  for (int i = 0; i < 15; i++) {
    pwm1.setPWM(i, 0, SERVOMIN); pwm2.setPWM(i, 0, SERVOMIN);
    delay(2);
  }
}
//Aciona os pinos
void analisarCaractere(String inputMessage) {
  for (int k = 0; k < inputMessage.length(); k += numeroCelulas) {
    Serial.println(k);
    int posicaogeral = k;
    int ajust1 = 0; int ajust2 = 0;
    for (int i = 0; i < numeroCelulas; i++) {
      char inputChar = inputMessage.charAt(posicaogeral + i);
      if (i <= 1) {
        if (strchr("abcdefghklmnopqruvxyz12345678", inputChar)) pwm1.setPWM(ajust1, 0, SERVOMAX);
        if (strchr("bfghijlpqrstvw026789", inputChar)) pwm1.setPWM(ajust1 + 1, 0, SERVOMAX);
        if (strchr("klmnopqrstuvxyz*", inputChar)) pwm1.setPWM(ajust1 + 2, 0, SERVOMAX);
        if (strchr("cdfgijmnpqstwxy034679*", inputChar)) pwm1.setPWM(ajust1 + 3, 0, SERVOMAX);
        if (strchr("deghjnoqrtwyz04578*", inputChar)) pwm1.setPWM(ajust1 + 4, 0, SERVOMAX);
        if (strchr("uvwxyz*", inputChar)) pwm1.setPWM(ajust1 + 5, 0, SERVOMAX);
        ajust1 = 6;
      } else {
        if (strchr("abcdefghklmnopqruvxyz12345678", inputChar)) pwm2.setPWM(ajust2, 0, SERVOMAX);
        if (strchr("bfghijlpqrstvw026789", inputChar)) pwm2.setPWM(ajust2 + 1, 0, SERVOMAX);
        if (strchr("klmnopqrstuvxyz*", inputChar)) pwm2.setPWM(ajust2 + 2, 0, SERVOMAX);
        if (strchr("cdfgijmnpqstwxy034679*", inputChar)) pwm2.setPWM(ajust2 + 3, 0, SERVOMAX);
        if (strchr("deghjnoqrtwyz04578*", inputChar)) pwm2.setPWM(ajust2 + 4, 0, SERVOMAX);
        if (strchr("uvwxyz*", inputChar)) pwm2.setPWM(ajust2 + 5, 0, SERVOMAX);
        ajust2 = 6;
      }
    }

    delay(tempo_letra); // Mantém posição por 4s
    for (int i = 0; i < 15; i++) {
      pwm1.setPWM(i, 0, SERVOMIN); pwm2.setPWM(i, 0, SERVOMIN);
      delay(2);
    }
  }

  for (int i = 0; i < 15; i++) {
    pwm1.setPWM(i, 0, SERVOMIN); pwm2.setPWM(i, 0, SERVOMIN);
    delay(2);
  }
  delay(1500);
}
