#include <stdio.h>  //Для printf, fgetc, fopen, fclose, feof
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define N 200
#define STOP ';'

void perror (const char * msg);

char * read_word (char *str)
{
	while ((*str != STOP) && (*str != '\0')) {
		str++;
	}
	
	if (*str == STOP) {
		return (str + 1);
	}
	else 
		return (str);
}

char * read_n_word (char *str, int n, char * res)
{
	int nword = 0;
	char * x;
	for (x = str; (*x != '\0') && (nword < n - 1); nword++) {
		x = read_word (x);
	}
	char * next_x;
	
	next_x = read_word(x);
	memcpy(res, x, next_x - x - 1);
	res[next_x - x - 1] = '\0';
	
	return res;
}


int handling_line(char *str)
{
	printf("%s", str);
    
    return 0;
}


int main (void)
{  

	FILE *fp = fopen ("ejudge_results.csv","r");

	if (fp == NULL) {                  //вывод ошибок
        perror("ejudge_results.csv");
        exit(errno);
    }
	
	char string[N], *rline;
	int first_line = 1;
	/*
	while ((rline = fgets(string, N, fp)) != NULL) {
		if (!first_line) {
			handling_line(rline);
		}
		else {
			first_line = 0;
		}
	}
	*/
	char res[N];
	char *pres;
	
	while ((rline = fgets(string, N, fp)) != NULL) {
		if (!first_line) {
			pres = read_n_word(rline, 2, res);
			printf("%s, ", pres);
			pres = read_n_word(rline, 16, res);
			printf("%s\n", pres);
		}
		else {
			first_line = 0;
		}
	}

	if (fclose (fp) == EOF) {
		printf ("\n !!!!!ошибка!!!!! \n");
	}
	
	return 0;
} 