/*bibliotecas*/
#include <stdio.h>
#include <stdlib.h>

/*struck*/
typedef struct { 
    int dia;
    int mes;
    int ano;
} Data;

/*fun�oes*/

int AnoBissexto(int ano) // Fun��o que verifica se um ano � bissexto
{
    if (ano % 4 == 0) // Um ano � bissexto se for divis�vel por 4.
	{
        if (ano % 100 == 0) // Se for divis�vel por 100, deve tamb�m ser divis�vel por 400 para ser bissexto.
		{
            return ano % 400 == 0;
        }
        return 1;  // O ano � bissexto
    }
    return 0;  // O ano n�o � bissexto
}

int DataValida(int dia, int mes, int ano) // Fun��o que verifica se uma data � v�lida
{
    if (ano < 0 || mes < 1 || mes > 12 || dia < 1)// Verifica se o ano, m�s e dia est�o dentro dos intervalos v�lidos.
        return 0;  // Data inv�lida se o ano � negativo, m�s fora do intervalo ou dia menor que 1

    int diasPorMes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Se o ano for bissexto e o m�s for fevereiro, ajusta o n�mero de dias para 29
    if (AnoBissexto(ano) && mes == 2)
        diasPorMes[1] = 29;

    return dia <= diasPorMes[mes - 1];  // Verifica se o dia est� dentro do n�mero m�ximo de dias do m�s
}


Data adicionarDias(Data data, int diasParaAdicionar) // Fun��o que adiciona um n�mero de dias a uma data
{
    int diasPorMes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (AnoBissexto(data.ano))
        diasPorMes[1] = 29;  // Ajusta fevereiro se o ano for bissexto

    data.dia += diasParaAdicionar;

    // Ajusta a data se o n�mero de dias ultrapassar o n�mero de dias no m�s
    while (data.dia > diasPorMes[data.mes - 1]) 
	{
        data.dia -= diasPorMes[data.mes - 1];
        data.mes++;
        // Se o m�s ultrapassar dezembro, ajusta o m�s e o ano
        if (data.mes > 12) 
		{
            data.mes = 1;
            data.ano++;
            diasPorMes[1] = AnoBissexto(data.ano) ? 29 : 28;  // Ajusta fevereiro se o ano for bissexto
        }
    }

    return data;
}


// Adiciona o intervalo de dias repetidamente at� o final do ano e armazena as datas em um array.
void calcularDatasDeTreinamento(Data dataInicial, int intervalo, Data* datas, int* contagem) // Fun��o que calcula as datas de treinamento com base em um intervalo
{
    Data dataAtual = dataInicial;
    int indice = 0;
    while (dataAtual.ano == dataInicial.ano)
	{
        datas[indice++] = dataAtual;
        dataAtual = adicionarDias(dataAtual, intervalo);  // Adiciona o intervalo � data atual
    }
    *contagem = indice;  // Define a contagem de datas calculadas
}

