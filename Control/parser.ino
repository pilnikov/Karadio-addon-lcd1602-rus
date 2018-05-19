void parser1(String inStr)
{
  /*
    ##SYS.DATE#: 2018-03-30T19:07:55+00:00
    ##CLI.NAMESET#: 9 Шансон
    ##CLI.ICY0#: 101.ru: Shanson2
    ##CLI.ICY3#: http:\/\/www.101.ru\/
    ##CLI.META#: BILLIY'S BAND - Простим Друг Друга - 0:00
    ##CLI.VOL#: 129
    ##CLI.PLAYING#
  */

  DBG_OUT_PORT.println("\n Start parsing");

  char inChr; //Обрабатываемый символ
  byte    i = 0;
  String nStr = ""; // Имя переменной
  String vStr = ""; // значение
  bool tag_v;

  for ( uint16_t n = 0; n < inStr.length(); n++)
  {
    inChr = inStr[n];
    if (inStr[n] == '\n')
    {
      if (vStr > "")
      {
        //       DBG_OUT_PORT.print("name...") ; DBG_OUT_PORT.println(nStr);
        //       DBG_OUT_PORT.print("value..."); DBG_OUT_PORT.println(vStr);

        byte vInd = 0;
        if      (nStr == "##SYS.DATE#:")    vInd =  1;
        else if (nStr == "##CLI.NAMESET#:") vInd =  2;
        else if (nStr == "##CLI.ICY0#:")    vInd =  3;
        else if (nStr == "##CLI.ICY3#:")    vInd =  4;
        else if (nStr == "##CLI.META#:")    vInd =  5;
        else if (nStr == "##CLI.VOL#:")     vInd =  6;
        else if (nStr == "##CLI.PLAYING#")  vInd =  7;

        //      DBG_OUT_PORT.print("vInd..."); DBG_OUT_PORT.println(vInd);
        //     if (vInd > 0) DBG_OUT_PORT.print(' ');

        vStr.trim();

        switch (vInd)
        {
          case 1:
            char lstr[25]; //объявляем буфер в 30 символов под эту самую дату и время
            tmElements_t dt;
            vStr.toCharArray(lstr, 25) ; //копируем дату и время в буфер
            breakTime(now(), dt); //Записываем в структуру dt (содержащую элементы час минута секунда год) текущее время в контроллере (в дурине)
            int year, month, day, hour, minute, second; //объявляем переменные под год месяц день недели и.т.д
            sscanf(lstr, "%04d-%02d-%02dT%02d%02d%02d", &(year), &(month), &(day), &(hour), &(minute), &(second)); //переносим (разбираем) строчку с датой на отдельные кусочки (день месяц год и.т.д)
            dt.Year = year - 1970; dt.Month = month; dt.Day = day; //заменяем кусочки структуры dt значениями из нашей принятой и разобранной строки с датой и временем
            dt.Hour = hour; dt.Minute = minute; dt.Second = second;
            setTime(makeTime(dt)); //записываем в timestamp(штамп/оттиск времени в формате UNIX time (количество секунд с 1970 года) значение времени сформированное в структуре dt
            break;
          case 2:
            break;
          case 3:
            sta = vStr;
            break;
          case 4:
            break;
          case 5:
            stl = vStr;
            break;
          case 6:
            //          prog[i].tod = vStr.toInt();
            //          DBG_OUT_PORT.print(prog[i].tod);
            break;
          case 7:
            //          prog[i].tod = vStr.toInt();
            //          DBG_OUT_PORT.print(prog[i].tod);
            break;
        }
      }

      nStr = ""; // clear the string "low name" for new input:
      vStr = ""; // clear the string "value"    for new input:
      tag_v = false;
    }
    else
    {
      if (!tag_v) nStr += (char)inChr; //Формируем Имя переменной
      if (inStr[n] == ':') tag_v = true;
      else
      {
        if (tag_v) vStr += (char)inChr; //Формируем значение
      }
    }
  }
  DBG_OUT_PORT.println("\n End parsing");
}

