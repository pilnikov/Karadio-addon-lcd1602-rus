
void ssd_init()
{
  pinMode(PIN_PLAYING, OUTPUT);
  digitalWrite(PIN_PLAYING, LOW);

  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  if (x == 84)  u8g2.setFont(u8g2_font_5x8_tf);
  else u8g2.setFont(u8g2_font_6x12_t_cyrillic);  //  u8g.setFont(u8g2_font_6x13_tf);

  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();

  yy = u8g2.getDisplayHeight();
  x = u8g2.getDisplayWidth();

  y = u8g2.getAscent() - u8g2.getDescent() + 1;

  // Character array pointers

  char msg0[40] = {"(c)KaraWin+Пильников"};
  char msg1[40] = {"V1.1 Русская Версия"};
  char msg2[40] = {"Karadio"};
  char msg3[40] = {"IP: "};
  strcat(msg3, oip);

  u8g2.firstPage();
  do      u8g2.drawStr(x / 2 - (u8g2.getStrWidth(msg2) / 2), yy / 2, msg2);
  while ( u8g2.nextPage() );
  delay(500);

  strcat (lline[0], msg2);
  strcat (lline[1], msg0);
  strcat (lline[2], msg1);
  strcat (lline[3], msg3);

  draw();
  delay(1000);
}


////////////////////////////////////////

void ssd_loop(void)
{
  // scrolling control and draw control
  if (loopScroll > 0 ) // 250ms
  {
    loopScroll = 0;
    if (askDraw) // something to display
    {
      askDraw = false;
      draw();
    }
    else scroll();
  }
  // ntp control and first info demand
  if (loopDate >= 5)
  {
    loopDate = 0;
    if (itAskTime) askTime();
  }
}

////////////////////////////////////////
void cleaner(byte sel)
{
  switch (sel)
  {
    case 0:
      for (int j = 0; j < BUFLEN; j++) nameset[j] = NULL;
      break;
    case 1:
      for (int j = 0; j < BUFLEN; j++) station[j] = NULL;
      break;
    case 2:
      for (int j = 0; j < BUFLEN; j++) genre[j] = NULL;
      break;
    case 3:
      for (int j = 0; j < BUFLEN; j++) title[j] = NULL;
      break;
    case 4:
      for (int j = 0; j < BUFLEN; j++) line[j] = NULL;
      break;
  }
}

////////////////////////////////////////
void writer(byte sel)
{
  for (int j = 0; j < BUFLEN; j++) lline[sel][j] = NULL;
  switch (sel)
  {
    case 0:
      strcat (lline[0], nameset);
      DBG_OUT_PORT.printf("\n line 0 ...%s\n", lline[sel]);
      break;
    case 1:
      strcat (lline[1], station);
      DBG_OUT_PORT.printf("\n line 1 ...%s\n", lline[sel]);
      break;
    case 2:
      strcat (lline[2], genre);
      DBG_OUT_PORT.printf("\n line 2 ...%s\n", lline[sel]);
      break;
    case 3:
      strcat (lline[sel], title);
      DBG_OUT_PORT.printf("\n line 3 ...%s\n", lline[sel]);
      break;
  }
}

////////////////////////////////////////
// draw all lines
void draw()
{
  char strsec[30]; // prepare line for date & time
  if (x == 84)
    sprintf(strsec, "%02d:%02d:%02d  %02d-%02d",      hour(), minute(), second(), day(), month()        );
  else
    sprintf(strsec, "%02d:%02d:%02d  %02d-%02d-%04d", hour(), minute(), second(), day(), month(), year());

  u8g2.firstPage();
  do
  {
    // i - line num; y - vertical pos (Height of a line); x - horizontal pos (Width); yy - Height of screen;

    u8g2.drawBox(0, 0, x, y); // draw white back in first line

    for (int i = 0; i < LINES; i++)
    {
      if (i == 0)u8g2.setDrawColor(0);
      else u8g2.setDrawColor(1);

      u8g2.setCursor (pos[i], y * i);
      u8g2.print(lline[i]); // draw other line ##1-5
    }

    u8g2.drawHLine(0, yy - 4 * y + 1, x); // line
    u8g2.drawHLine(0, yy - 3 * y + 1, x); // line
    u8g2.drawFrame(1, yy - 1.5 * y, x - 1, 3);  //frame
    u8g2.drawHLine(1, yy - 1.5 * y + 1, ((uint16_t)(x * volume) / 255)); // bar
    u8g2.drawStr(x / 2 - (u8g2.getStrWidth(strsec) / 2), yy - y, strsec);    // date & time line
  }
  while (u8g2.nextPage());
}

////////////////////////////////////////
// scroll each line
void scroll()
{
  for (int i = 0; i < LINES; i++)
  {
    if (pos[i] > - 255)
    {
      pos[i]--;
      askDraw = true;
    }
    else pos[i] = 0;
  }
}

// ------------------------------------------------------------------------- timer2 interrupt 30c
void timer2()
{
  cli();//stop interrupts
  radio_snd("cli.info", true);
  sei();//allow interrupts
}

// ------------------------------------------------------------------------- timer1 interrupt 4Hz
void timer1()
{
  if (loopTime % 4 == 0) // 1hz
  {
    loopDate++;
    if (!syncTime) itAskTime = true; // first synchro if not done
    askDraw = true;
  }
  if ((++loopTime % 7200) == 0) itAskTime = true; // refresh ntp time every 30Mn
}

