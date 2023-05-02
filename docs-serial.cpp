#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <locale.h>

using namespace std;

//int k = 0;

typedef struct {
	double pontuacao;
} Assunto;

typedef struct {
	list<Assunto> assuntos;
} Documento;

typedef struct {
	list<Documento> lista_documentos;
} Armario;

// Ler o conte�do do arquivo e guardar em estruturas do programa
bool ler_ficheiro(list<Documento>& documentos, int dados[], const char *nomeFile)
{
	// Abrir o arquivo para leitura
	ifstream arquivo(nomeFile);

	// Verificar se o arquivo foi aberto corretamente
	if(!arquivo) {
		cerr << "N�o foi poss�vel abrir o arquivo!!" << endl;
		return false;
	}

	int n_armarios, n_docs, n_assuntos; //vir�o do ficheiro
	string linha;
	//stringstream() converte uma string em inteiro
	getline(arquivo, linha);
	std::stringstream(linha) >> n_armarios;
	dados[0] = n_armarios;

	getline(arquivo, linha);
	std::stringstream(linha) >> n_docs;
	dados[1] = n_docs;

	getline(arquivo, linha);
	std::stringstream(linha) >> n_assuntos;
	dados[2] = n_assuntos;

	string token;

	Assunto assuntAux;
	Documento docAux;

	while( getline(arquivo, linha) ) {
		linha.erase(0, 2); //erase() de string deleta 2 caracteres a partir da posi��o 0
		istringstream ss(linha); //uma esp�cie de tokenizer em c, converte a string linha
		while ( getline(ss, token, ' ') ) //enquanto existirem mais ' '
		{
			istringstream(token) >> assuntAux.pontuacao; //converter a string token em const *char e depois em double

			docAux.assuntos.push_back(assuntAux); //guarda as pontua��es dos assuntos numa vari�vel auxiliar
		}
		documentos.push_back(docAux); //guarda na lista de documentos

		docAux.assuntos.clear(); //limpar o documento auxiliar para recome�ar
	}
	
	// Fechar o arquivo
	arquivo.close();
	return true;
}

