#pragma once
#include "Lexer.h"
#include "Parser.h"

class Tester {
public:
  void static TestLab1() {
    vector<string> testExpressions = {
      // Валідні вирази
      "3.15 + 4545.313",
      "3 + 5",
      "x * (y + 2)",
      "sin(x) + cos(y)",
      "2 * (3 + 4) / 5",
      "sqrt(a) - log(b)",
      "(x + y) * (z - w)",
      "3 + 5 * (2 - 4)",
      "x / (y - z) * a",
      "abs(-5) + round(4.67)",
      "log(10) + sqrt(16)",

      // Невалідні вирази - починаються з невалідних символів
      "* 3 + 5",
      "/ 2 - 1",
      ") x + 5",
      "+ 7 * 8",
      "- (3 + 5)",

      // Невалідні вирази - закінчуються невалідними символами
      "3 + 5 *",
      "(x - y /",
      "sqrt(16) +",
      "(a + b -",

      // Невалідні вирази - неспівпадаючі дужки
      "3 + (4 - 5",
      "(2 + 3)) * 7",
      "(x * (y + z)",
      "((a - b) + c",
      "log(10 + sqrt(25)) + abs(x",

      // Невалідні вирази - порожні дужки
      "x * () + 5",
      "sin() + cos(y)",
      "(3 + ) * 4",
      "log( + b)"
    };

    cout << "=== АНАЛІЗАТОР АРИФМЕТИЧНИХ ВИРАЗІВ ===" << endl << endl;

    for (size_t i = 0; i < testExpressions.size(); i++) {
      string expr = testExpressions[i];
      cout << "Вираз #" << (i + 1) << ": \"" << expr << "\"" << endl;

      // Лексичний аналіз
      Lexer lexer(expr);
      vector<MyToken> tokens = lexer.tokenize();

      cout << "Токени: ";
      for (const auto& token : tokens) {
        if (token.type == END) break;
        cout << "[" << token.value << "] ";
      }
      cout << endl;

      // Синтаксичний аналіз
      Parser parser(tokens);
      bool isValid = parser.parse();

      if (isValid) {
        cout << "Результат: ВАЛІДНИЙ" << endl;
      }
      else {
        cout << "Результат: НЕВАЛІДНИЙ" << endl;
        parser.printErrors();
      }

      cout << "----------------------------------------" << endl << endl;
    }
	}
};