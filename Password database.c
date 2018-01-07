#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Definição de uma estrutura composta por duas informações: 1) Identificação para uma senha (exemplo: Banco, Wi-Fi, etc); 2) Senha, ambos de tamanho máximo de 32 caracteres
typedef struct database{
	char label[32];
	char password[32];
} pwdatabase;

pwdatabase passwords;

//Cria um ponteiro para arquivos
FILE *pointer; 

//CRIAR UM NOVO REGISTRO DE SENHA
int createpw(){
	int offset = 0;
	char label[32];
	char password[32];
		
	printf("\n----------------------------------------------------------------\n");
    printf("\n                     NOVO REGISTRO DE SENHA\n\nDigite uma identificacao para a senha que voce deseja armazenar(max. 32 caracteres):\n\nNova identificacao: ");
    scanf("%s", label);

    offset = findpos(label, 1); //Busca por duplicidade de registros no banco de dados. A função retorna -1, em caso positivo, e o seu deslocamento no arquivo/offset, em caso negativo;
    if(offset == -1){
    	printf("\nA identificacao digitada ja existe no banco de dados.\n");
    	printf("\n----------------------------------------------------------------\n\n");
    	return -1;
	}

	strncpy(passwords.label, label, 32);	
    printf("\nDigite a senha que voce deseja armazenar(max. 32 caracteres):\n\nNova senha: ");
    scanf("%s", passwords.password);
	
	pointer = fopen("database.dat", "r+"); //Abre arquivo do banco de dados para iniciar a sua leitura/escrita
    fseek(pointer, 0, SEEK_END); //Posiciona o ponteiro no final do arquivo do banco de dados
    fwrite(&passwords, 1, sizeof(passwords), pointer); //Escreve os dados coletados no banco de dados
    
    fclose(pointer); //Fecha o arquivo do banco de dados
    printf("\nRegistro criado com sucesso!\n\nSua senha para %s e: %s\n", passwords.label, passwords.password);
    printf("\n----------------------------------------------------------------\n\n");
    return 0;
}

//VERIFICAR UMA SENHA EXISTENTE
int readpw(){
	int offset;
	int option;
	char label[32];
	
	pointer = fopen("database.dat", "r+"); //Abre o arquivo do banco de dados
		
	while(1){
		offset = 0;
		printf("\n----------------------------------------------------------------\n");
		printf("\n                      VERIFICACAO DE SENHAS\n\nSelecione a opcao desejada:\n\n1) Mostrar lista de todas as identificacoes registradas\n2) Verificar a senha vinculada a uma identificacao\n0) Retornar ao menu principal\n\nOpcao selecionada: ");
		scanf("%d", &option);
		printf("\n");

		switch(option){
			case 1:
				showlist(); //Mostra todas as identificações de todos os registros disponíveis no banco de dados
				break;
			case 2:
				printf("Digite a identificacao da senha que voce deseja verificar:\n\nIdentificacao selecionada: ");
    			scanf("%s", label);
	   		 	offset = findpos(label, 2); //Busca pela posição da identificação da senha desejada
    			if(offset == -1){
    				printf("\nIdentificacao nao encontrada no banco de dados.\n");
    				break;
				}
   		 		fseek(pointer, offset, SEEK_SET); //Posiciona o ponteiro do banco de dados na posição referente à identificação pesquisada
    			fread(&passwords, 1 , sizeof(passwords), pointer); //Realiza a leitura dos dados contidos entre a posição atual e a posição deslocada por sizeof(passwords)/64 caracteres do ponteiro do arquivo aberto
				printf("\nSua senha para %s e: %s\n", passwords.label, passwords.password);
				fclose(pointer); //Fecha o arquivo do banco de dados
				printf("\n----------------------------------------------------------------\n\n");
				return 0;
			case 0:
				fclose(pointer); //Fecha o arquivo do banco de dados
				printf("\n----------------------------------------------------------------\n\n");
				return -1;
			default:
				printf("\nOpcao invalida.\n");
				break;
		}
	}
}

