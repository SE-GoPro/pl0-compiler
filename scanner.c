#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "scanner.h"

FILE* f;
TokenType token;
int number;
char ident[MAX_IDENT_LEN + 1];

int c;

TokenType checkKeyword(char* str) {
  for (int i = 0; i < KEYWORDS_CNT; i++) {
    if (strcmp(str, keywordMap[i].value) == 0) return keywordMap[i].token;
  }
  return IDENT;
}

int fGetCh() {
  return fgetc(f);
}

TokenType getToken() {
  int idx = 0;
  int digitCnt = 0;
  int isComment = 0;
  ident[0] = '\0';
  number = 0;

  if (isspace(c)) while(isspace(c = fGetCh()));
  
  if (isalpha(c)) {
    do {
      if (idx == MAX_IDENT_LEN) {
        ident[idx] = '\0';
        printf("ERROR: Name %s... is too long", ident);
        return NONE;
      }
      ident[idx++] = toupper(c);
    } while (isalnum(c = fGetCh()));
    ident[idx] = '\0';
    return checkKeyword(ident);
  };
  
  if (isdigit(c)) {
    do {
      if (digitCnt > MAX_NUMBER_LEN) {
        printf("ERROR: Number %d... is too large", number);
        return NONE;
      }
      digitCnt++;
      number = 10 * number + (int) (c - '0');
    } while (isdigit(c = fGetCh()));
    return NUMBER;
  };
  
  if (c == '+') { c = fGetCh(); return PLUS; };
  
  if (c == '-') { c = fGetCh(); return MINUS; };
  
  if (c == '*') { c = fGetCh(); return TIMES; };
  
  if (c == '/') { c = fGetCh(); return SLASH; };
   
  if (c == '>') {
    c = fGetCh();
    if (c == '=') { c = fGetCh(); return GEQ; } else return GTR;
  };

  if (c == '<') {
    c = fGetCh();
    if (c == '=') { c = fGetCh(); return LEQ; }
    else if (c == '>') { c = fGetCh(); return NEQ; }
    else return LSS;
  };

  if (c == '=') { c = fGetCh(); return EQU; };
  
  if (c == '(') {
    c = fGetCh();
    if (c == '*') {
      isComment = 1;
      c = fGetCh();
      while (isComment) {
        if (c == '*') {
          c = fGetCh();
          if (c == ')') {
            isComment = 0;
            c = fGetCh();
          }
        } else {
          c = fGetCh();
        }
      }
      return COMMENT;
    } else {
      return LPARENT;
    }
  };

  if (c == ')') { c = fGetCh(); return RPARENT; };

  if (c == '[') { c = fGetCh(); return LBRACK; };
  
  if (c == ']') { c = fGetCh(); return RBRACK; };

  if (c == '.') { c = fGetCh(); return PERIOD; };

  if (c == ',') { c = fGetCh(); return COMMA; };

  if (c == ';') { c = fGetCh(); return SEMICOLON; };

  if (c == ':') {
    c = fGetCh();
    if (c == '=') { c = fGetCh(); return ASSIGN; }
    else {
      printf("ERROR: Unkown symbol ':'");
      return NONE;
    }
  }

  if (c == '%') { c = fGetCh(); return PERCENT; }

  if (!isspace(c) && !isalnum(c)
    && c != '+' && c != '-' && c != '*' && c != '/'
    && c != '>' && c != '<' && c != '='
    && c != '(' && c != ')' && c != '[' && c != ']'
    && c != '.' && c != ',' && c != ';'
    && c != ':' && c != '%' && c != EOF) {
      printf("ERROR: Unknown symbol '%c'", c);
      return NONE;
    }

  return NONE;
}

void lex(const char* fileName) {
  if ((f = fopen(fileName, "rt")) == NULL) {
    printf("NotFound: No such file: %s", fileName);
  } else {
    c = ' ';
    do {
      token = getToken();
      if (token != NONE && token != COMMENT) {
        printf("%s", tokenKey[token]);
        if (token == IDENT) printf("(%s)\n", ident);
        else if (token == NUMBER) printf("(%d)\n", number);
        else printf("\n");
      }
    } while (token != NONE);
  }
  fclose(f);
}
