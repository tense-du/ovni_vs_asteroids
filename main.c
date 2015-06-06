#include <gtk/gtk.h>
void key_released (GtkWidget *widget, GdkEvent *event, gpointer data)
{
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Up) {
      g_print("UP has been released\n");
        }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Down) {
        g_print ("DOWN has been released\n");
          }
}

void key_pressed(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Up) {
    g_print("UP has been pressed\n");
  }
  if (((GdkEventKey*)event)->keyval == GDK_KEY_Down) {
    g_print ("DOWN has been pressed\n");
  }
}

int main( int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  GtkWidget *event_box;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkButton");
  gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  event_box = gtk_event_box_new();
  gtk_container_add (GTK_CONTAINER(window), event_box);

  darea = gtk_drawing_area_new();
  gtk_container_add( GTK_CONTAINER(event_box), darea );

  gtk_widget_show (event_box);
  gtk_widget_show (darea);

  gtk_widget_set_size_request (event_box, 230, 150);
  gtk_widget_set_size_request (darea, 230, 150);
  g_signal_connect (G_OBJECT (window), "key-press-event",
      G_CALLBACK(key_pressed), NULL);
  g_signal_connect (G_OBJECT (window), "key-release-event",
      G_CALLBACK(key_released), NULL);

  g_signal_connect(G_OBJECT(window), "destroy", 
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
