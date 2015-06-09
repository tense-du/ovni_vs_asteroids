#include "battlefield.h"
#include <stdio.h>

struct _GalaxyPrivate
{
  gint length;
  gint width;
};

static void galaxy_class_init (GalaxyClass *klass)
{
  g_type_class_add_private (klass, sizeof (GalaxyPrivate));
}

static void galaxy_init (Galaxy*self)
{
  self->priv = GALAXY_GET_PRIVATE (self);
  self->priv->length = 2000;
  self->priv->width = 500;
}

G_DEFINE_TYPE (Galaxy, galaxy, G_TYPE_OBJECT)

