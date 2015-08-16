CXX=clang++-3.5 -stdlib=libstdc++
CPPFLAGS=-I./src -I./src/gen-cpp2 --std=c++14 -O0
LINK=-lmitie -lthrift -lthriftcpp2 -lfolly -lglog -latomic -pthread

OBJ = $(addprefix ./src/, \
		main.o \
		MitieNerProcessor.o \
	)

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

runner: $(OBJ) $(THRIFT_OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) $(THRIFT_OBJ) -o $@ $(LINK)

run: runner
	./runner ./test_data/MITIE-models/english/ner_model.dat ./test_data/cnn_article.txt

clean:
	rm -f runner src/*.o

.PHONY: clean run

$(THRIFT_SRC): ./src/NerTagger.thrift
	python -m thrift_compiler.main -o src --gen cpp2 src/NerTagger.thrift

services: $(THRIFT_SRC)

py-services:
	thrift1 -o src --gen py src/NerTagger.thrift

.PHONY: py-services