// Fun��o que encontra datas comuns em v�rios arrays de datas
// Verifica se uma data est� presente em todos os arrays fornecidos e armazena essas datas comuns.
void encontrarDatasComuns(Data* atletismo, int contagemA, Data* natacao, int contagemN, Data* rugby, int contagemR, Data* judo, int contagemJ, Data* datasComuns, int* contagemComuns) {
    *contagemComuns = 0;
    int i, j, k, l;
    // Itera sobre todas as datas de atletismo
    for (i = 0; i < contagemA; i++) 
	{
        // Verifica se a data de atletismo est� em natacao
        for (j = 0; j < contagemN; j++) 
		{
            if (atletismo[i].dia == natacao[j].dia && atletismo[i].mes == natacao[j].mes && atletismo[i].ano == natacao[j].ano)
			{
                // Se a data est� em natacao, verifica se tamb�m est� em rugby
                for (k = 0; k < contagemR; k++) 
				{
                    if (atletismo[i].dia == rugby[k].dia && atletismo[i].mes == rugby[k].mes && atletismo[i].ano == rugby[k].ano) 
					{
                        // Se a data est� em rugby, verifica se tamb�m est� em judo
                        for (l = 0; l < contagemJ; l++) {
                            if (atletismo[i].dia == judo[l].dia && atletismo[i].mes == judo[l].mes && atletismo[i].ano == judo[l].ano) 
							{
                                datasComuns[*contagemComuns] = atletismo[i];  // Adiciona a data comum ao array
                                (*contagemComuns)++;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Fun��o que retorna o dia da semana para uma determinada data
// Usa a f�rmula de Zeller para calcular o dia da semana e retorna o nome do dia correspondente.
const char* obterDiaDaSemana(int dia, int mes, int ano) 
{
    int d = dia;
    int m = (mes + 9) % 12 + 1;
    int a = ano - m / 10;
    int diaDaSemana = (d + (2 * m) + (3 * (m + 1) / 5) + a + (a / 4) - (a / 100) + (a / 400) + 2) % 7;
    const char* diasDaSemana[] = { "Domingo", "Segunda-feira", "Ter�a-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "S�bado" };
    return diasDaSemana[diaDaSemana];
}

// Fun��o que calcula a diferen�a em dias entre duas datas
// Converte as datas em dias totais e calcula a diferen�a absoluta entre elas.
int calcularDiferencaDias(Data data1, Data data2) 
{
    int dia1 = data1.dia + data1.mes * 30 + data1.ano * 365;
    int dia2 = data2.dia + data2.mes * 30 + data2.ano * 365;
    return abs(dia2 - dia1);
}

// Fun��o que salva as datas de treinamento em um arquivo
// Cria um arquivo de texto e escreve as datas de atletismo, nata��o, rugby e jud�, formatadas em uma tabela.
void salvarDatasEmArquivo(const char* nomeArquivo, Data dataInicial, Data dataFinal, Data* atletismo, int contagemA, Data* natacao, int contagemN, Data* rugby, int contagemR, Data* judo, int contagemJ) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    // Cabe�alho
    fprintf(arquivo, "_____________________________________________________________________________________________________________________________\n");
    fprintf(arquivo, "                                                                                                                             \n");
    fprintf(arquivo, "                        As modalidades treinam de %02d/%02d/%04d (%s) a %02d/%02d/%04d (%s)\n",
        dataInicial.dia, dataInicial.mes, dataInicial.ano, obterDiaDaSemana(dataInicial.dia, dataInicial.mes, dataInicial.ano),
        dataFinal.dia, dataFinal.mes, dataFinal.ano, obterDiaDaSemana(dataFinal.dia, dataFinal.mes, dataFinal.ano));
    fprintf(arquivo, "                                                                                                                             \n");
    fprintf(arquivo, "_____________________________________________________________________________________________________________________________\n\n");

    // Tabela de datas
    fprintf(arquivo, "            DATAS ATLETISMO                 DATAS NATA��O                   DATAS RUGBY                    DATAS JUD�\n");
    fprintf(arquivo, "            (a cada 2 dias)                (a cada 3 dias)                (a cada 5 dias)                (a cada 9 dias)\n");
    fprintf(arquivo, "_____________________________________________________________________________________________________________________________\n");
    fprintf(arquivo, "                                                                                                                             \n");

    int maxContagem = contagemA > contagemN ? (contagemA > contagemR ? (contagemA > contagemJ ? contagemA : contagemJ) : (contagemR > contagemJ ? contagemR : contagemJ)) : (contagemN > contagemR ? (contagemN > contagemJ ? contagemN : contagemJ) : (contagemR > contagemJ ? contagemR : contagemJ));

    int i;
    for (i = 0; i < maxContagem; i++) {
        // Imprime o n�mero da linha
        fprintf(arquivo, "%3d |  ", i + 1);

        // Imprime a data de atletismo, se dispon�vel
        if (i < contagemA) {
            fprintf(arquivo, "%02d/%02d/%04d - %-14s   ", atletismo[i].dia, atletismo[i].mes, atletismo[i].ano, obterDiaDaSemana(atletismo[i].dia, atletismo[i].mes, atletismo[i].ano));
        } else {
            fprintf(arquivo, "                        ");
        }

        // Imprime a data de nata��o, se dispon�vel
        if (i < contagemN) {
            fprintf(arquivo, "%02d/%02d/%04d - %-14s   ", natacao[i].dia, natacao[i].mes, natacao[i].ano, obterDiaDaSemana(natacao[i].dia, natacao[i].mes, natacao[i].ano));
        } else {
            fprintf(arquivo, "                        ");
        }

        // Imprime a data de rugby, se dispon�vel
        if (i < contagemR) {
            fprintf(arquivo, "%02d/%02d/%04d - %-14s   ", rugby[i].dia, rugby[i].mes, rugby[i].ano, obterDiaDaSemana(rugby[i].dia, rugby[i].mes, rugby[i].ano));
        } else {
            fprintf(arquivo, "                        ");
        }

        // Imprime a data de jud�, se dispon�vel
        if (i < contagemJ) {
            fprintf(arquivo, "%02d/%02d/%04d - %-14s", judo[i].dia, judo[i].mes, judo[i].ano, obterDiaDaSemana(judo[i].dia, judo[i].mes, judo[i].ano));
        } else {
            fprintf(arquivo, "                      ");
        }

        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

void salvarDatasComunsEmArquivo(const char* nomeArquivo, Data* datasComuns, int contagemComuns) {
    // Abre o arquivo com o nome especificado para escrita ("w").
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        // Se n�o conseguiu abrir o arquivo, exibe uma mensagem de erro e sai da fun��o.
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    // Calcula o n�mero de dias entre as datas comuns
    int diasEntreComuns = 0;
    if (contagemComuns > 1) {
        // Se houver mais de uma data comum, calcula a diferen�a em dias entre as duas primeiras datas.
        diasEntreComuns = calcularDiferencaDias(datasComuns[0], datasComuns[1]);
    }

    // Cabe�alho do arquivo
    fprintf(arquivo, "__________________________________________________________________________________________________________________________________\n");
    fprintf(arquivo, "                                                                                                                             \n");
    fprintf(arquivo, "                                  As 4 modalidades se encontram a cada %d dias.\n", diasEntreComuns);
    fprintf(arquivo, "__________________________________________________________________________________________________________________________________\n\n");

    // Cabe�alho da tabela de datas
    fprintf(arquivo, "            DATAS ATLETISMO                 DATAS NATA��O                   DATAS RUGBY                    DATAS JUD�\n");
    fprintf(arquivo, "            (a cada 2 dias)                (a cada 3 dias)                (a cada 5 dias)                (a cada 9 dias)\n");
    fprintf(arquivo, "__________________________________________________________________________________________________________________________________\n");
    fprintf(arquivo, "                                                                                                                             \n");

    int i;
    for (i = 0; i < contagemComuns; i++) {
        // Para cada data comum, imprime o n�mero da linha e as datas lado a lado
        fprintf(arquivo, "%3d |  %02d/%02d/%04d - %14s     %02d/%02d/%04d - %14s     %02d/%02d/%04d - %14s     %02d/%02d/%04d - %14s\n",
                i + 1,  // N�mero da linha
                datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano, obterDiaDaSemana(datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano),
                datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano, obterDiaDaSemana(datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano),
                datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano, obterDiaDaSemana(datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano),
                datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano, obterDiaDaSemana(datasComuns[i].dia, datasComuns[i].mes, datasComuns[i].ano));
    }

    // Finaliza o arquivo
    fprintf(arquivo, "__________________________________________________________________________________________________________________________________\n");

    fclose(arquivo);  // Fecha o arquivo ap�s terminar a escrita
}
/*variaveis e array/vetor*/
// Declara arrays para armazenar as datas de treinamento de cada modalidade
    Data atletismo[366], natacao[366], rugby[366], judo[366];
    int contagemA, contagemN, contagemR, contagemJ;
// Declara um array para armazenar as datas comuns a todas as modalidades
    Data datasComuns[366];
    int contagemComuns;
    

/*corpo do programa*/
int main() {
    Data dataInicial;    // Declara uma vari�vel para armazenar a data inicial fornecida pelo usu�rio
    printf("Digite a data inicial (dd mm aaaa): ");
    scanf("%d %d %d", &dataInicial.dia, &dataInicial.mes, &dataInicial.ano);

    // Verifica se a data fornecida � v�lida
    if (!DataValida(dataInicial.dia, dataInicial.mes, dataInicial.ano)) {
        printf("Data inv�lida.\n");
        return 1;  // Sai do programa se a data for inv�lida
    }

    // Calcula as datas de treinamento para cada modalidade
    calcularDatasDeTreinamento(dataInicial, 2, atletismo, &contagemA);
    calcularDatasDeTreinamento(dataInicial, 3, natacao, &contagemN);
    calcularDatasDeTreinamento(dataInicial, 5, rugby, &contagemR);
    calcularDatasDeTreinamento(dataInicial, 9, judo, &contagemJ);
    
    // Encontra as datas que s�o comuns a todas as modalidades
    encontrarDatasComuns(atletismo, contagemA, natacao, contagemN, rugby, contagemR, judo, contagemJ, datasComuns, &contagemComuns);

    // Salva as datas comuns a todas as modalidades em outro arquivo
    salvarDatasComunsEmArquivo("DATASCOMUNS.TXT", datasComuns, contagemComuns);
    
    Data dataFinal = adicionarDias(dataInicial, 365);  // Data final um ano depois, ajuste conforme necess�rio
	salvarDatasEmArquivo("DATASTREINOS.TXT", dataInicial, dataFinal, atletismo, contagemA, natacao, contagemN, rugby, contagemR, judo, contagemJ);// Salva as datas de treinamento em um arquivo

    // Mensagem final indicando que os dados foram salvos
    printf("Datas salvas nos arquivos.\n");

    return 0;  // Termina o programa
}

 
