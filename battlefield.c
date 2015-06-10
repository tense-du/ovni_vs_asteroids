#include "battlefield.h"
#include "car_gobject.h"
#include <stdio.h>
#include <glib.h>

typedef struct
{
  gfloat ordinate_x;
  gfloat ordinate_y;
  gint asteroid_size;
} Asteroid;

struct _GalaxyPrivate
{
  Car *car;
  gfloat length;
  gfloat width;
  GList *asteroids;
};

static void galaxy_class_init (GalaxyClass *klass)
{
  g_type_class_add_private (klass, sizeof (GalaxyPrivate));
}

static void galaxy_init (Galaxy*self)
{
  self->priv->car = NULL;
  self->priv = GALAXY_GET_PRIVATE (self);
  self->priv->length = 2000;
  self->priv->width = 500;
  self->priv->asteroids = NULL;
}

G_DEFINE_TYPE (Galaxy, galaxy, G_TYPE_OBJECT)

void galaxy_set_car (Galaxy *galaxy, Car *car)
{
  galaxy->priv->car = car;
}

void create_asteroids (Galaxy*self)
{
  gint x;
  Asteroid* current_asteroid;
  GList *tmp;

  for (x = 0; x < self->priv->length; x += 200) {
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
  Asteroid *current = (Asteroid*)self->priv->asteroids->data;
  if (car_get_current_distance(self->priv->car) >= self->priv->length) {
    g_print ("VICTOIRE!");
  }
  if (car_get_current_distance (self->priv->car) > current->ordinate_x) {
    self->priv->asteroids = self->priv->asteroids->next;
  }
}

