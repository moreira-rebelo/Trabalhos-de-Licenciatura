/*
 ▄▄▄▄▄▄▄▄▄▄▄   ▄▄▄▄▄▄▄▄▄   ▄▄▄▄▄▄▄▄▄▄▄
▐░░░░░░░░░░░▌ ▐░░░░░░░░░▌ ▐░░░░░░░░░░░▌
▐░█▀▀▀▀▀▀▀▀▀ ▐░█░█▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌
▐░▌          ▐░▌▐░▌    ▐░▌▐░▌       ▐░▌    Trabalho Prático de Sistemas Embebidos e de Tempo Real
▐░▌ ▄▄▄▄▄▄▄▄ ▐░▌ ▐░▌   ▐░▌▐░█▄▄▄▄▄▄▄█░▌    Allan Sales Aleluia, a21990
▐░▌▐░░░░░░░░▌▐░▌  ▐░▌  ▐░▌▐░░░░░░░░░░░▌    Francisco Moreira Rebêlo, a16443
▐░▌ ▀▀▀▀▀▀█░▌▐░▌   ▐░▌ ▐░▌ ▀▀▀▀▀▀▀▀▀█░▌    José Carlos Paschoal, a 15926
▐░▌       ▐░▌▐░▌    ▐░▌▐░▌          ▐░▌
▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄█░█░▌ ▄▄▄▄▄▄▄▄▄█░▌
▐░░░░░░░░░░░▌ ▐░░░░░░░░░▌ ▐░░░░░░░░░░░▌
 ▀▀▀▀▀▀▀▀▀▀▀   ▀▀▀▀▀▀▀▀▀   ▀▀▀▀▀▀▀▀▀▀▀

 A arquitetura do sistema pode ser consultada aqui: https://www.tinkercad.com/things/6fuR2PdYajt-g09arquitetura?sharecode=DZn1LgPJ6uCre0JBuYuB8YrbdNv2KH3S8VHToi4_N8Y
*/

#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <MQTT.h>

#include "DHT.h"
#include "pitches.h"

// Definição do endereço MAC e IP do dispositivo
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 1, 15};

// -----------------------  Definições de pinos e tipos de sensores/dispositivos  -----------------------
#define DHTPIN 2
#define DHTTYPE DHT11
#define SS_PIN 5
#define RST_PIN 9
#define LED_RED 8
#define LED_GREEN 7
#define LED_BLUE 6
#define BUZZER 4
#define BUT_INTERRUPT 3

// -----------------------  Inicialização dos objetos e variáveis globais  -----------------------
EthernetClient net;
MQTTClient client;
DHT dht(DHTPIN, DHTTYPE);
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte nuidPICC[4];
float tempSetpoint = 21;
float tempActual;
String fechaduraStatus = "LOCKED";

// -----------------------  Void Setup  -----------------------
void setup()
{
    Serial.begin(9600);      // Inicia a comunicação serial
    Ethernet.begin(mac, ip); // Inicia a conexão Ethernet com o endereço MAC e IP especificados

    client.begin("192.168.1.14", net); // Inicia o cliente MQTT com o endereço do servidor MQTT
    client.onMessage(messageReceived); // Define a função de callback para mensagens MQTT

    connect(); // Estabelece a conexão com o servidor MQTT

    dht.begin();     // Inicializa o sensor DHT
    SPI.begin();     // Inicializa a comunicação SPI
    rfid.PCD_Init(); // Inicializa o módulo RFID

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(BUT_INTERRUPT, INPUT_PULLUP);

    // Define a chave para o módulo RFID
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    attachInterrupt(digitalPinToInterrupt(BUT_INTERRUPT), interrupt, RISING);

}

// -----------------------  Void Loop  -----------------------
void loop()
{
    client.loop(); // Mantém a conexão MQTT ativa

    // Reconecta ao servidor MQTT, se necessário
    if (!client.connected())
    {
        connect();
    }

    rfidRead(); // Função para ler o cartão RFID

    ac(tempSetpoint, tempActual);   // Chama a função para controlar o sistema de ar condicionado

    fechadura(fechaduraStatus);     // Chama a função para controlar o estado da fechadura

    readDht(); // Chama a função para ler a temperatura

}

// -----------------------  Funções auxiliares -----------------------

