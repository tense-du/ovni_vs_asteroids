#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <glib-object.h>
#include "car_gobject.h"
#include <glib.h>
#include <gtk/gtk.h>
#include <cairo.h>

#define BATTLEFIELD_TYPE                \
  (battlefield_get_type())
#define BATTLEFIELD(o)                  \
  (G_TYPE_CHECK_INSTANCE_CAST ((o), BATTLEFIELD_TYPE, Battlefield))
#define BATTLEFIELD_CLASS(c)            \
  (G_TYPE_CHECK_CLASS_CAST ((c), BATTLEFIELD_TYPE, BattlefieldCass))
#define IS_BATTLEFIELD(o)               \
  (G_TYPE_CHECK_INSTANCE_TYPE ((o), BATTLEFIELD_TYPE))
#define IS_BATTLEFIELD_CLASS(c)         \
  (G_TYPE_CHECK_CLASS_TYPE ((c),  BATTLEFIELD_TYPE)) 
#define BATTLEFIELD_GET_CLASS(o)        \
  (G_TYPE_INSTANCE_GET_CLASS ((o), BATTLEFIELD_TYPE, BattlefieldClass))
#define BATTLEFIELD_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
      BATTLEFIELD_TYPE,                    \
      BattlefieldPrivate))

typedef struct _Battlefield             Battlefield;
typedef struct _BattlefieldClass        BattlefieldClass;
typedef struct _BattlefieldPrivate      BattlefieldPrivate;


typedef struct
{
  gint x;
  gint y;
  gint size;
  cairo_surface_t * image;
} Asteroid;


struct _Battlefield
{
  GObject parent;

  BattlefieldPrivate * priv;
};

struct _BattlefieldClass
{
  GObjectClass parent;
};

GType battlefield_get_type (void);

void create_asteroids (Battlefield *self);

void battlefield_set_up (Battlefield *battlefield, Car *car);

void battlefield_update (Battlefield *self);

gboolean battlefield_check_collision (Battlefield *self);

GList * battlefield_get_asteroids (Battlefield *self);

#endif /* BATTLEFIELD_H */
