#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <set>
#include <sstream>
#include <Windows.h>

using namespace std;
enum MyTokenType {
  NUMBER,      // Числа (константи)
  VARIABLE,    // Змінні
  FUNCTION,    // Функції (sin, cos, sqrt, log, abs, round)
  PLUS,        // +
  MINUS,       // -
  MULTIPLY,    // *
  DIVIDE,      // /
  LPAREN,      // (
  RPAREN,      // )
  END,         // Кінець виразу
  INVALID      // Невалідний токен
};

// Структура токену
struct MyToken {
  MyTokenType type;
  string value;
  int position;

  MyToken(MyTokenType t, string v, int p) : type(t), value(v), position(p) {}
};


class Lexer {
private:
  string expression;
  size_t pos;
  set<string> functions;

public:
  Lexer(const string& expr) : expression(expr), pos(0) {
    functions = { "sin", "cos", "sqrt", "log", "abs", "round", "func1" };
  }
  // Пропустити пробіли
  void skipWhitespace();

  // Отримати наступний токен
  MyToken getNextToken();

  // Отримати всі токени
  vector<MyToken> tokenize();
};