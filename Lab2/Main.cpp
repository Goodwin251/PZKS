#include "ExpressionTree.h"
#include <iostream>
#include <string>
#include "Windows.h"

using namespace std;

bool isValidExpression(const string& expr) {
  int brackets = 0;
  bool lastWasOperator = true;

  for (size_t i = 0; i < expr.length(); i++) {
    char c = expr[i];

    if (isspace(c)) continue;

    if (c == '(') {
      brackets++;
      lastWasOperator = true;
    }
    else if (c == ')') {
      brackets--;
      if (brackets < 0) return false;
      lastWasOperator = false;
    }
    else if (c == '+' || c == '*' || c == '/') {
      if (lastWasOperator) return false;
      lastWasOperator = true;
    }
    else if (c == '-') {
      lastWasOperator = true;
    }
    else if (isalnum(c) || c == '.') {
      lastWasOperator = false;
    }
    else {
      return false;
    }
  }

  return brackets == 0 && !lastWasOperator;
}

int main() {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  cout << "Лабораторна робота №2: Автоматичне розпаралелювання арифметичного виразу" << endl;
  cout << "Для виходу введіть 'exit'" << endl << endl;

  while (true) {
    string expression;
    cout << "Введіть арифметичний вираз: ";
    getline(cin, expression);

    if (expression == "exit") {
      break;
    }

    if (expression.empty()) {
      continue;
    }

    if (!isValidExpression(expression)) {
      cout << "Помилка: вираз '" << expression << "' не є валідним." << endl << endl;
      continue;
    }

    cout << "Вхідний вираз '" << expression << "' є валідним." << endl;

    try {
      ExpressionTree tree(expression);
      tree.display();
    }
    catch (const exception& e) {
      cout << "Помилка при обробці виразу: " << e.what() << endl;
    }

    cout << endl;
  }

  return 0;
}