void escrever_ficheiro(string str){
	// abre o arquivo para escrita
	ofstream arquivo;
	if (remove("docs.out") != 0);

	arquivo.open("docs.out", ios::app);

	// verifica se o arquivo foi aberto com sucesso
	if( arquivo.is_open() ) {
		// escreve uma string no arquivo
		arquivo << str;

		// fecha o arquivo
		arquivo.close();
	}
	else {
		// exibe uma mensagem de erro se o arquivo n�o p�de ser aberto
		cout << "N�o foi poss�vel abrir o arquivo para escrita.";
	}
}
/*
void imprimir_vector_docs(vector<Documento> vet){
	Documento docAux;
	Assunto assuntAux;
	list<Documento>::iterator itDoc;
	list<Assunto>::iterator itAssunt;
	cout << "\nImpress�o " << k << ": \n";
	for(vector<Documento>::iterator iii = vet.begin(); iii != vet.end(); iii++){
		docAux = *iii;
		for(itAssunt = docAux.assuntos.begin(); itAssunt != docAux.assuntos.end(); itAssunt++) {
			assuntAux = *itAssunt;
			cout << fixed << std::setprecision(2) << assuntAux.pontuacao << " ";
		}
		cout << "\n";
	}
	cout <<"\n";
	k++;
}
*/
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Portuguese");

	int dados[3]; //dados[0] = n_armarios; dados[1] = n_docs; dados[2] = n_assuntos;

	string nomeFile = "docs.in";
	if(argc == 3){
		nomeFile = argv[1];
		std::stringstream(argv[2]) >> dados[0];
	}
	else if(argc > 3){
		cerr << "Mais par�metros do que deve ter!";
		return 1;
	}

	list<Documento> documentos_lista;
	
	const char* file = nomeFile.c_str();
	
	if ( ler_ficheiro(documentos_lista, dados, file) ){
		cout << dados[0] << "\n" << dados[1] << "\n" << dados[2] << "\n" << endl;
	}

	if( argc == 3 ) //se tem 3 par�metros na linha de comando
		std::stringstream(argv[2]) >> dados[0];
	
	Assunto assuntAux;
	Documento docAux;

	//declarando os iteradores das list
	list<Documento>::iterator itDoc;
	list<Assunto>::iterator itAssunt;

	vector<Documento> docs_vet(documentos_lista.size());
	docs_vet.clear(); //limpar o vector

	int i = 0;
    // Definindo os assuntos de cada documento no arm�rio
	for(itDoc = documentos_lista.begin(); itDoc != documentos_lista.end(); itDoc++){
		docAux = *itDoc;
		docs_vet.push_back(docAux);
		i++;
	}
	
    int n_armarios = dados[0];
	int n_docs = dados[1];
    int n_assuntos = dados[2];

	// Passo 1: inicializa��o
    int j;
    vector<int> assignments(n_docs);
    for(i = 0; i < n_docs; i++){
    	assignments[i] = i % n_armarios; //destacando/distribuindo os documentos nos arm�rios
	}

    while(1){
		// Passo 2: encontre a localiza��o de cada arm�rio
		vector<Documento> armarios_pos;
		armarios_pos.clear();
		//Documento armarios_pos[MAX_ARMARIOS];
		//armarios_pos � localiza��o dos arm�rios, necess�ria para se definir qual arm�rio est� mais pr�ximo a cada documento, no passo 3

		//fixa no arm�rio e anda pelos documentos calculando as m�dias
		for (i = 0; i < n_armarios; i++) //para cada arm�rio, calcula a m�dia de assuntos dos documentos
		{
			vector<double> somatorio_assuntos(n_assuntos);
			somatorio_assuntos.clear();
			int count = 0;
			j = 0; //j aqui � o �ndice dos documentos

			//percorrer os documentos
			for (itDoc = documentos_lista.begin(); itDoc != documentos_lista.end(); itDoc++){
                if (assignments[j] == i) //se documento j pertence ao arm�rio i
				{
					docAux = *itDoc;
					int indexAssunt = 0;

					//percorrer os assuntos neles
					//fazer soma e guardar na posi��o correspondente do vector medias
					for( itAssunt = docAux.assuntos.begin(); itAssunt != docAux.assuntos.end(); itAssunt++ ){
						assuntAux = *itAssunt;
						somatorio_assuntos[indexAssunt] += assuntAux.pontuacao;
						indexAssunt++;
					}
                    count++; //count � a quantidade de documentos atribu�dos a cada arm�rio
                }
                j++;
            }
			docAux.assuntos.clear();
            if (count > 0){ //se o arm�rio tem pelo menos um documento
				for(int indexAssunt = 0; indexAssunt < n_assuntos; indexAssunt++){
				//nesse for, ele mete o valor de medias no arm�rio correspondente
					//assuntAux = *itAssunt;
					
					//gravar a m�dia de assuntos dos documentos do armario i
					assuntAux.pontuacao = (double) somatorio_assuntos[indexAssunt] / count;
					
					docAux.assuntos.push_back(assuntAux);
				}
				armarios_pos.push_back(docAux);
            }
        }

        // Passo 3: verifique se alguma cidade est� mais pr�xima de outro helic�ptero
        bool changed = false;
		
		//fixar no documento e achar o arm�rio mais pr�ximo a esse documento
		j = 0;
		for(vector<Documento>::iterator itVet = docs_vet.begin(); itVet != docs_vet.end(); itVet++)
		{
			double menor_distancia = 100000000000;
			int armario_mais_proximo = assignments[j];

			docAux = *itVet;
			
			i = 0;
			//for (i = 0; i < n_armarios; i++)
			for( vector<Documento>::iterator itVet2 = armarios_pos.begin(); itVet2 != armarios_pos.end(); itVet2++  )
			{
            	double d = 0, distAux = 0;

				Documento docAux2 = *itVet2;
            	list<Assunto>::iterator itAssunt2 = docAux2.assuntos.begin();
				itAssunt = docAux.assuntos.begin();

            	for(; itAssunt2 != docAux2.assuntos.end(); itAssunt2++ ){
					Assunto assuntAux2 = *itAssunt2;
            		assuntAux = *itAssunt;
					distAux = assuntAux2.pontuacao - assuntAux.pontuacao; //armarios_pos[i].x - docs_vet[j].x
					d += distAux * distAux;
					itAssunt++;
				}
            	
				//Passo 3: para cada documento, calcule a dist�ncia �s m�dias de cada arm�rio e mova o
				//documento para o arm�rio com menor dist�ncia;
				if (d < menor_distancia)
				{
					menor_distancia = d; //aqui acha a menor dist�ncia at� agora
					armario_mais_proximo = i; //aqui guarda a posi��o no vector daquele que possui a menor dist�ncia
				}
				i++;
			}
            if(assignments[j] != armario_mais_proximo)
			{
                assignments[j] = armario_mais_proximo;
                changed = true;
            }
            j++;
        }

        // Passo 4: repita at� convergir
        if (!changed)
            break;
    }

    // Salvar no ficheiro e Print da atribui��o de cidades a helic�pteros final
    string str_saida;
    std::stringstream ss;
    
	for (j = 0; j < n_docs; j++){
		printf("%d: %d \n", j, assignments[j]);
        ss << j << ": " << assignments[j] << " \n";
		str_saida = ss.str();
	}
	
	escrever_ficheiro(str_saida);
    printf("\n");

    return 0;
}

