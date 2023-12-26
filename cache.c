//Daniel Martins Vieira 11215719
//Frederico Bulhões de Souza Ribeira 11208440
//Olavo Morais Borges Pereira 11297792
//Vinicius Silva Fernandes Kuhlmann 11215751

#include <stdio.h>
#include <stdlib.h>

typedef struct{//struct usado para armazenar uma linha de Cache
	char bitValidade;//Diz se a linha é válida
	int data;//Armazena o conteúdo da linha
	int tag;//Armazena o rótulo da linha
}linhaCache;

typedef linhaCache* Cache;//Cache é um vetor de linhaCache
typedef int* MemoriaPrincipal;//MemoriaPrincipal é um vetor de inteiros

//Cria e retorna um vetor de LinhaCache
Cache CriarCache(int nLinhas){//Número de linhas na Cache
	if(nLinhas<=0){//Se for um número de linhas inválido
		return NULL;
	}
	
	Cache retorno=(Cache)malloc(nLinhas*sizeof(linhaCache));//Aloca espaço para o vetor de linhasCaches
	for(int i=0;i<nLinhas && retorno!=NULL;i++){//Executa nLinhas vezes
		retorno[i].bitValidade='0';//Define o bit de validade da i-ésima linha como '0'
	}
	return retorno;//Retorna o endereço alocado
}

//Cria e retorna um vetor de inteiros
MemoriaPrincipal CriarMemoriaPrincipal(int tamanho){//tamanho do vetor
	if(tamanho<=0){//Se o tamanho for inválido
		return NULL;
	}

	MemoriaPrincipal retorno=(MemoriaPrincipal)malloc(tamanho*sizeof(int));//Aloca espaço para o vetor de inteiros
	for(int i=0;i<tamanho;i++){//Define a i-ésima posição com i
		retorno[i]=i;
	}
	return retorno;//Retorna o endereço alocado
}

//Exibe a Memórica Cache
void ExibirCache(Cache mc,int tamanhoCache){//Memória Cache a ser exibida, número de linhas na memória
	for(int i=0;i<tamanhoCache;i++){//Executa tamanhoCache vezes
		printf("Linha %d validade: %c tag:%d dado %d\n",i,mc[i].bitValidade,mc[i].tag,mc[i].data);//Exibe o bit de validade, a tag e o dado na i-ésima linha
	}
}

//Exibe a Memória Principal
void ExibirMemoriaPrincipal(MemoriaPrincipal mc,int tamanhoMemoriaPrincipal){//Memória a ser exibida, tamanho da memória
	for(int i=0;i<tamanhoMemoriaPrincipal;i++){//Executa tamanho vezes 
		printf("Posicao %d dado %d\n",i,mc[i]);//Exibe o i-ésimo elemento da memória
	}
}

//Retorna em qual linha da cache um bloco deve mapeado usando mapeamento associativo por conjunto
int MapeamentoAssociativoPorConjunto(Cache mc,int tamanhoCache,int nConjuntos,MemoriaPrincipal mp,int tamanhoMemoriaPrincipal,int posicaoMemoriaPrincipal,int* erro){//Memória Cache, tamanho da memória Cache, número de conjuntos da Cache
	//Memória principal, tamanho da memória principal e posição na memória principal do bloco a ser copiado para a cache
        int aux=(posicaoMemoriaPrincipal%nConjuntos)*(tamanhoCache/nConjuntos);//Primeira linha do conjunto em que o bloco tem q ser inserido
        int retorno=-1;
	for(int i=aux;i<aux+(tamanhoCache/nConjuntos);i++){//Varre o conjunto posição por posição
                if(mc[i].bitValidade=='0'){//Se a i-ésima posição está livre
                        retorno=i;//Define para inserir na linha i da cache
                        break;//Sai do for
                }
        }

        if(retorno==-1){//Se não encontrou nenhuma posição livre
        	*erro=1;//Informa que houve um erro
		retorno=aux;//Define o retorno como a primeira linha do conjunto
	}

        return retorno;//Retorna a linha que vai receber o bloco
}


//Não são necessárias já que todas as funções de mapeamento podem ser definidas como um Mapeamento Associativo por Conjunto
/*
int MapeamentoDireto(Cache mc,int tamanhoCache,MemoriaPrincipal mp,int tamanhoMemoriaPrincipal,int posicaoMemoriaPrincipal){
	int retorno=posicaoMemoriaPrincipal%tamanhoCache;
	if(mc[retorno].bitValidade!='0'){
		printf("Colisão na posição %d da Cache\n",retorno);
	}
	return retorno;
}

int MapeamentoAssociativo(Cache mc,int tamanhoCache,MemoriaPrincipal mp,int tamanhoMemoriaPrincipal,int posicaoMemoriaPrincipal){
	int retorno=-1;
	for(int i=0;i<tamanhoCache;i++){
		if(mc[i].bitValidade=='0'){
			retorno=i;
			break;
		}
	}
	if(retorno==-1){
		printf("Cache cheia, substituindo a linha 0 da cache\n");
		retorno=0;
	}
	return retorno;

}
*/

