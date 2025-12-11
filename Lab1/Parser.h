#pragma once
#include "Lexer.h"
class Parser {
private:
  vector<MyToken> tokens;
  size_t currentPos;
  vector<string> errors;

  // Стани автомату
  enum State {
    START,           // Початковий стан
    AFTER_NUMBER,    // Після числа/змінної
    AFTER_OPERATOR,  // Після оператора
    AFTER_LPAREN,    // Після відкритої дужки
    AFTER_FUNCTION,  // Після функції
    ACCEPT,           // Кінцевий стан (вираз валідний)
    MYERROR          // Стан помилки
  };

  State currentState;
  int openParenCount;
  int closeParenCount;

public:
  Parser(const vector<MyToken>& toks) : tokens(toks), currentPos(0),
    currentState(START),
    openParenCount(0),
    closeParenCount(0) {
  }

  // Основний метод аналізу
  bool parse();
private:
  bool processStart(const MyToken& token);

  bool processAfterNumber(const MyToken& token);

  bool processAfterOperator(const MyToken& token);

  bool processAfterLParen(const MyToken& token);

  bool processAfterFunction(const MyToken& token);

public:
  void printErrors() const;

  vector<string> getErrors() const;
};
