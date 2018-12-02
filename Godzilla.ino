#include <Arduboy2.h>
Arduboy2 arduboy;
Sprites sprites;

#include "images.h"
#include "vars.h"

// MISC FUNCTIONS //
bool dirPressed() {
  if (arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON) ||
      arduboy.pressed(UP_BUTTON)   || arduboy.pressed(DOWN_BUTTON)) {
    return true;
  }
  return false;
}

void limitXAtWorldBounds(float & x, float & dx, int w) {
  float new_x       = x;
  float new_dx      = dx;
  float world_right = WORLD_WIDTH*TILE_SIZE;
  
  if (new_x <= 0) {
    new_x = 0;
    new_dx = 0;
  } else if (new_x+w >= world_right) {
    new_x = world_right-w;
    new_dx = 0;
  }

  x  = new_x;
  dx = new_dx;
}

void limitYAtWorldBounds(float & y, float & dy, int h) {
  float new_y       = y;
  float new_dy      = dy;
  float world_bottom = WORLD_HEIGHT*TILE_SIZE;
  
  if (new_y <= 0) {
    new_y = 0;
    new_dy = 0;
  } else if (new_y+h >= world_bottom) {
    new_y = world_bottom-h;
    new_dy = 0;
  }

  y  = new_y;
  dy = new_dy;
}

float minf(float a, float b) {
  return (a < b) ? a : b;
}

float maxf(float a, float b) {
  return (a > b) ? a : b;
}

float lerp(float value0, float value1, float factor) {
  return (((1 - factor) * value0) + (factor * value1));
}

// CAMERA FUNCTIONS //
void moveCamera() {
  if (arduboy.pressed(LEFT_BUTTON)) {
    cam_x -= 0.25;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    cam_x += 0.25;
  }
  if (arduboy.pressed(UP_BUTTON)) {
    cam_y -= 0.25;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    cam_y += 0.25;
  }
}

void getCameraOffset() {
  if (arduboy.pressed(LEFT_BUTTON)) {
    cam_x_off = CAM_OFFSET;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    cam_x_off = -CAM_OFFSET;
  }
  if (arduboy.pressed(UP_BUTTON)) {
    cam_y_off = CAM_OFFSET;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    cam_y_off = -CAM_OFFSET;
  }

  if (dirPressed() == false) {
    //cam_x_off = 0;
    cam_y_off = 0;
  }
}

void cameraFollow(float tx, float ty) {
  //cam_x = -tx+52.0;
  //cam_y = -ty+32.0;
  cam_x = lerp(cam_x, -tx+52 + cam_x_off, 0.1);
  cam_y = lerp(cam_y, -ty+32 + cam_y_off, 0.1);
}

void limitCamera() {
  cam_x = minf(cam_x, 0);
  cam_x = maxf(cam_x, -(WORLD_WIDTH*TILE_SIZE)+WIDTH);
  cam_y = minf(cam_y, 0);
  cam_y = maxf(cam_y, -(WORLD_HEIGHT*TILE_SIZE)+HEIGHT);
}

// WORLD MAP FUNCTIONS //
void drawWorldMap() {
  for(int y=0; y<WORLD_HEIGHT; y++) {
    for(int x=0; x<WORLD_WIDTH; x++) {
      int tile_index = pgm_read_byte(&world_map[y][x]) -1;
      if (tile_index >= 0) {
        arduboy.drawBitmap(x*TILE_SIZE + cam_x, y*TILE_SIZE + cam_y, tiles[tile_index], TILE_SIZE, TILE_SIZE, BLACK);
      }
    }
  }
}

// BUILDING FUNCTIONS //

void drawBuildings() {
  for(int i=0; i<SMALL_BUILDING_COUNT; i++) {
    int x = small_buildings_pos[i][0];
    int y = small_buildings_pos[i][1];
    //arduboy.println(small_buildings_pos[0][0]);
    //arduboy.println(small_buildings_pos[0][1]);
    arduboy.drawBitmap((x*TILE_SIZE) + cam_x, (y*TILE_SIZE) + cam_y, building_small_img, 8, 8, BLACK);
  }
}

//////////////////////////// MAIN LOOP START ///////////////////////////////

void setup() {
  arduboy.boot();
  Serial.begin(9600);
}

void loop() {
  // Update
  if(!arduboy.nextFrame())
    return;

  //moveCamera();
  getCameraOffset();
  cameraFollow(dino_x, dino_y);
  limitCamera();
  
  updateDino();

  // Draw
  arduboy.clear();
  arduboy.fillScreen();
  drawWorldMap();
  drawBuildings();
  drawDino();

  Serial.write(arduboy.getBuffer(), WIDTH * HEIGHT / 8);
  arduboy.display();
}
