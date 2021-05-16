#include <stdio.h>  //Для printf, fgetc, fopen, fclose, feof
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define N 200
#define STOP ';'

void perror (const char * msg);

int Number_lines (char * file)
{
	errno = 0;
	FILE * fp;
	fp = fopen (file,"r");
	
	if (fp == NULL) { 						//вывод ошибок
        perror(file);
        exit(errno);
    }
	
	int i = 0;
	size_t t;
	char * string = NULL;
	while ((getline(&string, &t, fp)) != -1) {
		i++;
	}
	
	if (fclose (fp) == EOF) {
		printf ("\n !!!!!ошибка!!!!! \n");
	}
	return i;
};

int Number_words (char * str)
{
	int i = 1;
	for (int j = 0; j < strlen(str); j++) {
		if (str[j] == STOP)
			i++;
	}
	return i;
};


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
	int nword = 1;
	char * x;
	for (x = str; (*x != '\0') && (nword < n); nword++) {
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

void fill_array_columns(int * array, char * str, char * filter_str)
{
	char str_word[N];
	char filter_word[N];
	char * p_str;
	char * p_filter;
	for (int k = 0; k < Number_words(filter_str); k++) {
		p_filter = read_n_word(filter_str, k + 1, filter_word);
		for (int t = 0; t < Number_words(str); t++) {
			p_str = read_n_word(str, t + 1, str_word);
			if (!strcmp(p_str, p_filter))				
				array[t] = 1;
		}
	}
};

void fill_array_rows(int * array, char * file_name, char * filter_str, int num)
{
	char res[N];
	char *pres;
	size_t len;
	char * string = NULL;
	FILE * fp;
	fp = fopen (file_name, "r");	
	char filter_word[N];
	char * p_filter;
	int num_line = 0;
	//читаем ненужную строчку с общей информацией
	getline(&string, &len, fp);
	
	while ((getline(&string, &len, fp)) != -1) {
		pres = read_n_word(string, 2, res);
		for (int t = 0; t < num; t++) {
			p_filter = read_n_word(filter_str, t + 1, filter_word);
			if (!strcmp(p_filter, pres)) {
				array[num_line] = 1;
			}
		}
		num_line++;
	}
};

int main (int argc, char * argv[])
{  
	errno = 0;
	FILE * fp;
	fp = fopen (argv[1],"r");
	
	if (fp == NULL) { 						//вывод ошибок
        perror(argv[1]);
        exit(errno);
    }

	errno = 0;
	FILE * file_filter;
	file_filter = fopen (argv[2],"r");
	
	if (file_filter == NULL) { 				//вывод ошибок
        perror(argv[2]);
        exit(errno);
    }
	
	// считаем линии
	int lines_num;
	lines_num = Number_lines (argv[1]);
	//printf("%d \n", lines_num);
	
	char * string = NULL;
	size_t len_file;
	
	//считаем слова в строке 1, столько и в остальных
	getline(&string, &len_file, fp);
	int words_num;
	words_num = Number_words(string);
	//printf("%d \n", words_num);
	
	
	//массив для столбцов
	int *columns = malloc(N * sizeof(int));
	memset(columns, 0, 4 * lines_num);
	
	//массив для строк
	int *rows = malloc(words_num * sizeof(int));
	memset(rows, 0, 4 * words_num);
	
	//обработка 1 строки фильтра -- имена
	size_t len_filter;
	char * string_filter = NULL;
	getline(&string_filter, &len_filter, file_filter);
	int words_num_filter;
	words_num_filter = Number_words(string_filter);
	fill_array_rows(rows, argv[1], string_filter, words_num_filter);
	/*
	for (int i = 0; i < lines_num; i++) {
		printf("%d ", rows[i]);
	}
	printf("\n");
	*/
	
	//обработка 2 строки фильтра -- задачи
	getline(&string_filter, &len_filter, file_filter);
	fill_array_columns(columns, string, string_filter);
	/*
	for (int i = 0; i < words_num; i++) {
		printf("%d ", columns[i]);
	}
	printf("\n");
	*/
	
	char res[N];
	char *pres;
	char temp[N];
	int counter = 0;
	while ((getline(&string, &len_filter, fp)) != -1) {
		if (rows[counter]) {
			pres = read_n_word(string, 2, res);
			printf("%s: ", pres);
			int solved_tasks = 0;
			for (int t = 0; t < words_num; t++) {
				if (columns[t]) {
					char* readed_word;
					readed_word = read_n_word(string, t + 1, temp);
					printf("Балл = [%s] ", readed_word);
					if (isdigit(readed_word[0]))
						solved_tasks++;
				}
			}
			printf(" Решил всего: %d \n", solved_tasks);
		}
		counter++;
	}
	
	//завершение
	if (fclose (fp) == EOF) {
		printf ("\n !!!!!ошибка!!!!! \n");
	}
	if (fclose (file_filter) == EOF) {
		printf ("\n !!!!!ошибка!!!!! \n");
	}
	free(rows);
	free(columns);
	return 0;
} 