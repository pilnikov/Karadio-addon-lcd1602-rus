
void irq_set()
{
  //------------------------------------------------------ interrupts

  uint8_t irq = 10;
  if (millis() - irq_end[1] > 3600000L)  irq = 1;
  else if (millis() - irq_end[2] > 1800000L)  irq = 2;
  else if (millis() - irq_end[3] >   60000L)  irq = 3;
  else if (millis() - irq_end[4] >   55000L)  irq = 4;
  else if (millis() - irq_end[6] >      500)  irq = 6;
  else if (millis() - irq_end[7] >      200)  irq = 7;
  else if (millis() - irq_end[8] >      190)  irq = 8;
  else if (millis() - irq_end[9] >       40)  irq = 9;

  switch (irq)
  {
    case 1:
      firq1();
      irq_end[1] = millis();
      break;

    case 2:
      irq_end[2] = millis();
      break;

    case 3:
      irq_end[3] = millis();
      break;

    case 4:
      firq4();
      irq_end[4] = millis();
      break;

    case 6:
      firq6();
      irq_end[6] = millis();
      break;

    case 7:
      firq7();
      irq_end[7] = millis();
      break;

    case 8:
      firq8();
      irq_end[8] = millis();
      break;

    case 9:
      firq9();
      irq_end[9] = millis();
      break;
  }
}

void firq1()
{
  start_wifi();
}

void firq4()
{
}

void firq6()
{
}

void firq7()
{
  if (web_cli || web_ap)
  {
    server.handleClient();
    if (debug_level == 2) DBG_OUT_PORT.printf ("Serv sec %u\n", (millis() - serv_ms) / 1000);
    ArduinoOTA.handle();
    yield();
  }
}

void firq8()
{
}

void firq9()
{
}
