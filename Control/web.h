void web_setup();

void start_serv();
void  stop_serv();

void handlejWiFi();
void handleSetWiFi();

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

