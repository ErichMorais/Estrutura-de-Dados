#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

/*
****FEITO****	-envia_msg(): permite a um participante enviar uma mensagem a um outro participante da rede,
	a qual deve ser colocada na sua fila de mensagens a serem enviadas;
	
****FEITO****	– consulta_msgs_a_enviar(): informa quantas mensagens existem na fila de msgs a enviar de um
	participante;
	
****FEITO****	– consulta_msgs_recebidas(): informa quantas mensagens existem na fila de msgs recebidas por
	um participante;
	
****FEITO - PARCIAL ****	– le_msg(): retira uma mensagem da fila de mensagens recebidas por um participante;
	
***FEITOOO*****	– inclui_participante( ): cria um novo nodo, cria para ele um identificador e o insere na rede;
	
***FEITO****	– exclui_participante(): retira o participante com a identificação id da rede;
	
***FEITOOO*****	– consulta_participantes( ): informa quais são os participantes da rede naquele momento,
				podendo fornecer informações sobre ele (por exemplo número de mensagens na fila, quantas
				mensagens enviou, etc);
	
*	– consulta_token( ): informa o status do token naquele momento (se possui msg ou não, caso
	possua pode informar origem e destino, conteúdo da msg, etc);
	
*	– ativa_rede( ): coloca a rede em operação, ou reinicia seu funcionamento, colocando o token em
	circulação. A partir da ativação da rede, o token circula automaticamente. A ativação inicial da
	rede pode ser feita de forma automática;
	
*	– desativa_rede( ): pára o funcionamento da rede, desativando a circulação do token.
	
*	– ...Outras operações, com outras funcionalidades para a rede.
*/

struct Nodo{
	int id;					//id do usuario
	char nome[40];			//nome do usuario
	struct Mensagem *rmsg;	//mensagens recebidas
	struct Mensagem *emsg;	//mensagens enviadas
	struct Nodo *prox;		//proximo uruario
	struct Nodo *ant;		//usuario anterior
};

struct Token{
	int idr;				//id do usuario que vai receber
	int ide;				//id do usuaria que enviou
	char msg[100];			//mensagem
	int pos;				//posição do token
};
	
struct Mensagem{
	char msg[100];			//mensagem
	int eid;				//id do usuario que enviou
	int rid;				//id do usuario que vai receber
	struct Mensagem *prox;	//proxima mensagem
	struct Mensagem *ant;	//mensagem anterior
};

typedef struct Nodo nodo;
typedef struct Mensagem msg;
typedef struct Token tok;

/* --------Funções---------- */

void insere_nodo(nodo **adm, int *quant);					//insere novo usuario
void menu(nodo **adm, int *quant, char opc, tok **token);				//menu principal
void escreve(nodo *adm);									//escreve lista de usuarios com informaçoes
nodo * enviar_msg(nodo *adm, int quant);					//enviar mensagem
nodo * procura_usuario(nodo *adm, int id);					//procurar usuario
void ler_msg(msg *msg, nodo *adm);							//escreve mensagems na lista
int conta_msg(msg *lmsg);									//conta numero de msg na lista
nodo * excluir(nodo *adm, int id);							//Exclue um Usuario
nodo * menu_token(nodo *adm, tok **token);						//Menu Toekn
int conta_total_rmsg(nodo *adm);							//Conta Total de MSGs Recebidas
int conta_total_emsg(nodo *adm);							//Conta Total de MSGs a serem Envidas
tok * pega_msg(tok *token, nodo **adm);						//Pega Mensagens para envio


