#include "inputhandlertask.h"

#include "main.h"
#define DEBOUNCE_TIME 50
#define DOUBLE_TAP_TIME 300
#define HOLD_TIME 301
struct ButtonStates {
  int button_current_value = 0;
  int button_last_value = HIGH;
  unsigned long button_debounce_start = 0;
  unsigned long button_single_tap_start_time = 0;
  unsigned long button_last_press_time = 0;
  bool button_pressed = false;
  bool button_double_tap_detected = false;
  bool button_held = false;

  int button_pin;
  int led_pin;
  int button_number;
  int button_map_relay;
  bool isBackupButton = false;
};

ButtonStates button1;
ButtonStates button2;
ButtonStates button3;
ButtonStates button4;
ButtonStates button5;

ButtonStates backupbutton1;
ButtonStates backupbutton2;
ButtonStates backupbutton3;
ButtonStates backupbutton4;
ButtonStates backupbutton5;

int bb1, bb2, bb3, bb4, bb5;

void checkButton(ButtonStates &button);
void backUpButton(int backUp_button_Number, int button_number, int relay_Num,
                  int led_Num, int bb_number);
void checkOutput(OutputStates &output, ButtonStates &input,
                 ButtonStates &backupbutton);

BluetoothSerial *serialbt = 0;

AT_HUB_State_t *global = 0;

void inputhandlerTask(void *pvParameters) {
  LOG("Starting");

  global = (AT_HUB_State_t *)pvParameters;
  serialbt = &global->SerialBT;

  button1.button_pin = BTN1;
  button1.led_pin = LED1;
  button1.button_number = 1;
  button1.button_map_relay = RELAY1;
  button2.button_pin = BTN2;
  button2.led_pin = LED2;
  button2.button_number = 2;
  button2.button_map_relay = RELAY2;
  button3.button_pin = BTN3;
  button3.led_pin = LED3;
  button3.button_number = 3;
  button3.button_map_relay = RELAY3;
  button4.button_pin = BTN4;
  button4.led_pin = LED4;
  button4.button_number = 4;
  button4.button_map_relay = RELAY4;
  button5.button_pin = BTN5;
  button5.led_pin = LED5;
  button5.button_number = 5;
  button5.button_map_relay = RELAY5;

  backupbutton1.button_pin = BACKUP_BTN1;
  backupbutton1.led_pin = LED1;
  backupbutton1.button_number = 1;
  backupbutton1.isBackupButton = true;
  backupbutton1.button_map_relay = RELAY1;
  backupbutton2.button_pin = BACKUP_BTN2;
  backupbutton2.led_pin = LED2;
  backupbutton2.button_number = 2;
  backupbutton2.isBackupButton = true;
  backupbutton2.button_map_relay = RELAY2;
  backupbutton3.button_pin = BACKUP_BTN3;
  backupbutton3.led_pin = LED3;
  backupbutton3.button_number = 3;
  backupbutton3.isBackupButton = true;
  backupbutton3.button_map_relay = RELAY3;
  backupbutton4.button_pin = BACKUP_BTN4;
  backupbutton4.led_pin = LED4;
  backupbutton4.button_number = 4;
  backupbutton4.isBackupButton = true;
  backupbutton4.button_map_relay = RELAY4;
  backupbutton5.button_pin = BACKUP_BTN5;
  backupbutton5.led_pin = LED5;
  backupbutton5.button_number = 5;
  backupbutton5.isBackupButton = true;
  backupbutton5.button_map_relay = RELAY5;

  global->output1.backupbutton = BACKUP_BTN1;
  global->output1.outputrelay_pin = RELAY1;
  global->output1.state_from_bt = LOW;
  global->output1.outputled_pin = LED1;

  global->output2.backupbutton = BACKUP_BTN2;
  global->output2.outputrelay_pin = RELAY2;
  global->output2.state_from_bt = LOW;
  global->output2.outputled_pin = LED2;

  global->output3.backupbutton = BACKUP_BTN3;
  global->output3.outputrelay_pin = RELAY3;
  global->output3.state_from_bt = LOW;
  global->output3.outputled_pin = LED3;

  global->output4.backupbutton = BACKUP_BTN4;
  global->output4.outputrelay_pin = RELAY4;
  global->output4.state_from_bt = LOW;
  global->output4.outputled_pin = LED4;

  global->output5.backupbutton = BACKUP_BTN5;
  global->output5.outputrelay_pin = RELAY5;
  global->output5.state_from_bt = LOW;
  global->output5.outputled_pin = LED5;

  // b4 we output we should set the level so the matrix dosnt activate by
  // acident

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

  for (;;) {
    // LOG("Hello From InputHandler:");
    // Serial.println(xPortGetCoreID());
    checkButton(button1);
    checkButton(button2);
    checkButton(button3);
    checkButton(button4);
    checkButton(button5);

    checkButton(backupbutton1);
    checkButton(backupbutton2);
    checkButton(backupbutton3);
    checkButton(backupbutton4);
    checkButton(backupbutton5);

    // backUpButton(BACKUP_BTN1, 1, RELAY1, LED1, bb1);
    // backUpButton(BACKUP_BTN2, 2, RELAY2, LED2, bb2);
    // backUpButton(BACKUP_BTN3, 3, RELAY3, LED3, bb3);
    // backUpButton(BACKUP_BTN4, 4, RELAY4, LED4, bb4);
    // backUpButton(BACKUP_BTN5, 5, RELAY5, LED5, bb5);

    checkOutput(global->output1, button1, backupbutton1);
    checkOutput(global->output2, button2, backupbutton2);
    checkOutput(global->output3, button3, backupbutton3);
    checkOutput(global->output4, button4, backupbutton4);
    checkOutput(global->output5, button5, backupbutton5);

    vTaskDelay(1);
  }
}

