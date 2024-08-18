#include "converte.h"
#include <stdio.h>
#include <stdlib.h>

/* Danilo Barbosa 2211700 3WA */
/* João Henrique  2210993 3WA */

int contaBytes_UTF8(unsigned char primeiro_byte) {
  if ((primeiro_byte & 0x80) == 0) {
    // Início com "0" um caractere de 1 byte
    return 1;
  } else if ((primeiro_byte & 0xE0) == 0xC0) {
    // Início com "110" um caractere de 2 bytes
    return 2;
  } else if ((primeiro_byte & 0xF0) == 0xE0) {
    // Início com "1110" um caractere de 3 bytes
    return 3;
  } else if ((primeiro_byte & 0xF8) == 0xF0) {
    // Início com "11110" um caractere de 4 bytes
    return 4;
  } else {
    return 0;
  }
}

int contaBytes_Varint(unsigned int valor) {
  if (valor < 256) {
    return 1; // 1 byte
  } else if (valor < 65536) {
    return 2; // 2 byte
  } else if (valor < 16777216) {
    return 3; // 3 byte
  } else if (valor < 4294967295) {
    return 4; // 4 byte
  } else {
    // Valor muito grande, não é um Varint válido
    return 0;
  }
}

// Função auxiliar para converter um UTF-8 de tamanho 2 em Varint
unsigned int utf8tovarint2(unsigned int n) {
  unsigned short result = 0x0;
  unsigned char segundoB;
  unsigned char primeiroB;
  unsigned char verificacaoByte;
  unsigned int primeiroBAux;
  unsigned int segundoBAux;

  primeiroBAux = n << 19;
  primeiroBAux = primeiroBAux >> 27;
  primeiroB = primeiroBAux;
  segundoBAux = n << 26;
  segundoBAux = segundoBAux >> 26;
  segundoB = segundoBAux;

  verificacaoByte = primeiroB << 6;
  segundoB |= verificacaoByte;

  primeiroB = primeiroB >> 1;

  segundoB |= 0b10000000;

  result |= segundoB;
  result = result << 8;
  result |= primeiroB;
  return result;
}

// Função auxiliar para converter um UTF-8 de tamanho 3 em Varint
unsigned int utf8tovarint3(unsigned int n) {
  unsigned int result = 0x0;
  unsigned int aux1 = 0;
  unsigned int aux2 = 0;
  unsigned char terceiroB;
  unsigned char segundoB;
  unsigned char primeiroB;
  unsigned char verificaoByte1 = 0;
  unsigned char verificaoByte2 = 0;
  unsigned int primeiroBAux;
  unsigned int segundoBAux;
  unsigned int terceiroBAux;

  primeiroBAux = n << 12;
  primeiroBAux = primeiroBAux >> 28;
  primeiroB = primeiroBAux;

  segundoBAux = n << 18;
  segundoBAux = segundoBAux >> 26;
  segundoB = segundoBAux;

  terceiroBAux = n << 25;
  terceiroBAux = terceiroBAux >> 25;
  terceiroB = terceiroBAux;

  verificaoByte1 = primeiroB << 6;
  segundoB |= verificaoByte1;

  primeiroB = primeiroB >> 2;

  verificaoByte2 = segundoB << 7;
  verificaoByte2 >>= 1;
  terceiroB |= verificaoByte2;
  segundoB = segundoB >> 1;

  if (primeiroB == 0) {
    aux1 |= terceiroB;
    aux1 <<= 8;
    aux1 |= 0x8000;

    aux2 |= segundoB;
    result |= aux1;
    result |= aux2;

    return result;
  }
  aux1 |= terceiroB;
  aux1 <<= 16;
  aux1 |= 0x800000;

  aux2 |= segundoB;
  aux2 <<= 8;
  aux2 |= 0x8000;

  result |= aux1;
  result |= aux2;
  result |= primeiroB;

  return result;
}