//ATUALIZAR UMA SENHA EXISTENTE
int updatepw(){
	int offset;
	int option;
	char label[32];
	char password[32];
	char confirm[3];
	
	while(1){
		offset = 0;
		printf("\n----------------------------------------------------------------\n");
		printf("\n                        ATUALIZACAO DE SENHA\n\nSelecione uma das opcoes abaixo:\n\n1) Mostrar lista de todas as identificacoes registradas\n2) Modificar a senha vinculada a uma identificacao\n0) Retornar ao menu principal\n\nOpcao selecionada: ");
		scanf("%d", &option);
		printf("\n");
	
		switch(option){
			case 1:
				showlist(); //Mostra todas as identificações de todos os registros disponíveis no banco de dados
				break;
			case 2:
				printf("Digite a identificacao da senha que voce deseja atualizar:\n\nIdentificacao selecionada: ");
    			scanf("%s" , label);
    			offset = findpos(label, 2); //Busca pela posição da identificação da senha desejada
    			if(offset == -1){
    				printf("\nIdentificacao nao encontrada no banco de dados.\n");
    				break;
				}
				pointer = fopen("database.dat", "r+"); //Abre o arquivo do banco de dados
    			fseek(pointer, offset, SEEK_SET); //Posiciona o ponteiro do banco de dados na posição referente à identificação pesquisada
    			fread(&passwords, 1 , sizeof(passwords), pointer); //Realiza a leitura dos dados contidos entre a posição atual e a posição deslocada por sizeof(passwords)/64 caracteres do ponteiro do arquivo aberto
    			fclose(pointer); //Fecha o arquivo do banco de dados
				while(1){
    				printf("\nA sua senha atual para %s e: %s\n\nDigite SIM para alterar a sua senha ou NAO para cancelar a operacao.\n\nOpcao selecionada: ", passwords.label, passwords.password);
    				scanf("%s", confirm);
					if(strcmp(confirm, "SIM") == 0){
						moveandremove(offset); //Remove o registro lido na posição offset do banco de dados
						pointer = fopen("database.dat", "r+"); //Abre o arquivo do banco de dados
						strncpy(passwords.label, label, 32); //Copia 32 caracteres da variável label para a variável label da estrutura passwords
    					printf("\nDigite a nova senha(max. 32 caracteres):\n\nNova senha: ");
    					scanf("%s", passwords.password);
    					fseek(pointer, 0, SEEK_END); //Posiciona o ponteiro no final do arquivo do banco de dados
    					fwrite(&passwords, 1 , sizeof(passwords), pointer); //Escreve os dados da estrutura passwords no arquivo do banco de dados
    					printf("\nSua nova senha para %s e: %s\n", passwords.label, passwords.password);
    					fclose(pointer); //Fecha o arquivo do banco de dados
    					printf("\nRegistro modificado com sucesso!\n");
    					printf("\n----------------------------------------------------------------\n\n");
    					return 0;
					}
					else{
						if(strcmp(confirm, "NAO") == 0){
							fclose(pointer); //Fecha o arquivo do banco de dados
							printf("\n----------------------------------------------------------------\n\n");
							return -1;
						}
						else{
							printf("\nOpcao invalida.\n");
						}
					}
				}
				break;
			case 0:
				fclose(pointer);
				printf("\n----------------------------------------------------------------\n\n");
				return -1;
			default:
				printf("\nOpcao invalida.\n");
				break;
		}
	}
}

//DELETAR UM REGISTRO EXISTENTE
int deletepw(){
	int offset;
	int option;
	char label[32];
		
	while(1){
		offset = 0;
		printf("\n----------------------------------------------------------------\n");
		printf("\n                      REMOCAO DE REGISTRO\n\nSelecione uma das opcoes abaixo:\n\n1) Mostrar lista de todas as identificacoes\n2) Excluir um registro de senha do banco de dados\n0) Retornar ao menu principal\n\nOpcao selecionada: ");
		scanf("%d", &option);
		printf("\n");

		switch(option){
			case 1:
				showlist();
				break;
			case 2:
				printf("Digite a identificacao do registro que voce deseja excluir do banco de dados:\n\n");
    			scanf("%s", label);
	   		 	offset = findpos(label, 2); //Busca pela posição da identificação da senha desejada
    			if(offset == -1){
    				printf("\nIdentificacao nao encontrada no banco de dados.\n");
    				break;
				}
				moveandremove(offset); //Remove o registro lido na posição offset do banco de dados
				printf("\nRegistro deletado com sucesso!\n");
				printf("\n----------------------------------------------------------------\n\n");
				return 0;
			case 0:
				fclose(pointer); //Abre o arquivo do banco de dados
				printf("\n----------------------------------------------------------------\n\n");
				return -1;
			default:
				printf("\nOpcao invalida.\n");
				break;
		}
	}
}

