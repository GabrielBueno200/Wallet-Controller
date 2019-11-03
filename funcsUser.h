#ifndef FUNCSUSER_H
#define FUNCSUSER_H

void logout();
void redefineSaldo(gchar* novoDado);
void redefineNome(GtkDialog *caixaDialogo, gint response_id, GtkWidget* campoNovoNome);
void mudarNome(GtkWidget* btn);
void deletaConta(GtkDialog *janelaAviso, gint response_id);
void confirmaDelAcc(GtkWidget *btn, struct camposUser* dados);


#endif /* FUNCSUSER_H */

