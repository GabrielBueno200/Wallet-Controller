
#ifndef FUNCSCATEGORIAS_H
#define FUNCSCATEGORIAS_H

void limpaCombobox(GtkWidget* combobox);
void refazCombobox(GtkWidget* combobox);
void adicionaCategoriaCombobox(GtkWidget* combobox);
void deletaCategoriaCombobox(GtkWidget* combobox, int pos);
void adicionaCategoria(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados);
void addNomeCategoria(GtkWidget *btn, struct camposUser* dados);
void deletaCategoria(GtkDialog *janelaAviso, gint response_id, struct camposUser* dados);
void confirmaDelCategoria(GtkWidget *btn, struct camposUser* dados);
void setaNome(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados);
void renomearCategoria(GtkWidget *btn, struct camposUser* dados);

#endif /* FUNCSCATEGORIAS_H */

