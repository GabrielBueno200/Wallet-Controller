#ifndef FUNCSCADASTRO_H
#define FUNCSCADASTRO_H
#define MAX 100

struct Pessoa{
    char Nome[MAX];
    char CPF[MAX]; 
    char Senha[MAX];
    double Saldo;
};

int criaPastaUsuario(char* CPF);
void armazenaCategoria(GtkTextBuffer* buffer, char* CPF);
void armazenaPessoa(struct Pessoa pessoa, GtkWidget* windowForm);
void definePessoa(GtkWidget *btn, struct dadosCad* dados);

#endif /* FUNCSCADASTRO_H */

