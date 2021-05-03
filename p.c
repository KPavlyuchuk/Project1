#include <stdio.h>  //Для printf, fgetc, fopen, fclose, feof
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define N 200
#define STOP ';'

char logins[N][N] = {"0", "0", "0", "0", "0", "0", "0", "0", "0"};
char * point_logins[N];

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
	if ((next_x - x) == 0) {
		res[0] = '\0';
		return res;
	}
		
	memcpy(res, x, next_x - x - 1);
	res[next_x - x - 1] = '\0';
	
	return res;
}


int handling_line(char *str)
{
	printf("%s", str);
    
    return 0;
}


int main (int argc, char * argv[])
{  

	FILE * fp;
	fp = fopen (argv[1],"r");
	
	FILE * file_filter;
	file_filter = fopen (argv[2],"r");
	
	if (fp == NULL) { 						//вывод ошибок
        perror(argv[1]);
        exit(errno);
    }
	
		
	if (file_filter == NULL) { 				//вывод ошибок
        perror(argv[2]);
        exit(errno);
    }
	
	char *point_filter;
	char string_filter[N];
	point_filter = fgets(string_filter, N, file_filter);
	int counter;
	// читаем от 2 слова

	for (counter = 0; logins[counter][0] != '\0'; counter++) {
		point_logins[counter] = read_n_word(point_filter, counter + 2, logins[counter]);
	}
	
	/*
	for (counter = 0; *point_logins[counter] != '\0'; ++counter) {
		printf("%s\n", point_logins[counter]);
	}
	*/
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
			for (counter = 0; *point_logins[counter] != '\0'; ++counter) {
				if (!strcmp(point_logins[counter], pres)) {
					printf("%s ", pres);
					pres = read_n_word(rline, 16, res);
					printf("%s\n", pres);
				}
			}
		}
		else {
			first_line = 0;
		}
	}

	if (fclose (fp) == EOF) {
		printf ("\n !!!!!ошибка!!!!! \n");
	}
	if (fclose (file_filter) == EOF) {
		printf ("\n !!!!!ошибка!!!!! \n");
	}
	return 0;
} 