int main()
{

	nodo *adm = NULL;
	tok *token = NULL;
	int quant = 0;
	int opc = 0;
	
	printf("\n---------------------------------------------------------------------------------\n");
	printf("0. Sair\n");						//SAI do sistema
	printf("1. Enviar Mensagem\n");				//Envia uma mensagem
	printf("2. Inserir Usuario\n");				//insere novo usuario no sistema
	printf("3. Escrever Lista de Usuarios\n");	//escreve lista de usuarios com dados
	printf("4. Mensagens para envio\n");		//escreve mensagems que estao para serem enviadas
	printf("5. Mensagens recebidas\n");			//escreve mensagem recebidas depois limpa a lista
	printf("6. Quantidade Mensagens\n");
	printf("7. Inicia Token\n");	
	printf("8. Menu Token\n");
	printf("9. Excluir Usuario\n");
	printf("10. Total de Mensagens a serem enviada na lista de usuarios\n");
	printf("11. Total de Mensagens recebista na lista de usuarios\n");
	printf("12. DESATIVAR REDE\n");
	fflush(stdin);
	scanf("%d",&opc);
	menu(&adm,&quant,opc,&token);
	
	system("pause");
}

void menu(nodo **adm, int *quant, char opc, tok **token)			//Menu principal
{
	system("cls");
	int tmp;
	nodo *tn = NULL;
	switch(opc)
	{
		case 0:		//-----sai do programa
			return;
		case 1:
			if((*adm) == NULL)
			{
				printf("	--- Lista de usuarios vazia ---\n\n");
				break;
			}
			system("cls");
			(*adm) = enviar_msg(*adm,*quant);
			break;
		case 2:		//-----insere novos usuarios
			system("cls");
			insere_nodo(adm,&(*quant));
			break;
		case 3:		//-----escreve lista de usuarios
			system("cls");
			escreve(*adm);
			break;
		case 4:		//-----Envia uma mensagem
			if((*adm) == NULL)
			{
				printf("	--- Lista de usuarios vazia ---\n\n");
				break;
			}
			system("cls");
			printf("\n ----- Mensagens para envio -----\n");
			printf("Selecione o Usuario\n");
			fflush(stdin);
			scanf("%d",&tmp);
			tn = procura_usuario(*adm,tmp);
			system("cls");
			if(tn == NULL)
				printf("Usuario informado não existe\n\n");
			else
			{
				printf("--- Mensagems de %s	ID: %d ---\n\n",tn->nome,tn->id);
				ler_msg(tn->emsg,*adm);
			}
			break;
		case 5:		//-----leitura de todas as mensagens recebidas
			if((*adm) == NULL)
			{
				printf("	--- Lista de usuarios vazia ---\n\n");
				break;
			}
			system("cls");
			printf("\n ----- Mensagens recebidas -----\n");
			printf("Selecione o Usuario\n");
			fflush(stdin);
			scanf("%d",&tmp);
			tn = procura_usuario(*adm,tmp);
			system("cls");
			if(tn == NULL)
				printf("Usuario informado não existe\n\n");
			else
			{
				if(tn->rmsg == NULL)
					printf("Usuario não possue mensagem a serem lidas!\n\n");
				else
				{
					printf("--- Mensagems de %s	ID: %d ---\n\n",tn->nome,tn->id);
					ler_msg(tn->rmsg,*adm);
					tn->rmsg = NULL;
				}
			}		
			break;
		case 6:		//-----informa quantidade de mensagems
			if((*adm) == NULL)
			{
				printf("	--- Lista de usuarios vazia ---\n\n");
				break;
			}
			system("cls");
			printf("--- Quantidade de Mensagens na Caixa de Envi ---\n");
			printf("Selecione o Usuario\n");
			fflush(stdin);
			scanf("%d",&tmp);
			tn = procura_usuario(*adm,tmp);
			system("cls");
			if(tn == NULL)
				printf(" --- Usuario não existente na Lista --- \n\n");
			else
			{
				printf("--- Mensagems de %s	ID: %d ---\n\n",tn->nome,tn->id);
				printf("Quantidade na caixa de envio: %d\n",conta_msg(tn->emsg));
				printf("Quantidade na caixa de entrada: %d",conta_msg(tn->rmsg));
			}
			break;
		case 7:		//Menu do token
			system("cls");
			(*adm) = menu_token(*adm,&(*token));
			system("cls");
			break;
		case 8:		//Inicia o token
			system("cls");
			printf("	----	INICIAR TOKEN	----\n\n");
			if((*token) == NULL)
			{
				(*token) = pega_msg(*token,&(*adm));
			}
			else if((*token)->pos != -1)
				printf("	-O token ja foi iniciado!\n");
			else
			{
				(*token) = pega_msg(*token,&(*adm));
			}
			break;
		case 9:		//-----exclue um usuario
			system("cls");
			printf(" --- Excluir Usuario ---\n\n");
			printf("ID do Usuario:\n");
			fflush(stdin);
			scanf("%d",&tmp);
			if(procura_usuario(*adm,tmp) == NULL)
				printf(" --- Usuario não existente na Lista --- \n\n");
			else
				(*adm) = excluir(*adm,tmp);
			break;
		case 10:
			printf(" --- Total de mensagens na lista: %d\n",conta_total_emsg((*adm)));
			system("pause");
			system("cls");	
			break;
		case 11:
			printf(" --- Total de mensagens na lista: %d\n",conta_total_rmsg((*adm)));
			system("pause");
			system("cls");
			break;
		case 12:
			system("cls");
			printf("	--- DESATIVAR TOKEN ---\n\n");
			printf("0. CANCELAR\n");
			printf("1. DESATIVAR\n");
			fflush(stdin);
			scanf("%d",&opc);
			while(opc != 0 && opc != 1)
			{
				printf("Opção invalida\n");
				system("pause");
				system("cls");
				printf("	--- DESATIVAR TOKEN ---\n\n");
				printf("0. CANCELAR\n");
				printf("1. DESATIVAR\n");
			}
			if(opc == 0)
				break;
			system("cls");
			printf(" --- REDE DESATIVADA ---\n\n");
			printf("	0. SAIR do programa.\n");
			printf("	1. REATIVAR REDE\n");
			fflush(stdin);
			scanf("%d",&opc);
			while(opc != 0 && opc != 1)
			{
				printf("Opção invalida\n");
				system("pause");
				system("cls");
				printf(" --- REDE DESATIVADA ---\n\n");
				printf("	0. SAIR do programa.\n");
				printf("	1. REATIVAR REDE\n");
				fflush(stdin);
				scanf("%d",&opc);
			}
			if(opc == 0)
				return;
			break;
		default:
			system("cls");
			printf("\n\n----OPÇÃO INVALIDA -----\n\n");
			break;
	}
	printf("\n---------------------------------------------------------------------------------\n");
	printf("0. Sair\n");						//SAI do sistema
	printf("1. Enviar Mensagem\n");				//Envia uma mensagem
	printf("2. Inserir Usuario\n");				//insere novo usuario no sistema
	printf("3. Escrever Lista de Usuarios\n");	//escreve lista de usuarios com dados
	printf("4. Mensagens para envio\n");		//escreve mensagems que estao para serem enviadas
	printf("5. Mensagens recebidas\n");			//escreve mensagem recebidas depois limpa a lista
	printf("6. Quantidade Mensagens\n");
	printf("7. Menu Token\n");	
	printf("8. Inicia Token\n");
	printf("9. Excluir Usuario\n");
	printf("10. Total de Mensagens a serem enviada na lista de usuarios\n");
	printf("11. Total de Mensagens recebista na lista de usuarios\n");
	printf("12. DESATIVAR REDE\n");
	fflush(stdin);
	scanf("%d",&opc);
	menu(&(*adm),&(*quant),opc,&(*token));
	
}

