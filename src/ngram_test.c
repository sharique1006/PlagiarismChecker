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

int AreSameVectors(char *A[], char *B[], int n) {
	for(int i = 0; i < n; i++) {
		if(strcmp(A[i], B[i]) != 0) {
			return 0;
		}
	}
	return 1;
}

float ngram_score(int n) {
	char *cngram[n];
	char *tngram[n];

	int shared = 0;
	int total = 0;

	for(int i = 0; i <= num_test_tokens - n; i++) {
		total += 1;
		for(int j = i; j < i+n; j++) {
			tngram[j-i] = test_tokens[j];
		}
		for(int k = 0; k <= num_corpus_tokens - n; k++) {
			for(int l = k; l < k+n; l++) {
				cngram[l-k] = corpus_tokens[l];
			}
			if(AreSameVectors(tngram, cngram, n) == 1) {
				shared += 1;
				break;
			}
		}
	}
	return 1.0 * shared/total;
}

float ngram_test() {
	float sc1 = ngram_score(3);
	float sc2 = ngram_score(5);
	float sc3 = ngram_score(7);
	float score = 10.0 * pow((3*sc1 + 5*sc2 + 7*sc3)/15, 0.4);
	return score;
}