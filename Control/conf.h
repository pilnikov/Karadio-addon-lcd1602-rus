/********************************************************** Config
  struct conf_data_t
  {
  char      sta_ssid[33];
  char      sta_pass[33];
  char      ap_ssid[17];
  char      ap_pass[17];
  char      radio_addr[17];
  char      test[3];
  };
*/


#ifndef _wc_h
#define _wc_h

#define FW_Ver 1.0 //16.05.17 added udp debug console

// ------------------------------------------------------------- Include
//#include "WiFiManager.h"                // https://github.com/tzapu/WiFiManager
#include <LiquidCrystal_I2C.h>

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Encoder.h>
// -----------------------------------------------------------
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <IRremoteESP8266.h>
#endif

#if defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#endif

#include <ArduinoJson.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>

#include <Netwf.h>
#include <Sysf2.h>

//#define DEBUG_UDP

// ------------------------------------------------------ ConsoleUDP
#ifdef DEBUG_UDP
#include <udp_cons.h>
udp_cons print_console_udp;
#define DBG_OUT_PORT print_console_udp
#else
#define DBG_OUT_PORT Serial
#endif


// ----------------------------------- Typedef
#include <Udt.h>

conf_data_t conf_data;

// ----------------------------------- Web Pages
#include "web.h"

#if defined(ESP8266)
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
#endif

bool web_ap        = false;
bool web_cli       = false;
unsigned long serv_ms = 60000;

static const char* name_week[] = {"", "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"};

// ----------------------------------- Force define func name

void fs_setup(char*);

conf_data_t loadConfig();
void saveConfig(conf_data_t);
conf_data_t defaultConfig();

bool sta_init();
bool sta_check();
bool start_client();
void stop_client();

bool ap_init();

void start_wifi();
void stop_wifi();


void wifi_conn(byte, byte, byte);
void sta_msg(byte, uint8_t, uint8_t, bool, uint16_t);

// ---------------------------------------------------- WiFi Default
static const char  ap_ssid_def[] PROGMEM = "Radio_Pult";
static const char  ap_pass_def[] PROGMEM = "12345678";
static const char sta_ssid_def[] PROGMEM = "My_WiFi";
static const char sta_pass_def[] PROGMEM = "12345678";


// ---------------------------------------------------- Common

unsigned long   irq_end[10];
unsigned long   setting_ms  = 0;
unsigned long   telnet_time  = millis();

uint8_t         debug_level  = 0; // 0 - отключен

// ---------------------------------------------------- Constructors
IPAddress IP_Addr;

File fsUploadFile;

LiquidCrystal_I2C * lcd;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

Encoder myEnc(D5, D6);

SF fsys;
NF nsys;

//----------------------------------------------------- SSD1306

// nams <--> num of line
#define STATIONNAME 0
#define STATION1  1
#define STATION2  2
#define IP        3
#define GENRE     2
#define TITLE1    3
#define TITLE2    4
#define VOLUME    5


#define PIN_PLAYING 2


// Karadio specific data
#define BUFLEN  180
#define LINES  4
char line[BUFLEN]; // receive buffer
char station[BUFLEN]; //received station
char title[BUFLEN]; // received title
char nameset[BUFLEN]; // the local name of the station
char genre[BUFLEN]; // the genre of the station
char lline[LINES][BUFLEN] ; // array of ptr of n lines
int16_t pos[LINES] = {0} ; //array of index for scrolling
int16_t volume;
uint8_t _index = 0;
uint8_t loopScroll = 0;
uint8_t loopDate = 0;
char oip[17];

uint16_t y ;    //Height of a line
uint16_t yy;    //Height of screen
uint16_t x ;    //Width

bool syncTime = false;
bool askDraw  = false;
bool itAskTime = true;
unsigned long loopTime = 0;
//------------------------------------------------------------------------------------------- LCD 1602

const uint8_t lcd_row = 2;
const uint8_t lcd_col = 16;

String str = String();
String cli_icy0 = fsys.lcd_rus("Название станции");
String cli_meta = fsys.lcd_rus("Проигрывается сейчас");

int cur_sym_pos[lcd_row] = {0, 0};
unsigned long lcd_scroll_time[lcd_row] = {millis(), millis()};

#endif /* _wc_h */


