#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

typedef struct {
  int pin;
  int midi_val;
  String name;
  
} key;

key keyboard[13];
int keyOn = 13;
int prevKeyStatus = 0;
int midi_val = 24;

void setup() {
  keyboard[0] = (key) {36, 24, "C"};
  keyboard[1] = (key) {38, 25, "cs"};
  keyboard[2] = (key) {42, 26, "d"};
  keyboard[3] = (key) {26, 27, "ds"};
  keyboard[4] = (key) {28, 28, "e"};
  keyboard[5] = (key) {48, 29, "f"};
  keyboard[6] = (key) {34, 30, "fs"};
  keyboard[7] = (key) {44, 31, "g"};
  keyboard[8] = (key) {24, 32, "gs"};
  keyboard[9] = (key) {46, 33, "a"};
  keyboard[10] = (key) {32, 34, "as"};
  keyboard[11] = (key) {40, 35, "b"};
  keyboard[12] = (key) {30, 36, "c"};
  keyboard[13] = (key) {52, 37, "keyOn"};

  Serial.begin(31250);
  MIDI.begin(1);
  // Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(keyboard[keyOn].pin, INPUT_PULLUP);
  for (int i = 0; i <= 13; i++){
    pinMode(keyboard[i].pin, INPUT_PULLUP);      
  }
  prevKeyStatus = digitalRead(keyboard[keyOn].pin);
  for (int i = 0; i <= 13; i++){
    digitalRead(keyboard[i].pin);      
  }
}

void loop() {
  bool keyChanged = false;
  int keyStatus = digitalRead(keyboard[keyOn].pin);
  digitalWrite(LED_BUILTIN, !keyStatus);
  keyChanged = (keyStatus != prevKeyStatus);
  if (keyChanged) {
    prevKeyStatus = keyStatus;
  }
  if (keyChanged) {
    if (keyStatus == 0) {
      for (int i = 0; i < 13; i++){
        digitalRead(keyboard[i].pin);
        digitalRead(keyboard[i].pin);
        // Serial.print(digitalRead(keyboard[i].pin));
        if (digitalRead(keyboard[i].pin) == 0) {
          midi_val = keyboard[i].midi_val;
          break;
        }
      }
      MIDI.sendNoteOn(midi_val + 12, 127, 1);
      // Serial.print(" ");
      // Serial.println(midi_val);
    } else {
      MIDI.sendNoteOff(midi_val + 12, 0, 1);
      // Serial.println("NOTE OFF");      
    }
    
  }
}