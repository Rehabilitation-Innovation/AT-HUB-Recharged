#pragma once
#include <BluetoothSerial.h>
#define BLUETOOTH_HANDLER_CORE 1
#define BLUETOOTH_TASK_PRIORITY 0
#define INPUT_TASK_PRIORITY 0
#define INPUT_HANDLER_CORE 0

#define BTN1 38
#define BTN2 34
#define BTN3 35
#define BTN4 26
#define BTN5 25

#define BACKUP_BTN1 32
#define BACKUP_BTN2 33
#define BACKUP_BTN3 27
#define BACKUP_BTN4 14
#define BACKUP_BTN5 12

#define LED1 13
#define LED2 15
#define LED3 2
#define LED4 4
#define LED5 0

#define RELAY1 19
#define RELAY2 20
#define RELAY3 7
#define RELAY4 8
#define RELAY5 5

// button state machine.
typedef enum
{
    BUTTON_UNPRESSED = 0,
    BUTTON_PRESSED = 1,
    BUTTON_DOUBLE_PRESSED = 2,
    BUTTON_HELD = 3,
    BUTTON_RELEASD = 4 // probably wont use but here just in case.
} Button_State_t;

// stores global state and can keep presistance for multiple button presses at the same time.
typedef struct
{
    Button_State_t input_1;
    Button_State_t input_2;
    Button_State_t input_3;
    Button_State_t input_4;
    Button_State_t input_5;

    int output1;
    int output2;
    int output3;
    int output4;
    int output5;

} AT_HUB_Input_Ouput_States_t;

typedef enum
{
    AT_HUB_INPUT_1 = BTN1,
    AT_HUB_INPUT_2 = BTN2,
    AT_HUB_INPUT_3 = BTN3,
    AT_HUB_INPUT_4 = BTN4,
    AT_HUB_INPUT_5 = BTN5,

} AT_HUB_Inputs_t;

typedef enum
{
    AT_HUB_OUTPUT_1 = RELAY1,
    AT_HUB_OUTPUT_2 = RELAY2,
    AT_HUB_OUTPUT_3 = RELAY3,
    AT_HUB_OUTPUT_4 = RELAY4,
    AT_HUB_OUTPUT_5 = RELAY5,

} AT_HUB_Outputs_t;

// Input output multiplexer
// This struct defines the variables for what output each input controls.
typedef struct
{
    AT_HUB_Outputs_t in1_controls = AT_HUB_OUTPUT_1; // Ex. assign a output type to this for in 1 to control.
    AT_HUB_Outputs_t in2_controls = AT_HUB_OUTPUT_2;
    AT_HUB_Outputs_t in3_controls = AT_HUB_OUTPUT_3;
    AT_HUB_Outputs_t in4_controls = AT_HUB_OUTPUT_4;
    AT_HUB_Outputs_t in5_controls = AT_HUB_OUTPUT_5;
} AT_HUB_Input_Output_Map_t;

struct OutputStates
{
    int outputrelay_pin;
    int outputled_pin;
    int state_from_bt;
    int backupbutton;
};

typedef struct
{
    BluetoothSerial SerialBT; // Global serial (Safe Access I think LMAO!)
    // AT_HUB_Input_Ouput_States_t *global_input_output_states;
    // AT_HUB_Input_Output_Map_t *global_input_output_mapping;

    OutputStates output1;
    OutputStates output2;
    OutputStates output3;
    OutputStates output4;
    OutputStates output5;
    bool btScanAsync = true;
    bool btScanSync = true;
} AT_HUB_State_t;

#define LOG(x) Serial.println("[" __FILE__ "]" x)