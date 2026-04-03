#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "Liste.h"
#include "lecture_fichier.h"
#include "simulation_gui.h"
#include "csv_export.h"
#include "ordonnancement.h"

typedef struct {
    GtkWidget *combo_algo;
    GtkWidget *spin_quantum;
    GtkWidget *treeview;
    GtkWidget *label_stats;
    GtkWidget *label_fichier;

    char fichier_charge[512];

    SimulationResultat *resultat;
} WidgetsApp;

/**
 * Supprime toutes les données du tableau GTK
 * @param treeview : tableau à vider
 */
static void vider_treeview(GtkTreeView *treeview) {
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    if (model) {
        g_object_unref(model);
    }

    GList *colonnes = gtk_tree_view_get_columns(treeview);
    for (GList *l = colonnes; l != NULL; l = l->next) {
        gtk_tree_view_remove_column(treeview, GTK_TREE_VIEW_COLUMN(l->data));
    }
    g_list_free(colonnes);
}

/**
 * Affiche les résultats dans le tableau GTK
 * et met à jour les statistiques en bas
 * @param app : structure contenant les widgets
 */
static void afficher_resultat_dans_tableau(WidgetsApp *app) {
    if (!app->resultat) return;

    vider_treeview(GTK_TREE_VIEW(app->treeview));

    int nb_colonnes = app->resultat->nb_colonnes;

    GType *types = malloc(sizeof(GType) * nb_colonnes);
    for (int i = 0; i < nb_colonnes; i++) {
        types[i] = G_TYPE_STRING;
    }

    GtkListStore *store = gtk_list_store_newv(nb_colonnes, types);
    free(types);

    for (int i = 0; i < app->resultat->nb_lignes; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);

        for (int j = 0; j < nb_colonnes; j++) {
            gtk_list_store_set(store, &iter, j, app->resultat->cellules[i][j], -1);
        }
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(app->treeview), GTK_TREE_MODEL(store));

    for (int j = 0; j < nb_colonnes; j++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
            app->resultat->entetes[j], renderer, "text", j, NULL
        );
        gtk_tree_view_append_column(GTK_TREE_VIEW(app->treeview), col);
    }

    char stats[2048];
    char ligne1[256] = "";
    char ligne2[256] = "";
    char ligne3[256] = "";

    strcat(ligne1, "Restitution : ");
    strcat(ligne2, "Reponse : ");
    strcat(ligne3, "Attente : ");

    for (int i = 0; i < app->resultat->nb_processus; i++) {
        char tmp[128];

        snprintf(tmp, sizeof(tmp), "P%d=%d  ", app->resultat->pid[i], app->resultat->restitution[i]);
        strcat(ligne1, tmp);

        snprintf(tmp, sizeof(tmp), "P%d=%d  ", app->resultat->pid[i], app->resultat->reponse[i]);
        strcat(ligne2, tmp);

        snprintf(tmp, sizeof(tmp), "P%d=%d  ", app->resultat->pid[i], app->resultat->attente[i]);
        strcat(ligne3, tmp);
    }

    snprintf(stats, sizeof(stats),
             "%s\n%s\n%s\nRestitution moyenne : %.2f\nReponse moyenne : %.2f\nAttente moyenne : %.2f\nTaux d'occupation CPU : %.2f %%",
             ligne1,
             ligne2,
             ligne3,
             app->resultat->restitution_moyenne,
             app->resultat->reponse_moyenne,
             app->resultat->attente_moyenne,
             app->resultat->occupation_cpu);

    gtk_label_set_text(GTK_LABEL(app->label_stats), stats);
}

/**
 * Callback bouton "Charger un fichier"
 * Ouvre un explorateur et sauvegarde le chemin
 */
static void on_charger_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    WidgetsApp *app = (WidgetsApp *)user_data;

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Choisir un fichier",
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Annuler", GTK_RESPONSE_CANCEL,
        "_Ouvrir", GTK_RESPONSE_ACCEPT,
        NULL
    );

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *nom_fichier = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        strncpy(app->fichier_charge, nom_fichier, sizeof(app->fichier_charge) - 1);
        app->fichier_charge[sizeof(app->fichier_charge) - 1] = '\0';

        gtk_label_set_text(GTK_LABEL(app->label_fichier), app->fichier_charge);
        g_free(nom_fichier);
    }

    gtk_widget_destroy(dialog);
}

/**
 * Callback bouton "Lancer la simulation"
 * - lit le fichier
 * - choisit l'algorithme
 * - lance la simulation
 * - affiche le résultat
 */
