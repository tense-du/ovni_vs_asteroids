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
  gboolean end_race;
  gboolean victory;
  gboolean dead;
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
  self->priv->end_race = FALSE;
  self->priv->victory = FALSE;
  self->priv->dead = FALSE;
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
      current_asteroid->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/asteroids/1.png");
    }
    else if (current_asteroid->size == 2) {
     current_asteroid->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/asteroids/2.png");
    }
    else if (current_asteroid->size == 3) {
      current_asteroid->image = cairo_image_surface_create_from_png ("/home/tense_du/Downloads/asteroids/3.png");
    }
    else {
      break;
    }
    self->priv->asteroids = g_list_append (self->priv->asteroids, current_asteroid);
  }
  for (tmp = self->priv->asteroids; tmp; tmp = tmp->next) {
    current_asteroid = (Asteroid*)tmp->data;
    g_print ("A x: %d, y: %d, asteroide de taille %d\n", current_asteroid->x, current_asteroid->y, current_asteroid->size);
  }
}

void battlefield_update (Battlefield *self)
{
  Asteroid *current;

  if (self->priv->asteroids == NULL) {
    if (self->priv->dead) {
      self->priv->victory = FALSE;
      if (self->priv->end_race == FALSE) {
        if (car_get_y (self->priv->car) >= self->priv->y) {
          self->priv->end_race = TRUE;
          g_print ("LOOOSER");
        }
      }
    }
    else {
      self->priv->victory = TRUE;
      if (car_get_y(self->priv->car) >= self->priv->y) {
        if (self->priv->end_race == FALSE) {
          self->priv->end_race = TRUE;
          g_print ("VICTOIRE!");
        }
      }
    }
  }
  else {
    current = (Asteroid*)self->priv->asteroids->data;
    if (car_get_y (self->priv->car) >= current->y + current->size * MULTIPLIER) {
      self->priv->asteroids = self->priv->asteroids->next;
    }
  }
  battlefield_check_collision (self);
}

gboolean battlefield_check_collision (Battlefield *self)
{
  Asteroid *current;
  guint64 car_y = car_get_y (self->priv->car);
  gint car_x = car_get_x (self->priv->car);

  if (self->priv->asteroids == NULL) {
    return FALSE;
  }
 
  current = (Asteroid*)self->priv->asteroids->data;

  if (car_y >= (guint64)current->y && car_y < (guint64)current->y + current->size * MULTIPLIER) {
    if (car_x >= current->x && car_x < current->x + current->size * MULTIPLIER) {
      self->priv->dead = TRUE;
      return TRUE;
    }
  }
  return FALSE;
}

