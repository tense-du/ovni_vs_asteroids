#include "car_gobject.h"
#include <stdio.h>


struct _CarPrivate
{
  gint tank_capacity;
  gfloat current_tank_level;
  gfloat current_speed;
  gboolean accelerating;
  gboolean decelerating;

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
}

G_DEFINE_TYPE (Car, car, G_TYPE_OBJECT)

void set_accelerating (Car *self, gboolean accelerating)
{
  self->priv->accelerating = accelerating;
}

void set_decelerating (Car *self, gboolean decelerating)
{
  self->priv->decelerating = decelerating;
}
void slow_down (Car *self)
{
  self->priv->current_speed -= 0.02;
  if (self->priv->current_speed < 0)
    self->priv->current_speed = 0;
}

void decelerating (Car *self)
{
  self->priv->current_speed -= 0.04;
  if (self->priv->current_speed <0) 
    self->priv->current_speed = 0;
}

void car_update (Car *self)
{
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
    g_print("Habeudeubeudeu\n");
    return FALSE;
  }
  self->priv->current_speed += 0.15;
  self->priv->current_tank_level -= 0.000027778;
  g_print ("VROUUUUUM\n");
  return TRUE;
}

gfloat car_get_current_speed (Car *self)
{
  return self->priv->current_speed;
}

gboolean fill_tank (Car * self, gint fuel_quantity)
{
  if (self->priv->current_tank_level + fuel_quantity > self->priv->tank_capacity) {
    return FALSE;
  }
  self->priv->current_tank_level += fuel_quantity;
  return TRUE;
}

