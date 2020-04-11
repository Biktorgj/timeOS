/* Vibra */
#ifndef _VIBRA_
#define _VIBRA_

#define VIBRA 16

class Vibra
{
  public:
    Vibra() {

    }
    void init() {
      pinMode(VIBRA, OUTPUT);
      delay(20);
      pinMode(VIBRA, INPUT);
    }
    void doVibrate(int milis = 50) {
      pinMode(VIBRA, OUTPUT);
      delay(milis);
      pinMode(VIBRA, INPUT);
    }
};

#endif
