CXX=clang++-3.5 -stdlib=libstdc++
INC=-I./src -I./src/gen-cpp2 -I./external/gtest-1.7.0-min/include -I./external/gmock-1.7.0/include
CPPFLAGS=$(INC) --std=c++14 -O0
LINK=-lmitie -lthrift -lthriftcpp2 -lfolly -lglog -latomic -pthread
TEST_OBJ = $(addprefix ./src/test/, \
		test_MitieTokenizer.o \
		test_MitieNerProcessor.o \
		test_NerThriftService.o \
		runner.o \
	)

LIB_OBJ = $(addprefix ./src/, \
		NerThriftService.o \
		MitieNerProcessor.o \
		MitieEntityExtractor.o \
		MitieTokenizer.o \
	)

MAIN_OBJ = ./src/main.o $(LIB_OBJ)

THRIFT_ITEMS = $(addprefix ./src/gen-cpp2/, \
	NerTagger \
	NerTagger_client \
	NerTagger_processmap_binary \
	NerTagger_processmap_compact \
	NerTagger_types \
)
THRIFT_SRC = $(addsuffix .cpp, $(THRIFT_ITEMS))
THRIFT_OBJ = $(addsuffix .o, $(THRIFT_ITEMS))


%.o:%.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

runner: $(MAIN_OBJ) $(THRIFT_OBJ)
	$(CXX) $(CPPFLAGS) $(MAIN_OBJ) $(THRIFT_OBJ) -o $@ $(LINK)

GTEST_LIB = ./external/gtest-1.7.0-min/gtest-all.o
GMOCK_LIB = ./external/gmock-1.7.0/src/gmock-all.o

test_runner: $(LIB_OBJ) $(THRIFT_OBJ) $(TEST_OBJ)
	$(CXX) $(CPPFLAGS) $(TEST_OBJ) $(LIB_OBJ) $(THRIFT_OBJ) -o $@ $(GMOCK_LIB) $(GTEST_LIB) $(LINK)

run: runner
	./runner ./test_data/MITIE-models/english/ner_model.dat ./test_data/cnn_article.txt

clean:
	rm -f runner src/*.o src/test/*.o

.PHONY: clean run

$(THRIFT_SRC): ./src/NerTagger.thrift
	python -m thrift_compiler.main -o src --gen cpp2 src/NerTagger.thrift

services: $(THRIFT_SRC)

py-services:
	thrift1 -o src --gen py src/NerTagger.thrift

.PHONY: py-services

