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

#include "BQ27427.h"

// Set BATTERY_CAPACITY to the design capacity of your battery.
const unsigned int BATTERY_CAPACITY = 1000; // e.g. 850mAh battery

BQ27427 lipo(1000U);

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

void printBatteryStats()
{
    // Read battery stats from the BQ27427
    unsigned int soc = lipo.soc();  // Read state-of-charge (%)
    unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
    int current = lipo.current(AVG); // Read average current (mA)
    unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
    unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
    int power = lipo.power(); // Read average power draw (mW)
    int health = lipo.soh(); // Read state-of-health (%)

    // Now print out those values:
    String toPrint = String(soc) + "% | ";
    toPrint += String(volts) + " mV | ";
    toPrint += String(current) + " mA | ";
    toPrint += String(capacity) + " / ";
    toPrint += String(fullCapacity) + " mAh | ";
    toPrint += String(power) + " mW | ";
    toPrint += String(health) + "%";

    Serial.println(toPrint);
}

void setup()
{

    Serial.begin(115200);
    // SerialBT.begin("AT-HUB-1"); // Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");

    if (pdPASS == xTaskCreatePinnedToCore(
        bluetoothTask,
        "Task Bluetooth",
        4096,
        (void*)&global_state, //(void *)buttonstates,
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
        (void*)&global_state, //(void *)buttonstates,
        1,
        &bluetoothtask_handle, 1))
    {
        Serial.println("Success Created task Input handler");
    }
    else
    {
        Serial.println("Handler task creation error");
    }


    if (!lipo.begin()) // begin() will return true if communication is successful
    {
        // If communication fails, print an error message and loop forever.
        Serial.println("Error: Unable to communicate with BQ27427.");
        Serial.println("  Check wiring and try again.");
        Serial.println("  (Battery must be plugged into Battery Babysitter!)");
        while (1);
    }
    Serial.println("Connected to BQ27427!");

}

void loop()
{
    printBatteryStats();
    delay(100);
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