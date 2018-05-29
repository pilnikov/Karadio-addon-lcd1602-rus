bool sta_init()
{
  if (debug_level == 15) DBG_OUT_PORT.println( "Client initialized");

  bool result = WiFi.getMode() == WIFI_STA;;
  if (!result)
  {
    wifi_conn(1, 0, conf_data.type_disp); //сообщение на индикатор
    WiFi.disconnect();
    WiFi.mode( WIFI_STA );
    while ( WiFi.getMode() != WIFI_STA );
    WiFi.begin( conf_data.sta_ssid, (conf_data.sta_pass[0] == 0) ? NULL : conf_data.sta_pass);
    result = WiFi.getMode() == WIFI_STA;
  }
  return result;
}

bool sta_check()
//-------------------------------------------------------------- Установка соединения с точкой доступа WIFi
{
  if (debug_level == 15)  DBG_OUT_PORT.println( "Connect established");

  bool result = WiFi.status() == WL_CONNECTED;
  if (!result)
  {
    uint8_t j = 0;

    while (j < 6 && !result)
    {
      result = WiFi.status() == 3;
      wifi_conn(3, WiFi.status(), conf_data.type_disp); //сообщение на индикатор
      if (!result)
      {
        delay(2000);
        j++;
      }
    }

    if (result)
    {
      IP_Addr = WiFi.localIP();
      wifi_conn(4, 0, conf_data.type_disp); //выдать радостную новость на индикатор
      WiFi.setAutoConnect(true);
      WiFi.setAutoReconnect(true);
      serv_ms = millis();
    }
    else wifi_conn(6, 0, conf_data.type_disp); //выдать нерадостную новость на индикатор
  }
  return result;
}

//-------------------------------------------------------------- ap_init
bool ap_init()
{
  if (debug_level == 15) DBG_OUT_PORT.println( "AP mode started");

  bool result = WiFi.getMode() == WIFI_AP;
  if (!result)
  {
    wifi_conn(2, 0, conf_data.type_disp); //сообщение на индикатор

    WiFi.disconnect();
    WiFi.mode( WIFI_AP );
    while ( WiFi.getMode() != WIFI_AP );

    WiFi.softAP(conf_data.ap_ssid, (conf_data.ap_pass[0] == 0) ? NULL : conf_data.ap_pass);

    if  (WiFi.getMode() == WIFI_AP)
    {
      IP_Addr = WiFi.softAPIP();
      wifi_conn(5, 0, conf_data.type_disp); //выдать радостную новость на индикатор
      serv_ms = millis();
    }
    else wifi_conn(6, 0, conf_data.type_disp); //выдать нерадостную новость на индикатор

    result = WiFi.getMode() == WIFI_AP;
    if (result && web_cli)  web_cli = false;
  }
  return result;
}

//------------------------------------------------------------ Start_client
bool start_client()
{
  if (debug_level == 15) DBG_OUT_PORT.printf( "\r\nTrue connection as client\n" );

  bool result = sta_init() && sta_check();
  if (result && web_ap)  web_ap = false;

  return result;
}

//-------------------------------------------------------------- Stop_wifi
void stop_wifi()
{
  if (web_cli)     //Останавливаем клиента
  {
    if (debug_level == 15) DBG_OUT_PORT.println( "Client stopped");
  }
  if (web_ap)     //Останавливаем АР
  {
    if (debug_level == 15) DBG_OUT_PORT.println( "AP stopped");
  }
  web_ap   = false;
  web_cli  = false;
# if defined(ESP8266) || defined(ESP32) 
  WiFi.mode(WIFI_OFF);
# endif
  WiFi.disconnect();
}

//-------------------------------------------------------------- check_wifi
void start_wifi()
{
  if (debug_level == 15)  DBG_OUT_PORT.println( "Start wifi");

  if (!web_ap && !web_cli) web_cli = start_client();  // Запускаем клиента и пытаемся подключиться к АР
  if (!web_ap && !web_cli) web_ap  = ap_init();       // Не удалось подключиться клиентом - создаем АР
}

