#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <glib-object.h>
#include "car_gobject.h"
#include <glib.h>
#include <gtk/gtk.h>

#define GALAXY_TYPE                \
  (galaxy_get_type())
#define GALAXY(o)                  \
  (G_TYPE_CHECK_INSTANCE_CAST ((o), GALAXY_TYPE, Galaxy))
#define GALAXY_CLASS(c)            \
  (G_TYPE_CHECK_CLASS_CAST ((c), GALAXY_TYPE, GalaxyCass))
#define IS_GALAXY(o)               \
  (G_TYPE_CHECK_INSTANCE_TYPE ((o), GALAXY_TYPE))
#define IS_GALAXY_CLASS(c)         \
  (G_TYPE_CHECK_CLASS_TYPE ((c),  GALAXY_TYPE)) 
#define GALAXY_GET_CLASS(o)        \
  (G_TYPE_INSTANCE_GET_CLASS ((o), GALAXY_TYPE, GalaxyClass))
#define GALAXY_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
      GALAXY_TYPE,                    \
      GalaxyPrivate))

typedef struct _Galaxy             Galaxy;
typedef struct _GalaxyClass        GalaxyClass;
typedef struct _GalaxyPrivate      GalaxyPrivate;
struct _Galaxy
{
  GObject parent;

  GalaxyPrivate * priv;
};

struct _GalaxyClass
{
  GObjectClass parent;
};

GType galaxy_get_type (void);

void create_asteroids (Galaxy*self);

void galaxy_set_car (Galaxy *galaxy, Car *car);

void battlefield_update (Galaxy *galaxy);

#endif /* BATTLEFIELD_H */
