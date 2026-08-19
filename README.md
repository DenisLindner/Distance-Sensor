# Sensor de Distância com ESP32

Projeto de um sensor de distância baseado no **ESP32** e no sensor ultrassônico **HC-SR04**. A distância medida é exibida em um display OLED, enquanto um buzzer emite alertas sonoros cada vez mais frequentes conforme um objeto se aproxima.

## Funcionalidades

- Mede distâncias com o sensor ultrassônico HC-SR04.
- Exibe a distância em centímetros em um display OLED SSD1306.
- Informa quando o objeto está fora do alcance ou quando o sensor está desconectado.
- Emite alertas sonoros progressivos para objetos a até 30 cm.
- Exibe as medições no monitor serial a 115200 baud.

## Componentes

- 1 ESP32 DOIT DevKit V1
- 1 sensor ultrassônico HC-SR04
- 1 display OLED SSD1306 128x64 com comunicação I2C
- 1 buzzer ativo
- Resistores para divisor de tensão no pino ECHO
- Protoboard e jumpers

> [!CAUTION]
> O pino ECHO do HC-SR04 pode fornecer 5 V, mas as entradas do ESP32 operam em 3,3 V. Use um divisor de tensão ou conversor de nível lógico entre o ECHO e o GPIO 32 para proteger a placa.

## Ligações

### HC-SR04

| HC-SR04 | ESP32 |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | GPIO 5 |
| ECHO | GPIO 32, por meio de divisor de tensão |

### Display OLED SSD1306

| OLED | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

O endereço I2C configurado para o display é `0x3C`.

### Buzzer

| Buzzer | ESP32 |
|---|---|
| Positivo | GPIO 19 |
| Negativo | GND |

## Alertas de proximidade

O buzzer permanece desligado quando a distância é superior a 30 cm. Dentro dessa faixa, o intervalo entre os bipes diminui conforme o objeto se aproxima:

| Distância | Intervalo entre bipes |
|---|---:|
| Até 2 cm | 10 ms |
| Até 5 cm | 100 ms |
| Até 10 cm | 250 ms |
| Até 15 cm | 350 ms |
| Até 20 cm | 500 ms |
| Até 30 cm | 1000 ms |
| Acima de 30 cm | Buzzer desligado |

Cada bipe permanece ativo por 50 ms.

## Como executar

Este projeto utiliza o [PlatformIO](https://platformio.org/) com o framework Arduino.

1. Clone o repositório e acesse a pasta do projeto:

   ```bash
   git clone <URL_DO_REPOSITORIO>
   cd Distance-Sensor
   ```

2. Conecte o ESP32 ao computador por USB.

3. Compile o firmware:

   ```bash
   pio run
   ```

4. Envie o firmware para a placa:

   ```bash
   pio run --target upload
   ```

5. Abra o monitor serial:

   ```bash
   pio device monitor
   ```

Também é possível abrir a pasta no VS Code com a extensão PlatformIO IDE e usar os comandos **Build**, **Upload** e **Serial Monitor** pela barra de ferramentas.

## Dependências

As bibliotecas são instaladas automaticamente pelo PlatformIO conforme o arquivo `platformio.ini`:

- Adafruit GFX Library
- Adafruit SSD1306

## Estrutura do projeto

```text
Distance-Sensor/
|-- include/          # Arquivos de cabeçalho
|-- lib/              # Bibliotecas locais
|-- src/
|   `-- main.cpp      # Código principal do firmware
|-- test/             # Testes do projeto
|-- platformio.ini    # Configuração do PlatformIO
`-- README.md         # Documentação
```

## Funcionamento

O ESP32 envia um pulso de 10 microssegundos ao pino TRIG do HC-SR04 e mede o tempo de retorno recebido no pino ECHO. Esse tempo é convertido em centímetros, considerando a velocidade do som, e o resultado é apresentado no OLED e no monitor serial.

Caso nenhum eco seja recebido dentro de 30 milissegundos, o sistema desliga o buzzer e informa que o objeto está fora do alcance ou que o sensor pode estar desconectado.
