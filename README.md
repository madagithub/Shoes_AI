פרוייקט מוצג נעליים AI

במהלך הפעלת המוצג התעוררו כמה בעיות שגרמו לקריסת המוצג.  
רוב הבעיות נבעו מכך שהמוצג מחובר לרשת האינטרנט על גבי הדפדפן  Firefox  במערכת הפעלה Linux Mint מבוסס Ubuntu  
בנוסף מחוברים מקלדת ועכבר וזה גרם למבקרים ביוזמתם לצאת מהתוכנה ולגלוש למקומות אחרים.
בשלב ראשון כדי להתגבר על זה, הגדרתי בפרוקסי שהגישה בדפדפן מאושרת רק לכתובת 127.0.0.1 שזה האתר הפנימי שרק מושך מידע מ AI וגוגל API. 
בשלב שני עשינו עבודה של ימי הביניים, כלומר לפתוח מקלדות פיזית ולחצות בין המעגלים בפס דבק שחור כדי לנטרל לחיצה במקשים שלא אמורים לפעול.
גם הדבר הזה לא עזר, כי לא ניתן היה לנטרל את המקשים כמו ctrl  ו alt מכוון שהם משתמשים בהחלפת שפה במוצג.  לדוגמא אם אני מחזיק מקש alt  ובו זמנית לוחץ על הגלגלת בעכבר , אז נפתח תפריט ובסופו יש QUIT
כדי לפתור את הבעיה הזו , השתמשתי ב – Arduino  וכפתור לחיצה פיזי שידמה את הקומבינציה החלפת השפה alt-shift  ובכך אוכל לנטרל את הקומבינציה הזו במקלדת.

לשם כך (בעזרת אמיר) נכתב קוד ראשוני ב- Arduino שמדמה את הקומבינציה alt-shift
העבודה בוצעה על Arduino Leonardo HID  פין 2 ו – ground
בתוך הקוד הוספתי גם אפשרות לכפתור נוסף שיעזור למדריכים לאתחל את המוצג בלחיצת כפתור נוסף, כמו שהיה במוצג פיצה
וחיבור לפין 3 ו - ground ממול


#include <Keyboard.h>  // Include the Keyboard library

const int ALT_SHIFT_KEY = 2;   // Pin connected to the button for Alt+Shift
const int REBOOT_KEY = 3;      // Pin connected to the button for reboot
int altShiftState = LOW;       // Current state of the button for Alt+Shift
int altShiftPrev = LOW;        // Previous state of the button for Alt+Shift

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
    // Debounce
    delay(50);
    if (digitalRead(ALT_SHIFT_KEY) == LOW) {
      // Press and release Alt+Shift keys
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(100); // Wait for stable
      Keyboard.releaseAll();
      delay(100); // Wait for debounce
    }
  }

  // Store current state for comparison in the next iteration
  altShiftPrev = altShiftState;

  // Read the state of the reboot button
  int rebootState = digitalRead(REBOOT_KEY);

  // Check if the reboot button is pressed
  if (rebootState == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(REBOOT_KEY) == LOW) {
      // Ensure the layout is set to English before rebooting
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(100); // Wait for stable
      Keyboard.releaseAll();
      delay(100); // Wait for debounce

      // Open Terminal (Ctrl + Alt + T)
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('t');
      delay(100);
      Keyboard.releaseAll();
      delay(1000); // Wait for Terminal to open

      // Type sudo reboot without prompt
      Keyboard.print("sudo reboot");
      delay(100); // Wait for typing
      Keyboard.press(KEY_RETURN);
      delay(100); // Ensure the command is sent
      Keyboard.releaseAll();
      delay(100); // Final debounce delay
    }
  }
}


בגלל שמערכת הלינוקס יותר מאובטחת וכדי לבצע אתחול עם Arduino יש צורך לבצע שינויים ב Terminal  של הלינוקס:
sudo visudo
לאחר מכן להקיש סיסמא של המערכת. במוזיאון היא mada
לרדת לסוף העמוד ולהוסיף החרגה
Mada ALL=(ALL)  NOPASSWD: /sbin/reboot
ללחוץ ctrl+x ו enter לשמירה

המשמעות של ההחרגה הזאת, שניתנה הרשאה ל Arduino להכניס סיסמא.
לפני ה – , reboot Arduino  ינסה להעביר את ההקלדה לאנגלית ואז יבצע פקודה sudo reboot
לכן אם אולי יהיה צורך ללחוץ פעמיים על כפתור האתחול עד שהמערכת לא תזהה אנגלית ותבצע את האתחול. זה רק בתנאי עם מותקנת שפה שלישית. אם זה רק אנגלית ועברית, אמור לעבוד מלחיצה אחת.


לאחר מכן עליתי על עוד סוג של בעיה. ברגע ששפת הקלדה בעברית, אז המקשים W Q הופכים להיות לסימנים  / '   ושני אלו בהקשה בדפדפן פותחים למטה שורת חיפוש שנותן לצאת מהתוכנה.
לכן במערכת לינוקס בתפריט keyboard layout   יש אופציה לנטרל את שני הסימנים. חשוב בזמן הנטרול שמערכת תהיה בפריסת מקלדת HEB
Linux Mint's built-in Custom Keyboard Shortcuts feature doesn't directly support disabling specific keys. However, you can still create a custom keyboard shortcut that effectively disables the "/" key by assigning it a command that does nothing.
Here's how you can do it:
1.	Open Custom Shortcuts Settings:
•	Go to the main menu and search for "Keyboard" or "Keyboard Shortcuts" in the system settings.
•	Look for the option related to custom keyboard shortcuts and open it.
2.	Add a New Shortcut:
•	In the Custom Shortcuts settings, look for an option to add a new shortcut or custom action.
•	Click on "Add" or a similar button to add a new shortcut.
3.	Configure the Shortcut:
•	In the dialog box that appears, you'll typically have fields to input a name, command, and shortcut key combination.
•	For the name, you can enter something like "Disable Slash Key."
•	For the command, enter a command that does nothing. You can use echo with a harmless message. For example:
•	For the shortcut key combination, press the "/" key on your keyboard.
4.	Save the Shortcut:
•	After configuring the shortcut, save it by clicking "Add" or "OK" depending on the interface.
5.	Test the Shortcut:
•	Once the shortcut is added, test it by pressing the "/" key.
•	If everything is set up correctly, pressing "/" should not have any noticeable effect.








