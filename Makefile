all: compile move

compile:
	@ gcc -c src/fileParser.c
	@ gcc -c src/tokenize_test.c
	@ gcc -c src/ngram_test.c
	@ gcc -c src/cosine_test.c
	@ gcc -c src/PlagiarismChecker.c
	@ gcc fileParser.o tokenize_test.o ngram_test.o cosine_test.o PlagiarismChecker.o -o plagChecker -lm

move:
	@ mv fileParser.o obj
	@ mv tokenize_test.o obj
	@ mv ngram_test.o obj
	@ mv cosine_test.o obj
	@ mv PlagiarismChecker.o obj

run:
	@ ./plagChecker test_file.txt corpus_files

clean:
	@ rm -rf ./obj/*.o
	@ rm -rf plagChecker
	@ rm -rf ./output/PlagiarismScores.txt
	@ rm -rf ./output/PlagiarismScores
	@ rm -rf ./output/TimeTaken.txt
	@ rm -rf ./output/TimeTaken