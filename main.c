#include <gtk/gtk.h>
#include <string.h>
#include "car_gobject.h"
#include "battlefield.h"
#include "config.h"
#include <cairo.h>


typedef struct _Simulation
{
  Battlefield *battlefield;
  Car *car;
  GtkWidget *battlefield_display;
  GtkWidget *speed_label;
  GtkWidget *x_label;
  GtkWidget *y_label;
} Simulation;


static void do_drawing (cairo_t *cr, Simulation *self)
{
  GList *asteroids = battlefield_get_asteroids (self->battlefield);
  Asteroid *current;
  gint dist, width, height;
  cairo_surface_t *car_image;
  cairo_surface_t *asteroid_image;
  double xs, ys;

  car_image = car_get_image (self->car);

  width = cairo_image_surface_get_width (car_image);
  height = cairo_image_surface_get_height (car_image);
  xs = 1 / ((double) width/ (double) CAR_SIZE);
  ys = 1 / ((double)height /(double) CAR_SIZE);

  cairo_save (cr);
  cairo_scale (cr, xs, ys);
  cairo_set_source_surface(cr, car_image, (double)car_get_x (self->car) * ((double)width/(double)CAR_SIZE), (600 - CAR_SIZE) * ((double)height / (double)CAR_SIZE));
  cairo_paint(cr);
  cairo_restore (cr);

  for (asteroids; asteroids != NULL; asteroids= asteroids->next){
    current = (Asteroid*)asteroids->data;
    dist = current->y - car_get_y(self->car);
    if (600 - dist > 0) {
      asteroid_image = current->image;
      dist = 600 - dist;
      width = cairo_image_surface_get_width (asteroid_image);
      height = cairo_image_surface_get_height (asteroid_image);
      xs = 1 / ((double) width/ ((double) current->size * (double)MULTIPLIER));
      ys = 1 / ((double)height /((double) current->size * (double)MULTIPLIER));
      cairo_save(cr);
      cairo_scale (cr, xs, ys);
      cairo_set_source_surface (cr, asteroid_image, (double) current->x * ((double)width / ((double)current->size * (double)MULTIPLIER)), (dist - current->size * MULTIPLIER) * ((double)height / ((double)current->size * (double)MULTIPLIER)));
      cairo_paint (cr);
      cairo_restore (cr);
    }
    else {
      break;
    }
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
  gchar *x_text = g_strdup_printf ("X: %d", car_get_x (self->car));
  gchar *y_text = g_strdup_printf ("Y: %d", car_get_y (self->car));

  gtk_label_set_text (GTK_LABEL (self->speed_label), speed_text);
  g_free (speed_text);
  gtk_label_set_text (GTK_LABEL (self->x_label), x_text);
  g_free (x_text);
  gtk_label_set_text (GTK_LABEL (self->y_label), y_text);
  g_free (y_text);
  gtk_widget_queue_draw_area (self->battlefield_display, 0, 0, 800, 600);
}

  static gboolean
update_simulation (Simulation *self)
{
  car_update (self->car);

  battlefield_update (self->battlefield);

  display_car (self);

  return TRUE;
}

int main( int argc, char *argv[])
{
  Car *c = NULL;
  Battlefield *battlefield = NULL;
  GtkWidget *window;
  GtkWidget *vbox;
  Simulation *simulation = g_malloc0 (sizeof (Simulation));

  gtk_init(&argc, &argv);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous (GTK_BOX (vbox), FALSE);
  c = g_object_new (CAR_TYPE, NULL);
  battlefield = g_object_new (BATTLEFIELD_TYPE, NULL);

  fill_tank (c, 50);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "KIKABOE");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  gtk_container_add (GTK_CONTAINER (window), vbox);

  simulation->battlefield_display = gtk_drawing_area_new();
  gtk_widget_set_size_request (simulation->battlefield_display, 800, 600);
  gtk_box_pack_start (GTK_BOX (vbox), simulation->battlefield_display, FALSE, FALSE, 0);

  simulation->x_label = gtk_label_new("");
  gtk_box_pack_start (GTK_BOX (vbox), simulation->x_label,FALSE, FALSE, 0);
  gtk_label_set_text (GTK_LABEL (simulation->x_label), "X");

  simulation->y_label = gtk_label_new ("");
  gtk_box_pack_start(GTK_BOX(vbox), simulation->y_label, FALSE, FALSE, 0);
  gtk_label_set_text (GTK_LABEL(simulation->y_label), "DISTANCE");

  simulation->speed_label = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (vbox), simulation->speed_label, FALSE, FALSE, 0); 
  gtk_label_set_text (GTK_LABEL (simulation->speed_label), "FIXME");

  g_signal_connect (G_OBJECT (window), "key-press-event",
      G_CALLBACK(key_pressed), c);
  g_signal_connect (G_OBJECT (window), "key-release-event",
      G_CALLBACK(key_released), c);

  g_signal_connect (G_OBJECT(simulation->battlefield_display), "draw",
      G_CALLBACK(on_draw_event), simulation);

  g_signal_connect(G_OBJECT(window), "destroy", 
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  simulation->battlefield = battlefield;

  simulation->car = c;

  battlefield_set_up (simulation->battlefield, simulation->car);

  g_timeout_add (1000/60,(GSourceFunc)update_simulation, simulation);

  gtk_main();

  return 0;
}
