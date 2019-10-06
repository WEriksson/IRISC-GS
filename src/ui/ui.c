/* -----------------------------------------------------------------------------
 * Component Name: ui
 * Author(s): William Eriksson
 * Parent Component: [if applicable]
 * Purpose: 
 *
 * -----------------------------------------------------------------------------
 */
#include <gtk/gtk.h>
#include <string.h>
#include <sys/mman.h>

#include "global_utils.h"
#include "ui.h"



GtkWidget   *window;
GtkWidget   *fixed1;
GtkWidget   *scroll_tc;
GtkWidget   *view_tc;
GtkWidget   *logg_tc;
GtkWidget   *scroll_tm;
GtkWidget   *view_tm;
GtkWidget   *logg_tm;
GtkWidget   *input_tc;
GtkWidget   *tc_button;
GtkWidget   *timer;

int	countDownMax, countCurrent;
int	currentRow;

GtkTextBuffer	*text_buffer_tc;
GtkTextBuffer	*text_buffer_tm;

GtkBuilder *builder;

gboolean timer_handler(GtkWidget *);

pthread_mutex_t downlink_mutex;

int init_ui(int argc, char *argv[]){

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file ("/home/zodox/Desktop/IRISC-GS/gtk_parts/part1.glade");
 
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title (GTK_WINDOW (window), "IRISC - Ground Station");

    gtk_builder_connect_signals(builder, NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    scroll_tc = GTK_WIDGET(gtk_builder_get_object(builder, "scroll_tc"));
	view_tc = GTK_WIDGET(gtk_builder_get_object(builder, "view_tc"));
    logg_tc = GTK_WIDGET(gtk_builder_get_object(builder, "logg_tc"));
    scroll_tm = GTK_WIDGET(gtk_builder_get_object(builder, "scroll_tm"));
	view_tm = GTK_WIDGET(gtk_builder_get_object(builder, "view_tm"));
    logg_tm = GTK_WIDGET(gtk_builder_get_object(builder, "logg_tm"));
    input_tc = GTK_WIDGET(gtk_builder_get_object(builder, "input_tc"));
    tc_button = GTK_WIDGET(gtk_builder_get_object(builder, "tc_button"));
    timer = GTK_WIDGET(gtk_builder_get_object(builder, "timer"));


    text_buffer_tc = gtk_text_view_get_buffer (GTK_TEXT_VIEW(logg_tc));
    text_buffer_tm = gtk_text_view_get_buffer (GTK_TEXT_VIEW(logg_tm));

    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(logg_tc), 0);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(logg_tm), 0);
    
    tm_buffer = mmap(NULL, 512, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0); // shared memory

    tm_update_flag = mmap(NULL, 32, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0); // shared memory

	*tm_update_flag = 0;

    //	The following function returns an ID of the event that can be
//	used to destroy it. Not used here.

	guint ID = g_timeout_add_seconds(1, (GSourceFunc) timer_handler, timer);

//	Time units smaller than seconds are possible (higher overhead):
//	guint ID = g_timeout_add(250, (GSourceFunc) timer_handler, data_pointer);

	countCurrent = countDownMax = 0;

    gtk_widget_show(window);

    gtk_main();

    return SUCCESS;
}

void on_tc_button_clicked (GtkButton *b) {

    char temp[9];

    get_time(temp);

    gchar *buffer;
    GtkTextIter begin, end;
    buffer = gtk_editable_get_chars(GTK_EDITABLE(input_tc), 0, -1);

    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer_tc), &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer_tc), &end, (gint) -1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tc), &begin, "\n", -1);

    gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tc), &end, 0, -1);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tc), &end, temp, -1);

    gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tc), &end, 0, -1);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tc), &end, ": ", -1);
    
    gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tc), &end, 0, -1);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tc), &end, buffer, -1);
}

int print_telemetry(char buffer[512]){

    GtkTextIter begin, end;

    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer_tm), &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer_tm), &end, (gint) -1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tm), &begin, buffer, -1);


    //gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tm), &end, 0, -1);
    //gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tm), &end, temp, -1);
    
    //gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tm), &end, 0, -1);
    //gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tm), &end, ": ", -1);

    //gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer_tm), &begin, (gint) 0);
    //gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tm), &begin, 0, 100);
    //gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tm), &begin, ": ", -1);

    //gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(text_buffer_tm), &begin, (gint) 0);
    //gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(text_buffer_tm), &begin, 0, 100);
    //gtk_text_buffer_insert(GTK_TEXT_BUFFER(text_buffer_tm), &begin, buffer, -1);

    

    return SUCCESS;
}

gboolean timer_handler(GtkWidget *timer) {

    printf("Timer Begin\n");

    time_t t = time(0);
    gtk_label_set_text(GTK_LABEL(timer), ctime(&t));    // update time of day

    if(tm_update_flag){
        pthread_mutex_lock(&downlink_mutex);
        print_telemetry(tm_buffer);
        strcpy(tm_buffer, "");
        *tm_update_flag=0;
        pthread_mutex_unlock(&downlink_mutex);
    }

    printf("Timer End\n");

    return TRUE; // FALSE kills the timer
}