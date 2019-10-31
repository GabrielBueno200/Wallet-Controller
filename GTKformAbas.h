#ifndef _GTKFORMABAS_H_
#define _GTKFORMABAS_

struct dadosCad{
    GtkWidget*  janela;
    GtkWidget* campoNome;
    GtkWidget* campoCPF;
    GtkWidget* campoSaldo;
    GtkWidget* campoSenha;
    GtkTextBuffer* buffer;
};

struct dadosLogin{
    GtkWidget*  janela;
    GtkWidget*  campoCPF;
    GtkWidget*  campoSenha;
};

void adicionaAbaLogin(GtkWidget* windowForm, 
                      GtkWidget* formAbas, 
                      char* titulo, GtkWidget* ntAba);

void adicionaAbaCadastro(GtkWidget* windowForm, 
                         GtkWidget* formAbas, 
                         char* titulo, GtkWidget* ntAba);

#endif