#include "ExpressionTree.h"
#include <stack>
#include <cctype>
#include <iostream>
#include <sstream>

ExpressionTree::ExpressionTree(const string& expression) {
  vector<string> tokens = tokenize(expression);
  tokens = optimize(tokens);
  vector<string> postfix = infixToPostfix(tokens);
  root = buildTree(postfix);
}

bool ExpressionTree::isOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

bool ExpressionTree::isOperand(const string& token) {
  if (token.empty()) return false;
  if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-' && isdigit(token[1]))) {
    return true;
  }
  return isalpha(token[0]);
}

int ExpressionTree::getPriority(char op) {
  if (op == '+' || op == '-') return 1;
  if (op == '*' || op == '/') return 2;
  return 0;
}

vector<string> ExpressionTree::tokenize(const string& expr) {
  vector<string> tokens;
  string current = "";

  for (size_t i = 0; i < expr.length(); i++) {
    char c = expr[i];

    if (isspace(c)) continue;

    if (isalnum(c) || c == '.') {
      current += c;
    }
    else if (isOperator(c) || c == '(' || c == ')') {
      if (!current.empty()) {
        tokens.push_back(current);
        current = "";
      }
      tokens.push_back(string(1, c));
    }
  }

  if (!current.empty()) {
    tokens.push_back(current);
  }

  return tokens;
}

