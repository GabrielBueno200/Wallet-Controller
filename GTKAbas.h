#ifndef GTKABAS_H
#define GTKABAS_H
#define MAX 100

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
    GtkWidget* campoDesc;
    GtkWidget* tipo;
};


struct Transacao{
    int Data[3];
    int Horario[2];
    double valor;
    char descricao[MAX*2];
    struct CategoriasUsuario* Categoria; 
};


GtkWidget *AdicionaAbaSobreNos (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);
GtkWidget *AdicionaAbaCategorias (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);
GtkWidget *AdicionaAbaTransacoes (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);
void mudaCategoria(GtkComboBox *comboboxConta, struct camposUser* dados);
GtkWidget *AdicionaAbaUsuario (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba);



#endif /* GTKABAS_H */

