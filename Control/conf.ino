conf_data_t loadConfig()
{
  conf_data_t data;

  File configFile = SPIFFS.open("/config.json", "r");

  if (!configFile)
  {
    DBG_OUT_PORT.println("Config file not found, load default setting");
    data = defaultConfig();
  }
  else
  {
    size_t size = configFile.size();

    if (size > 500 || size < 5)
    {
      DBG_OUT_PORT.println("Config file size is wrong, load default setting");
      data = defaultConfig();
    }
    else
    {
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      // We don't use String here because ArduinoJson library requires the input
      // buffer to be mutable. If you don't use ArduinoJson, you may as well
      // use configFile.readString instead.
      configFile.readBytes(buf.get(), size);

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());

      if (!json.success()) DBG_OUT_PORT.println("Failed to parse config file");
      else
      {
        strncpy(data.sta_ssid,   json["sta_ssid"],   33);
        strncpy(data.sta_pass,   json["sta_pass"],   33);
        strncpy(data.ap_ssid,    json["ap_ssid"],    17);
        strncpy(data.ap_pass,    json["ap_pass"],    17);
        strncpy(data.radio_addr, json["radio_addr"], 17);

        data.type_disp        = json["type_disp"];
        configFile.close();

        strncpy(oip, data.radio_addr, 17);
      }
    }
  }
  return data;
}

void saveConfig(conf_data_t data)
{
  if (debug_level == 3) DBG_OUT_PORT.println( "Start saving conf_data to config.json");

  if ( data.ap_ssid[0] == ' ' || data.ap_ssid[0] == 0) strncpy( data.ap_ssid, ap_ssid_def, sizeof(ap_ssid_def));

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["sta_ssid"]            = data.sta_ssid;
  json["sta_pass"]            = data.sta_pass;
  json["ap_ssid"]             = data.ap_ssid;
  json["ap_pass"]             = data.ap_pass;
  json["radio_addr"]          = data.radio_addr;
  json["type_disp"]           = data.type_disp;

  File configFile = SPIFFS.open("/config.json", "w");

  if (!configFile)
  {
    DBG_OUT_PORT.println("Failed to open config file for writing");
    return;
  }
  else
  {
    size_t size = configFile.size();

    if (size > 500 || size < 5)
    {
      DBG_OUT_PORT.println("Config file size is wrong true create new one");
      configFile.close();
      SPIFFS.remove("/config.json");
      configFile = SPIFFS.open("/config.json", "w");
    }
  }

  json.printTo(configFile);
  DBG_OUT_PORT.println( "End write buffer to file");
  configFile.close();
  delay(1000);
  ESP.restart();
  //  return true;
}

conf_data_t defaultConfig()
{
  conf_data_t data;
  if (debug_level == 3) DBG_OUT_PORT.println( "Start inital conf_data with config.json");

  strncpy(data.sta_ssid,   "MyWiFi",       33);
  strncpy(data.sta_pass,   "MyPass",       33);

  strncpy(data.ap_ssid,    "Radio_Pult",   17);
  strncpy(data.ap_pass,    "12345678",     17);
  strncpy(data.radio_addr, "192.168.1.33", 17);
  data.type_disp        = 0;
  return data;
}

