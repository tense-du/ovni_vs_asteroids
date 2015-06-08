#ifndef CAR_GOBJECT_H
#define CAR_OBJECT_H

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

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

gboolean is_accelerating (Car *self);

gfloat car_get_current_speed (Car *self);

void button_clicked(GtkWidget *widget, gpointer data);
#endif /* CAR_GOBJECT_H */