// Função auxiliar para converter um UTF-8 de tamanho 4 em Varint
unsigned int utf8tovarint4(unsigned int n) {
  unsigned int result = 0x0;
  unsigned int aux1 = 0;
  unsigned int aux2 = 0;
  unsigned int aux3 = 0;
  unsigned char quartoB;
  unsigned char terceiroB;
  unsigned char segundoB;
  unsigned char primeiroB;
  unsigned char verificaoByte1 = 0;
  unsigned char verificaoByte2 = 0;
  unsigned char verificaoByte3 = 0;
  unsigned int primeiroBAux;
  unsigned int segundoBAux;
  unsigned int terceiroBAux;
  unsigned int quartoBAux;

  primeiroBAux = n << 5;
  primeiroBAux = primeiroBAux >> 29;
  primeiroB = primeiroBAux;

  segundoBAux = n << 10;
  segundoBAux = segundoBAux >> 26;
  segundoB = segundoBAux;

  terceiroBAux = n << 18;
  terceiroBAux = terceiroBAux >> 26;
  terceiroB = terceiroBAux;

  quartoBAux = n << 26;
  quartoBAux = quartoBAux >> 26;
  quartoB = quartoBAux;

  verificaoByte3 = terceiroB << 6;
  quartoB |= verificaoByte3;
  terceiroB >>= 1;

  verificaoByte2 = segundoB << 6;
  verificaoByte2 >>= 1;
  terceiroB |= verificaoByte2;
  segundoB >>= 2;

  verificaoByte1 = primeiroB << 4;
  segundoB |= verificaoByte1;
  primeiroB >>= 3;

  aux1 = quartoB;
  aux1 <<= 16;
  aux1 |= 0x800000;

  aux2 = terceiroB;
  aux2 <<= 8;
  aux2 |= 0x8000;

  result |= aux1;
  result |= aux2;
  result |= segundoB;

  result |= aux3;
  result |= aux1;
  result |= aux2;
  result |= primeiroB;

  return result;
}

// Função auxiliar para converter um Varint em um UTF-8
unsigned int varinttoutf(unsigned int n, int quantidadByte) {
  unsigned int result;
  unsigned char quartoB = 0x0;
  unsigned char terceiroB = 0x0;
  unsigned char segundoB;
  unsigned char primeiroB;
  unsigned char aux1;
  unsigned int primeiroBAux;
  unsigned int segundoBAux;
  unsigned int aux2;

  if (quantidadByte == 1) {
    result = n;
    return result;

  } else if (quantidadByte == 2) {
    primeiroB = n >> 8;
    primeiroB <<= 1;
    primeiroB >>= 1;

    segundoB = n;

    aux1 = primeiroB;
    aux1 >>= 6;

    segundoB <<= 1;
    segundoB |= aux1;
    primeiroB <<= 2;
    primeiroB >>= 2;

    aux1 = segundoB;
    aux1 >>= 5;

    if (aux1) {
      segundoB <<= 2;
      segundoB >>= 2;
      aux1 >>= 1;

      aux1 |= 0xE0;
      result = aux1;
      result <<= 16;

      aux2 = segundoB;
      aux2 |= 0x80;
      aux2 <<= 8;
      result |= aux2;

      aux2 = primeiroB;
      aux2 |= 0x80;
      result |= aux2;

      return result;
    }

    segundoB |= 0xC0;
    result = segundoB;
    result <<= 8;
    result |= primeiroB;
    result |= 0x80;

    return result;
  } else { // quantidadByte = 3
    primeiroBAux = n << 9;
    primeiroBAux >>= 25;
    primeiroB = primeiroBAux;

    segundoBAux = n << 17;
    segundoBAux >>= 25;
    segundoB = segundoBAux;

    terceiroB = n;

    aux1 = primeiroB;
    aux1 <<= 1;
    aux1 >>= 7;
    segundoB <<= 1;
    segundoB |= aux1;
    primeiroB <<= 2;
    primeiroB >>= 2;

    aux1 = terceiroB;
    aux1 <<= 1;
    aux1 >>= 3;
    quartoB = aux1;
    terceiroB <<= 4;
    terceiroB >>= 4;
    aux1 = segundoB;
    aux1 >>= 6;
    segundoB <<= 2;
    segundoB >>= 2;
    terceiroB <<= 2;
    terceiroB |= aux1;

    if (quartoB) {
      quartoB = terceiroB >> 5;
      terceiroB <<= 3;
      terceiroB >>= 3;
      quartoB |= 0xF0;
      primeiroB |= 0x80;
      segundoB |= 0x80;
      terceiroB |= 0x80;

      result = quartoB;
      result <<= 24;
      aux2 = terceiroB;
      aux2 <<= 16;
      result |= aux2;
      aux2 = segundoB;
      aux2 <<= 8;
      result |= aux2;
      result |= primeiroB;

      return result;
    }
    terceiroB |= 0xE0;
    primeiroB |= 0x80;
    segundoB |= 0x80;
    result = terceiroB;

    result <<= 16;
    aux2 = segundoB;
    aux2 <<= 8;
    result |= aux2;
    result |= primeiroB;

    return result;
  }
}

