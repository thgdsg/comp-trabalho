CXX = g++
CXXFLAGS = -std=c++11 -Wall

target: etapa5

etapa5: lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o tac.o
	$(CXX) $(CXXFLAGS) lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o tac.o -o etapa5

*.o: *.cpp
	$(CXX) $(CXXFLAGS) $< -c

lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l

parser.tab.hpp: parser.tab.cpp
	bison -d -o parser.tab.cpp parser.ypp

parser.tab.cpp: parser.ypp
	bison parser.ypp -d -v

clean:
	rm etapa5 lex.yy.cpp parser.tab.cpp parser.tab.hpp parser.output *.o