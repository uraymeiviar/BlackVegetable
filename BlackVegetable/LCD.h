#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#define RGB(r, g, b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))

#define BLACK     RGB(  0,  0,  0)
#define BLUE      RGB(  0,  0,255)
#define RED       RGB(255,  0,  0)
#define GREEN     RGB(  0,255,  0)
#define CYAN      RGB(  0,255,255)
#define MAGENTA   RGB(255,  0,255)
#define YELLOW    RGB(255,255,  0)
#define WHITE     RGB(255,255,255)

#define LIGHTGREY RGB(192, 192, 192)
#define GREY      RGB(127, 127, 127)
#define DARKGREY  RGB( 64,  64,  64)
#define TURQUOISE RGB(  0, 128, 128)
#define PINK      RGB(255, 128, 192)
#define OLIVE     RGB(128, 128,   0)
#define PURPLE    RGB(128,   0, 128)
#define ORANGE    RGB(255, 128,  64)
#define LIGHTBLUE RGB( 64,  64, 255)
#define LIGHTGREEN RGB(128, 255,128)
#define LIGHTRED  RGB(255,128,128)
#define DARKGREEN RGB( 0,  64, 0)
#define DARKRED   RGB(72,  0,  0)

MCUFRIEND_kbv lcd;

void initLCD(void)
{
  const uint16_t screenWidth = 320;
  const uint16_t screenHeight = 480;

  lcd.reset();
  uint16_t LCD_ID = lcd.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(LCD_ID, HEX);
  //    if (LCD_ID == 0xD3D3) LCD_ID = 0x9481; // write-only shield
  if (LCD_ID == 0xD3D3) LCD_ID = 0x9486; // write-only shield

  Serial.print("TFT Dimension = ");
  //screenWidth = tft.width();
  //screenHeight = tft.height();
  Serial.print(screenWidth);
  Serial.print(" x ");
  Serial.println(screenHeight);
  
  lcd.begin(LCD_ID);
  //tft.setRotation(1);
  lcd.fillScreen(BLACK);
  lcd.setCursor(1, 1);
  lcd.setTextColor(GREY);
  lcd.setTextSize(1);
  lcd.print(DeviceName);
  lcd.drawLine(0,11,360,11,GREY);
}