/*Busca a posição da string label no arquivo database.dat
Option = 1: se label existir no arquivo database.dat, retorna -1; caso contrário retorna a posição do final do arquivo
Option = 2: se label não existir no arquivo database.dat, retorna -1; caso contrário retorna a posição inicial da string procurada*/
int findpos(char label[32], int option){
	int offset = 0;
	
	pointer = fopen("database.dat", "r+"); //Abre o arquivo do banco de dados
	
	switch(option){
		case 1:
			while(1){ //Varredura do arquivo em busca da string label, iniciando na posição 0, até a posição do arquivo (ou a sua posição final, caso a string não seja encontrada)
    			fseek(pointer, offset, SEEK_SET); //Posiciona o ponteiro do arquivo na posição offset a partir do início do arquivo (SEEK_SET)
    			fread(&passwords, 1, sizeof(passwords), pointer); //Lê a posição atual do ponteiro do arquivo
    			if(strcmp(passwords.label, label) == 0){ //Compara a string label com a string contida na posição atual do arquivo
    				fclose(pointer); //Fecha o arquivo do banco de dados
    				return -1;
				}
				else{
					if(feof(pointer) == 0){ //EOF = End of File; caso o ponteiro atinja o final do arquivo, feof(pointer) = 0
						offset += sizeof(passwords); //Incrementa o offset do ponteiro do arquivo em 64 bytes/caracteres
					}
					else{
						fclose(pointer); //Fecha o arquivo do banco de dados
						return offset;
					}
				}
			}
		break;
		case 2:
			while(1){ //Varredura do arquivo em busca da string label, iniciando na posição 0, até a posição do arquivo (ou a sua posição final, caso a string não seja encontrada)
    			fseek(pointer, offset, SEEK_SET); //Posiciona o ponteiro do arquivo na posição offset a partir do início do arquivo (SEEK_SET)
    			fread(&passwords, 1, sizeof(passwords), pointer); //Lê a posição atual do ponteiro do arquivo
    			if(strcmp(passwords.label, label) == 0){ //Compara a string label com a string contida na posição atual do arquivo
    				fclose(pointer); //Fecha o arquivo do banco de dados
    				return offset;
				}
				else{
					if(feof(pointer) == 0){ //EOF = End of File; caso o ponteiro atinja o final do arquivo, feof(pointer) = 0
						offset += sizeof(passwords);
					}
					else{
						fclose(pointer); //Fecha o arquivo do banco de dados
						return -1;
					}
				}
			}
		break;
	}	
}