void insere_nodo(nodo **adm, int *quant)			//insere novo usuario
{
	char nome[40];
	printf("Nome: \n");
	fflush(stdin);
	scanf("%40[^\n]",nome);
	strupr(nome);
	nodo *n=(nodo*)malloc(sizeof(nodo));
	strcpy(n->nome,nome);
	n->rmsg = NULL;
	n->emsg = NULL;
	n->id = *quant;
	(*quant) = (*quant)+1;
	
	if(*adm == NULL)		//se a lista esta vazia
	{
		*adm = n;
		n->ant = *adm;
		n->prox = *adm;
	}
	else{
		nodo *tmp = *adm;
		while(tmp->prox != (*adm))
		{
			tmp = tmp->prox;
		}
		tmp->prox = n;
		n->ant = tmp;
		n->prox = *adm;
		(*adm)->ant = n;
	}
	return;
}

void escreve(nodo *adm)								//Escreve Lista de usuarios com informaçoes
{
	if(adm == NULL)
	{
		printf("Lista vazia\n");
		return;
	}
	else
	{
		nodo *tmp = adm;
		printf("\nNome: %s	\n",tmp->nome);
		printf("	ID:	%d\n",tmp->id);
		printf("	Numero de Mensagens não lidas: %d\n",	conta_msg(tmp->rmsg));
		printf("	Numero de Mensagens para enviar: %d\n\n", conta_msg(tmp->emsg));
		tmp = tmp->prox;
		while(tmp != adm)
		{
			printf("\nNome: %s	\n",tmp->nome);
			printf("	ID:	%d\n",tmp->id);
			printf("	Numero de Mensagens não lidas: %d\n",	conta_msg(tmp->rmsg));
			printf("	Numero de Mensagens para enviar: %d\n\n", conta_msg(tmp->emsg));
			tmp = tmp->prox;
		}
	}
}

