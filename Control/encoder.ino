//#include "ClickEncoder.h"

//ClickEncoder *encoder;
//unsigned timerEncoder = 0;


long oldPosition  = -999;
boolean isButtonPressed = false;
long lastUpdateMillis = 0;

void handleKey() {
  isButtonPressed = true;
}

void enc_setup() {
  pinMode(D3, INPUT_PULLUP);
  attachInterrupt(D3, handleKey, RISING);
}

void enc_loop() {
  long newPosition = myEnc.read();

  if (isButtonPressed && millis() - lastUpdateMillis > 50) // программное устранение дребезга:
  {
    isButtonPressed = false;
    lastUpdateMillis = millis();
    //myEnc.write(0);  // сбрасываем счетчик:
    if (newPosition != oldPosition)
    {
      if (newPosition > oldPosition) radio_snd("cli.prev", false);
      delay (200);
      if (newPosition < oldPosition) radio_snd("cli.next", false);
      delay (200);
    }
  }
  else
  {
    if (newPosition != oldPosition)
    {
      if (newPosition > oldPosition) radio_snd("cli.vol-", false);
      delay (200);
      if (newPosition < oldPosition) radio_snd("cli.vol+", false);
      delay (200);
    }
  }
  oldPosition = newPosition;
  //DBG_OUT_PORT.println(newPosition);
}

/*
  void translateENC()
  {
  enum modeStateEncoder { encVolume, encStation } ;
  static modeStateEncoder stateEncoder = encVolume;
  int16_t newValue = 0;
  ClickEncoder::Button newButton = ClickEncoder::Open;
  static int16_t oldValue = 0;
  //  static ClickEncoder::Button oldButton = ClickEncoder::Open;

       ClickEncoder::Open
       ClickEncoder::Closed
       ClickEncoder::Pressed
       ClickEncoder::Held
       ClickEncoder::Released
       ClickEncoder::Clicked
       ClickEncoder::DoubleClicked

  newValue = - encoder->getValue();
  newButton = encoder->getButton();

  if (newValue != 0)
  {
    //    Serial.print("Encoder: ");Serial.println(newValue);
    // reset our accelerator
    if ((newValue > 0) && (oldValue < 0)) oldValue = 0;
    if ((newValue < 0) && (oldValue > 0)) oldValue = 0;
  }
  else
  {
    // lower accelerator
    if (oldValue < 0) oldValue++;
    if (oldValue > 0) oldValue--;
  }

  if (newButton != ClickEncoder::Open)
  {
    if (newButton == ClickEncoder::Clicked) {
      //startStop();
    }
    if (newButton == ClickEncoder::DoubleClicked)
    {
      //(stateScreen == smain) ? Screen(stime) : Screen(smain0);
    }
    if (newButton == ClickEncoder::Held)
    {
      //      if (newValue != 0)  stationNum(atoi(futurNum) + newValue);
    }
  }
  else {
    if (newValue != 0)
    {
      //    Serial.print("Value: ");Serial.println(newValue);
      //    Serial.print("Volume: ");Serial.println(volume+newValue+(oldValue*2));
      //     setVol(volume + newValue + (oldValue * 3));
    }
    if (newValue != 0)
    {
      //    Serial.print("Value: ");Serial.println(newValue);
      //      stationNum(atoi(futurNum) + newValue);
    }
  }
  oldValue += newValue;
  }
*/
