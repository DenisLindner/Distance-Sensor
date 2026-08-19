#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// HC-SR04 e buzzer
constexpr uint8_t PINO_TRIG = 5;
constexpr uint8_t PINO_ECHO = 32;
constexpr uint8_t PINO_BUZZ = 19;

// OLED
constexpr uint8_t PINO_SDA = 21;
constexpr uint8_t PINO_SCL = 22;

constexpr int LARGURA_TELA = 128;
constexpr int ALTURA_TELA = 64;
constexpr int RESET_OLED = -1;
constexpr uint8_t ENDERECO_OLED = 0x3C;

Adafruit_SSD1306 tela(
    LARGURA_TELA,
    ALTURA_TELA,
    &Wire,
    RESET_OLED
);

void mostrarDistancia(float distanciaCm) {
    tela.clearDisplay();
    tela.setTextColor(SSD1306_WHITE);

    tela.setTextSize(1);
    tela.setCursor(0, 0);
    tela.println("Sensor HC-SR04");
    tela.drawLine(0, 11, 127, 11, SSD1306_WHITE);

    tela.setCursor(0, 18);
    tela.println("Distancia:");

    tela.setTextSize(2);
    tela.setCursor(0, 35);
    tela.print(distanciaCm, 1);
    tela.print(" cm");

    tela.display();
}

void mostrarSemLeitura() {
    tela.clearDisplay();
    tela.setTextColor(SSD1306_WHITE);

    tela.setTextSize(1);
    tela.setCursor(0, 0);
    tela.println("Sensor HC-SR04");
    tela.drawLine(0, 11, 127, 11, SSD1306_WHITE);

    tela.setCursor(0, 25);
    tela.println("Objeto fora");
    tela.println("do alcance!");

    tela.display();
}

void emitirBip(unsigned long intervalo) {
    digitalWrite(PINO_BUZZ, HIGH);
    delay(50);

    digitalWrite(PINO_BUZZ, LOW);
    delay(intervalo);
}

void controlarBuzzer(float distanciaCm) {
    if (distanciaCm <= 2) {
        emitirBip(10);
    } else if (distanciaCm <= 5) {
        emitirBip(100);
    } else if (distanciaCm <= 10) {
        emitirBip(250);
    } else if (distanciaCm <= 15) {
        emitirBip(350);
    } else if (distanciaCm <= 20) {
        emitirBip(500);
    } else if (distanciaCm <= 30) {
        emitirBip(1000);
    } else {
        digitalWrite(PINO_BUZZ, LOW);
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(PINO_TRIG, OUTPUT);
    pinMode(PINO_ECHO, INPUT);
    pinMode(PINO_BUZZ, OUTPUT);

    digitalWrite(PINO_TRIG, LOW);
    digitalWrite(PINO_BUZZ, LOW);

    // Inicia o I2C: SDA no GPIO 21 e SCL no GPIO 22
    Wire.begin(PINO_SDA, PINO_SCL);

    if (!tela.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
        Serial.println("Erro: tela OLED nao encontrada!");

        while (true) {
            delay(100);
        }
    }

    tela.clearDisplay();
    tela.setTextColor(SSD1306_WHITE);
    tela.setTextSize(1);
    tela.setCursor(10, 25);
    tela.println("Sistema iniciado!");
    tela.display();

    Serial.println();
    Serial.println("HC-SR04 e OLED iniciados!");

    delay(1500);
}

void loop() {
    // Dispara o pulso ultrassônico
    digitalWrite(PINO_TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(PINO_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PINO_TRIG, LOW);

    // Aguarda o sinal de retorno
    unsigned long duracao = pulseIn(PINO_ECHO, HIGH, 30000UL);

    if (duracao == 0) {
        Serial.println(
            "Objeto fora do alcance ou sensor desconectado."
        );

        digitalWrite(PINO_BUZZ, LOW);
        mostrarSemLeitura();
    } else {
        float distanciaCm = duracao * 0.0343f / 2.0f;

        Serial.print("Distancia: ");
        Serial.print(distanciaCm, 1);
        Serial.println(" cm");

        mostrarDistancia(distanciaCm);
        controlarBuzzer(distanciaCm);
    }

    delay(10);
}