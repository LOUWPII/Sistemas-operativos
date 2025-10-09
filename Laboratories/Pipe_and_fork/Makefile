# 1. Definir el compilador (Variable estándar de Make para el compilador C)
CC = gcc

# 2. Definir los ejecutables
EJECUTABLES = lab01 lab02 lab03

# Regla por defecto: compila todos los ejecutables
all: $(EJECUTABLES)

# 3. Reglas explícitas de compilación usando $(CC)

lab01: lab01.c
	$(CC) -o lab01 lab01.c -Wall

lab02: lab02.c
	$(CC) -o lab02 lab02.c -Wall

lab03: lab03.c
	$(CC) -o lab03 lab03.c -Wall

.PHONY: all