//Faz a transferência da memória principal para a memória Cache
void CarregarNaCache(Cache mc,int tamanhoCache,int nConjuntos,MemoriaPrincipal mp,int tamanhoMemoriaPrincipal,int funcaoMapeamento,int posicaoMemoriaPrincipal){
	int erro=0;//Variávei que armazena se houve erro ou não
	int linhaCache=MapeamentoAssociativoPorConjunto(mc,tamanhoCache,nConjuntos,mp,tamanhoMemoriaPrincipal,posicaoMemoriaPrincipal,&erro);//Encontra a linha para inserir o novo bloco
	if(erro!=0){//Se houve um erro
		if(funcaoMapeamento==0){//Se é um mapeamento direto
			printf("Colisão na posição %d da cache\n",linhaCache);//Exibe mensagem de erro
		}else{//Se for mapeamento associativo
			printf("Cache cheia, substituindo a linha %d da cache\n",linhaCache);
		}
	}
	//Atualiza a linha em que o bloco vai ser inserido
	mc[linhaCache].bitValidade='1';//Define o bit de validade como '1'
	mc[linhaCache].tag=posicaoMemoriaPrincipal;//Define a tag da linha como o endereço do bloco na memória principal
	mc[linhaCache].data=mp[posicaoMemoriaPrincipal];//Define o conteúdo da linha como o conteúdo do bloco
}

int main(){

	int tamanhoCache,tamanhoMemoriaPrincipal;//Armazenam os tamanhos das memórias
	
	printf("Digite o tamanho da memória principal\n");
	scanf("%d",&tamanhoMemoriaPrincipal);
	MemoriaPrincipal mp=CriarMemoriaPrincipal(tamanhoMemoriaPrincipal);//Cria a memória principal

	printf("Digite o tamanho da memória cache\n");
	scanf("%d",&tamanhoCache);
	Cache mc=CriarCache(tamanhoCache);//Cria a memória cache

	if(mc==NULL || mp==NULL){//Se houve algum erro ao criar as memórias
		printf("Erro ao criar memórias\n");
		return 0;//Encerra a main
	}
	
	int funcaoMapeamento=-1;
	int nConjuntos;
	while(funcaoMapeamento==-1){
		//Lê a função de mapeamento desejada
		printf("Digite a função de mapeamento:\n");
		printf("Digite 0 para mapeamento direto\n");
		printf("Digite 1 para mapeamento associativo\n");
		printf("Digite 2 para mapeamento associativo por conjunto\n");
		scanf("%d",&funcaoMapeamento);

		if(funcaoMapeamento!=0 && funcaoMapeamento!=1 && funcaoMapeamento!=2){//Se o usuário não digitou uma opção válida
			printf("Digite uma opção válida\n");
			funcaoMapeamento=-1;//Define como -1 para continuar no loop
		}else if(funcaoMapeamento==0){//Se for Mapeamento Direto
			nConjuntos=tamanhoCache;//Define o número de conjuntos igual ao número de linhas na Cache
		}else if(funcaoMapeamento==1){//Se for Mapeamento Associativo
			nConjuntos=1;//Define o número de conjuntos como 1
		}else{//Se for Mapeamento Associativo por Conjunto
			//Lê o conjunto
			printf("Digite o número de conjuntos\n");
			scanf("%d",&nConjuntos);
			if(nConjuntos<=0 || tamanhoCache%nConjuntos!=0){//Se for uma quantidad inválida de conjuntos
				printf("Por favor, digite um número de conjuntos tal que não sobre resto ao dividir o tamanho da cache pelo número de conjuntos\n");
				funcaoMapeamento=-1;//Define a função como -1 para continuar no loop
			}
		}
	}

	//Exibe as operações disponíveis
	int op=0;
	printf("Digite um número >=0 e < tamanho da memória principal para copiar a posição na cache\n");
	printf("Digite -1 para exibir as memórias\n");
	printf("Digite -2 para encerrar o programa\n");
	while(op!=-2){//Enquanto usuário não digitar -2
		scanf("%d",&op);//Lê a operação do usuário
		if(op>=0 && op<tamanhoMemoriaPrincipal){//Se for uma transferência para a cache
			CarregarNaCache(mc,tamanhoCache,nConjuntos,mp,tamanhoMemoriaPrincipal,funcaoMapeamento,op);
		}else if(op==-1){//Se for para exibir as memórias
			printf("Cache:\n");
			ExibirCache(mc,tamanhoCache);//Exibe a cache
			printf("\nMemoria principal:\n");
			ExibirMemoriaPrincipal(mp,tamanhoMemoriaPrincipal);//Exibe a memória principal
		}
	}//Sai do while quando usuário digitar -2

	if(mc!=NULL){//Se Memória Cache não for nula
		free(mc);//Libera o espaço alocado
	}
	if(mp!=NULL){//Se Memória principal não for nula
		free(mp);//Libera o espaço alocado
	}
	
	return 0;//Encerra o programa
		
}
