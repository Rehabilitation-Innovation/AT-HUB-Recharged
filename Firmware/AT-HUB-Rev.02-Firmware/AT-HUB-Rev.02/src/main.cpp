#include "main.h"
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/StackMacros.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "stdbool.h"
#include <Arduino.h>
#include <BluetoothSerial.h>

#include "inputhandlertask.h"

#include "blutoothtask.h"

#include "Wire.h"

#include "pins_arduino.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

TaskHandle_t bluetoothtask_handle;
TaskHandle_t inputtask_handle;

AT_HUB_State_t global_state;

void init_hardware();

void setup()
{

  Serial.begin(115200);
  // SerialBT.begin("AT-HUB-1"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  if (pdPASS == xTaskCreatePinnedToCore(
                    bluetoothTask,
                    "Task Bluetooth",
                    4096,
                    (void *)&global_state, //(void *)buttonstates,
                    1,
                    &bluetoothtask_handle, 0))
  {
    Serial.println("Success Created task Bluetooth");
  }
  else
  {
    Serial.println("Blu tooth task creation error");
  }

  if (pdPASS == xTaskCreatePinnedToCore(
                    inputhandlerTask,
                    "Task Input Handler",
                    4096,
                    (void *)&global_state, //(void *)buttonstates,
                    1,
                    &bluetoothtask_handle, 1))
  {
    Serial.println("Success Created task Input handler");
  }
  else
  {
    Serial.println("Handler task creation error");
  }

}
void loop()
{

}

void init_hardware()
{
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN5, INPUT_PULLUP);

  pinMode(BACKUP_BTN1, INPUT_PULLUP);
  pinMode(BACKUP_BTN2, INPUT_PULLUP);
  pinMode(BACKUP_BTN3, INPUT_PULLUP);
  pinMode(BACKUP_BTN4, INPUT_PULLUP);
  pinMode(BACKUP_BTN5, INPUT_PULLUP);

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY4, LOW);
  digitalWrite(RELAY5, LOW);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
}