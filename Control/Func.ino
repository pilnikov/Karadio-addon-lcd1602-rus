
void radio_snd (String cmd, bool rcv)
{

  if (millis() - telnet_time > 30000)
  {
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
        client.print(cmd + "\r\n");
        if (rcv)
        {
          DBG_OUT_PORT.println("\n Start rcv");
          char tmp = client.read();
          unsigned long st_time = millis();
          while (tmp != '\r' && millis() - st_time < 2000 )
          {
            tmp = client.read();
            switch (tmp)
            {
              case '\n' :
                if (_index == 0) break;

              case '\r' :
                line[_index] = 0; // end of string
                _index = 0;
                parse_k(line);
                break;
              case 0xFF :
                break;

              default : // put the received char in line

                if (tmp < 0xFF ) line[_index++] = tmp;
                if (_index > BUFLEN - 1) // next line
                {
                  DBG_OUT_PORT.println(F("overflow"));
                  line[_index] = 0;
                  parse_k(line);
                  _index = 0;
                }
            }
            delay(2);
          }
        }
        DBG_OUT_PORT.println("\n End rcv");
        client.stop();
        DBG_OUT_PORT.println("\n End communication over telnet");
      }
    }
    telnet_time = millis();
  }
}

////////////////////////////////////////
// parse the karadio received line and do the job
void parse_k(char* line)
{
  DBG_OUT_PORT.println("\n Start parsing");
  char* ici;
  removeUtf8((byte*)line);

  DBG_OUT_PORT.printf("\n Inline %s\n", line);

  ////// Meta title
  if ((ici = strstr(line, "META#: ")) != NULL)
  {
    cleaner(3);
    strcpy(title, ici + 7);
    writer(3);
    DBG_OUT_PORT.printf("\n Title ...%s\n", title);
    askDraw = true;
  }
  else
    ////// ICY4 Description
    if ((ici = strstr(line, "ICY4#: ")) != NULL)
    {
      cleaner(2);
      strcpy(genre, ici + 7);
      writer(2);
      DBG_OUT_PORT.printf("\n Genree ...%s\n", genre);
      askDraw = true;
    }
    else
      ////// ICY0 station name
      if ((ici = strstr(line, "ICY0#: ")) != NULL)
      {
        cleaner(1);
        if (strlen(ici + 7) == 0) strcpy (station, nameset);
        else strcpy(station, ici + 7);
        writer(1);
        DBG_OUT_PORT.printf("\n Station name ...%s\n", station);
        askDraw = true;
      }
      else
        ////// STOPPED
        if ((ici = strstr(line, "STOPPED")) != NULL)
        {
          cleaner(3);
          digitalWrite(PIN_PLAYING, LOW);
          strcpy(title, "STOPPED");
          writer(3);
          askDraw = true;
        }
        else
          //////Nameset
          if ((ici = strstr(line, "NAMESET#: ")) != NULL)
          {
            cleaner(0);
            strcpy(nameset, ici + 9);
            writer(0);
            DBG_OUT_PORT.printf("\n Nameset ...%s\n", nameset);
          }
          else
            //////Playing
            if ((ici = strstr(line, "PLAYING#")) != NULL)
            {
              digitalWrite(PIN_PLAYING, HIGH);
              if (strcmp(title, "STOPPED") == 0)
              {
                askDraw = true;
              }
            }
            else
              //////Volume
              if ((ici = strstr(line, "VOL#:")) != NULL)
              {
                volume = atoi(ici + 6);
                askDraw = true;;
                DBG_OUT_PORT.printf("\n Volume ...%03d\n", volume);
              }
              else
                //////Date Time  ##SYS.DATE#: 2017-04-12T21:07:59+01:00
                if ((ici = strstr(line, "SYS.DATE#:")) != NULL)
                {
                  if (*(ici + 11) != '2') //// invalid date. try again later
                  {
                    askDraw = true;
                    return;
                  }
                  char lstr[30];
                  strcpy(lstr, ici + 11);

                  tmElements_t dt;
                  breakTime(now(), dt); //Записываем в структуру dt (содержащую элементы час минута секунда год) текущее время в контроллере (в дурине)
                  int year, month, day, hour, minute, second; //объявляем переменные под год месяц день недели и.т.д
                  sscanf(lstr, "%04d-%02d-%02dT%02d:%02d:%02d", &(year), &(month), &(day), &(hour), &(minute), &(second)); //переносим (разбираем) строчку с датой на отдельные кусочки (день месяц год и.т.д)
                  dt.Year = year - 1970; dt.Month = month; dt.Day = day; //заменяем кусочки структуры dt значениями из нашей принятой и разобранной строки с датой и временем
                  dt.Hour = hour; dt.Minute = minute; dt.Second = second;
                  setTime(makeTime(dt)); //записываем в timestamp(штамп/оттиск времени в формате UNIX time (количество секунд с 1970 года) значение времени сформированное в структуре dt
                  syncTime = true;
                  DBG_OUT_PORT.printf("\n Current time is %02d:%02d:%02d  %02d-%02d-%04d\n", hour, minute, second, day, month, year);
                }
  cleaner(4);
  DBG_OUT_PORT.println("\n End parsing");
}

