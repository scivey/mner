CXX=clang++-3.5 -stdlib=libstdc++
CPPFLAGS=-I./src --std=c++14 -O0
LINK=-lmitie -lfolly -lglog -latomic -pthread

OBJ = $(addprefix ./src/, \
		main.o \
		MitieNerProcessor.o \
	)

%.o:%.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

runner: $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $@ $(LINK)

run: runner
	./runner ./test_data/MITIE-models/english/ner_model.dat ./test_data/cnn_article.txt

clean:
	rm -f runner src/*.o

.PHONY: clean run
