#include "bitmaps.h"

class FlyingObject {
  protected:
    int16_t x;
    int16_t y;

    uint8_t w, h;
    int16_t direction_of_travel;

    uint32_t last_update;
    uint32_t update_interval;
  public:

    bool is_collision(FlyingObject *other);
    virtual const uint16_t *get_bitmap() = 0;
    virtual void update() = 0;
    int16_t getX();
    int16_t getY();
    uint8_t getW();
    uint8_t getH();
};

inline bool __point_within_range(int16_t point, int16_t low_limit, int16_t high_limit) {
  return (low_limit <= point) & (point <= high_limit);
}

bool FlyingObject::is_collision(FlyingObject *other) {
  if (this->w > other->w) {
    if (!(__point_within_range(other->x, this->x, this->x + this->w) || __point_within_range(other->x + other->w, this->x, this->x + this->w))) {
      return false;
    }
  } else {
    if (!(__point_within_range(this->x, other->x, other->x + other->w) || __point_within_range(this->x + this->w, other->x, other->x + other->w))) {
      return false;
    }
  }

  if (this->h > other->h) {
    if (!(__point_within_range(other->y, this->y, this->y + this->h) || __point_within_range(other->y + other->h, this->y, this->y + this->h))) {
      return false;
    }
  } else {
    if (!(__point_within_range(this->y, other->y, other->y + other->h) || __point_within_range(this->y + this->h, other->y, other->y + other->h))) {
      return false;
    }
  }
  return true;
}

int16_t FlyingObject::getX() {
  return this->x;
}
int16_t FlyingObject::getY() {
  return this->y;
}

uint8_t FlyingObject::getW() {
  return this->w;
}
uint8_t FlyingObject::getH() {
  return this->h;
}


class Missile: public FlyingObject {
  private:
    uint8_t state;
  public:
    Missile();

    const uint16_t * get_bitmap();
    void update();
    bool is_active();
    void shoot_missile();
    void missile_hit_target();

};

void Missile::update() {
  if (this->state == 0) { // Missile not active
    return;
  } else if (this->state == 1) { //Missle flying
    
  }


}

void Missile::missile_hit_target() {
  this->y = 500;
  this->state = 0;
}

const uint16_t *Missile::get_bitmap() {
  return missile1;
}

class Rock: public FlyingObject {
  private:
    uint8_t state;

  public:
    const uint16_t * get_bitmap();
    void update();

    bool is_inactive();
    void start_new_rock();
    void hit_by_missile();
};

bool Rock::is_inactive() {
  return this->state == 0;
}

void Rock::start_new_rock() {
  this->y = 290;
  this->x = random(10, 410);

  this->state = 1;
}

void Rock::hit_by_missile() {
  this->state = 2;
}

const uint16_t *Rock::get_bitmap() {
  return rock1;
}

void Rock::update() {
  if (this->state == 1) {
    this->y -= 10;
  }
  return;
}
