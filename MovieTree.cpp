#include <string>
#include "MovieTree.hpp"
#include <iostream>
#include <fstream>
//using namespace std;

MovieTree::MovieTree(){
  root = nullptr;
}



void deleteLL(TreeNode *node){
  LLMovieNode *curr = node->head;
  LLMovieNode *next;
  while(curr != nullptr){
    next = curr->next;
    delete curr;
    curr = next;
  }
  delete node;
}

void deleteTree(TreeNode *node){
  if(node == nullptr){
    return;
  }
  deleteTree(node->leftChild);
  deleteTree(node->rightChild);
  deleteLL(node);
}

MovieTree::~MovieTree(){
  if (root){
    deleteTree(root);
  }
}

TreeNode *getRightEndFromLeftChild(TreeNode *node){
  if(node->rightChild == nullptr){
    return node;
  }
  while(node->rightChild !=nullptr){
    node = node->rightChild;
  }
  return node;
}

TreeNode *restructureTree(TreeNode *node){
  if(node->parent == nullptr){
    if(node->leftChild != nullptr && node->rightChild != nullptr){
      TreeNode *temp = node->leftChild;
      TreeNode *rightMost = getRightEndFromLeftChild(temp);
      rightMost->rightChild = node->rightChild;
      node->rightChild->parent = rightMost;
      node->leftChild->parent = nullptr;
      delete node;
      return temp;
    }
    else if(node->leftChild == nullptr && node->rightChild != nullptr){
      TreeNode *temp = node;
      node->rightChild->parent = nullptr;
      delete node;
      return temp->rightChild;
    }
    else if(node->leftChild != nullptr && node->rightChild == nullptr){
      TreeNode *temp = node;
      node->leftChild->parent = nullptr;
      delete node;
      return temp->leftChild;
    }
    else{
      delete node;
      return nullptr;
    }
  }
  if(node->leftChild != nullptr && node->rightChild != nullptr){
    TreeNode *temp = node->leftChild;
    TreeNode *rightMost = getRightEndFromLeftChild(temp);
    rightMost->rightChild = node->rightChild;
    node->rightChild->parent = rightMost;
    node->leftChild->parent = node->parent;
    delete node;
  }
  else if(node->leftChild == nullptr && node->rightChild != nullptr){
    if(node->titleChar < node->parent->titleChar){//left child
      node->parent->leftChild = node->rightChild;
      node->rightChild->parent = node->parent;
      delete node;
    }
    else{
      node->parent->rightChild = node->rightChild;
      node->rightChild->parent = node->parent;
      delete node;
    }
  }
  else if(node->leftChild != nullptr && node->rightChild == nullptr){
    if(node->titleChar < node->parent->titleChar){//left child
      node->parent->leftChild = node->rightChild;
      node->rightChild->parent = node->parent;
      delete node;
    }
    else{
      node->parent->rightChild = node->leftChild;
      node->leftChild->parent = node->parent;
      delete node;
    }
  }
  else{
    if(node->titleChar < node->parent->titleChar){
      node->parent->leftChild = nullptr;
      delete node;
    }
    else{
      node->parent->rightChild = nullptr;
      delete node;
    }
  }
  return nullptr;
}

TreeNode *checkChar(std::string title, TreeNode *node){
  if(node->titleChar == title.at(0)){
    return node;
  }
  if(title.at(0) < node->titleChar && node->leftChild != nullptr){ //I might need to add the check for node->rightChild != nullptr here and make if statements inside
    return checkChar(title, node->leftChild);
  }
  else if(title.at(0) > node->titleChar && node->rightChild != nullptr){
    return checkChar(title, node->rightChild);
  }
  else{
    return nullptr;
  }
}

TreeNode *deleteNode(std::string title, TreeNode *node){
  LLMovieNode *prev;
  LLMovieNode *curr = node->head;
  if(curr->title == title && curr->next == nullptr){
    delete curr;
    TreeNode *temp = restructureTree(node);
    return temp;
  }
  else if(curr->title == title){
    node->head = node->head->next;
    delete curr;
  }
  else{
    while(curr->title != title){
      prev = curr;
      curr = curr->next;
    }
    prev->next = curr->next;
    delete curr;
  }
  return nullptr;
}

