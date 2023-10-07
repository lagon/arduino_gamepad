#include <TFT_HX8357.h>
#include <User_Setup.h>

TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library


#include "bitmaps.h"
#include "keyboard.h"
#include "rocks_and_missiles.h"



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Nazdar!");

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  init_key_pins();
}

void draw_bitmap(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint16_t *bitmap, const uint16_t bitmap_len) {
  tft.setWindow(x, y, x + width - 1, y + height - 1);
  uint16_t remaining_px = bitmap_len;
  uint16_t offset = 0;
  uint16_t buffer [255];  
  uint8_t to_copy = 0;

  while (remaining_px > 0) {
    to_copy = remaining_px > 254 ? 254 : remaining_px;
    memcpy_P(buffer, bitmap + offset, to_copy * 2);

    Serial.println("");
    tft.pushColors(buffer, to_copy);
    remaining_px = remaining_px - to_copy;
    offset = offset + to_copy;
  }
}

void startup_screen() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);

  tft.setTextFont(4);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ROCK SHOOTER", 240, 160, 4);

  tft.setTextFont(2);
  tft.setCursor(240, 280);
  tft.drawString("Press <FIRE> to continue", 240, 280, 2);
}



void maybe_create_new_rock(Rock *all_rocks[]) {
    if (random(1000) < 100) {
      int8_t new_rock_id = -1;
      for (int i = 0; i < 16; i++) {
        if (all_rocks[i]->is_inactive()) {
          new_rock_id = i;
          break;
        }
      }
      if (new_rock_id >= 0) {
        all_rocks[new_rock_id]->start_new_rock();
      }
    } 
}

void update_missiles(Missile *all_missiles[]) {
  for (uint8_t i = 0; i < 4; i++) {
    all_missiles[i]->update();
  }
}

uint8_t check_hits(Missile *all_missiles[], Rock *all_rocks[]) {
  uint8_t num_hits = 0;
  for (uint8_t i_m = 0; i_m < 4; i_m++) {
    uint16_t x_m = all_missiles[i_m]->getX();
    uint16_t w_m = all_missiles[i_m]->getW();

    for (uint8_t i_r = 0; i_r < 16; i_r++) {
      uint16_t x_r = all_rocks[i_m]->getX();
      uint16_t w_r = all_rocks[i_m]->getW();

      if ((max(x_m + w_m, x_r + w_r) - min(x_m, x_r)) < (w_m + w_r)) {
        if (all_missiles[i_m]->getY() + all_missiles[i_m]->getW() >= all_rocks[i_m]->getY()) {
          all_missiles[i_m]->missile_hit_target();
          all_rocks[i_m]->hit_by_missile();
          num_hits++;
        }
      }
    }
  }
  return num_hits;
}

bool update_rocks(Rock *all_rocks[]) {
  for (uint8_t i_r = 0; i_r < 16; i_r++) {
    all_rocks[i_r]->update();
    if (all_rocks[i_r]->getY() < 50) {
      return true;
    }
  }
  return false;
}




uint16_t game_play() {
  Missile *all_missiles [4];
  for (uint8_t i = 0; i < 4; i++) {
    all_missiles[i] = new Missile();
  }

  Rock *all_rocks[16];
  for (uint8_t i = 0; i < 16; i++) {
    all_rocks[i] = new Rock();
  }

  Rocket *rocket = new Rocket();

  uint16_t score = 0;

  while (rocket->get_lives() > 0) {
    maybe_create_new_rock(all_rocks);
    update_missiles(all_missiles);
    uint8_t num_rocks_destroyed = check_hits(all_missiles, all_rocks);
    score = score + num_rocks_destroyed;
    bool rock_hit_rocket = update_rocks(all_rocks);

    if (rock_hit_rocket == false) {
      uint16_t key_status = read_all_key_status();

      maybe_start_missile(all_missiles, key_status);
      move_rocket(rocket);
    } else {
      rocket->drop_life();
      clear_all_missiles(all_missiles);
      clear_all_rocks(all_rocks);

      draw_kill_screen(rocket);
      delay(2000);
    }

    if (rocket->get_lives() > 0) {
      redraw_screen(all_missiles, all_rocks, rocket);
    }
  }
  return score;
};

void finish_screen(uint16_t score) {
  
};


void loop() {
  startup_screen();
  uint16_t key_status = read_all_key_status();
  while (is_key_pressed(KEY_MIDDLE, key_status) == 0) {
    delay(1);
    key_status = read_all_key_status();
  }

  uint16_t score = game_play();

  finish_screen(score);
  while (is_key_pressed(KEY_MIDDLE, key_status) == 0) {
    delay(1);
    key_status = read_all_key_status();
  }



  // Serial.println("Pokus");
  // draw_bitmap(100, 100, space_ship1_width, space_ship1_height, space_ship1, space_ship1_byte_len);
  // draw_bitmap(150, 100, missle1_width, missle1_height, missle1, missle1_byte_len);
  // draw_bitmap(200, 100, rock1_width, rock1_height, rock1, rock1_byte_len);
  // draw_bitmap(250, 100, rock_exp1_width, rock_exp1_height, rock_exp1, rock_exp1_byte_len);
  // draw_bitmap(300, 100, rock_exp2_width, rock_exp2_height, rock_exp2, rock_exp2_byte_len);
  // draw_bitmap(350, 100, rock_exp3_width, rock_exp3_height, rock_exp3, rock_exp3_byte_len);
  // delay(10000000);
}
