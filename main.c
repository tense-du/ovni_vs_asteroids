#include <gtk/gtk.h>
#include "car_gobject.h"


typedef struct _Simulation
{
  Car *car;
  GtkWidget *speed_label;
  GtkWidget *position_label;
  GtkWidget *distance_label;
} Simulation;

static void key_released (GtkWidget *widget, GdkEvent *event, Car *car)
{
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Up) {
    car_set_accelerating (car, FALSE);
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Down) {
    car_set_decelerating (car, FALSE);
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Right) {
    car_set_strafing (car, CAR_RIGHT, FALSE);
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Left) {
   car_set_strafing (car, CAR_LEFT, FALSE);
  }
}

static void key_pressed(GtkWidget *widget, GdkEvent *event, Car *car)
{
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Up) {
    car_set_accelerating (car, TRUE);
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Down) {
    car_set_decelerating (car, TRUE);
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Right) {
    car_set_strafing (car, CAR_RIGHT, TRUE);
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Left) {
    car_set_strafing (car, CAR_LEFT, TRUE);
  }
}

  static void
display_car (Simulation *s)
{
  gchar *speed_text = g_strdup_printf ("SPEED  %f", car_get_current_speed (s->car));
  gchar *distance_text = g_strdup_printf ("DISTANCE %f", car_get_current_distance (s->car));
  gchar *position_text = g_strdup_printf ("POSITION %f", car_get_current_position (s->car));
  gtk_label_set_text (GTK_LABEL (s->speed_label), speed_text);
  g_free (speed_text);
  gtk_label_set_text (GTK_LABEL (s->distance_label), distance_text);
  g_free (distance_text);
  gtk_label_set_text (GTK_LABEL (s->position_label), position_text);
  g_free (position_text);
}

  static gboolean
update_simulation (Simulation *s)
{
  car_update (s->car);

  display_car (s);

  return TRUE;
}

int main( int argc, char *argv[])
{
  Car *c = NULL;
  GtkWidget *window;
  GtkWidget *vbox;
  Simulation *simulation = g_malloc0 (sizeof (Simulation));

  gtk_init(&argc, &argv);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  c = g_object_new (CAR_TYPE, NULL);

  fill_tank (c, 50);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "KIKABOE");
  gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  gtk_container_add (GTK_CONTAINER (window), vbox);

  simulation->position_label = gtk_label_new("");
  gtk_box_pack_start (GTK_BOX (vbox), simulation->position_label, TRUE, TRUE, 0);
  gtk_label_set_text (GTK_LABEL (simulation->position_label), "POSITION");

  simulation->distance_label = gtk_label_new ("");
  gtk_box_pack_start(GTK_BOX(vbox), simulation->distance_label, TRUE, TRUE, 0);
  gtk_label_set_text (GTK_LABEL(simulation->distance_label), "DISTANCE");

  simulation->speed_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (vbox), simulation->speed_label, TRUE, TRUE, 0); 
  gtk_label_set_text (GTK_LABEL (simulation->speed_label), "FIXME");

  g_signal_connect (G_OBJECT (window), "key-press-event",
      G_CALLBACK(key_pressed), c);
  g_signal_connect (G_OBJECT (window), "key-release-event",
      G_CALLBACK(key_released), c);

  g_signal_connect(G_OBJECT(window), "destroy", 
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  simulation->car = c;
  g_timeout_add (1000 / 60,(GSourceFunc)update_simulation, simulation);

  gtk_main();

  return 0;
}