//======================================================================

int varint2utf(FILE *arq_entrada, FILE *arq_saida) {
  if (arq_entrada == NULL) {
    printf("Erro: Falha na abertura de arquivo de entrada.\n");
    return -1;
  }

  if (arq_saida == NULL) {
    printf("Erro: Falha na abertura de arquivo de saída.\n");
    return -1;
  }

  int ch;
  unsigned char chUns;
  unsigned char auxC;
  unsigned char varAux;
  unsigned int aux;
  unsigned int varint_value = 0x00;
  int num_bytes;
  int i = 0;

  while ((ch = fgetc(arq_entrada)) != EOF) {

    chUns = (unsigned char)ch;
    auxC = chUns >> 7;
    varint_value <<= 8;
    aux = chUns;
    if (auxC == 1) {

      varint_value |= aux;
      i++;
    } else {
      varint_value |= chUns;
      num_bytes = contaBytes_Varint(varint_value);

      unsigned int utf_value = varinttoutf(varint_value, num_bytes);
      for (int j = 3; j >= 0; j--) {
        varAux = (utf_value >> 8 * j);
        if (varAux != 0) {
          fputc(varAux, arq_saida);
        }
      }
      varint_value = 0;
      i = 0;
    }
  }
  return 0;
}

//======================================================================

//======================================================================

int utf2varint(FILE *arq_entrada, FILE *arq_saida) {
  if (arq_entrada == NULL) {
    printf("Erro: Falha na abertura de arquivo de entrada.\n");
    return -1;
  }

  if (arq_saida == NULL) {
    printf("Erro: Falha na abertura de arquivo de saída.\n");
    return -1;
  }
  char ch;
  unsigned int aux;
  unsigned char auxResult;
  unsigned int temp;
  unsigned int result;
  int verificadorUtf = 0;
  int verificaByteFinal;
  int qtdBytesPrimeiroByteUtf;
  int percorreInt;

  while ((ch = fgetc(arq_entrada)) != EOF) {

    int n_bytes = contaBytes_UTF8((unsigned char)ch);

    if (n_bytes > 1) {

      ch = (unsigned char)ch;
      temp = ch;
      qtdBytesPrimeiroByteUtf = n_bytes;
      temp <<= (8 * (n_bytes - 1));

      verificadorUtf = 1;
      if (n_bytes == 4) {
        percorreInt = n_bytes - 1;
      } else {
        percorreInt = n_bytes;
      }
      verificaByteFinal = n_bytes - 1;

    } else {
      if (verificadorUtf == 0) {
        fputc(ch, arq_saida);
      } else if (verificaByteFinal > 1) {
        aux = (unsigned char)ch;
        aux <<= 8 * (verificaByteFinal - 1);
        temp |= aux;
        verificaByteFinal--;
      } else {
        aux = (unsigned char)ch;
        temp |= aux;
        if (qtdBytesPrimeiroByteUtf == 4) {
          result = utf8tovarint4(temp);
        } else if (qtdBytesPrimeiroByteUtf == 3) {
          result = utf8tovarint3(temp);
        } else {
          result = utf8tovarint2(temp);
        }

        while (percorreInt > 0) {
          auxResult = result >> (8 * (percorreInt - 1));
          if (auxResult != 0) {
            fputc(auxResult, arq_saida);
          }
          percorreInt--;
        }
        verificaByteFinal = 0;
        verificadorUtf = 0;
      }
    }
  }
  return 0;
}
