# Karadio-addon-lcd1602 + SSD1306-rus
Это выносной "пульт" для Karadio. База - ESP8266. Обмен с хостом по Telnet. Экраны - LCD1602 с кирилицей + SSD1306 тоже по русски. 

Среда - Arduino IDE 1.8.5.  

Файлы из папочки "Data" загружать плагином ESP8266 Sketch Data Upload Plugin

Недостающие библиотеки смотреть у авторов и у меня в метеостанции.

Подключение: I2C - GPIO4(SDA) GPIO5(SCL) Кнопки на аналоговый вход, ИК - GPIO14 Кнопка энкодера GPIO0 сам энкодер GPIO12 GPIO13 (кнопки и энкодер пока не дописаны)

Подключение к WiFi через коннект менеджер (подробности тут https://github.com/tzapu/WiFiManager/tree/development)
