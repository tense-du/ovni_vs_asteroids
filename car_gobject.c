#include "car_gobject.h"
#include <stdio.h>


struct _CarPrivate
{
  gint tank_capacity;
  gfloat current_tank_level;
  gfloat current_speed;
  gboolean accelerating;
  gboolean decelerating;
  gfloat position;
  gfloat strafing;
  gfloat distance;

};

static void car_class_init (CarClass *klass)
{

  g_type_class_add_private (klass, sizeof(CarPrivate));
}

static void car_init (Car *self)
{
  self->priv = CAR_GET_PRIVATE (self);
  self->priv->tank_capacity = 50; 
  self->priv->current_tank_level = 0;
  self->priv->current_speed = 0;
  self->priv->accelerating = FALSE;
  self->priv->decelerating = FALSE;
  self->priv->position = 0;
  self->priv->strafing = 0;
  self->priv->distance = 0;
}

G_DEFINE_TYPE (Car, car, G_TYPE_OBJECT)

void car_set_accelerating (Car *self, gboolean accelerating)
{
  self->priv->accelerating = accelerating;
}

void car_set_strafing (Car *self, CarDirection dir, gboolean on_off)
{
  if (on_off == FALSE) {
    self->priv->strafing = 0;
  }
  if (dir == CAR_RIGHT) {
    self->priv->strafing = 0.01;
  } else {
    self->priv->strafing = -0.01;
  }
}
void car_set_decelerating (Car *self, gboolean decelerating)
{
  self->priv->decelerating = decelerating;
}
static void slow_down (Car *self)
{
  self->priv->current_speed -= 0.02;
  if (self->priv->current_speed < 0)
    self->priv->current_speed = 0;
}

static void decelerating (Car *self)
{
  self->priv->current_speed -= 0.5;
  if (self->priv->current_speed <0) 
    self->priv->current_speed = 0;
}

void car_update (Car *self)
{
  gfloat per_second = self->priv->current_speed / 3.6;
  self->priv->distance += per_second / 60 ;
  self->priv->position += self->priv->strafing;

  if (self->priv->position > 1) {
    self->priv->position = 1;
  }
  if (self->priv->position < -1) {
    self->priv->position = -1;
  }
  if (self->priv->accelerating) {
    speed_up (self);
  } if (self->priv->decelerating) {
    decelerating (self);
  } else {
    slow_down (self);
  }
}

gboolean is_accelerating (Car *self)
{
  return self->priv->accelerating;
}

gboolean speed_up (Car *self)
{
  if (self->priv->current_tank_level -1 < 0) {
    return FALSE;
  }
  self->priv->current_speed += 0.15;
  self->priv->current_tank_level -= 0.000027778;
  return TRUE;
}

gfloat car_get_current_speed (Car *self)
{
  return self->priv->current_speed;
}

gfloat car_get_current_position (Car *self)
{
  return self->priv->position;
}

gfloat car_get_current_distance (Car *self)
{
  return self->priv->distance;
}

gboolean fill_tank (Car * self, gint fuel_quantity)
{
  if (self->priv->current_tank_level + fuel_quantity > self->priv->tank_capacity) {
    return FALSE;
  }
  self->priv->current_tank_level += fuel_quantity;
  return TRUE;
}


