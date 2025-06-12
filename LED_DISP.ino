#include <Adafruit_GFX.h>  //依存ライブラリ
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/sio.h"
#include "hardware/structs/sio.h"

#define ADDA 10
#define ADDB 9
#define ADDC 8
#define ADDD 6

#define OE 7
#define DR1 5
#define DR2 3
#define DG1 4
#define DG2 2
#define STB 1
#define CLK 0

GFXcanvas1 disp_r_bg(128,32);  //赤のバックバッファ
GFXcanvas1 disp_g_bg(128,32);  //緑のバックバッファ

GFXcanvas1 disp_r(128,32);  //赤のVRAM
GFXcanvas1 disp_g(128,32);  //緑のVRAM

// 描画初期化コード
void setup1(){

}

// LED駆動部初期化コード。いじるな。
void setup(){
  gpio_init(ADDA);
  gpio_set_dir(ADDA, GPIO_OUT);

  gpio_init(ADDB);
  gpio_set_dir(ADDB, GPIO_OUT);

  gpio_init(ADDC);
  gpio_set_dir(ADDC, GPIO_OUT);

  gpio_init(ADDD);
  gpio_set_dir(ADDD, GPIO_OUT);
  
  gpio_init(OE);
  gpio_set_dir(OE, GPIO_OUT);

  gpio_init(DG1);
  gpio_set_dir(DG1, GPIO_OUT);
  
  gpio_init(DG2);
  gpio_set_dir(DG2, GPIO_OUT);
  
  gpio_init(DR1);
  gpio_set_dir(DR1, GPIO_OUT);

  gpio_init(DR2);
  gpio_set_dir(DR2, GPIO_OUT);
  
  gpio_init(STB);
  gpio_set_dir(STB, GPIO_OUT);
  
  gpio_init(CLK);
  gpio_set_dir(CLK, GPIO_OUT);

  sio_hw->gpio_clr = 1 << ADDA;
  sio_hw->gpio_clr = 1 << ADDB;
  sio_hw->gpio_clr = 1 << ADDC;
  sio_hw->gpio_clr = 1 << ADDD;
  sio_hw->gpio_clr = 1 << OE;
  sio_hw->gpio_clr = 1 << STB;
  sio_hw->gpio_clr = 1 << CLK;
  sio_hw->gpio_set = 1 << DR1;
  sio_hw->gpio_set = 1 << DR2;
  sio_hw->gpio_set = 1 << DG1;
  sio_hw->gpio_set = 1 << DG2;

}

//描画部コード。ここに描画用コードを書くこと。
//Adafruit gfx libraryは
//https://learn.adafruit.com/adafruit-gfx-graphics-library?view=all
//を参考すること。

void loop1(){
  disp_g_bg.fillScreen(0);
  disp_r_bg.fillScreen(0);
  disp_g_bg.drawChar(0, 0, "Hello world!", 1, 0, 4);
  //memcpy()でバックバッファからVRAMに転送する。赤と緑のVRAMそれぞれに転送する。
  memcpy(disp_r.getBuffer(), disp_r_bg.getBuffer(), 128 * 32 / 8);
  memcpy(disp_g.getBuffer(), disp_g_bg.getBuffer(), 128 * 32 / 8);
  delay(200);
}


//LED駆動部ルーチン。いじるな。
void loop() {
    for(int j=0;j<16;j++){
      sio_hw->gpio_clr = 1 << STB;
      for(int i=0;i<128;i++){
        sio_hw->gpio_clr = 1 << CLK;
            if(disp_g.getPixel(i,j)){
              sio_hw->gpio_clr = (1 << DG1);
            }else{
              sio_hw->gpio_set = (1 << DG1);
            }
            if(disp_g.getPixel(i,j+16)){
              sio_hw->gpio_clr = (1 << DG2);
            }else{
              sio_hw->gpio_set = (1 << DG2);
            }
            if(disp_r.getPixel(i,j)){
              sio_hw->gpio_clr = (1 << DR1);
            }else{
              sio_hw->gpio_set = (1 << DR1);
            }
            if(disp_r.getPixel(i,j+16)){
              sio_hw->gpio_clr = (1 << DR2);
            }else{
              sio_hw->gpio_set = (1 << DR2);
            }
          
              /*
              digitalWrite(DG1,disp_g.getPixel(i,j)?LOW:HIGH);
              digitalWrite(DG2,disp_g.getPixel(i,j+16)?LOW:HIGH);
              digitalWrite(DR1,disp_r.getPixel(i,j)?LOW:HIGH);
              digitalWrite(DR2,disp_r.getPixel(i,j+16)?LOW:HIGH);
              */
              /*
              sio_hw->gpio_clr = 1 << DG1;
              sio_hw->gpio_clr = 1 << DG2;
              */
        sio_hw->gpio_set = 1 << CLK;
      }
      sio_hw->gpio_set = 1 << STB;
      if(j&1){
        sio_hw->gpio_set = 1 << ADDA;
      }else{
        sio_hw->gpio_clr = 1 << ADDA;
      }
      if(j&2){
        sio_hw->gpio_set = 1 << ADDB;
      }else{
        sio_hw->gpio_clr = 1 << ADDB;
      }
      if(j&4){
        sio_hw->gpio_set = 1 << ADDC;
      }else{
        sio_hw->gpio_clr = 1 << ADDC;
      }
      if(j&8){
        sio_hw->gpio_set = 1 << ADDD;
      }else{
        sio_hw->gpio_clr = 1 << ADDD;
      }
      sio_hw->gpio_set = 1 << OE;
      sio_hw->gpio_clr = 1 << OE;
    }
}