nodo * enviar_msg(nodo *adm, int quant)				//Envia Mensagems
{
	int id;
	nodo *tmp = adm;
	
	system("cls");
	printf("--- Enviar Mensagem ---\n");
	printf("Selecione o usuario que vai enviar a mensagem:(0 ate %d)\n",quant-1);
	fflush(stdin);
	scanf("%d",&id);
	tmp = procura_usuario(adm,id);
	if(tmp == NULL)
	{
		printf("Usuario informado não existe\n\n");
		return adm;
	}
	
	char ttmp[100];
	printf("Mensagem:\n");
	fflush(stdin);
	scanf("%100[^\n]",ttmp);
	int idd;
	printf("ID do usuario que ira receber a mensagem:\n");
	fflush(stdin);
	scanf("%d",&idd);
	msg *n=(msg*)malloc(sizeof(msg));
	n->ant = NULL;
	n->prox = NULL;
	n->eid = id;
	n->rid = idd;
	strcpy(n->msg,ttmp);
	if(tmp->emsg == NULL)
	{
		tmp->emsg = n;
	}
	else
	{
		n->prox = tmp->emsg;
		tmp->emsg->ant = n;
		tmp->emsg = n;
//		msg *mtmp = tmp->emsg;
//		while(mtmp->prox != NULL)
//		{
//			mtmp = mtmp->prox;
//		}
//		mtmp->prox = n;
//		n->ant = mtmp;
	}
	return adm;
}

void ler_msg(msg *msg, nodo *adm)					//Escreve Mensagens na tela
{
	if(msg == NULL)
	{
		printf("Lista de Mensagens Vazia\n");
	}
	else
	{
		printf("---- Mensagens ----\n\n");
		while(msg != NULL)
		{
			if((procura_usuario(adm,msg->rid)) != NULL)
				printf("Destinatario: 	%s	ID: %d\n",(procura_usuario(adm,msg->rid))->nome,msg->rid);
			else
				printf("Destinatario: Não existente ID: %d\n",msg->rid);
			printf("Emisor:	%s	ID: %d\n",(procura_usuario(adm,msg->eid))->nome,msg->eid);
			printf("	Mensagem:	%s\n\n",msg->msg);
			msg = msg->prox;
		}
	}
}