// Lê a temperatura atual do sensor DHT e publica no MQTT
void readDht()
{
    tempActual = dht.readTemperature();

    String temperatureString = String(tempActual);
    client.publish("actualTemp", temperatureString.c_str());
    //Serial.println(tempActual);

}

// Lê o cartão RFID e realiza ações correspondentes
void rfidRead()
{
    if (!rfid.PICC_IsNewCardPresent())
        return;

    if (!rfid.PICC_ReadCardSerial())
        return;

    for (byte i = 0; i < 4; i++)
    {
        nuidPICC[i] = rfid.uid.uidByte[i];
    }

    cardVerification(rfid.uid.uidByte, rfid.uid.size);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

// Verifica se o cartao é válido
void cardVerification(byte *buffer, byte bufferSize)
{
    // Definição dos cartões válidos
    byte predeterminedUID1[] = {0x54, 0x11, 0xB3, 0xCB};
    byte predeterminedUID2[] = {0x23, 0x31, 0x88, 0x0C};

    // Variáveis locais
    bool uidMatch = false;

    // Imprime o UID lido
    Serial.print("\nUID Lido: ");
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
    Serial.println(); // Print a new line after displaying the UID

    // Verificar se o buffer dos cartões é válido
    for (byte i = 0; i < bufferSize; i++)
    {
        if (buffer[i] == predeterminedUID1[i] || buffer[i] == predeterminedUID2[i])
        {
            uidMatch = true;
        }
        else
        {
            uidMatch = false;
            break;
        }
    }

    // Verifica se o cartão é válido
    if (uidMatch && (bufferSize == sizeof(predeterminedUID1)))
    {
        Serial.println("Cartão válido!");
        fechaduraStatus = "UNLOCKED";

        // Converte o UID de array para string
        String uidString = "";
        for (byte i = 0; i < bufferSize; i++)
        {
            uidString += String(buffer[i], HEX);
        }
        client.publish("uid", uidString.c_str());
    }
    else
    {
        Serial.println("Cartão inválido!");
        accessDenied();
    }
}

// Estabelece a conexão com o servidor MQTT
void connect()
{
    Serial.print("A ligar ao servidor MQTT...");
    while (!client.connect("arduino", "public", "public"))
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nLigado!");

    client.subscribe("fechaduraActuator");
    client.subscribe("tempSetpoint");
}

// Função de callback para mensagens MQTT recebidas
void messageReceived(String &topic, String &payload)
{
    if (topic == "fechaduraActuator" && payload == "ON")
    {
        fechaduraStatus = "UNLOCKED";
    }

    if (topic == "tempSetpoint")
    {
        tempSetpoint = payload.toFloat();
        Serial.print("Received Temperature Setpoint: ");
        Serial.println(payload);
    }
}

// Controla o sistema de ar condicionado com base na temperatura atual e setpoint
void ac(float tempSetpoint, float tempActual)
{
    if (tempActual < tempSetpoint)
    {
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
        // Serial.println("Aquecer");
        client.publish("acStatus", "Heating");
    }
    else if (tempActual > tempSetpoint)
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, HIGH);
        // Serial.println("Arrefecer");
        client.publish("acStatus", "Cooling");
    }
    else if (tempActual == tempSetpoint)
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
        // Serial.println("Desligado");
        client.publish("acStatus", "OFF");
    }
}

// Controla o estado da fechadura
void fechadura(String &fechaduraStatus)
{
    if (fechaduraStatus == "LOCKED")
    {
        // Serial.println("Locked");
        client.publish("fechaduraStatus", "Locked");
    }
    else if (fechaduraStatus == "UNLOCKED")
    {
        Serial.println("Unlocked");
        client.publish("fechaduraStatus", "Unlocked");
        accessGranted();
        fechaduraStatus = "LOCKED";
    }
}

// Quando acesso concedido
void accessGranted()
{
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    tone(BUZZER, NOTE_C5, 100);
    delay(50);
    tone(BUZZER, NOTE_G5, 100);
    delay(50);
    tone(BUZZER, NOTE_C6, 100);
    delay(2500);
    digitalWrite(LED_GREEN, LOW);
}

// Quando acesso negado
void accessDenied()
{
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    tone(BUZZER, NOTE_C6, 100);
    delay(100);
    tone(BUZZER, NOTE_C5, 1000);
    delay(2500);
    digitalWrite(LED_RED, LOW);
}

void interrupt() {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    tone(BUZZER, NOTE_C6, 100);
    
}
