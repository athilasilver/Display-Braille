#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(&Wire, 0x40); Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(&Wire, 0x41);

#define SERVOMIN  205 // Pulso mínimo (~1ms)
#define SERVOMAX  409 // Pulso máximo (~2ms)

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  pwm1.begin(); pwm1.setPWMFreq(50);  //Inicia e define a frequência típica para servos (50 Hz)
  pwm2.begin(); pwm2.setPWMFreq(50);  //Inicia e define a frequência típica para servos (50 Hz)
}
void loop() {
  Serial.println("Girando pwm1 para posição máxima");// Gira o pino 0 de pwm1 para posição máxima
  pwm1.setPWM(0, 0, SERVOMAX); // parâmetros são (canal, inicio do pulso, fim do pulso)
  Serial.println("Girando pwm2 para posição mínima");// Gira o pino 0 de pwm2 para posição mínima
  pwm2.setPWM(0, 0, SERVOMIN);
  delay(1000);
  
  Serial.println("Girando pwm1 para posição mínima");// Gira o pino 0 de pwm1 para posição mínima
  pwm1.setPWM(0, 0, SERVOMIN);
  Serial.println("Girando pwm2 para posição máxima");// Gira o pino 0 de pwm2 para posição máxima
  pwm2.setPWM(0, 0, SERVOMAX);
  delay(1000);
}
