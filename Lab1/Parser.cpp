#include "Parser.h"

bool Parser::parse()
{
  errors.clear();
  currentState = START;
  currentPos = 0;
  openParenCount = 0;
  closeParenCount = 0;

  // Перевірка на невалідні символи
  for (const auto& token : tokens) {
    if (token.type == INVALID) {
      errors.push_back("Помилка: невалідний символ '" + token.value +
        "' на позиції " + to_string(token.position));
      return false;
    }
  }

  // Перевірка порожнього виразу
  if (tokens.empty() || (tokens.size() == 1 && tokens[0].type == END)) {
    errors.push_back("Помилка: порожній вираз");
    return false;
  }

  // Автомат
  while (currentPos < tokens.size() && currentState != MYERROR) {
    MyToken currentToken = tokens[currentPos];

    switch (currentState) {
    case START:
      if (!processStart(currentToken)) return false;
      break;

    case AFTER_NUMBER:
      if (!processAfterNumber(currentToken)) return false;
      break;

    case AFTER_OPERATOR:
      if (!processAfterOperator(currentToken)) return false;
      break;

    case AFTER_LPAREN:
      if (!processAfterLParen(currentToken)) return false;
      break;

    case AFTER_FUNCTION:
      if (!processAfterFunction(currentToken)) return false;
      break;

    default:
      break;
    }

    currentPos++;
  }

  // Перевірка кінцевого стану
  if (currentState != AFTER_NUMBER) {
    errors.push_back("Помилка: вираз не може закінчуватись оператором або відкритою дужкою");
    return false;
  }

  // Перевірка дужок
  if (openParenCount != closeParenCount) {
    errors.push_back("Помилка: кількість відкритих дужок (" +
      to_string(openParenCount) + ") не дорівнює кількості закритих (" +
      to_string(closeParenCount) + ")");
    return false;
  }

  currentState = ACCEPT;
  return true;
}

bool Parser::processStart(const MyToken& token)
{
  switch (token.type) {
  case NUMBER:
  case VARIABLE:
    currentState = AFTER_NUMBER;
    return true;

  case LPAREN:
    openParenCount++;
    currentState = AFTER_LPAREN;
    return true;

  case FUNCTION:
    currentState = AFTER_FUNCTION;
    return true;

  case MINUS:
  case PLUS:
    // Унарний мінус/плюс на початку
    currentState = AFTER_OPERATOR;
    return true;

  case MULTIPLY:
  case DIVIDE:
    errors.push_back("Помилка: вираз не може починатись з оператора '" +
      token.value + "'");
    currentState = MYERROR;
    return false;

  case RPAREN:
    errors.push_back("Помилка: вираз не може починатись з закритої дужки");
    currentState = MYERROR;
    return false;

  case END:
    errors.push_back("Помилка: порожній вираз");
    currentState = MYERROR;
    return false;

  default:
    errors.push_back("Помилка: неочікуваний токен на початку виразу");
    currentState = MYERROR;
    return false;
  }
}

bool Parser::processAfterNumber(const MyToken& token)
{
  switch (token.type) {
  case PLUS:
  case MINUS:
  case MULTIPLY:
  case DIVIDE:
    currentState = AFTER_OPERATOR;
    return true;

  case RPAREN:
    closeParenCount++;
    if (closeParenCount > openParenCount) {
      errors.push_back("Помилка: зайва закрита дужка на позиції " +
        to_string(token.position));
      currentState = MYERROR;
      return false;
    }
    currentState = AFTER_NUMBER;
    return true;

  case END:
    return true;

  case LPAREN:
    errors.push_back("Помилка: відсутній оператор перед відкритою дужкою на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;

  case NUMBER:
  case VARIABLE:
    errors.push_back("Помилка: відсутній оператор між операндами на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;

  default:
    errors.push_back("Помилка: неочікуваний токен після операнда");
    currentState = MYERROR;
    return false;
  }
}

bool Parser::processAfterOperator(const MyToken& token)
{
  switch (token.type) {
  case NUMBER:
    if (tokens[currentPos - 1].type == DIVIDE && token.value == "0")
    {
      errors.push_back("Помилка: неможливо ділити на нуль " +
        to_string(token.position));
      currentState = MYERROR;
      return false;
    }

  case VARIABLE:
    currentState = AFTER_NUMBER;
    return true;

  case LPAREN:
    openParenCount++;
    currentState = AFTER_LPAREN;
    return true;

  case FUNCTION:
    currentState = AFTER_FUNCTION;
    return true;

  case PLUS:
  case MINUS:
  case MULTIPLY:
  case DIVIDE:
    errors.push_back("Помилка: подвійний оператор на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;

  case RPAREN:
    errors.push_back("Помилка: оператор перед закритою дужкою на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;

  case END:
    errors.push_back("Помилка: вираз не може закінчуватись оператором");
    currentState = MYERROR;
    return false;

  default:
    errors.push_back("Помилка: неочікуваний токен після оператора");
    currentState = MYERROR;
    return false;
  }
}

bool Parser::processAfterLParen(const MyToken& token)
{
  switch (token.type) {
  case NUMBER:
  case VARIABLE:
    currentState = AFTER_NUMBER;
    return true;

  case LPAREN:
    openParenCount++;
    currentState = AFTER_LPAREN;
    return true;

  case FUNCTION:
    currentState = AFTER_FUNCTION;
    return true;

  case MINUS:
  case PLUS:
    // Унарний оператор після дужки
    currentState = AFTER_OPERATOR;
    return true;

  case MULTIPLY:
  case DIVIDE:
    errors.push_back("Помилка: оператор '" + token.value +
      "' після відкритої дужки на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;

  case RPAREN:
    errors.push_back("Помилка: порожні дужки на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;

  case END:
    errors.push_back("Помилка: незакрита дужка");
    currentState = MYERROR;
    return false;

  default:
    errors.push_back("Помилка: неочікуваний токен після відкритої дужки");
    currentState = MYERROR;
    return false;
  }
}

bool Parser::processAfterFunction(const MyToken& token)
{
  if (token.type == LPAREN) {
    openParenCount++;
    currentState = AFTER_LPAREN;
    return true;
  }
  else {
    errors.push_back("Помилка: очікується '(' після функції на позиції " +
      to_string(token.position));
    currentState = MYERROR;
    return false;
  }
}

void Parser::printErrors() const
{
  for (const auto& error : errors) {
    cout << error << endl;
  }
}

vector<string> Parser::getErrors() const
{
  return errors;
}
