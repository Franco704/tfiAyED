#include "tfi.h"
#include <gtk/gtk.h>

UserSystem userSystem;

static void on_activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Sistema de Usuarios");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *button = gtk_button_new_with_label("Crear SuperUsuario");
    g_signal_connect(button, "clicked", G_CALLBACK([](GtkWidget *widget, gpointer data) {
        userSystem.set_SU();
    }), NULL);

    gtk_container_add(GTK_CONTAINER(window), button);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}