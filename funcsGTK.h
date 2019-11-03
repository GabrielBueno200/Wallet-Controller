#ifndef FUNCSGTK_H
#define FUNCSGTK_H

void criaLabel(GtkWidget** label, gchar* texto, gchar* classe, gint x, gint y, GtkWidget *layout);
void criaCampo(GtkWidget**campo, gint nCarac, gint larguraCarac, gchar* placeholder, 
               gint x, gint y, gboolean edit, gboolean haveText, gchar* texto, GtkWidget*layout);

void criaCombobox(GtkWidget** combobox, GtkWidget* layout, gint x, gint y);
void criaBtn(GtkWidget** btn, gchar* texto, gint x, gint y, GtkWidget* layout);
void criaTextArea(GtkWidget** textarea, GtkTextBuffer ** buffer, 
        gint largura, gint altura, gint x, gint y,
        gchar* classe, gboolean edit, GtkWidget* layout);
void insereTextoTextarea(GtkTextBuffer** buffer, GtkTextIter* iter, gchar* texto);
int radioBtnAtivo(GtkWidget* radioBtn);
void mensagemAviso(GtkWidget* windowForm, gchar* mensagem);
void limpaBuffer(GtkTextBuffer* buffer);
void importaCss(GtkCssProvider **cssProvider);

#endif /* FUNCSGTK_H */

