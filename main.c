#include <gtk/gtk.h>
#include <string.h>
#include "car_gobject.h"
#include "battlefield.h"

typedef struct _Simulation
{
  Galaxy *galaxy;
  Car *car;
  GtkWidget *battlefield;
  GtkWidget *current_asteroid;
  GtkWidget *speed_label;
  GtkWidget *position_label;
  GtkWidget *distance_label;
} Simulation;


static void do_drawing (cairo_t *cr, Simulation *self)
{
  GList *asteroids = battlefield_get_asteroids (self->galaxy);
  Asteroid *current;

  cairo_rectangle(cr, car_get_x(self->car), 580, 20, 20);

  while (asteroids != NULL) {
    current = (Asteroid*)asteroids->data;
    cairo_rectangle (cr, current->x, 580 - (current->y - car_get_y(self->car)), current->size * 15, current->size * 15);
    asteroids = asteroids->next;
  }

  cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
  cairo_set_line_width(cr, 1);


  cairo_stroke_preserve(cr);
  cairo_fill(cr);

}
static gboolean on_draw_event (GtkWidget *widget, cairo_t *cr, Simulation *self)
{
  do_drawing (cr, self);

  return FALSE;
}

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
display_car (Simulation *self)
{
  gchar *speed_text = g_strdup_printf ("SPEED  %f", car_get_current_speed (self->car));
  gchar *distance_text = g_strdup_printf ("DISTANCE %d", car_get_y (self->car));
  gchar *position_text = g_strdup_printf ("POSITION %d", car_get_x (self->car));
  gtk_label_set_text (GTK_LABEL (self->speed_label), speed_text);
  g_free (speed_text);
  gtk_label_set_text (GTK_LABEL (self->distance_label), distance_text);
  g_free (distance_text);
  gtk_label_set_text (GTK_LABEL (self->position_label), position_text);
  g_free (position_text);
  gtk_widget_queue_draw_area (self->battlefield, 0, 0, 800, 600);
}

  static gboolean
update_simulation (Simulation *s)
{
  car_update (s->car);

  battlefield_update (s->galaxy);

  display_car (s);

  return TRUE;
}

int main( int argc, char *argv[])
{
  Car *c = NULL;
  Galaxy *galaxy = NULL;
  GtkWidget *window;
  GtkWidget *vbox;
  Simulation *simulation = g_malloc0 (sizeof (Simulation));

  gtk_init(&argc, &argv);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous (GTK_BOX (vbox), FALSE);
  c = g_object_new (CAR_TYPE, NULL);
  galaxy = g_object_new (GALAXY_TYPE, NULL);

  fill_tank (c, 50);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "KIKABOE");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 1000);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  gtk_container_add (GTK_CONTAINER (window), vbox);

  simulation->battlefield = gtk_drawing_area_new();
  gtk_widget_set_size_request (simulation->battlefield, 800, 600);
  gtk_box_pack_start (GTK_BOX (vbox), simulation->battlefield, FALSE, FALSE, 0);

  simulation->position_label = gtk_label_new("");
  gtk_box_pack_start (GTK_BOX (vbox), simulation->position_label,FALSE, FALSE, 0);
  gtk_label_set_text (GTK_LABEL (simulation->position_label), "POSITION");

  simulation->current_asteroid = gtk_label_new("");
  gtk_box_pack_start (GTK_BOX(vbox), simulation->current_asteroid, FALSE, FALSE, 0);
  gtk_label_set_text (GTK_LABEL(simulation->current_asteroid), "ASTEROID");


  simulation->distance_label = gtk_label_new ("");
  gtk_box_pack_start(GTK_BOX(vbox), simulation->distance_label, FALSE, FALSE, 0);
  gtk_label_set_text (GTK_LABEL(simulation->distance_label), "DISTANCE");

  simulation->speed_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (vbox), simulation->speed_label, FALSE, FALSE, 0); 
  gtk_label_set_text (GTK_LABEL (simulation->speed_label), "FIXME");

  g_signal_connect (G_OBJECT (window), "key-press-event",
      G_CALLBACK(key_pressed), c);
  g_signal_connect (G_OBJECT (window), "key-release-event",
      G_CALLBACK(key_released), c);

  g_signal_connect (G_OBJECT(simulation->battlefield), "draw",
      G_CALLBACK(on_draw_event), simulation);

  g_signal_connect(G_OBJECT(window), "destroy", 
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  simulation->galaxy = galaxy;

  simulation->car = c;

  galaxy_set_car (galaxy, c);

  create_asteroids (galaxy);

  g_timeout_add (1000 / 60,(GSourceFunc)update_simulation, simulation);

  gtk_main();

  return 0;
}
