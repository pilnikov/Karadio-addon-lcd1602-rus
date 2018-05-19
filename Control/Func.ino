
void lcd_mov_str(String tape, uint8_t dline, unsigned long dutty)
{
  if (millis() - lcd_scroll_time[dline] > dutty)
  {
    lcd_scroll_time[dline] = millis();
    cur_sym_pos[dline]++;
  }

  uint16_t y = tape.length();
  if (cur_sym_pos[dline] > y + lcd_col) cur_sym_pos[dline] = 0; // End of scrolling

  else
  {
    for (uint8_t p = 0; p < lcd_col; p++)
    {
      char buf[16];
      int16_t i = cur_sym_pos[dline] + p - lcd_col;

      buf[p] = ' '; //Init buffer

      if (i >= 0 && i < y) buf[p] = tape[i]; //Write char in buffer

      lcd.setCursor(p, dline);
      lcd.print(buf[p]); //Draw char in lcd
    }
  }
}



String radio_snd (String cmd, bool rcv)
{
  //  char radio_addr[17] = "192.168.1.39";

  DBG_OUT_PORT.println("\n Start communication over telnet");
  String out = "No connect with Radio";
  if (web_cli)
  {
    WiFiClient client;
    const int port = 23;

    if (!client.connect(conf_data.radio_addr, port))
    {
      client.stop();
      out = "connection failed";
    }
    else
    {
      DBG_OUT_PORT.println("\n Start snd");
      client.print(cmd + "\r\n");
      DBG_OUT_PORT.println("\n End snd");
      DBG_OUT_PORT.println("\n Start rcv");
      if (rcv)   out = client.readStringUntil('\r');
      DBG_OUT_PORT.println("\n End rcv");
      client.stop();
    }
  }
  DBG_OUT_PORT.println("\n End communication over telnet");
  return out;
}

void parse()
{
  str = fsys.lcd_rus(radio_snd("cli.info", true));
  str.trim();

  removeUtf8(str);
  parser1(str);
}

void removeUtf8(String characters)
{
  int _index = 0;
  while (characters[_index])
  {
    if ((characters[_index] >= 0xc2) && (characters[_index] <= 0xc3)) // only 0 to FF ascii char
    {
      //      Serial.println((characters[_index]));
      characters[_index + 1] = ((characters[_index] << 6) & 0xFF) | (characters[_index + 1] & 0x3F);
      int sind = _index + 1;
      while (characters[sind]) {
        characters[sind - 1] = characters[sind];
        sind++;
      }
      characters[sind - 1] = 0;

    }
    _index++;
  }
}

void fs_setup()
{
  DBG_OUT_PORT.print("\n");

  if (!SPIFFS.begin())     DBG_OUT_PORT.println("Failed to mount file system");
  else
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), fsys.formatBytes(fileSize).c_str());
    }
    DBG_OUT_PORT.printf("\n");
  }
}

void wifi_conn( byte par, byte sta, byte disp)
{
  if (par < 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  if ( par == 1) lcd.print(fsys.lcd_rus( "Подключаемся к" ));

  if ( par == 2) lcd.print(fsys.lcd_rus( "Создаем" ));

  if (par < 3) lcd.setCursor(0, 1);

  if ( par == 1) lcd.print( conf_data.sta_ssid );

  if ( par == 2) lcd.print(fsys.lcd_rus( "точку доступа" ));

  if ( par == 3)
  {
    lcd.setCursor(15, 1);
    lcd.print(sta);
  }

  if (par == 4 || par == 5)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print( "IP: ");
    lcd.print(IP_Addr[3]);
    sta_msg(disp, 0, 1, true, 2000);
  }

  if (par == 6)
  {
    lcd.clear();
    sta_msg(disp, 0, 1, false, 2000);
  }
}

void sta_msg(byte disp, uint8_t _row, uint8_t _colum, bool sta, uint16_t _delay)
{
  lcd.setCursor(_row, _colum);
  if (sta) lcd.print(fsys.lcd_rus("  Успешно!  "));
  else     lcd.print(fsys.lcd_rus("  Косяк!   "));

  delay(_delay);
  lcd.clear();
}
