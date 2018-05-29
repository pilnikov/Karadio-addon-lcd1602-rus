#include "conf.h"

void setup()
{
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

  Serial.setDebugOutput(true);

# ifdef DEBUG_UDP
  DBG_OUT_PORT.begin(4023, IPAddress(192, 168, 1, 44));
# endif


  //------------------------------------------------------  Инициализируем встроенную файловую систему SPIFFS

  fs_setup();

  //------------------------------------------------------  Читаем установки из EEPROM

  conf_data = loadConfig();

  //------------------------------------------------------  Запускаем I2C и проверяем наличие клиентов

  Wire.begin();
  DBG_OUT_PORT.printf("\n i2c scan \n");
  fsys.i2c_parse();

  //------------------------------------------------------  Инициализируем выбранный дисплей
  switch (conf_data.type_disp)
  {
    case 0:
      lcd_init();
      break;
    case 1:
      ssd_init();
      break;
    case 2:
      nokia_init();
      break;
  }
  //-------------------------------------------------------  Запускаем установку энкодера
  //enc_setup();

  //-------------------------------------------------------- Запускаем сетевые сервисы
  WiFi.disconnect();
  WiFi.mode( WIFI_OFF );
  /*
     // To run, set your ESP8266 build to 160MHz, update the SSID info, and upload.
     system_update_cpu_freq(SYS_CPU_160MHZ);
     WiFiManager wifiManager;
     wifiManager.autoConnect("Home");
     web_cli = true;
     DBG_OUT_PORT.println("connected...yeey :)");
  */

  start_wifi();


  //------------------------------------------------------ Подключаем OTA, SSDP и MDNS
  nsys.OTA_init(conf_data.ap_ssid, conf_data.ap_pass);
  MDNS.begin(conf_data.ap_ssid);
  nsys.ssdp_init();
  DBG_OUT_PORT.printf("Open http://%s", conf_data.ap_ssid);
  DBG_OUT_PORT.print(".local/edit to see the file browser\n");
  //------------------------------------------------------ Запускаем сервер
  web_setup();
  start_serv();

  //------------------------------------------------------ Запускаем парсер
  telnet_time = millis() + 31000;
  radio_snd("cli.info", true);
}


void loop()
{
  irq_set();

  switch (conf_data.type_disp)
  {
    case 0:
      lcd_mov_str(fsys.lcd_rus(station), 0 , 290);
      lcd_mov_str(fsys.lcd_rus(title), 1, 290);
      break;

    case 1:
      ssd_loop();
      break;

    case 2:
      nokia_loop();
      break;
  }


  //  enc_loop();
}


