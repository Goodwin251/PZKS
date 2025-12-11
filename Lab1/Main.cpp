#pragma once
#include "Tester.cpp"
// Головна функція для тестування
int main() {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  Tester::TestLab1();
  cout << "=== РЕЖИМ ВВЕДЕННЯ ВИРАЗІВ ===" << endl;
  cout << "Підтримувані операції: +, -, *, /" << endl;
  cout << "Підтримувані функції: sin, cos, sqrt, log, abs, round" << endl;
  cout << "Для виходу введіть 'exit' або 'вихід'" << endl << endl;

  while (true) {
    cout << "Введіть арифметичний вираз: ";
    string userExpr;
    getline(cin, userExpr);

    // Перевірка на вихід
    if (userExpr == "exit" || userExpr == "вихід" || userExpr == "EXIT") {
      cout << "Завершення роботи програми." << endl;
      break;
    }

    // Перевірка на порожній рядок
    if (userExpr.empty()) {
      cout << "Помилка: порожній вираз!" << endl << endl;
      continue;
    }

    cout << "\nАналіз виразу: \"" << userExpr << "\"" << endl;

    // Лексичний аналіз
    Lexer lexer(userExpr);
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
      cout << "Результат: ✓ ВАЛІДНИЙ ВИРАЗ" << endl;
    }
    else {
      cout << "Результат: ✗ НЕВАЛІДНИЙ ВИРАЗ" << endl;
      parser.printErrors();
    }

    cout << "\n----------------------------------------\n" << endl;
  }



  return 0;
}