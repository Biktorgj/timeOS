typedef struct {
  int current_app;
  unsigned long display_start_time;
  unsigned long display_end_time;
  unsigned long prevTick;
  int hh;
  int mm;
  int ss;
  /*
   *
   *  Power mode:
   *  0: Running
   *  1: Display off, running
   *  2: Powersave mode, accel on
   *  3: Deep sleep, accel off
   *
   */
  unsigned long power_mode;

} app_runtime;