static void on_lancer_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    WidgetsApp *app = (WidgetsApp *)user_data;

    int indice_algo = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_algo));
    int quantum = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->spin_quantum));

    if (app->resultat) {
        liberer_resultat(app->resultat);
        app->resultat = NULL;
    }

    if (strlen(app->fichier_charge) == 0) {
        gtk_label_set_text(GTK_LABEL(app->label_stats), "Charge d'abord un fichier d'entree.");
        return;
    }

    Liste liste = lire_processus(app->fichier_charge);
    if (!liste) {
        gtk_label_set_text(GTK_LABEL(app->label_stats), "Impossible de lire le fichier.");
        return;
    }

    int algo;
    switch (indice_algo) {
        case 0: algo = ALGO_FIFO; break;
        case 1: algo = ALGO_SJF; break;
        case 2: algo = ALGO_SJRF; break;
        case 3: algo = ALGO_RR; break;
        default: algo = ALGO_SJF; break;
    }

    if (algo != ALGO_RR) {
        quantum = 1;
    }

    app->resultat = simuler_gui(&liste, algo, quantum);
    afficher_resultat_dans_tableau(app);
}

/**
 * Callback bouton "Exporter CSV"
 * Permet de sauvegarder les résultats dans un fichier
 */
static void on_exporter_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    WidgetsApp *app = (WidgetsApp *)user_data;

    if (!app->resultat) return;

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Exporter en CSV",
        NULL,
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Annuler", GTK_RESPONSE_CANCEL,
        "_Enregistrer", GTK_RESPONSE_ACCEPT,
        NULL
    );

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "resultat.csv");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *nom_fichier = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        exporter_csv_gui(nom_fichier, app->resultat);
        g_free(nom_fichier);
    }

    gtk_widget_destroy(dialog);
}

/**
 * Fonction principale de l'interface graphique GTK
 * @param argc arguments
 * @param argv arguments
 * @return code de sortie
 */
int lancer_interface(int argc, char **argv) {
    gtk_init(&argc, &argv);

    WidgetsApp *app = calloc(1, sizeof(WidgetsApp));
    if (!app) return 1;

    app->fichier_charge[0] = '\0';
    app->resultat = NULL;

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulateur d'ordonnancement");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 720);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

    GtkWidget *label_algo = gtk_label_new("Algorithme :");
    gtk_box_pack_start(GTK_BOX(hbox), label_algo, FALSE, FALSE, 5);

    app->combo_algo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->combo_algo), "FIFO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->combo_algo), "SJF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->combo_algo), "SJRF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->combo_algo), "RR");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_algo), 1);
    gtk_box_pack_start(GTK_BOX(hbox), app->combo_algo, FALSE, FALSE, 5);

    GtkWidget *label_quantum = gtk_label_new("Quantum :");
    gtk_box_pack_start(GTK_BOX(hbox), label_quantum, FALSE, FALSE, 5);

    app->spin_quantum = gtk_spin_button_new_with_range(1, 20, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->spin_quantum), 2);
    gtk_box_pack_start(GTK_BOX(hbox), app->spin_quantum, FALSE, FALSE, 5);

    GtkWidget *btn_charger = gtk_button_new_with_label("Charger un fichier");
    gtk_box_pack_start(GTK_BOX(hbox), btn_charger, FALSE, FALSE, 5);

    GtkWidget *btn_lancer = gtk_button_new_with_label("Lancer la simulation");
    gtk_box_pack_start(GTK_BOX(hbox), btn_lancer, FALSE, FALSE, 5);

    GtkWidget *btn_exporter = gtk_button_new_with_label("Exporter CSV");
    gtk_box_pack_start(GTK_BOX(hbox), btn_exporter, FALSE, FALSE, 5);

    app->label_fichier = gtk_label_new("Aucun fichier charge");
    gtk_label_set_xalign(GTK_LABEL(app->label_fichier), 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), app->label_fichier, FALSE, FALSE, 5);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 5);

    app->treeview = gtk_tree_view_new();
    gtk_container_add(GTK_CONTAINER(scroll), app->treeview);

    app->label_stats = gtk_label_new("Aucun resultat pour le moment.");
    gtk_label_set_xalign(GTK_LABEL(app->label_stats), 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), app->label_stats, FALSE, FALSE, 8);

    g_signal_connect(btn_charger, "clicked", G_CALLBACK(on_charger_clicked), app);
    g_signal_connect(btn_lancer, "clicked", G_CALLBACK(on_lancer_clicked), app);
    g_signal_connect(btn_exporter, "clicked", G_CALLBACK(on_exporter_clicked), app);

    gtk_widget_show_all(window);
    gtk_main();

    if (app->resultat) {
        liberer_resultat(app->resultat);
    }
    free(app);

    return 0;
}