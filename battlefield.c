#include "battlefield.h"
#include "car_gobject.h"
#include "config.h"
#include <stdio.h>
#include <glib.h>
#include <cairo.h>

struct _BattlefieldPrivate
{
  Car *car;
  gfloat x;
  gfloat y;
  GList *asteroids;
  gboolean failure;
  gboolean success;
  cairo_surface_t *image;
};

static void battlefield_class_init (BattlefieldClass *klass)
{
  g_type_class_add_private (klass, sizeof (BattlefieldPrivate));
}

static void battlefield_init (Battlefield *self)
{
  self->priv = BATTLEFIELD_GET_PRIVATE (self);
  self->priv->car = NULL;
  self->priv->x= BATTLEFIELD_X;
  self->priv->y = BATTLEFIELD_Y;
  self->priv->asteroids = NULL;
  self->priv->failure = FALSE;
  self->priv->success = FALSE;
  self->priv->image = NULL;
}

G_DEFINE_TYPE (Battlefield, battlefield, G_TYPE_OBJECT)

GList *battlefield_get_asteroids (Battlefield *battlefield)
{
  return battlefield->priv->asteroids;
}

void battlefield_set_up (Battlefield *battlefield, Car *car)
{
  battlefield->priv->car = car;
  car_set_starting_point (battlefield->priv->car);
  create_asteroids (battlefield);
  battlefield->priv->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/battlefield/galaxy.png");
}

void create_asteroids (Battlefield *self)
{
  gint y;
  Asteroid* current_asteroid;
  GList *tmp;

  for (y = GAP; y < self->priv->y; y += GAP) {
    current_asteroid = g_malloc0(sizeof(Asteroid));
    current_asteroid->y = y;
    current_asteroid->size = g_rand_int_range (g_rand_new(), 1, 4);
    current_asteroid->x = g_rand_int_range (g_rand_new(), 0, self->priv->x +1 - current_asteroid->size * MULTIPLIER);
    if (current_asteroid->size == 1) {
      current_asteroid->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/Unicorns/1.png");
    }
    else if (current_asteroid->size == 2) {
      current_asteroid->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/Unicorns/2.png");
    }
    else if (current_asteroid->size == 3) {
      current_asteroid->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/Unicorns/3.png");
    }
    current_asteroid->taken_image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/Unicorns/taken.png");
    current_asteroid->taken = FALSE;
    self->priv->asteroids = g_list_append (self->priv->asteroids, current_asteroid);
  }
  for (tmp = self->priv->asteroids; tmp; tmp = tmp->next) {
    current_asteroid = (Asteroid*)tmp->data;
    g_print ("A x: %d, y: %d, asteroide de taille %d\n", current_asteroid->x, current_asteroid->y, current_asteroid->size);
  }
}

static void battlefield_check_overlap (Battlefield *self, Asteroid *asteroid)
{
  guint64 car_y = car_get_y (self->priv->car);
  gint car_x = car_get_x (self->priv->car);

  if (asteroid->x + asteroid->size * MULTIPLIER < car_x - CAR_SIZE /2 ||car_x + CAR_SIZE / 2 < asteroid->x || car_y + CAR_SIZE < asteroid->y || asteroid->y + asteroid->size * MULTIPLIER < car_y) {
    return;
  }
  else {
    asteroid->taken = TRUE;
    return;
  }
}
void battlefield_update (Battlefield *self)
{
  GList *tmp;
  Asteroid *current;


  if (car_get_y (self->priv->car) >= BATTLEFIELD_Y) {
    self->priv->success = TRUE;
    return;
  }

  tmp = self->priv->asteroids;

  if (tmp == NULL) {
    return;
  }
  
  current = (Asteroid*)tmp->data;
  if (car_get_y (self->priv->car) >= current->y + current->size * MULTIPLIER) {
    if (current->taken == FALSE) {
      self->priv->failure = TRUE;
    }
    tmp = self->priv->asteroids = self->priv->asteroids->next;
    current = (Asteroid*)self->priv->asteroids->data;
  }
  battlefield_check_overlap (self, current);
}

gboolean failure (Battlefield *self)
{
  if (self->priv->asteroids == NULL) {
    return FALSE;
  }

  if (!self->priv->failure) {
    return FALSE;
  }
  return TRUE;
}

gboolean success (Battlefield *self)
{
  if (self->priv->success) {
    return TRUE;
  }
  return FALSE;
}

cairo_surface_t * battlefield_get_image (Battlefield *self)
{
  return self->priv->image;
}
