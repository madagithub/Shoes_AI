#include <Keyboard.h>  // Include the Keyboard library

const int ALT_SHIFT_KEY = 2;   // Pin connected to the button for Alt+Shift
const int REBOOT_KEY = 3;      // Pin connected to the button for reboot
int altShiftState = LOW;       // Current state of the button for Alt+Shift
int altShiftPrev = LOW;        // Previous state of the button for Alt+Shift
bool altShiftPressed = false;  // Flag indicating if the button for Alt+Shift is pressed
int language = 0;              // Variable to track the current language

void setup() {
  // Setup button pins
  pinMode(ALT_SHIFT_KEY, INPUT_PULLUP);
  pinMode(REBOOT_KEY, INPUT_PULLUP);
  // Initialize the keyboard
  Keyboard.begin();
}

void loop() {
  // Read the state of the Alt+Shift button
  altShiftState = digitalRead(ALT_SHIFT_KEY);

  // Check if the Alt+Shift button is pressed
  if (altShiftState == LOW && altShiftPrev == HIGH) {
    // Wait for bouncing time and debounce
    delay(50);
    do {
      altShiftState = digitalRead(ALT_SHIFT_KEY);
    } while (altShiftState == LOW);

    // Press and release Alt+Shift keys
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(100); // Wait for stable
    Keyboard.releaseAll();
    delay(100); // Wait for debounce

    // Toggle language variable
    language = 1 - language;
  }

  // Read the state of the reboot button
  int rebootState = digitalRead(REBOOT_KEY);

  // Check if the reboot button is pressed
  if (rebootState == LOW) {
    delay(50); // Wait for bouncing time
    // Read the button again
    if (digitalRead(REBOOT_KEY) == LOW) {
      // Open Terminal (Ctrl + Alt + T)
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('t');
      delay(100);
      Keyboard.releaseAll();
      delay(1000); // Wait for Terminal to open

      // Type sudo reboot
      Keyboard.print("sudo reboot");
      delay(100); // Wait for typing
      Keyboard.press(KEY_RETURN);
      delay(100); // Wait for stable
      Keyboard.releaseAll();
      delay(100); // Wait for debounce
    }
  }

  // Store current state for comparison in the next iteration
  altShiftPrev = altShiftState;
}
