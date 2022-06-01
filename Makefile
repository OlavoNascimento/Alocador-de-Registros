# Nome do executável
EXECUTABLE=regalloc
CC = gcc

COMMONFLAGS = -fstack-protector-all -O2 -I .
CFLAGS = $(COMMONFLAGS)
CPPFLAGS = -std=c99 $(COMMONFLAGS)
BISONFLAGS =

SOURCES = $(wildcard *.c)
LEXER_FILE=lexer.y.c
PARSER_FILE=parser.tab.c

OBJECTS = $(PARSER_FILE:.c=.o) $(LEXER_FILE:.c=.o) $(SOURCES:.c=.o)

# Executar em modo de release.
all: $(EXECUTABLE)

# Executar em modo de debug.
debug: CFLAGS += -W -Wall -g -D REGALLOC_DEBUG
debug: CPPFLAGS += -W -Wall -g -D REGALLOC_DEBUG
debug: BISONFLAGS += -Wconflicts-sr -Wcounterexamples -t
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Criando executável..."
	$(CC) -o $@ $^

# Específica que cada arquivo .o deve ser gerado a partir de seu correspondente .c
%.o: %.c
# $@ é o alvo atual (arquivo .o)
# $^ representa a dependência atual (arquivo .c)
	@echo "Compilando $^..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

# Gera o código do lexer.
$(LEXER_FILE): lexer.l $(PARSER_FILE)
	@echo "Gerando $@..."
	flex lexer.l

# Gera o código do parser.
$(PARSER_FILE): parser.y
	@echo "Gerando $@..."
	bison $(BISONFLAGS) -d $<

clean:
	rm -f $(EXECUTABLE) $(PARSER_FILE) $(PARSER_FILE:.c=.h) stack.hh $(LEXER_FILE) *.o vgcore.*

