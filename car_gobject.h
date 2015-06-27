#ifndef CAR_GOBJECT_H
#define CAR_GOBJECT_H

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <cairo.h>

#define CAR_TYPE                \
    (car_get_type())
#define CAR(o)			\
	(G_TYPE_CHECK_INSTANCE_CAST ((o), CAR_TYPE, Car))
#define CAR_CLASS(c)            \
    (G_TYPE_CHECK_CLASS_CAST ((c), CAR_TYPE, CarClass))
#define IS_CAR(o)               \
    (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAR_TYPE))
#define IS_CAR_CLASS(c)         \
    (G_TYPE_CHECK_CLASS_TYPE ((c),  CAR_TYPE))
#define CAR_GET_CLASS(o)        \
    (G_TYPE_INSTANCE_GET_CLASS ((o), CAR_TYPE, CarClass))
#define CAR_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
                                 CAR_TYPE,                    \
                                 CarPrivate))

typedef enum 
{
  CAR_LEFT, 
  CAR_RIGHT,
} CarDirection;

typedef struct _Car		Car;
typedef struct _CarClass	CarClass;
typedef struct _CarPrivate	CarPrivate;

struct _Car
{
GObject parent;

CarPrivate * priv;
};

struct _CarClass
{
GObjectClass parent;
};

GType car_get_type (void);

void car_update (Car *self);

void car_set_starting_point (Car *self);

void car_set_accelerating (Car *self, gboolean accelerating);

void car_set_strafing (Car *self, CarDirection dir, gboolean x);

void car_set_decelerating (Car *self, gboolean decelerating);

gboolean is_accelerating (Car *self);

gfloat car_get_current_speed (Car *self);

gint car_get_x (Car *self);

gint car_get_y (Car *self);

cairo_surface_t *car_get_image (Car *self);
#endif /* CAR_GOBJECT_H */

