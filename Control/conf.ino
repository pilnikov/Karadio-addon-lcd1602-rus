conf_data_t loadConfig()
{
  conf_data_t data;

  File configFile = SPIFFS.open("/config.json", "r");

  if (!configFile)
  {
    DBG_OUT_PORT.println("Failed to open config file, true create new");
    initConfig();
  }
  else
  {
    size_t size = configFile.size();

    if (size > 500) DBG_OUT_PORT.println("Config file size is too large");
    else
    {
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      // We don't use String here because ArduinoJson library requires the input
      // buffer to be mutable. If you don't use ArduinoJson, you may as well
      // use configFile.readString instead.
      configFile.readBytes(buf.get(), size);

      StaticJsonBuffer<900> jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());

      if (!json.success()) DBG_OUT_PORT.println("Failed to parse config file");
      else
      {
        strncpy(data.sta_ssid,  " ", 33);
        strncpy(data.sta_pass,  " ", 33);
        strncpy(data.ap_ssid,   " ", 17);
        strncpy(data.ap_pass ,  " ", 17);
        strncpy(data.radio_addr," ", 17);

        strncpy(data.sta_ssid,  json["sta_ssid"],  33);
        strncpy(data.sta_pass,  json["sta_pass"],  33);
        strncpy(data.ap_ssid,   json["ap_ssid"],   17);
        strncpy(data.ap_pass,   json["ap_pass"],   17);
        strncpy(data.radio_addr,json["radio_addr"], 17);
      }
    }
  }
  return data;
}

bool saveConfig(conf_data_t data)
{
  if (debug_level == 3) DBG_OUT_PORT.println( "Start saving conf_data to config.json");

  if ( data.ap_ssid[0] == ' ' || data.ap_ssid[0] == 0) strncpy( data.ap_ssid, ap_ssid_def, sizeof(ap_ssid_def));

  StaticJsonBuffer<900> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["sta_ssid"]            = data.sta_ssid;
  json["sta_pass"]            = data.sta_pass;
  json["ap_ssid"]             = data.ap_ssid;
  json["ap_pass"]             = data.ap_pass;
  json["radio_addr"]          = data.radio_addr;
  
  File configFile = SPIFFS.open("/config.json", "w");

  if (!configFile) {
    DBG_OUT_PORT.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

bool initConfig()
{
  if (debug_level == 3) DBG_OUT_PORT.println( "Start inital conf_data with config.json");


  StaticJsonBuffer<900> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["sta_ssid"]            = sta_ssid_def;
  json["sta_pass"]            = sta_pass_def;
  json["ap_ssid"]             = ap_ssid_def;
  json["ap_pass"]             = ap_pass_def;
  json["radio_addr"]          = "192.168.4.200";

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    DBG_OUT_PORT.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

