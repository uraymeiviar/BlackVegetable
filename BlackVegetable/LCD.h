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
#define DARKGREY  RGB( 32,  32,  32)
#define TURQUOISE RGB(  0, 128, 128)
#define PINK      RGB(255, 128, 192)
#define OLIVE     RGB(128, 128,   0)
#define PURPLE    RGB(128,   0, 128)
#define ORANGE    RGB(255, 128,  64)
#define LIGHTORANGE    RGB(255, 192,  128)
#define DARKORANGE    RGB(128, 64,  32)
#define LIGHTBLUE RGB( 80,  80, 255)
#define LIGHTGREEN RGB(128, 255,128)
#define LIGHTRED  RGB(255,128,128)
#define DARKGREEN RGB( 0,  64, 0)
#define DARKRED   RGB(72,  0,  0)
#define BLACKGREEN RGB( 0,  24, 0)
#define LIGHTCYAN  RGB( 128,255,255)
#define DARKCYAN  RGB(  0, 80, 80)
#define LIGHTYELLOW    RGB(255,255,  128)
#define DARKYELLOW    RGB( 64, 64,  0)
#define LIGHTBLUE     RGB(128,128,255)
#define DARKBLUE     RGB(  32,  32, 80)
#define LIGHTMAGENTA   RGB(255,  128,255)
#define DARKMAGENTA   RGB(80,  32,80)

MCUFRIEND_kbv lcd;

void initLCD(void)
{
  Serial.println("initializing LCD...");
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
  Serial.println("LCD initialization done.");
}

void drawCircle(int x0, int y0, int radius, uint16_t color, int thickness = 1)
{
    lcd.startWrite();
    for(int t=0 ; t < thickness ; t++)
    {
      int x = radius-1-t;
      int y = 0;
      int dx = 1;
      int dy = 1;
      int err = dx - (radius << 1);
  
      while (x >= y)
      {
          lcd.writePixel(x0 + x, y0 + y, color);
          lcd.writePixel(x0 + y, y0 + x, color);
          lcd.writePixel(x0 - y, y0 + x, color);
          lcd.writePixel(x0 - x, y0 + y, color);
          lcd.writePixel(x0 - x, y0 - y, color);
          lcd.writePixel(x0 - y, y0 - x, color);
          lcd.writePixel(x0 + y, y0 - x, color);
          lcd.writePixel(x0 + x, y0 - y, color);
  
          if (err <= 0)
          {
              y++;
              err += dy;
              dy += 2;
          }
          
          if (err > 0)
          {
              x--;
              dx += 2;
              err += dx - (radius << 1);
          }
      }
    }
    lcd.endWrite();
}

void drawQuarterArc1(int x0, int y0, int radius, uint16_t color, int thickness = 1)
{
    for(int t=0 ; t < thickness ; t++)
    {
      int x = radius-1-t;
      int y = 0;
      int dx = 1;
      int dy = 1;
      int err = dx - (radius << 1);
  
      while (x >= y)
      {
          lcd.writePixel(x0 - x, y0 - y, color);
          lcd.writePixel(x0 - y, y0 - x, color);
  
          if (err <= 0)
          {
              y++;
              err += dy;
              dy += 2;
          }
          
          if (err > 0)
          {
              x--;
              dx += 2;
              err += dx - (radius << 1);
          }
      }
    }
}

#define sensorGridGlobalOffsetY   12
#define sensorGridGlobalOffsetX    0
#define sensorGridThickness        6
#define sensorGridValueTextHeight 14
#define sensorGridLabelHeight      8
#define sensorGridLabelWidth      64
#define sensorGridWidth           80
#define sensorGridHeight          sensorGridWidth
#define sensorGridVertSpace        4

int getSensorGridPosX(int gridX)
{
  return sensorGridGlobalOffsetX+gridX*sensorGridWidth;
}

int getSensorGridPosY(int gridY)
{
  return sensorGridGlobalOffsetY+gridY*sensorGridHeight + (sensorGridVertSpace*gridY);
}


