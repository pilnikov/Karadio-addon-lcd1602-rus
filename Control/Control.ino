

//#include "WiFiManager.h"                // https://github.com/tzapu/WiFiManager
#include "LiquidCrystal_I2C.h"

#include "conf.h"

#include <Encoder.h>

/////////////////////////////////////////////////////////////////


const uint8_t lcd_row = 2;
const uint8_t lcd_col = 16;

LiquidCrystal_I2C lcd(0x3F, lcd_col, lcd_row); // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(lcd_adr, lcd_col, lcd_row); // set the LCD address to 0x27 for a 16 chars and 2 line display

Encoder myEnc(D5, D6);

String str = String();
String sta = fsys.lcd_rus("Название станции");
String stl = fsys.lcd_rus("Проигрывается сейчас");

int cur_sym_pos[lcd_row] = {0, 0};
unsigned long lcd_scroll_time[lcd_row] = {millis(), millis()};

void setup()
{
  DBG_OUT_PORT.begin(115200);
  /*
    // To run, set your ESP8266 build to 160MHz, update the SSID info, and upload.
    system_update_cpu_freq(SYS_CPU_160MHZ);
    WiFiManager wifiManager;
    wifiManager.autoConnect("Home");
    web_cli = true;
    DBG_OUT_PORT.println("connected...yeey :)");
  */
  enc_setup();
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0); // первая строка
  lcd.print(fsys.lcd_rus("Привет мир!"));
  lcd.setCursor(0, 1); // вторая строка
  lcd.print(fsys.lcd_rus("Hello World!"));
  delay (1000);
  //------------------------------------------------------  Определяем консоль

  Serial.setDebugOutput(true);

# ifdef DEBUG_UDP
  DBG_OUT_PORT.begin(4023, IPAddress(192, 168, 111, 132));
# endif


  //------------------------------------------------------  Инициализируем встроенную файловую систему SPIFFS

  fs_setup();

  //------------------------------------------------------  Читаем установки из EEPROM

  conf_data = loadConfig();

  //------------------------------------------------------  Запускаем I2C и проверяем наличие клиентов

  DBG_OUT_PORT.println("\n i2c scan");
  ram_data = fsys.i2c_scan(conf_data);


  //-------------------------------------------------------- Запускаем сетевые сервисы
  WiFi.disconnect();
  WiFi.mode( WIFI_OFF );

  start_wifi();

  //------------------------------------------------------ Подключаем OTA, SSDP и MDNS
  nsys.OTA_init(conf_data.ap_ssid, conf_data.ap_pass);
  MDNS.begin(conf_data.ap_ssid);
  nsys.ssdp_init();
  DBG_OUT_PORT.printf("Open http://%s", ".local/edit to see the file browser\n", conf_data.ap_ssid);

  //------------------------------------------------------ Запускаем сервер
  web_setup();
  start_serv();

  //------------------------------------------------------ Запускаем парсер
  parse();
}


void loop()
{
  irq_set();
  lcd.setCursor(0, 0); // первая строка
  lcd.print(sta);
  lcd_mov_str( stl, 1, 290);
  if (cur_sym_pos[1] == 0) parse();
  enc_loop();
}


