#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Вузол дерева
struct Node {
  string value;
  shared_ptr<Node> left;
  shared_ptr<Node> right;

  Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
private:
  shared_ptr<Node> root;
  vector<string> optimizations;

  // Допоміжні функції
  bool isOperator(char c);
  bool isOperand(const string& token);
  int getPriority(char op);
  vector<string> tokenize(const string& expr);
  vector<string> infixToPostfix(const vector<string>& tokens);
  vector<string> optimize(vector<string> tokens);
  shared_ptr<Node> buildTree(const vector<string>& postfix);
  void printTree(shared_ptr<Node> node, int level);

public:
  ExpressionTree(const string& expression);
  void display();
  vector<string> getOptimizations() const { return optimizations; }
};