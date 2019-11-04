#ifndef FUNCSLOGIN_H
#define FUNCSLOGIN_H
#define MAX 100

struct usuarioLogado{
    int logged_in;
    char Nome[MAX];
    char CPF[MAX]; 
    char Senha[MAX];
    double Saldo;
    struct CategoriasUsuario* Categoria;
};

struct CategoriasUsuario{
    char Nome[MAX];
};

void removeQuebra(char* str);
void defineUsuarioLogado(char* postCpf, char* fileNomeUsuario, double fileSaldo);
int buscaPorCPF(GtkWidget* windowForm, FILE** arquivoUsuario, char* enderecoArquivo, char* postCpf);
void validaLogin(GtkWidget *btn, struct dadosLogin* dados);


#endif /* FUNCSLOGIN_H */