void MovieTree::deleteMovie(std::string title){
  if(root == nullptr){
    std::cout << "Movie: " << title << " not found, cannot delete." << std::endl;
    return;
  }
  else{
    if(root->rightChild == nullptr && root->leftChild == nullptr){
       deleteNode(title, checkChar(title,root));
       root = nullptr;
       return;
    }
    TreeNode *temp = deleteNode(title, checkChar(title,root));
    if(temp != nullptr){
      root = temp;
    }
  }
}

TreeNode *insertTreeNode(std::string title, TreeNode *node){
  if(checkChar(title, node) == nullptr){
    if(title.at(0) < node->titleChar && node->leftChild == nullptr){
      TreeNode *newNode = new TreeNode;
      node->leftChild = newNode;
      newNode->parent = node;
      newNode->titleChar = title.at(0);
      return newNode;
    }
    else if(title.at(0) > node->titleChar && node->rightChild == nullptr){
      TreeNode *newNode = new TreeNode;
      node->rightChild = newNode;
      newNode->parent = node;
      newNode->titleChar = title.at(0);
      return newNode;
    }
    else if(title.at(0) < node->titleChar){
      return insertTreeNode(title, node->leftChild);
    }
    else{
      return insertTreeNode(title, node->rightChild);
    }
  }
}

void insertLLNode(int ranking, std::string title, int year, float rating, TreeNode *node){
  LLMovieNode *newNode =  new LLMovieNode(ranking, title, year, rating);
  if(node->head == NULL){
    node->head = newNode;
  }
  else{
    LLMovieNode *temp = node->head;
    while(temp != nullptr){
      if(title < temp->title){
        newNode->next = node->head;
        node->head = newNode;
        break;
      }
      else if(temp->next == nullptr){
        temp->next = newNode;
        break;
      }
      else if(title > temp->title && title < temp->next->title){
        newNode->next = temp->next;
        temp->next = newNode;
        break;
      }
      temp = temp->next;
    }
  }
}

void MovieTree::addMovie(int ranking, std::string title, int year, float rating){
  if(root == NULL){
    TreeNode *node = new TreeNode;
    root = node;
    node->titleChar = title.at(0);
    insertLLNode(ranking, title, year, rating, node);
  }
  else{
    if(checkChar(title, root) == nullptr){
      TreeNode *temp = insertTreeNode(title, root);
      insertLLNode(ranking, title, year, rating, temp);
    }
    else{
      TreeNode *temp = checkChar(title, root);
      insertLLNode(ranking, title, year, rating, temp);
    }
  }
}

void printLL(TreeNode *node){
  LLMovieNode* temp = node->head;
  if(node != nullptr){
    std::cout << "Movies starting with letter: " << node->titleChar << std::endl;
  }
  while(temp != NULL){
    std::cout << " >> " << temp->title << " " << temp->rating << std::endl;
    temp = temp->next;
  }
}

void alphabeticTraverse(TreeNode *node){
  if(node == nullptr){
    return;
  }
  alphabeticTraverse(node->leftChild);
  printLL(node);
  alphabeticTraverse(node->rightChild);
}

void MovieTree::printMovieInventory(){
  if(root == nullptr){
    return;
  }
  alphabeticTraverse(root);
}


int main(int argc, char *argv[]){
  MovieTree MT;
  std::string title = "";
  std::string input = "0";
  std::string ranking = "";
  std::string year = "";
  std::string rating = "";
  std::ifstream movies(argv[1]);
  while(getline(movies, ranking, ',')){
    getline(movies, title, ',');
    getline(movies, year, ',');
    getline(movies, rating);
    MT.addMovie(stoi(ranking), title, stoi(year), stof(rating));
  }
  while(input != "3"){
    std::cout << "======Main Menu======" << std::endl;
    std::cout << "1. Print the inventory" << std::endl;
    std::cout << "2. Delete a movie" << std::endl;
    std::cout << "3. Quit" << std::endl;
    getline(std::cin,input);
    switch(stoi(input)){
      case 1:
        MT.printMovieInventory();
        break;
      case 2:
        std::cout << "Enter a movie title:" << std::endl;
        getline(std::cin, title);
        MT.deleteMovie(title);
        break;
      case 3:
        std::cout << "Goodbye!" << std::endl;
        break;
    }
  }
}
