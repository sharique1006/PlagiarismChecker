#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <time.h>
#include "../include/fileParser.h"
#include "../include/tokenize_test.h"
#include "../include/ngram_test.h"
#include "../include/cosine_test.h"

#define MAXLIST 100000

char *stopwords_tokens[MAXLIST];
int num_stopwords_tokens;
char *test_file;
char *corpus_folder;
char *test_tokens[MAXLIST];
int num_test_tokens;
struct TOKEN test_token_count_array[MAXLIST];
int t_freqs;
char *corpus_tokens[MAXLIST];
int num_corpus_tokens;
struct TOKEN corpus_token_count_array[MAXLIST];
int c_freqs;
char *all_tokens[MAXLIST];

void plotGraph1(const char gg[]) {
    FILE * pipe = popen ("gnuplot -persistent", "w");	
	fprintf(pipe,"set style data histograms\n");
    fprintf(pipe,"set style fill solid\n");
    fprintf(pipe,"plot './output/PlagiarismScores.txt' using 2:xtic(1) title 'Plagiarism Scores' lt rgb '#406090'\n");
	fprintf(pipe,"set term png\n");
	fprintf(pipe,"set output '%s'\n",gg);	
	fprintf(pipe,"replot\n");
}

void plotGraph2(const char gg[]) {
    FILE * pipe = popen ("gnuplot -persistent", "w");	
	fprintf(pipe,"set style data histograms\n");
    fprintf(pipe,"set style fill solid\n");
    fprintf(pipe,"plot './output/TimeTaken.txt' using 2:xtic(1) title 'Time Taken' lt rgb '#406090'\n");
	fprintf(pipe,"set term png\n");
	fprintf(pipe,"set output '%s'\n",gg);	
	fprintf(pipe,"replot\n");
}

int endswith(char *s) {
	size_t size = strlen(s);
	if(s[size-4] == '.' && s[size-3] == 't' && s[size-2] == 'x' && s[size-1] == 't') {
		return 1;
	}
	return 0;
}

// void display_tokens(char *tokens[], int size) {
// 	for(int i = 0; i < size; i++) {
// 		printf("%s\n", tokens[i]);
// 	}
// }

// void display_frequency(struct TOKEN token_count_array[], int size) {
// 	for(int i = 0; i < size; i++) {
// 		printf("%s : ", token_count_array[i].word);
// 		printf("%d\n", token_count_array[i].count);
// 	}
// }
 
int main(int argc, char *argv[]) {
	test_file = argv[1];
	corpus_folder = argv[2];
	DIR *corpus;
	struct dirent *dir_object;
	char corpus_file[100];

	num_stopwords_tokens = tokenize(stopwords_tokens, "include/stopwords.txt");
	//display_tokens(stopwords_tokens, num_stopwords_tokens);
	printf("test file: %s\n\n", test_file);
	num_test_tokens =  tokenize(test_tokens, test_file);
	//display_tokens(test_tokens, num_test_tokens);
	t_freqs = getFrequency(test_tokens, num_test_tokens, test_token_count_array);
	//display_frequency(test_token_count_array, t_freqs);

	FILE *scores = NULL;
	FILE *time = NULL;
    scores = fopen("./output/PlagiarismScores.txt","w");
    time = fopen("./output/TimeTaken.txt","w");
    double tokenize_test_time = 0.0, ngram_test_time = 0.0, cosine_test_time = 0.0;
	clock_t start, end;

	if((corpus = opendir(corpus_folder)) != NULL) {
		while ((dir_object = readdir (corpus)) != NULL) {
			if(endswith((char *)dir_object->d_name) == 1) {
				strcpy(corpus_file, corpus_folder);
			 	strcat(corpus_file, "/");
			 	strcat(corpus_file, (char *)dir_object->d_name);
			 	printf("%s\n", corpus_file);
				num_corpus_tokens = tokenize(corpus_tokens, corpus_file);
				//display_tokens(corpus_tokens, num_corpus_tokens);
				c_freqs = getFrequency(corpus_tokens, num_corpus_tokens, corpus_token_count_array);
				//display_frequency(corpus_token_count_array, c_freqs);
				start = clock();
				float tokenize_score = tokenize_test();
				end = clock();
				tokenize_test_time += (double)(end - start)/(double)(CLOCKS_PER_SEC);
				//printf("tokenize_score: %f/10\n", tokenize_score);
				start = clock();
				float ng_score = ngram_test();
				end = clock();
				ngram_test_time += (double)(end - start)/(double)(CLOCKS_PER_SEC);
				//printf("ngram_score: %f/10\n", ng_score);
				start = clock();
				float cos_score = cosine_test();
				end = clock();
				cosine_test_time += (double)(end - start)/(double)(CLOCKS_PER_SEC);
				//printf("cosine_score: %f/10\n", cos_score);

				float similarity_percentage = (3*tokenize_score + 4*ng_score + 3*cos_score);
				printf("similarity percentage: %f\n", similarity_percentage);
				fprintf(scores, "%s", dir_object->d_name);
				fprintf(scores, " %f\n", similarity_percentage);
				printf("\n");
			}
		}

	}
	fprintf(time, "%s %f\n", "tokenizetesttime", tokenize_test_time);
	fprintf(time, "%s %f\n", "ngramtesttime", ngram_test_time);
	fprintf(time, "%s %f\n", "cosinetesttime", ngram_test_time);
	plotGraph2("./output/TimeTaken");
	plotGraph1("./output/PlagiarismScores");
	return 0;
}