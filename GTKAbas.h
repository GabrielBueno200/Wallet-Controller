#ifndef GTKABAS_H
#define GTKABAS_H

struct camposUser{
  GtkTextBuffer* buffer;
  GtkTextIter iter;
  GtkWidget* label_Transacoes;
  GtkWidget* campoDialog;
};

struct camposTransacao{
    GtkWidget* campoDia; 
    GtkWidget* campoMes; 
    GtkWidget* campoAno; 
    GtkWidget* campoMinuto; 
    GtkWidget* campoHora;
    GtkWidget* campoGasto;
    GtkWidget* tipo;
};


struct Transacao{
    int Data[3];
    int Horario[2];
    double valor;
    struct CategoriasUsuario* Categoria; 
};


void categoriaChange(GtkComboBox *comboboxConta, struct camposUser* dados);
GtkWidget *AdicionaAbaSobreNos (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);
GtkWidget *AdicionaAbaCategorias(GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);
GtkWidget *AdicionaAbaTransacoes(GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);
GtkWidget *AdicionaAbaUsuario(GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);



#endif /* GTKABAS_H */