vector<string> ExpressionTree::optimize(vector<string> tokens) {
  // Спочатку оптимізуємо ланцюжки / та -
  vector<string> optimizedTokens;
  for (size_t i = 0; i < tokens.size(); i++) {
    if (i + 4 < tokens.size() &&
      isOperand(tokens[i]) &&
      (tokens[i + 1] == "/" || tokens[i + 1] == "-")) {

      // Знаходимо довжину ланцюжка
      char op = tokens[i + 1][0];
      size_t chainLen = 1;
      size_t j = i + 2;

      while (j < tokens.size() && isOperand(tokens[j]) &&
        j + 1 < tokens.size() && tokens[j + 1].length() == 1 && tokens[j + 1][0] == op) {
        chainLen++;
        j += 2;
      }

      // Якщо ланцюжок >= 3 операндів
      if (chainLen >= 3 && j < tokens.size() && isOperand(tokens[j])) {
        optimizedTokens.push_back(tokens[i]);
        optimizedTokens.push_back(tokens[i + 1]); // / або -
        optimizedTokens.push_back("(");

        // Додаємо всі операнди через * або +
        char newOp = (op == '/') ? '*' : '+';
        optimizedTokens.push_back(tokens[i + 2]);

        for (size_t k = i + 4; k <= j; k += 2) {
          optimizedTokens.push_back(string(1, newOp));
          optimizedTokens.push_back(tokens[k]);
        }

        optimizedTokens.push_back(")");

        ostringstream oss;
        oss << "Оптимізація ланцюжка " << op << ": " << (chainLen + 1) << " операндів";
        optimizations.push_back(oss.str());

        i = j;
        continue;
      }
    }

    optimizedTokens.push_back(tokens[i]);
  }

  tokens = optimizedTokens;

  // Потім звичайні оптимізації
  bool changed = true;

  while (changed) {
    changed = false;
    vector<string> newTokens;

    for (size_t i = 0; i < tokens.size(); i++) {
      // Унарний мінус
      if (tokens[i] == "-" && (i == 0 || tokens[i - 1] == "(")) {
        newTokens.push_back("0");
        newTokens.push_back("-");
        optimizations.push_back("Унарний мінус виконано: 1 раз");
        changed = true;
        continue;
      }

      // Множення на 0
      if (i + 2 < tokens.size()) {
        if (tokens[i + 1] == "*" && tokens[i + 2] == "0") {
          newTokens.push_back("0");
          optimizations.push_back("Множення на 0 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
        if (tokens[i] == "0" && tokens[i + 1] == "*") {
          newTokens.push_back("0");
          optimizations.push_back("Множення на 0 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
      }

      // Множення на 1
      if (i + 2 < tokens.size()) {
        if (tokens[i + 1] == "*" && tokens[i + 2] == "1") {
          newTokens.push_back(tokens[i]);
          optimizations.push_back("Множення на 1 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
        if (tokens[i] == "1" && tokens[i + 1] == "*") {
          newTokens.push_back(tokens[i + 2]);
          optimizations.push_back("Множення на 1 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
      }

      // Ділення на 1
      if (i + 2 < tokens.size()) {
        if (tokens[i + 1] == "/" && tokens[i + 2] == "1") {
          newTokens.push_back(tokens[i]);
          optimizations.push_back("Ділення на 1 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
      }

      // Додавання 0
      if (i + 2 < tokens.size()) {
        if (tokens[i + 1] == "+" && tokens[i + 2] == "0") {
          newTokens.push_back(tokens[i]);
          optimizations.push_back("Додавання 0 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
        if (tokens[i] == "0" && tokens[i + 1] == "+") {
          newTokens.push_back(tokens[i + 2]);
          optimizations.push_back("Додавання 0 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
      }

      // Віднімання 0
      if (i + 2 < tokens.size()) {
        if (tokens[i + 1] == "-" && tokens[i + 2] == "0") {
          newTokens.push_back(tokens[i]);
          optimizations.push_back("Віднімання 0 виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
      }

      // Ділення 0 на змінну
      if (i + 2 < tokens.size()) {
        if (tokens[i] == "0" && tokens[i + 1] == "/" && isOperand(tokens[i + 2])) {
          newTokens.push_back("0");
          optimizations.push_back("Ділення 0 на змінну виконано: 1 раз");
          i += 2;
          changed = true;
          continue;
        }
      }

      // Обчислення констант
      if (i + 2 < tokens.size()) {
        bool isNum1 = !tokens[i].empty() && (isdigit(tokens[i][0]) || tokens[i].find('.') != string::npos);
        bool isNum2 = !tokens[i + 2].empty() && (isdigit(tokens[i + 2][0]) || tokens[i + 2].find('.') != string::npos);

        if (isNum1 && isOperator(tokens[i + 1][0]) && isNum2) {
          double a = stod(tokens[i]);
          double b = stod(tokens[i + 2]);
          double result = 0;
          char op = tokens[i + 1][0];

          if (op == '+') result = a + b;
          else if (op == '-') result = a - b;
          else if (op == '*') result = a * b;
          else if (op == '/' && b != 0) result = a / b;
          else {
            newTokens.push_back(tokens[i]);
            continue;
          }

          ostringstream oss;
          oss << tokens[i] << " " << op << " " << tokens[i + 2] << " = " << result;
          optimizations.push_back(oss.str());

          ostringstream resultStr;
          if (result == (int)result) {
            resultStr << (int)result;
          }
          else {
            resultStr << result;
          }

          newTokens.push_back(resultStr.str());
          i += 2;
          changed = true;
          continue;
        }
      }

      newTokens.push_back(tokens[i]);
    }

    tokens = newTokens;
  }

  return tokens;
}

vector<string> ExpressionTree::infixToPostfix(const vector<string>& tokens) {
  vector<string> postfix;
  stack<string> operators;

  for (const string& token : tokens) {
    if (isOperand(token)) {
      postfix.push_back(token);
    }
    else if (token == "(") {
      operators.push(token);
    }
    else if (token == ")") {
      while (!operators.empty() && operators.top() != "(") {
        postfix.push_back(operators.top());
        operators.pop();
      }
      if (!operators.empty()) operators.pop();
    }
    else if (isOperator(token[0])) {
      while (!operators.empty() && operators.top() != "(" &&
        getPriority(operators.top()[0]) >= getPriority(token[0])) {
        postfix.push_back(operators.top());
        operators.pop();
      }
      operators.push(token);
    }
  }

  while (!operators.empty()) {
    postfix.push_back(operators.top());
    operators.pop();
  }

  return postfix;
}

shared_ptr<Node> ExpressionTree::buildTree(const vector<string>& postfix) {
  stack<shared_ptr<Node>> nodeStack;

  for (const string& token : postfix) {
    shared_ptr<Node> node = make_shared<Node>(token);

    if (isOperator(token[0]) && token.length() == 1) {
      node->right = nodeStack.top();
      nodeStack.pop();
      node->left = nodeStack.top();
      nodeStack.pop();
    }

    nodeStack.push(node);
  }

  return nodeStack.empty() ? nullptr : nodeStack.top();
}

void ExpressionTree::printTree(shared_ptr<Node> node, int level) {
  if (!node) return;

  // Виводимо поточний вузол
  for (int i = 0; i < level; i++) {
    cout << "   |";
  }

  if (level > 0) {
    cout << "___";
  }

  cout << node->value << endl;

  // Лівий нащадок
  if (node->left) {
    printTree(node->left, level + 1);
  }

  // Правий нащадок
  if (node->right) {
    printTree(node->right, level + 1);
  }
}

void ExpressionTree::display() {
  if (!optimizations.empty()) {
    cout << "\nСписок оптимізацій:" << endl;
    for (const string& opt : optimizations) {
      cout << opt << endl;
    }
  }

  cout << "\nДерево:" << endl;
  printTree(root, 0);
}