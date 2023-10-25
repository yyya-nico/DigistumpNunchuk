// Arduino board manager
//​http://digistump.com/package_digistump_index.json

// Usage:
// メニュー [ツール]->[ボード]内の[Board Manager]を選択
// [Board Manager] 内の”Digistump AVR Boards”を選択し[Install]をクリック
// インストール後メニュー [ツール]->[ボード]内に”Digispark (Default – 16.5mhz)”が追加されるので選択


#include <DigiKeyboard.h>
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

void setup() {
  initNunchuk();
}

void loop() {
  int     x      = 0;
  int     y      = 0;
  uint8_t button = 0;
  DigiKeyboard.sendKeyStroke(0);
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
    DigiKeyboard.print("x: ");
    DigiKeyboard.print(x - initXposi);
    DigiKeyboard.print(", y: ");
    DigiKeyboard.print(y - initYposi);
    DigiKeyboard.print(", xmax: ");
    DigiKeyboard.print(maxXposi);
    DigiKeyboard.print(", ymax: ");
    DigiKeyboard.print(maxYposi);
    int delay = y - initYposi;
    if(delay < 0) delay *= -1;
    delay = maxYposi - delay;
    DigiKeyboard.print(", delay: ");
    DigiKeyboard.println(delay);
    DigiKeyboard.delay(2000);
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

