/* Vibra */
#define VIBRA 16
void setupVibrator() {
  pinMode(VIBRA, OUTPUT);
  delay(20);
  pinMode(VIBRA, INPUT);

}

void doVibrate(int milis = 50) {
  pinMode(VIBRA, OUTPUT);
  delay(milis);
  pinMode(VIBRA, INPUT);
}
