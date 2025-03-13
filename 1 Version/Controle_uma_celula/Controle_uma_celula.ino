#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// Definição dos pinos dos servomotores
const int servo1Pin = 5;
const int servo2Pin = 19;
const int servo3Pin = 16;
const int servo4Pin = 12;
const int servo5Pin = 27;
const int servo6Pin = 25;

// Criação dos objetos Servo
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// Posições dos servos
const int posicaoAltaD = 50;
const int posicaoBaixaD = 80;

const int posicaoAltaE = 160;
const int posicaoBaixaE = 130;

// Definições WebServer
const char* ssid = "PET ELÉTRICA UFBA";
const char* password = "12345678";

AsyncWebServer server(80);

// Funções para movimentar os servos
void analisarCaractere(char inputChar) {
  servo1.write(posicaoBaixaD);
  servo2.write(posicaoBaixaD);
  servo3.write(posicaoBaixaD);
  servo4.write(posicaoBaixaE);
  servo5.write(posicaoBaixaE);
  servo6.write(posicaoBaixaE);

  if (strchr("abcdefghklmnopqruvxyz12345678", inputChar)) {
    servo1.write(posicaoAltaD);
  }
  if (strchr("bfghijlpqrstvw026789", inputChar)) {
    servo2.write(posicaoAltaD);
  }
  if (strchr("klmnopqrstuvxyz", inputChar)) {
    servo3.write(posicaoAltaD);
  }
  if (strchr("cdfgijmnpqstwxy034679", inputChar)) {
    servo4.write(posicaoAltaE);
  }
  if (strchr("deghjnoqrtwyz04578", inputChar)) {
    servo5.write(posicaoAltaE);
  }
  if (strchr("uvwxyz", inputChar)) {
    servo6.write(posicaoAltaE);
  }

  delay(1500);

  servo1.write(posicaoBaixaD);
  servo2.write(posicaoBaixaD);
  servo3.write(posicaoBaixaD);
  servo4.write(posicaoBaixaE);
  servo5.write(posicaoBaixaE);
  servo6.write(posicaoBaixaE);
  delay(500);
}

void identificarNumero() {
  servo1.write(posicaoAltaD);
  servo2.write(posicaoAltaD);
  servo3.write(posicaoAltaD);
  servo4.write(posicaoAltaE);
  delay(1500);
  servo1.write(posicaoBaixaD);
  servo2.write(posicaoBaixaD);
  servo3.write(posicaoBaixaD);
  servo4.write(posicaoBaixaE);
  delay(500);
}

void setup() {
  // Anexa os servos aos pinos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);
  servo6.attach(servo6Pin);

  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Configuração do Wi-Fi
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Ponto de Acesso IP: ");
  Serial.println(IP);

  // Página inicial
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"(
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
    )";
    request->send(200, "text/html", html);
  });

  // Processa a entrada do formulário e redireciona para a página de confirmação
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam("input1")) {
      inputMessage = request->getParam("input1")->value();
      Serial.println(inputMessage);

      // Analisa cada caractere da mensagem
      for (int i = 0; i < inputMessage.length(); i++) {
        char inputChar = inputMessage.charAt(i);
        if (isdigit(inputChar)) {
          identificarNumero();
        }
        analisarCaractere(inputChar);
      }
    } else {
      inputMessage = "Nenhuma mensagem enviada.";
    }

    String html = "A mensagem enviada para o Display Braille foi: " + inputMessage +
                  "<br><a href=\"/\">Voltar para a Página Inicial</a>";
    request->send(200, "text/html", html);
  });

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  // Nada necessário aqui, pois usamos AsyncWebServer
}   

