/********************************************************** Config

struct conf_data_t
 {
  char      sta_ssid[33];
  char      sta_pass[33];
  char      ap_ssid[17];
  char      ap_pass[17];
  char      radio_addr[17];
  char      test[3];
 } conf_data;
*/


#ifndef _wc_h
#define _wc_h

#define FW_Ver 1.0 //16.05.17 added udp debug console


// ------------------------------------------------------------- Include

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <IRremoteESP8266.h>

#include <ArduinoOTA.h>
//#include <Ticker.h>
#include <Udt.h>
#include <hw.h>
#include <Netwf.h>
#include <FS.h>
#endif


#include <ArduinoJson.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SPI.h>


#include <Sysf.h>
#include <Exts.h>

//#define DEBUG_UDP

// ------------------------------------------------------ ConsoleUDP
#ifdef DEBUG_UDP
#include <udp_cons.h>
udp_cons print_console_udp;
#define DBG_OUT_PORT print_console_udp
#else
#define DBG_OUT_PORT Serial
#endif


// ----------------------------------- Include
#include "web.h"

// ----------------------------------- Force define func name

void fs_setup(char*);

bool saveConfig(conf_data_t);
bool initConfig();

conf_data_t loadConfig();
conf_data_t read_conf_data();
conf_data_t init_conf_data();
void check_and_save_conf_data(conf_data_t);

bool sta_init();
bool sta_check();
bool start_client();
void stop_client();

bool ap_init();

void start_wifi();
void stop_wifi();


void wifi_conn(byte, byte, byte);
void sta_msg(byte, uint8_t, uint8_t, bool, uint16_t);

// ----------------------------------- Time
time_t cur_time;

// ----------------------------------- Web server
#if defined(ESP8266)
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
// ---------------------------------------------------- HW info
ESP8266HWInfo hwi;
#endif

bool web_ap        = false;
bool web_cli       = false;
unsigned long serv_ms = 60000;

static const char* name_week[] = {"", "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"};

// ---------------------------------------------------- WiFi Default
static const char  ap_ssid_def[] PROGMEM = "WiFi_Clock";
static const char  ap_pass_def[] PROGMEM = "";
static const char sta_ssid_def[] PROGMEM = "My_WiFi";
static const char sta_pass_def[] PROGMEM = "";


// ---------------------------------------------------- Common
const char ntp_server[] = "ru.pool.ntp.org";

bool                disp_on  = true;
unsigned long   irq_end[10];
unsigned long    setting_ms  = 0;

uint8_t           disp_mode  = 1;

uint8_t         debug_level  = 0; // 0 - отключен

// ---------------------------------------------------- Constructors
IPAddress IP_Addr;

File fsUploadFile;

conf_data_t conf_data;
ram_data_t ram_data;

SF fsys;
NF nsys;

#endif /* _wc_h */