nodo * procura_usuario(nodo *adm, int id)			//Procura Usuario
{
	if(adm == NULL)
	{
		return NULL;
	}
	else
	{
		nodo *tmp = adm;
		if(tmp->id == id)
		{
			return tmp;
		}
		else
		{
			tmp = tmp->prox;
			while(tmp != adm)
			{
				if(tmp->id == id)
				{
					return tmp;
				}
				else{
					tmp = tmp->prox;
				}
			}
			if(tmp == adm)
			{
				return NULL;
			}
		}
	}
}

int conta_msg(msg *lmsg)							//conta numero de msg na lista
{
	if(lmsg == NULL)
		return 0;
		
	else
	{
		int cont = 0;
		while(lmsg != NULL)
		{
			cont++;
			lmsg = lmsg->prox;
		}
		return cont;
	}
}

nodo * excluir(nodo *adm, int id)					//Exclui um usuario da lista
{
	if(adm == NULL || procura_usuario(adm,id) == NULL)
	{
		return adm;
	}
	else
	{
		nodo *tmp = adm;
		while(tmp->id != id)
		{
			tmp = tmp->prox;
		}
		tmp->ant->prox = tmp->prox;
		tmp->prox->ant = tmp->ant;
		free(tmp);
		printf("Usuario Excluirdo com Sucesso!!\n\n");
		return adm;
	}
}

nodo * menu_token(nodo *adm, tok **token)				//MENU Token
{
	nodo *tmp = adm;
	if(adm == NULL)
	{
		printf("Lista de usuarios vazia!\n");
		system("pause");
		return adm;
	}
	if((*token) == NULL || (*token)->pos == -1)
	{
		printf("	---- Rede não inicializada!! ----\n\n");
		system("pause");
		return adm;
	}
	
	int opc;
	printf("	---	MENU TOKEN	---\n\n");
	printf("0. Sair\n");
	printf("1. Mover Token\n");
	fflush(stdin);
	scanf("%d",&opc);
	system("cls");
	printf("	---	MENU TOKEN	---\n\n");
	while(opc != 0)
	{
		if(opc == 1)
		{
			if((*token)->idr != -1)
				printf("Mensagem de %s ID[%d] para %s ID[%d]\n\n",(procura_usuario(adm,(*token)->ide))->nome,(*token)->ide,
															(procura_usuario(adm,(*token)->idr))->nome,(*token)->idr);
			else
			{
				printf("Token esta vazio\n");
				printf("Verificando se usuario tem mensagem a serem enviadas\n");
				if((procura_usuario(adm,(*token)->pos))->emsg == NULL)
					printf("Usuario não tem mensagem a ser enviada. Verificando proximo usuario!\n");
				if((procura_usuario(adm,(*token)->pos))->prox->emsg == NULL)
				{
					if(conta_total_emsg((procura_usuario(adm,(*token)->pos))->prox) == 0)
					{
						printf("Não existe mais mensagens a serem entregues\n");
						system("pause");
						return adm;
					}
				//(*token)->pos = ((procura_usuario(adm,(*token)->pos))->prox->id);		
				}
				tmp = (procura_usuario(adm,(*token)->pos));
				(*token) = pega_msg((*token),&tmp);
			}
				
			printf("O Token estava em [%d] e foi para [%d]\n",(*token)->pos,(procura_usuario(adm,(*token)->pos))->prox->id);
			(*token)->pos = (procura_usuario(adm,(*token)->pos))->prox->id;
			if((*token)->idr != (*token)->pos)
				printf("	Não é o usuario de destino!\n");
			else
			{
				printf("	---Destinatario encontrado!\n");
				
				nodo *tmp = (procura_usuario(adm,(*token)->idr));
				msg *n = (msg*)malloc(sizeof(msg));
				n->ant = NULL;
				n->prox = NULL;
				n->eid = (*token)->ide;
				n->rid = (*token)->idr;
				strcpy(n->msg,(*token)->msg);
				if(tmp->rmsg == NULL)
					tmp->rmsg = n;
				else
				{
					tmp->rmsg->ant = n;
					n->prox = tmp->rmsg;
					tmp->rmsg = n;
				}
				(*token) = pega_msg((*token),&(procura_usuario(adm,(*token)->pos))->prox);
			}
		}
		else if(opc != 0)
			printf("Opção invalida\n\n");
		system("pause");
		system("cls");
		printf("	---	MENU TOKEN	---\n\n");
		printf("0. Sair\n");
		printf("1. Mover Token\n");
		fflush(stdin);
		scanf("%d",&opc);
	}
	return adm;
}

