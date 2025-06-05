#include "blutoothtask.h"

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "main.h"

#define BT_DISCOVER_TIME 10000

static bool btScanAsync = true;
static bool btScanSync = true;

AT_HUB_State_t* globalstate = NULL;

char Data[11];

void onDataRecieve(const uint8_t* buffer, size_t size);
void onConnect(boolean success);

void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
    Serial.printf("Found a device asynchronously: %s\n",
        pDevice->toString().c_str());
}

void event_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t* param) {
    if (ESP_SPP_CLOSE_EVT == event) {
        if (NULL != globalstate) {
            Serial.println("Bluetooth client disconnected.");
            globalstate->output1.state_from_bt = LOW;
            globalstate->output2.state_from_bt = LOW;
            globalstate->output3.state_from_bt = LOW;
            globalstate->output4.state_from_bt = LOW;
            globalstate->output5.state_from_bt = LOW;
        }
    }
}

void bluetoothTask(void* pvParameters) {
    globalstate = (AT_HUB_State_t*)pvParameters;

    LOG("Starting Blutoothhandler:");

    globalstate->SerialBT.begin("AT-HUB-3");

    if (globalstate->btScanAsync) {
        Serial.print("Starting asynchronous discovery... ");
        if (globalstate->SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
            Serial.println(
                "Findings will be reported in \"btAdvertisedDeviceFound\"");
            vTaskDelay(1000);
            Serial.print("Stopping discoverAsync... ");
            globalstate->SerialBT.discoverAsyncStop();
            Serial.println("stopped");
        }
        else {
            Serial.println(
                "Error on discoverAsync f.e. not working after a \"connect\"");
        }
    }

    if (globalstate->btScanSync) {
        Serial.println("Starting synchronous discovery... ");
        BTScanResults* pResults = globalstate->SerialBT.discover(BT_DISCOVER_TIME);
        if (pResults) {
            pResults->dump(&Serial);
        }
        else {
            Serial.println("Error on BT Scan, no result!");
        }
    }

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    vTaskDelay(100);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    vTaskDelay(100);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    vTaskDelay(100);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);

    if (NULL != globalstate) {
        globalstate->SerialBT.onData(onDataRecieve);
        globalstate->SerialBT.onAuthComplete(onConnect);
        globalstate->SerialBT.register_callback(event_callback);
    }
    for (;;) {
        // LOG("Hello From Blutooth");
        // Serial.println(xPortGetCoreID());

        // if (globalstate->SerialBT.hasClient())
        // {
        //     globalstate->output1.state_from_bt = LOW;
        //     globalstate->output2.state_from_bt = LOW;
        //     globalstate->output3.state_from_bt = LOW;
        //     globalstate->output4.state_from_bt = LOW;
        //     globalstate->output5.state_from_bt = LOW;
        // }
        if (NULL != globalstate) {
            globalstate->SerialBT.flush();
        }
        vTaskDelay(10);
    }
}

void onDataRecieve(const uint8_t* buffer, size_t size) {
    LOG("Got Message:");
    String m((char*)buffer, size);
    Serial.println(m);
    m.trim();
    if (m.startsWith("R"))  // control character is R for relay control
    {
        String R = m.substring(1, size);
        Serial.println(R);
        String relayID(R.charAt(0));
        String relayState(R.charAt(1));
        Serial.println("Turning Relay: " + relayID + " " +
            String(relayState.toInt() ? "On" : "Off"));
        switch (relayID.toInt()) {
        case 1:
            globalstate->output1.state_from_bt = relayState.toInt();
            break;
        case 2:
            globalstate->output2.state_from_bt = relayState.toInt();
            break;
        case 3:
            globalstate->output3.state_from_bt = relayState.toInt();
            break;
        case 4:
            globalstate->output4.state_from_bt = relayState.toInt();
            break;
        case 5:
            globalstate->output5.state_from_bt = relayState.toInt();
            break;
        }
    }
}

void onConnect(boolean success) { LOG("Device Connected: " + success); }