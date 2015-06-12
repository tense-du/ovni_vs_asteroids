#include "battlefield.h"
#include "car_gobject.h"
#include <stdio.h>
#include <glib.h>

typedef struct
{
  gint ordinate_x;
  gint ordinate_y;
  gint asteroid_size;
} Asteroid;

struct _GalaxyPrivate
{
  Car *car;
  gfloat length;
  gfloat width;
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
  self->priv->length = 1000;
  self->priv->width = 500;
  self->priv->asteroids = NULL;
  self->priv->end_race = FALSE;
  self->priv->victory = FALSE;
  self->priv->dead = FALSE;
}

G_DEFINE_TYPE (Galaxy, galaxy, G_TYPE_OBJECT)

void galaxy_set_car (Galaxy *galaxy, Car *car)
{
  galaxy->priv->car = car;
  car_set_starting_point (galaxy->priv->car, galaxy->priv->width);
}

void create_asteroids (Galaxy*self)
{
  gint x;
  Asteroid* current_asteroid;
  GList *tmp;

  for (x = 0; x < self->priv->length; x += 100) {
    current_asteroid = g_malloc0(sizeof(Asteroid));
    current_asteroid->ordinate_x = x;
    current_asteroid->ordinate_y = g_rand_int_range (g_rand_new(), 0, self->priv->width +1 );
    current_asteroid->asteroid_size = g_rand_int_range (g_rand_new(), 1, 4);
    self->priv->asteroids = g_list_append (self->priv->asteroids, current_asteroid);
  }
  for (tmp = self->priv->asteroids; tmp; tmp = tmp->next) {
    current_asteroid = (Asteroid*)tmp->data;
    g_print ("Au metre %d, position %d, asteroide de taille %d\n", current_asteroid->ordinate_x, current_asteroid->ordinate_y, current_asteroid->asteroid_size);
  }
}

void battlefield_update (Galaxy *self)
{
  Asteroid *current;

  if (self->priv->asteroids == NULL) {
    if (self->priv->dead) {
      self->priv->victory = FALSE;
      if (self->priv->end_race == FALSE) {
        if (car_get_current_distance (self->priv->car) >= self->priv->length) {
          self->priv->end_race = TRUE;
          g_print ("LOOOSER");
        }
      }
    }
    else {
      self->priv->victory = TRUE;
      if (car_get_current_distance(self->priv->car) >= self->priv->length) {
        if (self->priv->end_race == FALSE) {
          self->priv->end_race = TRUE;
          g_print ("VICTOIRE!");
        }
      }
    }
  }
  else {
    current = (Asteroid*)self->priv->asteroids->data;
    if (car_get_current_distance (self->priv->car) > current->ordinate_x) {
      self->priv->asteroids = self->priv->asteroids->next;
    }
  }
}

gboolean battlefield_check_collision (Galaxy*self)
{
  Asteroid *current;
  guint64 car_distance = car_get_current_distance (self->priv->car);
  gint car_position = car_get_current_position (self->priv->car);

  if (self->priv->asteroids == NULL) {
    return FALSE;
  }
  current = (Asteroid*)self->priv->asteroids->data;

  if (car_distance >= current->ordinate_x && car_distance <= current->ordinate_x+current->asteroid_size - 1) {
    if (car_position >= current->ordinate_y && car_position <= current->ordinate_y + current->asteroid_size - 1) {
      g_print ("Collision with size %d asteroid at level %d, position %d\n", current->asteroid_size, current->ordinate_x, current->ordinate_y);
      g_print ("Car at level %d, position %d\n", car_distance, car_position);
      self->priv->dead = TRUE;
      return TRUE;
    }
  }
  return FALSE;
}