tok * pega_msg(tok *token, nodo **adm)				//Pega Mensagens para envio;
{
	int vld = 0;
	if(token == NULL)
	{
		tok *n=(tok*)malloc(sizeof(tok));
		n->ide = -1;
		n->idr = -1;
		n->pos = 0;
		strcpy(n->msg,"TOKEN VAZIO");
		token = n;
	}
	if((*adm) == NULL)
	{
		system("cls");
		printf("	**** A Lista de Usarios esta VAZIA ****\n\n");
	}
	else if((*adm)->emsg == NULL)
	{
		if(conta_total_emsg((*adm)) == 0)
			printf("	--- Não tem mais mensagens a serem enviadas ---\n");
		token->pos = (*adm)->id;
		token->ide = -1;
		token->idr = -1;
		strcpy(token->msg,"TOKEN VAZIO");
	}
	else
	{
		msg *tmp = (*adm)->emsg;
		while(tmp->prox != NULL)
		{
			tmp = tmp->prox;
		}
		token->ide = tmp->eid;
		token->idr = tmp->rid;
		if(procura_usuario((*adm),tmp->rid) == NULL)
		{
			printf("\n	Usuario de destino da mensagem não existe!!\n");
			system("pause");
			vld = -1;
		}
		strcpy(token->msg,tmp->msg);
		token->pos = (*adm)->id;
		if(tmp->ant != NULL)
			tmp->ant->prox = NULL;
		else
			(*adm)->emsg = NULL;
		free(tmp);
		tmp = NULL;
		if(vld == -1)
		{
			printf("1. Para apagar mensagem\n");
			printf("2. Para modificar destinatario\n");
			int opc;
			fflush(stdin);
			scanf("%d",&opc);
			while(opc != 1 && opc != 2)
			{
				printf("Porfavor selecione uma opção valida!\n");
				system("pause");
				system("cls");
				printf("1. Para apagar mensagem\n");
				printf("2. Para modificar destinatario\n");
				fflush(stdin);
				scanf("%d",&opc);
			}
			if(opc == 2)
			{
				system("cls");
				while(procura_usuario((*adm),token->idr) == NULL)
				{
					printf("Mensagem: %s\n",token->msg);
					printf("Destinatario [%d] invalido.\n",token->idr);
					printf("Selecione novo destinatario\n");
					fflush(stdin);
					scanf("%d",&token->idr);
				}
			}
			else
			{
				return pega_msg(token,&(*adm));
			}
		}
	}
	return token;
}

int conta_total_emsg(nodo *adm)						//Conta Total de MSGs a serem Envidas
{
	if(adm == NULL)
		return 0;
	nodo *tmp = adm->prox;
	int cont = conta_msg(adm->emsg);
	while(tmp != adm)
	{
		cont = cont+conta_msg(tmp->emsg);
		tmp = tmp->prox;
	}
	return cont;
}

int conta_total_rmsg(nodo *adm)						//Conta Total de MSGs Recebidas
{
	if(adm == NULL)
		return 0;
	nodo *tmp = adm->prox;
	int cont = conta_msg(adm->rmsg);
	while(tmp != adm)
	{
		cont = cont+conta_msg(tmp->rmsg);
		tmp = tmp->prox;
	}
	return cont;
}

