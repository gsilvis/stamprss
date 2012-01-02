/* This file is part of rockfall, a simple Webkit-based rss reader */
/* rockfall is licensed under the GPL v3 */



#include <libgrss/libgrss.h>
#include <webkit/webkit.h>
#include <gtk/gtk.h>
#include <glib.h>

typedef struct {
  GtkWidget *win, *scroll, *view;
  FeedsPool *fp;
  time_t stamp;
  FeedItem *item;
  GAsyncQueue *items;
} Client;

void
feed_fetched (FeedsPool *pool,
              FeedChannel *feed,
              GList *items,
              gpointer user_data) {
  GList *iter;
  FeedItem *item;
  Client *c = (Client*) user_data;

  printf("Fetched a feed!\n");

  for (iter = items; iter; iter = g_list_next (iter)) {
    item = (FeedItem*) iter->data;
    if (feed_item_get_publish_time(item) >= c->stamp) {
      printf("timestamp %i\n", feed_item_get_publish_time(item));
      g_object_ref(item);
      if (c->item == NULL) {
        c->item = item;
        /* update the screen here! */
      } else {
        g_async_queue_push(c->items, item);
      }
    }
  }
}


gboolean
keypress_event (GtkWidget *widget,
                GdkEventKey *event,
                gpointer userdata) {
  /* for now, just do the same thing every time */
  Client *c = (Client*) userdata;
  if (c->item != NULL) {
    g_object_unref(c->item);
  }
  c->item = (FeedItem*) g_async_queue_try_pop(c->items);
  if (!c->item) {
    return true;
  }

  if (!feed_item_get_description(c->item)) {
    printf ("null description\n");
  }

  webkit_web_view_load_string(WEBKIT_WEB_VIEW(c->view),
                              feed_item_get_description(c->item),
                              "text/html", /* MIME type */
                              NULL, /* encoding (default chosen) */
                              ""); /* base URL (irrelevant here) */

  return TRUE;
}




int
main (int argc,
      char **argv) {

  gtk_init(&argc, &argv);
  /* Create the widgets */

  Client *c = malloc(sizeof(Client));
  c->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  c->scroll = gtk_scrolled_window_new(NULL, NULL);
  c->view = webkit_web_view_new();

  /* Place the WebKitWebView in the GtkScrolledWindow */
  gtk_container_add(GTK_CONTAINER(c->scroll), c->view);
  gtk_container_add(GTK_CONTAINER(c->win), c->scroll);

  g_signal_connect(G_OBJECT(c->win),
                   "delete_event",
                   G_CALLBACK(gtk_main_quit),
                   NULL);

  
  GError *error = NULL;
  FeedsGroup *fg = feeds_group_new();
  GList *feeds = feeds_group_parse_file(fg, "/home/george/Git/stamprss/feedlist.opml", error);

  if (!feeds) {
    printf("Error: %s\n", error->message);
  }

  printf ("The GList feeds has %i members\n", g_list_position(feeds, g_list_last(feeds)));

  /* c->stamp =    */
  c->items = g_async_queue_new();
  c->item = NULL;
  c->stamp = 1314472098;
  /* Fetch items */
  c->fp = feeds_pool_new();
  feeds_pool_listen(c->fp, feeds);
  g_signal_connect(c->fp, "feed-ready", G_CALLBACK(feed_fetched), c);
  feeds_pool_switch(c->fp, TRUE);

  
  /* Link spacebar, enter, and so forth to "next feed" */
  /* link q, etc. to "exit" */
  /* link Q, etc. to "exit don't save" */
  g_signal_connect(G_OBJECT(c->win), "key-press-event", G_CALLBACK(keypress_event), c);





                                   
  /* Show the result */
  gtk_window_set_default_size(GTK_WINDOW(c->win), 800, 600);
  gtk_widget_show_all(c->win);

  gtk_main();

  free(c);

  return 0;
}

