#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(&Wire, 0x40); Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(&Wire, 0x41);

#define SERVOMIN  205 // Pulso mínimo (~1ms)
#define SERVOMAX  409 // Pulso máximo (~2ms)

//Declarando as funções
void analisarCaractere(String inputString);
void testaSistema();

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  pwm1.begin(); pwm1.setPWMFreq(50);  //Inicia e define a frequência típica para servos (50 Hz)
  pwm2.begin(); pwm2.setPWMFreq(50);  //Inicia e define a frequência típica para servos (50 Hz)
  //Testa o sietam para ver se está tudo bem com os servos
  testaSistema();
}
void loop() {

  if (Serial.available() > 0) {
    delay(1000);
    //Lê a palavra da serial
    String inputString = Serial.readString();
    //Ajusta a string para exibir numeros
    inputString = ajustaStringNumero(inputString);
    Serial.println(inputString);
    // Completa a string para ser multiplo de 4 e analisa cada caractere da string
    inputString = completarString(inputString);
    analisarCaractere(inputString);
  }
}

// Função para analisar um caractere e controlar os servos
void analisarCaractere(String inputString) {
  // Move os servos de acordo com o caractere recebido
  for (int k = 0; k < inputString.length(); k+=4) {
    Serial.println(k);
    int posicaogeral = k;
    int ajust1 = 0; int ajust2 = 0;
    for (int i = 0; i<4; i++) {
      char inputChar = inputString.charAt(posicaogeral+i);
      if (i <= 1){
        if (strchr("abcdefghklmnopqruvxyz12345678", inputChar)) {
          pwm1.setPWM(ajust1, 0, SERVOMAX);
        }
        if (strchr("bfghijlpqrstvw026789;", inputChar)) {
          pwm1.setPWM(ajust1+1, 0, SERVOMAX);
        }
        if (strchr("klmnopqrstuvxyz*;", inputChar)) {
          pwm1.setPWM(ajust1+2, 0, SERVOMAX);
        }
        if (strchr("cdfgijmnpqstwxy034679*", inputChar)) {
          pwm1.setPWM(ajust1+3, 0, SERVOMAX);
        }
        if (strchr("deghjnoqrtwyz04578*", inputChar)) {
          pwm1.setPWM(ajust1+4, 0, SERVOMAX);
        }
        if (strchr("uvwxyz*", inputChar)) {
          pwm1.setPWM(ajust1+5, 0, SERVOMAX);
        }
        ajust1 = 6;
      }else{
        if (strchr("abcdefghklmnopqruvxyz12345678", inputChar)) {
            pwm2.setPWM(ajust2, 0, SERVOMAX);
          }
          if (strchr("bfghijlpqrstvw026789;", inputChar)) {
            pwm2.setPWM(ajust2+1, 0, SERVOMAX);
          }
          if (strchr("klmnopqrstuvxyz*;", inputChar)) {
            pwm2.setPWM(ajust2+2, 0, SERVOMAX);
          }
          if (strchr("cdfgijmnpqstwxy034679*", inputChar)) {
            pwm2.setPWM(ajust2+3, 0, SERVOMAX);
          }
          if (strchr("deghjnoqrtwyz04578*", inputChar)) {
            pwm2.setPWM(ajust2+4, 0, SERVOMAX);
          }
          if (strchr("uvwxyz*", inputChar)) {
            pwm2.setPWM(ajust2+5, 0, SERVOMAX);
          }
          ajust2 = 6;
      }
    }
    // Mantém os servos na posição por 1.5 segundos
    delay(4000);
    // Retorna todos os servos à posição baixa
    for (int i = 0; i < 15; i++) {
         pwm1.setPWM(i, 0, SERVOMIN); pwm2.setPWM(i, 0, SERVOMIN);
         delay(2);
    }
  }
  // Retorna todos os servos à posição baixa
  for (int i = 0; i < 15; i++) {
       pwm1.setPWM(i, 0, SERVOMIN); pwm2.setPWM(i, 0, SERVOMIN);
       delay(2);
  }
  delay(1500);
}
//Compla a string com espços para que toda frase ou palavra escrita seja multipla de 4
String completarString(String s) {
  int resto = s.length() % 4;
  if (resto != 0) {
    int faltando = 4 - resto;
    for (int i = 0; i < faltando; i++) {
      s += ' '; // Adiciona espaços para completar
    }
  }
  return s;
}
//Testa o sistema em um monimento sincrono de abaixar, levantar e abaixar todos os servos
void testaSistema (){
  // Retorna todos os servos à posição baixa
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
      if (i < entrada.length() - 1) {
        char proximo = entrada[i + 1];
        if (!isDigit(proximo) && proximo != ' ') {
          saida += ';';
        }
      }
    } else {
      saida += c;
    }
  }
  
  return saida;
}
