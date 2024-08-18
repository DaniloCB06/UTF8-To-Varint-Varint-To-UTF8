// Código original foi perdido então este código é apenas uma estipulação de como deveria ser
#ifndef CONVERTE_H
#define CONVERTE_H

#include <stdio.h>

// Funções para contar o número de bytes em UTF-8 e Varint
int contaBytes_UTF8(unsigned char primeiro_byte);
int contaBytes_Varint(unsigned int valor);

// Funções para converter UTF-8 em Varint
unsigned int utf8tovarint2(unsigned int n);
unsigned int utf8tovarint3(unsigned int n);
unsigned int utf8tovarint4(unsigned int n);

// Função para converter Varint em UTF-8
unsigned int varinttoutf(unsigned int n, int quantidadByte);

// Funções para converter arquivos de UTF-8 para Varint e vice-versa
int utf2varint(FILE *arq_entrada, FILE *arq_saida);
int varint2utf(FILE *arq_entrada, FILE *arq_saida);

#endif
