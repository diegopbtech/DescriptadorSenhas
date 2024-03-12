
//BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

//BIBLIOTECAS PARA CRIAÇÃO DE PROCESSOS
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//CÓDIGO FORNECIDO NA ATIVIDADE PROPOSTA PARA FAZER O ENCRIPTE DAS SENHAS

#define NUM_LETRAS 26
#define ASCII_A 65

//CÓDIGO FORNECIDO NA ATIVIDADE PROPOSTA PARA FAZER O CRIPTOGRAFAR DAS SENHAS
char* encrypt(const char* str, int tamanho) {
    char* str_result = (char*) malloc(sizeof(char)*tamanho);
    for (int i = 0; i < tamanho; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            int chave = str[i] - ASCII_A;
            str_result[i] = (str[i] - ASCII_A + chave) % NUM_LETRAS + ASCII_A;
        }
    }
    return str_result;
}

//FUNÇÃO CRIADA PARA DESCRIPTOGRAFAR A SENHA
//PARÂMETROS USADOS:
    // - string senha (PEGA A LINHA LIDA NO ARQUIVO)
    // - int n (RESPONSÁVEL POR PASSAR A POSIÇÃO DO NÚMERO DA LINHA DA SENHA DO ARQUIVO)
    // - int nDir (RESPONSÁVEL POR PASSAR O ENDEREÇO DO ARQUIVO A SER LIDO)
string descryptPass(string senha, int n, int nDir) {
  //VARIÁVEL USADA PARA ARMAZENAR CADA COMBINAÇÃO (OBS: SÃO ARMAZENADAS UMA POR VEZ E SEMPRE É SUBSTITUÍDA PELA PRÓXIMA COMBINAÇÃO CASO ESSA NÃO SEJA A REAL)
  char password[5];

  //STRING USADA PARA RECEBER A STRING CRIPTOGRAFADA E COMPARAR COM A DO ARQUIVO
  string pass;

  //ARRAY DE STRINGS COM TODOS OS ENDEREÇOS DOS ARQUIVOS DAS SENHAS RECUPERADAS
  string diretorioRecovery[10] = {
      "SenhasRecuperadas/0.txt", "SenhasRecuperadas/1.txt",
      "SenhasRecuperadas/2.txt", "SenhasRecuperadas/3.txt",
      "SenhasRecuperadas/4.txt", "SenhasRecuperadas/5.txt",
      "SenhasRecuperadas/6.txt", "SenhasRecuperadas/7.txt",
      "SenhasRecuperadas/8.txt", "SenhasRecuperadas/9.txt"};

  //SALVAR EM UM NOVO ARQUIVO USANDO O PARÂMETRO nDir, QUE É REFERENTE AO NÚMERO DO ARQUIVO QUE ESTÁ SENDO LIDO
  ofstream fileRecovery(diretorioRecovery[nDir], ios::out | ios::app);

  //4 LAÇOS DE REPETIÇÃO for ANINHADOS GERANDO TODAS AS POSSIBILIDADES DE SENHAS POSSÍVEIS
  for (char b = 65; b <= 90; b++) {
    password[0] = b;
    for (char c = 65; c <= 90; c++) {
      password[1] = c;
      for (char d = 65; d <= 90; d++) {
        password[2] = d;
        for (char e = 65; e <= 90; e++) {
          password[3] = e;
          password[4] = '\0';
          //A CADA COMBINAÇÃO POSSÍVEL, É ENVIADO PARA A FUNÇÃO encrypt E RETORNA A SENHA CRIPOGRAFADA PARA A STRING pass PARA FAZER A COMPARAÇÃO
          pass = encrypt(password, 4);
          
          //COMPARA A pass COM A senha QUE ESTÁ CONTIDA NO ARQUIVO
          if (pass == senha) {
            //INCREMENTA NO ARQUIVO
            fileRecovery << password << endl;
            //RETORNA A SENHA DESCOBERTA E ENCERRA O LAÇO
            return password;
          }
        }
      }
    }
  }
  fileRecovery.close();
  return "";
}

//FUNÇÃO PRINCIPAL

int main(int argc, char** argv){

    //ARRAY DE ifstream PARA LER CADA ARQUIVO
    ifstream files[10];
    //ARRAY DE STRING COM O ENDEREÇO DE CADA UM DOS ARQUIVOS
    string diretorio[10] = {"senhas/0.txt", "senhas/1.txt", "senhas/2.txt","senhas/3.txt","senhas/4.txt","senhas/5.txt","senhas/6.txt","senhas/7.txt","senhas/8.txt","senhas/9.txt"};

    //ARRAY DE PID_T PARA CRIAÇÃO DOS PROCESSOS
    pid_t recPass[10];

    //STRING USADA PARA PEGAR A LINHA CONTIDA NO ARQUIVO
    string linha;

    //ARRAY BIDIMENSIONAL PARA ARMAZENAR TODAS AS 10 POSSIBILIDADES CONTIDAS EM CADA UM DOS 10 ARQUIVOS, PARA QUE SEJA POSSÍVEL FAZER A EXIBIÇÃO.
    string passwordsRecovery[10][10];

    //VERIFICANDO SE OS ARQUIVOS FORAM ABERTOS
    for(int f=0;f<10;f++){
        files[f].open(diretorio[f]);
        if (!files[f].is_open()) {
            cout << "Erro ao abrir o arquivo " << diretorio[f] << endl;
        }
    }

    //CRIAÇÃO DOS PROCESSOS
    //SÃO 10 PROCESSOS, CADA UM DIRECIONADO A UM ARQUIVO.
    for(int a=0;a<10;a++){
        //CRIANDO UMA CÓPIA DO PROCESSO
        recPass[a] = fork();

        //VERIFICAR SE HOUVE SUCESSO NA CRIAÇÃO DO PROCESSO
        if(recPass[a] == 0){
            //VARIÁVEL DE CONTROLE DA LINHA LIDA
            int num = 1;
            //LENDO CADA UMA DAS LINHAS
            while(getline(files[a], linha)){
                string caracteres = "";
                for(int i = 0; i < 4; ++i){
                    char c = linha[i];
                    caracteres += linha[i];
                }
                //ADICIONANDO AO ARRAY
                passwordsRecovery[a][num-1] = descryptPass(caracteres, num, a);
                num++;
            }
            //EXIBIÇÃO DAS SENHAS QUEBRADAS DE TODOS OS ARQUIVOS
            cout << "\n===================================================================\n";
            cout << "Senhas de recuperação do arquivo " << a << endl;
            for(int s=0;s<10;s++){
                cout << "Senha " << s+1 << ":" << passwordsRecovery[a][s] << endl;
            }
            exit(0);
        }
    }

    //USO DA FUNÇÃO wait(NULL) PARA GARANTIR QUE O PROCESSO PAI IRÁ AGUARDAR POR TODOS OS PROCESSOS FILHOS
    for(int w=0;w<10;w++){
        wait(NULL);
    }

    //FECHANDO OS ARQUIVOS
    for (int c = 0; c < 10; c++) {
        files[c].close();
    }

    return 0;
}