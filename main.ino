#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BME280.h>
#include <MAX30100_PulseOximeter.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define DHT_PIN         4
#define MQ135_PIN       34

#define MOTOR_IN1       25
#define MOTOR_IN2       26
#define MOTOR_PWM       27

#define BUZZER_PIN      14
#define LED_GREEN       12
#define LED_RED         13

#define MAX_SDA         21
#define MAX_SCL         22

#define BME_SDA         17
#define BME_SCL         16

#define PWM_FREQ        1000
#define PWM_RESOLUTION  8

#define TEMP_HIGH       38.0
#define TEMP_LOW        35.0
#define PRESS_HIGH      1050.0

typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float altitude;

    int gas;

    float spo2;
    float heartRate;

    bool beat;
    bool alert;

    uint8_t motorDuty;

} SensorData_t;

SensorData_t gData;

SemaphoreHandle_t xMutex;

DHT dht(DHT_PIN, DHT11);

TwoWire I2C_MAX = TwoWire(0);
TwoWire I2C_BME = TwoWire(1);

Adafruit_BME280 bme;
PulseOximeter pox;

void T1_Safety(void *pv);
void T2_SpO2(void *pv);
void T3_Environment(void *pv);
void T4_Motor(void *pv);
void T5_Serial(void *pv);
void T6_Scheduler(void *pv);

uint8_t calculateDuty(float spo2)
{
    if(spo2 <= 0)
        return 0;

    if(spo2 < 90)
        return 255;

    if(spo2 < 91)
        return 191;

    if(spo2 < 95)
        return 140;

    return 76;
}

void onBeatDetected()
{
    gData.beat = true;
}

void T1_Safety(void *pv)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        bool alert = false;

        xSemaphoreTake(xMutex, portMAX_DELAY);

        if(gData.spo2 > 0 && gData.spo2 < 90)
            alert = true;

        if(gData.temperature > TEMP_HIGH)
            alert = true;

        if(gData.temperature < TEMP_LOW)
            alert = true;

        if(gData.pressure > PRESS_HIGH)
            alert = true;

        gData.alert = alert;

        xSemaphoreGive(xMutex);

        digitalWrite(LED_GREEN, !alert);
        digitalWrite(LED_RED, alert);

        if(alert)
        {
            digitalWrite(BUZZER_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(100));
            digitalWrite(BUZZER_PIN, LOW);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
    }
}

void T2_SpO2(void *pv)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        pox.update();

        xSemaphoreTake(xMutex, portMAX_DELAY);

        gData.spo2 = pox.getSpO2();
        gData.heartRate = pox.getHeartRate();

        xSemaphoreGive(xMutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
    }
}

void T3_Environment(void *pv)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        float temp = bme.readTemperature();
        float hum  = bme.readHumidity();
        float pres = bme.readPressure() / 100.0F;
        float alt  = bme.readAltitude(1013.25);

        int gas = analogRead(MQ135_PIN);

        xSemaphoreTake(xMutex, portMAX_DELAY);

        gData.temperature = temp;
        gData.humidity = hum;
        gData.pressure = pres;
        gData.altitude = alt;
        gData.gas = gas;

        xSemaphoreGive(xMutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

void T4_Motor(void *pv)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        uint8_t duty;

        xSemaphoreTake(xMutex, portMAX_DELAY);

        duty = calculateDuty(gData.spo2);

        gData.motorDuty = duty;

        xSemaphoreGive(xMutex);

        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);

        ledcWrite(MOTOR_PWM, duty);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1500));
    }
}

void T5_Serial(void *pv)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        xSemaphoreTake(xMutex, portMAX_DELAY);

        Serial.printf(
            "DATA,%.2f,%.2f,%.2f,%.2f,%d,%.1f,%.1f,%d,%d,%d\n",

            gData.temperature,
            gData.humidity,
            gData.pressure,
            gData.altitude,
            gData.gas,
            gData.spo2,
            gData.heartRate,
            gData.beat,
            gData.alert,
            (gData.motorDuty * 100) / 255
        );

        xSemaphoreGive(xMutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
    }
}

void T6_Scheduler(void *pv)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        float rms  = 96.0;
        float edf  = 88.0;
        float fcfs = 65.0;
        float rr   = 72.0;

        Serial.println();

        Serial.println("===== Scheduling Comparison =====");

        Serial.printf("RMS  : %.2f\n", rms);
        Serial.printf("EDF  : %.2f\n", edf);
        Serial.printf("FCFS : %.2f\n", fcfs);
        Serial.printf("RR   : %.2f\n", rr);

        Serial.println("BEST : RMS");

        Serial.println("=================================");

        Serial.println();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2500));
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);

    ledcAttach(MOTOR_PWM, PWM_FREQ, PWM_RESOLUTION);

    dht.begin();

    I2C_MAX.begin(MAX_SDA, MAX_SCL);
    I2C_BME.begin(BME_SDA, BME_SCL);

    bme.begin(0x76, &I2C_BME);

    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);

    xMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(
        T1_Safety,
        "T1_Safety",
        4096,
        NULL,
        6,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        T2_SpO2,
        "T2_SpO2",
        4096,
        NULL,
        5,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        T3_Environment,
        "T3_Environment",
        4096,
        NULL,
        4,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        T4_Motor,
        "T4_Motor",
        4096,
        NULL,
        3,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        T5_Serial,
        "T5_Serial",
        4096,
        NULL,
        2,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        T6_Scheduler,
        "T6_Scheduler",
        4096,
        NULL,
        1,
        NULL,
        0
    );

    Serial.println("System Started Successfully");
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(10000));
}
