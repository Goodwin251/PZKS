#pragma once
#include "Lexer.h"

// Клас синтаксичного аналізатора (кінцевий автомат)

void Lexer::skipWhitespace()
{
  while (pos < expression.length() && isspace(expression[pos])) {
      pos++;
  }
}

MyToken Lexer::getNextToken()
{
    skipWhitespace();

    if (pos >= expression.length()) {
      return MyToken(END, "", pos);
    }

    int startPos = pos;

    // Розпізнавання чисел
    if (isdigit(expression[pos])) {
      string number;
      bool hasDot = false;

      while (pos < expression.length() &&
        (isdigit(expression[pos]) || expression[pos] == '.')) {
        if (expression[pos] == '.') {
          if (hasDot) break; // Друга крапка - помилка
          hasDot = true;
        }
        number += expression[pos++];
      }

      return MyToken(NUMBER, number, startPos);
    }

    // Розпізнавання змінних та функцій
    if (isalpha(expression[pos])) {
      string identifier;

      while (pos < expression.length() &&
        (isalnum(expression[pos]) || expression[pos] == '_')) {
        identifier += expression[pos++];
      }

      // Перевірка чи це функція
      if (functions.find(identifier) != functions.end()) {
        return MyToken(FUNCTION, identifier, startPos);
      }

      return MyToken(VARIABLE, identifier, startPos);
    }

    // Розпізнавання операторів та дужок
      char ch = expression[pos++];
      switch (ch) {
      case '+': return MyToken(PLUS, "+", startPos);
      case '-': return MyToken(MINUS, "-", startPos);
      case '*': return MyToken(MULTIPLY, "*", startPos);
      case '/': return MyToken(DIVIDE, "/", startPos);
      case '(': return MyToken(LPAREN, "(", startPos);
      case ')': return MyToken(RPAREN, ")", startPos);
      default:  return MyToken(INVALID, string(1, ch), startPos);
    }
}

vector<MyToken> Lexer::tokenize()
{
  vector<MyToken> tokens;
  pos = 0;

  while (true) {
    MyToken token = getNextToken();
    tokens.push_back(token);
    if (token.type == END || token.type == INVALID) break;
  }

  return tokens;
}