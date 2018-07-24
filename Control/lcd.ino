void lcd_init()
{
  lcd = new LiquidCrystal_I2C(fsys.lcd_addr, lcd_col, lcd_row);

  lcd -> init();                      // initialize the lcd
  // Print a message to the lcd ->
  lcd -> backlight();

  lcd -> clear();
  lcd -> setCursor(0, 0); // первая строка
  lcd -> print(f_dsp.lcd_rus("Привет мир!"));
  lcd -> setCursor(0, 1); // вторая строка
  lcd -> print(f_dsp.lcd_rus("Hello World!"));
  delay (1000);
}

void lcd_mov_str(String tape, uint8_t dline, unsigned long dutty)
{
  if (millis() - lcd_scroll_time[dline] > dutty)
  {
    lcd_scroll_time[dline] = millis();
    cur_sym_pos[dline]++;
  }

  uint16_t y = tape.length();
  if (cur_sym_pos[dline] > y + lcd_col) cur_sym_pos[dline] = 0; // End of scrolling

  else
  {
    for (uint8_t p = 0; p < lcd_col; p++)
    {
      char buf[16];
      int16_t i = cur_sym_pos[dline] + p - lcd_col;

      buf[p] = ' '; //Init buffer

      if (i >= 0 && i < y) buf[p] = tape[i]; //Write char in buffer

      lcd -> setCursor(p, dline);
      lcd -> print(buf[p]); //Draw char in lcd
    }
  }
}