void fs_setup()
{
  bool isFSMounted = SPIFFS.begin();
  if (!isFSMounted)
  {
    DBG_OUT_PORT.println("\n Wait for FS Formatted ...");
    SPIFFS.format();
  }

  if (!SPIFFS.begin()) DBG_OUT_PORT.println("Failed to mount file system");
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


////////////////////////////////////////
void removeUtf8(byte *characters)
{
  int index = 0;
  while (characters[index])
  {
    if ((characters[index] >= 0xc2) && (characters[index] <= 0xc3)) // only 0 to FF ascii char
    {
      //      DBG_OUT_PORT.println((characters[index]));
      characters[index + 1] = ((characters[index] << 6) & 0xFF) | (characters[index + 1] & 0x3F);
      int sind = index + 1;
      while (characters[sind]) {
        characters[sind - 1] = characters[sind];
        sind++;
      }
      characters[sind - 1] = 0;

    }
    index++;
  }
}

////////////////////////////////////////
void askTime()
{
  if (itAskTime) // time to ntp. Don't do that in interrupt.
  {
    radio_snd("sys.date", true);
    itAskTime = false;
  }
}

void wifi_conn( byte par, byte sta, byte disp)
{
  switch (disp)
  {
    case 0:
      if (par < 3)
      {
        lcd -> clear();
        lcd -> setCursor(0, 0);
      }
      if ( par == 1) lcd -> print(fsys.lcd_rus( "Подключаемся к" ));

      if ( par == 2) lcd -> print(fsys.lcd_rus( "Создаем" ));

      if (par < 3) lcd -> setCursor(0, 1);

      if ( par == 1) lcd -> print( conf_data.sta_ssid );

      if ( par == 2) lcd -> print(fsys.lcd_rus( "точку доступа" ));

      if ( par == 3)
      {
        lcd -> setCursor(15, 1);
        lcd -> print(sta);
      }

      if (par == 4 || par == 5)
      {
        lcd -> clear();
        lcd -> setCursor(0, 0);
        lcd -> print( "IP: ");
        lcd -> print(IP_Addr[3]);
        sta_msg(disp, 0, 1, true, 2000);
      }

      if (par == 6)
      {
        lcd -> clear();
        sta_msg(disp, 0, 1, false, 2000);
      }
      break;
    case 1:
      break;
    case 2:
      break;
    default:
      break;
  }
}

void sta_msg(byte disp, uint8_t _row, uint8_t _colum, bool sta, uint16_t _delay)
{
  switch (disp)
  {
    case 0:
      lcd -> setCursor(_row, _colum);
      if (sta) lcd -> print(fsys.lcd_rus("  Успешно!  "));
      else     lcd -> print(fsys.lcd_rus("  Косяк!   "));

      delay(_delay);
      lcd -> clear();
      break;
    case 1:
      break;
    case 2:
      break;
    default:
      break;
  }
}