void checkOutput(OutputStates &output, ButtonStates &input,
                 ButtonStates &backupbutton) {
  if (!output.state_from_bt && !backupbutton.button_pressed &&
      !backupbutton.button_held) {
    digitalWrite(output.outputrelay_pin, LOW);
    digitalWrite(output.outputled_pin, LOW);
  }

  if (output.state_from_bt || backupbutton.button_pressed ||
      backupbutton.button_held) {
    digitalWrite(output.outputrelay_pin, HIGH);
    digitalWrite(output.outputled_pin, HIGH);
  }

  // digitalWrite(input.button_map_relay, !digitalRead(input.button_pin));
}

void checkButton(ButtonStates &button) {
  button.button_current_value = digitalRead(button.button_pin);
  if (button.button_current_value == LOW && button.button_last_value == HIGH &&
      (millis() - button.button_debounce_start) > DEBOUNCE_TIME) {
    button.button_debounce_start = millis();
    button.button_last_press_time = millis();
    button.button_pressed = true;
    if ((millis() - button.button_single_tap_start_time) < DOUBLE_TAP_TIME) {
      button.button_double_tap_detected = true;

    } else {
      button.button_double_tap_detected = false;
    }
    button.button_single_tap_start_time = millis();
  }

  if (button.button_current_value == HIGH && button.button_last_value == LOW &&
      (millis() - button.button_debounce_start) > DEBOUNCE_TIME) {
    button.button_debounce_start = millis();
    if (button.button_double_tap_detected) {
      Serial.println("Button " + String(button.button_number) +
                     " double tapped");
      String double_tap_message = "DTB" +
                                  String(button.isBackupButton ? "B" : "") +
                                  String(button.button_number);
      if (0 != serialbt) {
        serialbt->print(double_tap_message.c_str());
      }
      button.button_double_tap_detected = false;
      button.button_single_tap_start_time = 0;
    } else if ((millis() - button.button_last_press_time) < HOLD_TIME) {
      Serial.println("Button " + String(button.button_number) + " pressed");
      String tap_message = "TB" + String(button.isBackupButton ? "B" : "") +
                           String(button.button_number);

      if (0 != serialbt) {
        serialbt->print(tap_message.c_str());  // Convert String to C-string
      }
    } else {
      Serial.println("Button " + String(button.button_number) + " letGO");
      String hold_button_message = "LGB" +
                                   String(button.isBackupButton ? "B" : "") +
                                   String(button.button_number);

      if (0 != serialbt) {
        serialbt->print(
            hold_button_message.c_str());  // Convert String to C-string
      }

      button.button_held = false;
    }
    button.button_pressed = false;
  }

  if (button.button_pressed &&
      (millis() - button.button_last_press_time) >= HOLD_TIME) {
    Serial.println("Button " + String(button.button_number) + " held");
    String hold_button_message = "HB" +
                                 String(button.isBackupButton ? "B" : "") +
                                 String(button.button_number);
    if (0 != serialbt) {
      serialbt->print(
          hold_button_message.c_str());  // Convert String to C-string
    }
    button.button_pressed = false;
    button.button_held = true;
    button.button_single_tap_start_time = 0;
    button.button_last_press_time = 0;
  }

  button.button_last_value = button.button_current_value;

  if (button.isBackupButton) {
    if (LOW == digitalRead(button.button_pin)) {
      digitalWrite(button.button_map_relay, HIGH);
    }
  }
}

void backUpButton(int backUp_button_Number, int button_number, int relay_Num,
                  int led_Num, int bb_number) {
  bb_number = digitalRead(backUp_button_Number);
  if (bb_number == LOW) {
    Serial.println(String(button_number));
    digitalWrite(relay_Num, HIGH);
    digitalWrite(led_Num, HIGH);
  }
}