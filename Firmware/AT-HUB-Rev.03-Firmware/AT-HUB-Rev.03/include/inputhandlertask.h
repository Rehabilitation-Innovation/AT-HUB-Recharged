#pragma once
#include "blutoothtask.h"
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>



void inputhandlerTask(void *pvParameters);