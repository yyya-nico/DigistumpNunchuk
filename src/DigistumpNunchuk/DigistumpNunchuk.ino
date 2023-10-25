// Arduino board manager
//​http://digistump.com/package_digistump_index.json

// Usage:
// メニュー [ツール]->[ボード]内の[Board Manager]を選択
// [Board Manager] 内の”Digistump AVR Boards”を選択し[Install]をクリック
// インストール後メニュー [ツール]->[ボード]内に”Digispark (Default – 16.5mhz)”が追加されるので選択


#include <DigiMouse.h>
#include <Wire.h>

#define NUNCHK_ADDR (0x52)
#define COMPLETE_COUNT (5)
#define NUNCHK_Z_MASK 0x01
#define NUNCHK_C_MASK 0x02
#define POSITION_MARGIN 5

static int initXposi = 0;
static int initYposi = 0;
static int maxXposi = 0;
static int maxYposi = 0;
static int intervalCount = 0;
static bool disableCount = false;
static bool disableScroll = false;

void setup() {
  DigiMouse.begin();
  initNunchuk();
}

void loop() {
  int     x      = 0;
  int     y      = 0;
  uint8_t button = 0;
  if( nunchuckIsAvailable(&x, &y, &button) ){
    if(x - initXposi > maxXposi) {
      maxXposi = x - initXposi;
    }
    else if(x - initXposi < -maxXposi) {
      maxXposi = -(x - initXposi);
    }
    if(y - initYposi > maxYposi) {
      maxYposi = y - initYposi;
    }
    else if(y - initYposi < -maxYposi) {
      maxYposi = -(y - initYposi);
    }

    if(button & NUNCHK_C_MASK) { // c button
      if(intervalCount >= 1000/10) {
        DigiMouse.rightClick();
        intervalCount = 0;
        disableCount = true;
        disableScroll = true;
        DigiMouse.delay(10);
      }
      else {
        if(x - initXposi > POSITION_MARGIN || x - initXposi < -POSITION_MARGIN
        || y - initYposi > POSITION_MARGIN || y - initYposi < -POSITION_MARGIN) {
          intervalCount = 0;
          disableCount = true;
          if(y - initYposi > POSITION_MARGIN && !disableScroll) {
            DigiMouse.scrollV(1);
          }
          else if(y - initYposi < -POSITION_MARGIN && !disableScroll) {
            DigiMouse.scrollV(-1);
          }
          int delay1 = y - initYposi;
          if(delay1 < 0) delay1 *= -1;
          delay1 = (maxYposi - delay1)*100/maxYposi;
          if(x - initXposi > POSITION_MARGIN && !disableScroll) {
            DigiMouse.scrollH(1);
          }
          else if(x - initXposi < -POSITION_MARGIN && !disableScroll) {
            DigiMouse.scrollH(-1);
          }
          int delay2 = x - initXposi;
          if(delay2 < 0) delay2 *= -1;
          delay2 = (maxXposi - delay2)*100/maxXposi;
          DigiMouse.delay(delay1 < delay2 ? delay1 : delay2);
        }
        else if(!disableCount) {
          intervalCount++;
        }
      }
    }
    else {
      DigiMouse.moveX((signed char)(x - initXposi)*50/127); // stick position x
      DigiMouse.moveY(-(signed char)(y - initYposi)*50/127); // stick position y
      intervalCount = 0;
      disableCount = false;
      disableScroll = false;
    }

    if(button & NUNCHK_Z_MASK) { // z button 
      DigiMouse.leftClick();
    }
    else {
      DigiMouse.setButtons(0); //unclick all
    }
    DigiMouse.delay(10);
  }
  else{
    DigiMouse.moveX(0);
    DigiMouse.delay(10);
    DigiMouse.moveY(0);
    DigiMouse.delay(10);
    DigiMouse.setButtons(0); //unclick all
    DigiMouse.delay(10);
  }
  
}

void initNunchuk(void) {
  uint8_t     dummy = 0;

  Wire.begin();
  Wire.beginTransmission(NUNCHK_ADDR);
  Wire.write((uint8_t)0x40);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();  
  getInitPosition(&initXposi, &initYposi, &dummy);
  delay(5);
  getInitPosition(&initXposi, &initYposi, &dummy);
}

char decodeNunchukData (char x)
{
    x = (x ^ 0x17) + 0x17;
    return x;
}

boolean nunchuckIsAvailable(int *x, int *y, uint8_t *button)
{
  static uint8_t nunchuck_buf[6];
  int cnt = 0;

  Wire.requestFrom (NUNCHK_ADDR, 6);
  while (Wire.available ()) {
      nunchuck_buf[cnt] = decodeNunchukData( Wire.read() );
      cnt++;
  }
  
  Wire.beginTransmission(NUNCHK_ADDR);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();
  
  if(cnt >= COMPLETE_COUNT){
    *button = 0;
    if(!(nunchuck_buf[5] & 0x01)) *button  = NUNCHK_Z_MASK;    // z_button 
    if(!(nunchuck_buf[5] & 0x02)) *button |= NUNCHK_C_MASK;   // c_button  
    *x = nunchuck_buf[0];
    *y = nunchuck_buf[1];
    return true;
  }
  else{
    return false;
  }
}

void getInitPosition(int *x, int *y, uint8_t *button)
{
    nunchuckIsAvailable(x, y, button);
}

