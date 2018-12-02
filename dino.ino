
/////////////////////////////////////////////////////

//// DINOSAUR FUNCTIONS //
void dinoMove() {
  if (arduboy.pressed(LEFT_BUTTON)) {
    dino_dx -= dino_spd;
    dino_dir = 2;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    dino_dx += dino_spd;
    dino_dir = 0;
  }
  if (arduboy.pressed(UP_BUTTON)) {
    dino_dy -= dino_spd;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    dino_dy += dino_spd;
  }

  dino_dx *= fric;
  dino_dy *= fric;

  dino_x += dino_dx;
  dino_y += dino_dy;
}

void dinoCheckAttack() {
  if (arduboy.pressed(B_BUTTON)) {
    dino_attacking = true;
  } else {
    dino_attacking = false;
  }
}

void dinoAttack() {
  if (arduboy.everyXFrames(4)) {
    fire_frame = (fire_frame + 1) % 4;
  }
}

void animateDino() {
  if (dino_attacking) {
    dino_frame = dino_dir/2;
  } else {
    fire_frame = 0;
    if(arduboy.everyXFrames(30)) {
      if (dirPressed()) {
        dino_walk_frame = (dino_walk_frame + 1) % 2;
      }
    }
    dino_frame = dino_dir + dino_walk_frame;
  }
}

void updateDino() {
  dinoCheckAttack();
  if (!dino_attacking) {
    dinoMove();
    limitXAtWorldBounds(&dino_x, &dino_dx, 26);
    limitYAtWorldBounds(&dino_y, &dino_dy, 16);
  } else {
    dinoAttack();
  }
  animateDino();
}

void drawDinoWalk() {
  //sprites.drawPlusMask(dino_screen_x, dino_screen_y, dino_sprite, dino_dir);
  sprites.drawExternalMask(dino_x + cam_x, dino_y + cam_y, dino_walk_img, dino_walk_mask, dino_frame, dino_frame);
}

void drawDinoAttack() {
  sprites.drawExternalMask(dino_x + cam_x, dino_y + cam_y, dino_attack_img, dino_attack_mask, dino_frame, dino_frame);
  sprites.drawExternalMask(dino_x+13-(20*(dino_dir-1)-4*(dino_dir-2)) + cam_x, dino_y+2 + cam_y, fire_img, fire_mask, dino_dir*2 + fire_frame, dino_dir*2 + fire_frame);
}

void drawDinoDebug() {
  arduboy.println("dino x, dino y: ");
  arduboy.println((int)dino_x);
  arduboy.println((int)dino_y);
  arduboy.print("dino_dir: ");
  arduboy.print(dino_dir);
  arduboy.println();
  arduboy.print("dino_walk_frame: ");
  arduboy.print(dino_walk_frame);
}

void drawDino() {
  if (dino_attacking) {
    drawDinoAttack();
  } else {
    drawDinoWalk();
  }

  //drawDinoDebug();
}
