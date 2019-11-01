#ifndef _GTKFORMUSUARIO_H_
#define _GTKFORMUSUARIO_

void adicionaAbaLogin(GtkWidget* windowForm, GtkWidget* formAbas, char* titulo, GtkWidget* ntAba);
void adicionaAbaCadastro(GtkWidget* windowForm, GtkWidget* formAbas, char* titulo, GtkWidget* ntAba);
void criaAbas (GtkWidget* windowForm, GtkWidget* layoutLogin, GtkWidget* layoutCadastro);
void formFrame();


#endif