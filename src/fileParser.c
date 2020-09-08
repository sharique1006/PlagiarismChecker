#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#define MAXLIST 100000

extern char *stopwords_tokens[MAXLIST];
extern int num_stopwords_tokens;
extern char *test_file;
extern char *corpus_folder;
extern char *test_tokens[MAXLIST];
extern int num_test_tokens;
//extern struct TOKEN test_token_count_array[MAXLIST];
extern int t_freqs;
extern char *corpus_tokens[MAXLIST];
extern int num_corpus_tokens;
//extern struct TOKEN corpus_token_count_array[MAXLIST];
extern int c_freqs;
extern char *all_tokens[MAXLIST];

char *converToLowerCase(char *s) {
	int pos = 0;
	char str[1000];
	strcpy(str, s);
	for(int i = 0; s[i] != '\0'; i++) {
		int x = s[i];
		if(x >= 65 && x <= 90) {
			str[pos] = s[i] + 32;
			pos++;
		}
		else if((x >= 48 && x <= 57) || (x >= 97 && x <= 122)) {
			str[pos] = s[i];
			pos++;
		}
	}
	str[pos] = '\0';
	s = malloc(strlen(str)*sizeof(char));
	strcpy(s, str);
	return s;
}

int tokenize(char *tokens[MAXLIST], char *filename) {
	FILE *fptr;
	if((fptr = fopen(filename, "r")) == NULL) {
		printf("%s %s\n", "Error! opening file:", filename);
		exit(1);
	}
	char *line = NULL;
	size_t len = 0;
	char *temp[MAXLIST];
	int pos = 0;
	while(getline(&line, &len, fptr) != -1) {
		for(int i = 0; i < MAXLIST; i++) {
			temp[pos] = strsep(&line, " ");
			if(temp[pos] == NULL)
				break;
			pos++;
		}
	}
	int k = 0;
	char *temp_tokens[MAXLIST];
	for(int i = 0; i < pos; i++) {
		if(strcmp(temp[i], "\n") != 0) {
			char *res = converToLowerCase(temp[i]);
			if(strlen(res) != 0) {
				temp_tokens[k] = res;
				k++;
			}
		}
	}
	int index = 0;
	for(int i = 0; i < k; i++) {
		int found = 0;
		for(int j = 0; j < num_stopwords_tokens; j++) {
			if(strcmp(temp_tokens[i], stopwords_tokens[j]) == 0) {
				found = 1;
				break;
			}
		}
		if(found == 0) {
			tokens[index] = temp_tokens[i];
			index++;
		}
	}
	// for(int i = 0; i < k; i++) {
	// 	printf("%s\n", tokens[i]);
	// }
	return index;
}