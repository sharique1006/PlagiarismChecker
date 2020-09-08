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


static int myCompare(const void* a, const void* b) { 
    return strcmp(*(const char**)a, *(const char**)b); 
} 
 
void sort(char** arr, int n) { 
    qsort(arr, n, sizeof(const char*), myCompare); 
} 

float cosine_score(int c_vector[MAXLIST],int t_vector[MAXLIST], int total_num_tokens) {
	float dot_product = 0;
	float cdot_product = 0;
	float tdot_product = 0;
	for(int i = 0; i < total_num_tokens; i++) {
		dot_product += c_vector[i] * t_vector[i];
	}
	for(int i = 0; i < total_num_tokens; i++) {
		cdot_product += c_vector[i] * c_vector[i];
	}
	for(int i = 0; i < total_num_tokens; i++) {
		tdot_product += t_vector[i] * t_vector[i];
	}
	float sc = dot_product/(sqrt(cdot_product) * sqrt(tdot_product));
	return sc;
}

void getFrequencyCountVectorForCosineTest(int total_num_tokens, char *allthe_tokens[MAXLIST], int count_vector[MAXLIST], struct TOKEN token_count_array[MAXLIST], int freqs) {
	int pos = 0;
	for(int i = 0; i < total_num_tokens; i++) {
		int found = 0;
		int index = 0;
		for(int j = 0; j < freqs; j++) {
			if(strcmp(allthe_tokens[i], token_count_array[j].word) == 0) {
				index = j;
				found = 1;
				break;
			}
		}
		if(found == 1) {
			count_vector[pos] = token_count_array[index].count;
			pos++;
		}
		else {
			count_vector[pos] = 0;
			pos++;
		}
	}
}

float cosine_test() {
	for(int i = 0; i < t_freqs; i++) {
		all_tokens[i] = test_token_count_array[i].word;
	}
	int total_num_tokens = t_freqs;
	for(int j = 0; j < c_freqs; j++) {
		int found = 0;
		for(int k = 0; k < total_num_tokens;  k++) {
			if(strcmp(all_tokens[k], corpus_token_count_array[j].word) == 0) {
				found = 1;
				break;
			}
		}
		if (found == 0) {
			all_tokens[total_num_tokens] = corpus_token_count_array[j].word;
			total_num_tokens++; 
		}
	}
	sort(all_tokens, total_num_tokens);
	int c_vector[MAXLIST];
	int t_vector[MAXLIST];

	getFrequencyCountVectorForCosineTest(total_num_tokens, all_tokens, c_vector, corpus_token_count_array, c_freqs);
	getFrequencyCountVectorForCosineTest(total_num_tokens, all_tokens, t_vector, test_token_count_array, t_freqs);
	float score = 10.0 * cosine_score(c_vector, t_vector, total_num_tokens);
	return score;
}