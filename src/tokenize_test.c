#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "../include/token.h"

#define MAXLIST 100000

extern char *stopwords_tokens[MAXLIST];
extern int num_stopwords_tokens;
extern char *test_file;
extern char *corpus_folder;
extern char *test_tokens[MAXLIST];
extern int num_test_tokens;
extern struct TOKEN test_token_count_array[MAXLIST];
extern int t_freqs;
extern char *corpus_tokens[MAXLIST];
extern int num_corpus_tokens;
extern struct TOKEN corpus_token_count_array[MAXLIST];
extern int c_freqs;
extern char *all_tokens[MAXLIST];

int getCount(char *token, char *tokens[MAXLIST], int num_tokens) {
	int count = 0;
	for(int i = 0; i < num_tokens; i++) {
		if(strcmp(tokens[i], token) == 0) {
			count++;
		}
	}
	return count;
}

int getFrequency(char *tokens[MAXLIST], int num_tokens, struct TOKEN token_count_array[MAXLIST]) {
	char *already_present_words[MAXLIST];
	int pos = 0;
	for(int i = 0; i < num_tokens; i++) {
		int already_present = 0;
		for(int j = 0; j < pos; j++) {
			if(strcmp(tokens[i],  already_present_words[j]) == 0) {
				already_present = 1;
			}
		}
		if(already_present == 0) {
			int count = getCount(tokens[i], tokens, num_tokens);
			struct TOKEN tok = {tokens[i], count};
			token_count_array[pos] = tok;
			already_present_words[pos] = tokens[i];
			pos++;
		} 
	}
	return pos;
}

int getMin(int a, int b) {
	if(a < b) {
		return a;
	}
	return b;
}

int getMultiplier(char *word) {
	int x = strlen(word) * strlen(word);
	return x;
}

float tokenize_test() {
	int shared = 0;
	int total = 0;
	for(int i = 0; i < t_freqs; i++) {
		struct TOKEN tok = test_token_count_array[i];
		int found = 0;
		int index;
		for(int j = 0; j < c_freqs; j++) {
			if(strcmp(tok.word, corpus_token_count_array[j].word) == 0) {
				found = 1;
				index = j;
				break;
			}
		}
		if(found == 1) {
			shared += getMin(tok.count, corpus_token_count_array[index].count) * getMultiplier(tok.word); 
		}
		total += tok.count * getMultiplier(tok.word);
	}
	// printf("%s %d\n", "total: ", total);
	// printf("%s %d\n", "shared: ", shared);
	float tokenize_score = 10.0 * shared / total;
	return tokenize_score;
}