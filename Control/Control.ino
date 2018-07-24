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

  //-------------------------------------------------------- Запускаем WiFi

  start_wifi();
  
  DBG_OUT_PORT.println("WiFi started");

  //if You want to bilt in led is on after stat wifi - uncomment string below 
  //if (web_cli || web_ap) digitalWrite(LED_BUILTIN, LOW);

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
      lcd_mov_str(f_dsp.lcd_rus(station), 0 , 290);
      lcd_mov_str(f_dsp.lcd_rus(title), 1, 290);
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


