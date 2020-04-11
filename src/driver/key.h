/* Keypad: It only really handles setup of the gpio,
 *
 *  You can also read it status
 *
 */
#ifndef _KEY_
#define _KEY_

class Key
{
  public:
    Key() {
      pinMode(SIDE_BTN_OUT, OUTPUT);
      digitalWrite(SIDE_BTN_OUT, HIGH);
      pinMode(SIDE_BTN_IN, INPUT);
    }
    
    bool read() {
      return digitalRead(SIDE_BTN_IN);
    }
};

#endif
