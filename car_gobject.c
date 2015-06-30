#include "car_gobject.h"
#include "config.h"
#include <stdio.h>

struct _CarPrivate
{
  gint tank_capacity;
  gfloat current_tank_level;
  gfloat current_speed;
  gboolean accelerating;
  gboolean decelerating;
  gint x;
  guint64 y;
  gint strafing;
  cairo_surface_t *neutral;
  cairo_surface_t *left;
  cairo_surface_t *right;
  cairo_surface_t *forward;
  cairo_surface_t *forward_right;
  cairo_surface_t *forward_left;
};

static void
car_class_init (CarClass * klass)
{
  g_type_class_add_private (klass, sizeof (CarPrivate));
}

static void
car_init (Car * self)
{
  self->priv = CAR_GET_PRIVATE (self);
  self->priv->tank_capacity = TANK_CAPACITY;
  self->priv->current_tank_level = 0;
  self->priv->current_speed = 0;
  self->priv->accelerating = FALSE;
  self->priv->decelerating = FALSE;
  self->priv->x = 0;
  self->priv->y = 0;
  self->priv->strafing = 0;
  self->priv->neutral = NULL;
  self->priv->left = NULL;
  self->priv->right = NULL;
  self->priv->forward = NULL;
  self->priv->forward_right = NULL;
  self->priv->forward_left = NULL;
}

G_DEFINE_TYPE (Car, car, G_TYPE_OBJECT)

     void car_set_starting_point (Car * self)
{
  self->priv->x = (BATTLEFIELD_X / 2);
  self->priv->neutral =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/neutral.png");

  self->priv->left =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/neutral-left.png");
  self->priv->right =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/neutral-right.png");
  self->priv->forward =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/forward.png");
  self->priv->forward_right =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/forward-right.png");
  self->priv->forward_left =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/forward-left.png");
  self->priv->neutral =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/neutral.png");
  self->priv->neutral =
      cairo_image_surface_create_from_png
      ("/home/tense_du/Downloads/ship/neutral.png");
}

void
car_set_accelerating (Car * self, gboolean accelerating)
{
  self->priv->accelerating = accelerating;
}

void
car_set_strafing (Car * self, CarDirection dir, gboolean on_off)
{
  if (on_off == FALSE) {
    self->priv->strafing = 0;
  }
  if (on_off == TRUE) {
    if (dir == CAR_RIGHT) {
      self->priv->strafing = STRAFING;
    } else {
      self->priv->strafing = -STRAFING;
    }
  }
}

void
car_set_decelerating (Car * self, gboolean decelerating)
{
  self->priv->decelerating = decelerating;
}

static void
slow_down (Car * self)
{
  self->priv->current_speed -= SLOW_DOWN;
  if (self->priv->current_speed < 0)
    self->priv->current_speed = 0;
}

static void
decelerate (Car * self)
{
  self->priv->current_speed -= DECELERATE;
  if (self->priv->current_speed < 0)
    self->priv->current_speed = 0;
}

gboolean
speed_up (Car * self)
{
  if (self->priv->current_tank_level - 1 < 0) {
    return FALSE;
  }
  self->priv->current_speed += SPEED_UP;
  self->priv->current_tank_level -= 0.000027778;
  return TRUE;
}

void
car_update (Car * self)
{
  gfloat per_second = self->priv->current_speed / 0.00036;
  self->priv->y += per_second / 60;

  self->priv->x += self->priv->strafing;
  if (self->priv->x > BATTLEFIELD_X - CAR_SIZE / 2) {
    self->priv->x = BATTLEFIELD_X - CAR_SIZE / 2;
  }
  if (self->priv->x < CAR_SIZE / 2) {
    self->priv->x = CAR_SIZE / 2;
  }
  if (self->priv->accelerating) {
    speed_up (self);
  }
  if (self->priv->decelerating) {
    decelerate (self);
  }
  if (!self->priv->accelerating && !self->priv->decelerating) {
    slow_down (self);
  }
}

gboolean
is_accelerating (Car * self)
{
  return self->priv->accelerating;
}

gfloat
car_get_current_speed (Car * self)
{
  return self->priv->current_speed;
}

gint
car_get_x (Car * self)
{
  return self->priv->x;
}

gint
car_get_y (Car * self)
{
  return self->priv->y / 1000;
}

gboolean
fill_tank (Car * self, gint fuel_quantity)
{
  if (self->priv->current_tank_level + fuel_quantity >
      self->priv->tank_capacity) {
    return FALSE;
  }
  self->priv->current_tank_level += fuel_quantity;
  return TRUE;
}

cairo_surface_t *
car_get_image (Car * self)
{
  if (self->priv->accelerating) {
    if (self->priv->strafing == 0) {
      return self->priv->forward;
    }
    if (self->priv->strafing < 0) {
      return self->priv->forward_left;
    } else {
      return self->priv->forward_right;
    }
  }
  if (!self->priv->accelerating) {
    if (self->priv->strafing == 0) {
      return self->priv->neutral;
    }
    if (self->priv->strafing < 0) {
      return self->priv->left;
    } else {
      return self->priv->right;
    }
  }
}
