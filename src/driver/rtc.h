/*
static unsigned irq_counter = 0;
boolean LD = false;

void RTC0_IRQHandler(void) {
  irq_counter++;
  if (irq_counter % 8 == 0) {
    LD = !LD;
    digitalWrite(9, false);
  }
  NRF_RTC0->EVENTS_TICK = 0UL;
}

void stopRTC() {
  NRF_RTC0->TASKS_STOP = 0UL;
}
void setRTC() {
  //stop to set options
  stopRTC();

  NVIC_SetPriority(RTC0_IRQn, 15);
  NVIC_ClearPendingIRQ(RTC0_IRQn);
  NVIC_EnableIRQ(RTC0_IRQn);

  NRF_CLOCK->LFCLKSRC = (uint32_t)((CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);
  NRF_CLOCK->TASKS_LFCLKSTART = 1UL;

  NRF_RTC0->PRESCALER = (1<<12) -1;
  NRF_RTC0->EVTENSET = offsetof(NRF_RTC_Type, EVENTS_TICK);
  NRF_RTC0->INTENSET = offsetof(NRF_RTC_Type, EVENTS_TICK);
  NRF_RTC0->TASKS_START = 1UL;
}
*/