//Remove o arquivo contido na posição offset do banco de dados
int moveandremove(int offset){
	int length;
	
	pointer = fopen("database.dat", "r+"); //Abre o arquivo do banco de dados - database.dat
	fseek (pointer, 0, SEEK_END); //Posiciona o ponteiro do arquivo no final do arquivo para medir o seu tamanho total
  	length = ftell(pointer); //Armazena a quantidade de caracteres do arquivo, desde a sua posição inicial até a posição atual do ponteiro
	fseek(pointer, 0, SEEK_SET); //Posiciona o ponteiro do arquivo na posição 0 a partir do início do arquivo (SEEK_SET)
	fread(&passwords, 1 , offset, pointer); //Lê os caracteres contidos entre a posição atual do ponteiro (que se encontra no início do arquivo) e o deslocamento offset
	fclose(pointer); //Fecha o arquivo database.dat
	
	pointer = fopen("buffer.dat", "w"); //Limpa o arquivo buffer.dat, que será utilizado para armazenar os segmentos do banco de dados que desejamos manter
	fclose(pointer); //Fecha o arquivo buffer.dat
	
	pointer = fopen("buffer.dat", "r+"); //Reabre o arquivo buffer.dat para leitura/escrita
	fseek(pointer, 0, SEEK_SET); //Posiciona o ponteiro do arquivo na posição offset a partir do início do arquivo (SEEK_SET)
	fwrite(&passwords, 1, offset, pointer); //Escreve os dados lidos em database.dat
	fclose(pointer); //Fecha o arquivo buffer.dat
	
	pointer = fopen("database.dat", "r+"); //Abre o arquivo database.dat
	offset += sizeof(passwords); //Desloca o ponteiro do arquivo em 64 bytes/caracteres, de maneira a copiar apenas os dados relevantes
	fseek(pointer, offset, SEEK_SET);
	fread(&passwords, 1 , length - offset, pointer); //Lê os caracteres contidos entre a posição atual do ponteiro (que se encontra na posição offset) e o final do arquivo
	fclose(pointer); //Fecha o arquivo database.dat
	
	pointer = fopen("buffer.dat", "r+"); //Reabre o arquivo buffer.dat para leitura/escrita
	fseek(pointer, 0, SEEK_END); //Posiciona o ponteiro do arquivo na posição 0 a partir do início do arquivo (SEEK_END)
	fwrite(&passwords, 1, length - offset, pointer); //Escreve os dados lidos em database.dat
	fclose(pointer); //Fecha o arquivo buffer.dat
	
	pointer = fopen("database.dat", "w"); //Limpa o arquivo database.dat
	fclose(pointer); //Fecha o arquivo database.dat
	
	//Copia todos os dados do arquivo buffer.dat
	pointer = fopen("buffer.dat", "r+");
	fseek(pointer, 0, SEEK_END);
	length = ftell(pointer);
	fseek(pointer, 0, SEEK_SET);
	fread(&passwords, 1, length, pointer);
	fclose(pointer);
	
	//Escreve os dados lidos em buffer.dat no banco de dados
	pointer = fopen("database.dat", "r+");
	fwrite(&passwords, 1, length, pointer);
	fclose(pointer);
}

//Mostra uma lista contendo todas as identifiçacões dos registros contidos no banco de dados database.dat
int showlist(){
	int offset = 0;
	int counter = 1;
	
	pointer = fopen("database.dat", "r+"); //Abre o arquivo do banco de dados
	
	while(1){
		fseek(pointer, offset, SEEK_SET);
		fread(&passwords, 1 , sizeof(passwords), pointer);
		if(feof(pointer) == 0){
			if(counter == 1){
				printf("Lista de identificacoes:\n");
			}
			printf("%d - %s\n", counter, passwords.label);
			offset += sizeof(passwords);
			counter++;
		}
		else{
			if(counter == 1){
				printf("Nao ha registros no banco de dados.\n");
			}
			break;
		}
	}
	fclose(pointer);
	counter = 0;
}

int main(){
	int option;

//CRIA ARQUIVO DO BANCO DE DADOS, CASO AINDA NÃO EXISTA	
	pointer = fopen("database.dat", "r");
    if(pointer == NULL){
    	pointer = fopen("database.dat", "w");
	}
	fclose(pointer);
//CRIA ARQUIVO BUFFER, CASO AINDA NÃO EXISTA	
	pointer = fopen("buffer.dat", "r");
    if(pointer == NULL){
    	pointer = fopen("buffer.dat", "w");
	}
	fclose(pointer);
		
	//MENU PRINCIPAL
	while(1){
		printf("                         MENU PRINCIPAL\n\nInsira o codigo da funcao desejada:\n\n1) Criar novo registro de senha\n2) Verificar um registro de senha criado\n3) Atualizar uma senha registrada\n4) Excluir um registro de senha existente\n0) Sair do programa\n\nOpcao selecionada: ");
		scanf("%d", &option);
		switch(option){
			case 1:
				createpw();
				break;
			case 2:
				readpw();
				break;
			case 3:
				updatepw();
				break;
			case 4:
				deletepw();
				break;
			case 0:
				fclose(pointer);
				exit(0);
			default:
				printf("\nOpcao invalida.\n");
				break;
		}
	}
}

