#include "battlefield.h"
#include "car_gobject.h"
#include <stdio.h>
#include <glib.h>

struct _GalaxyPrivate
{
  Car *car;
  gfloat x;
  gfloat y;
  GList *asteroids;
  gboolean end_race;
  gboolean victory;
  gboolean dead;
};

static void galaxy_class_init (GalaxyClass *klass)
{
  g_type_class_add_private (klass, sizeof (GalaxyPrivate));
}

static void galaxy_init (Galaxy*self)
{
  self->priv = GALAXY_GET_PRIVATE (self);
  self->priv->car = NULL;
  self->priv->x= 800;
  self->priv->y = 2000;
  self->priv->asteroids = NULL;
  self->priv->end_race = FALSE;
  self->priv->victory = FALSE;
  self->priv->dead = FALSE;
}

G_DEFINE_TYPE (Galaxy, galaxy, G_TYPE_OBJECT)

GList *battlefield_get_asteroids (Galaxy *galaxy)
{
  return galaxy->priv->asteroids;
}

void galaxy_set_car (Galaxy *galaxy, Car *car)
{
  galaxy->priv->car = car;
  car_set_starting_point (galaxy->priv->car, galaxy->priv->x);
}

void create_asteroids (Galaxy*self)
{
  gint y;
  Asteroid* current_asteroid;
  GList *tmp;

  for (y = 0; y < self->priv->y; y += 200) {
    current_asteroid = g_malloc0(sizeof(Asteroid));
    current_asteroid->y = y;
    current_asteroid->x = g_rand_int_range (g_rand_new(), 0, self->priv->x +1 );
    current_asteroid->size = g_rand_int_range (g_rand_new(), 1, 4);
    self->priv->asteroids = g_list_append (self->priv->asteroids, current_asteroid);
  }
  for (tmp = self->priv->asteroids; tmp; tmp = tmp->next) {
    current_asteroid = (Asteroid*)tmp->data;
    g_print ("A x: %d, y: %d, asteroide de taille %d\n", current_asteroid->x, current_asteroid->y, current_asteroid->size);
  }
}

void battlefield_update (Galaxy *self)
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
    if (car_get_y (self->priv->car) >= current->y + current->size * 15) {
      self->priv->asteroids = self->priv->asteroids->next;
    }
  }
  battlefield_check_collision (self);
}

gboolean battlefield_check_collision (Galaxy*self)
{
  Asteroid *current;
  guint64 car_y = car_get_y (self->priv->car);
  gint car_x = car_get_x (self->priv->car);

  if (self->priv->asteroids == NULL) {
    return FALSE;
  }

  
  current = (Asteroid*)self->priv->asteroids->data;

  if (car_y >= current->y && car_y < current->y + current->size * 15) {
    if (car_x >= current->x && car_x < current->x + current->size * 15) {
      self->priv->dead = TRUE;
      return TRUE;
    }
  }
  return FALSE;
}

