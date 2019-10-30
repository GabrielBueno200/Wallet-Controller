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
void armazenaPessoa(struct Pessoa pessoa);
void definePessoa(GtkWidget *btn, struct dadosCad* dados);

#endif /* FUNCSCADASTRO_H */

