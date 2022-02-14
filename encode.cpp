// writing on a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>       
#include <algorithm>   
#include <functional>
#include <map>
#define null nullptr
using namespace std;

struct Node {
     char character;
     int frequency;
     bool isPar;
     Node *left;
     Node *right;
     Node() : character(), frequency() , left(nullptr), right(nullptr) {}
     Node(char x , bool q, int p) : character(x) , frequency(p), isPar(q), left(nullptr), right(nullptr) {}
     Node(char x , int i) : character(x) , frequency(i), isPar(false), left(nullptr), right(nullptr) {}
     Node(char x,  bool p ,int i, Node *left, Node *right) : character(x), isPar(p), frequency(i), left(left), right(right) {
    }
      bool operator<(const Node& rhs) const
    {
        return frequency < rhs.frequency;
    }
 };


vector<int> buildFrequencyTable(string filepath){
    ifstream myfile(filepath);
    string line;
    vector<int> frequencyTable(128,0);
    if( myfile.is_open() )
    {
        while(getline(myfile,line))
        {
            for(int i = 0 ; i < line.size(); i++){
                frequencyTable[int(line[i])] = frequencyTable[int(line[i])] + 1;
            }
              myfile.close();
        }
    }else{
        cout<< "file not open";
    }
    return frequencyTable;
}


Node* buildTree(priority_queue<Node*> &nodeHeap){
    while(nodeHeap.size() > 1){
        Node* p1 = nodeHeap.top();
        nodeHeap.pop();
        Node* p2 = nodeHeap.top();
        nodeHeap.pop();
        Node* internal = new Node('$',1,p1->frequency + p2->frequency);
        internal->left = p1; 
        internal->right = p2;
        nodeHeap.push(internal);
    }
    return nodeHeap.top();
}

void generateCode(Node* Root, map<char,string> &encoder,string result){
    string res = result;
    if(Root->left == nullptr && Root->right == nullptr){
        encoder.insert(pair<char,string>(Root->character,res));
    }
    if(Root->left != nullptr){
        generateCode(Root->left,encoder,"0" + res);
    }
    if(Root->right != nullptr){
        generateCode(Root->right,encoder,"1" + res);
    }
}

string generateNewString(string filePath){
    ifstream unCompress(filePath);
    string total = "";
    string line;
    if(unCompress.is_open()){
        while(getline(unCompress,line)){
            total = total + line;
        }   
    }
    return total;
}

void writeNewFile(string filePath,string encodedString){
    ofstream encodedFile(filePath);
        if(encodedFile.is_open()){
             encodedFile << encodedString;
        }
}

string encodeString(string unencoded, map<char,string> &Scheme){
    string encoded = "";
    for(int i = 0 ; i < unencoded.length() ; i++){
        cout << char(unencoded[i]) << "  " << Scheme[unencoded[i]] << endl;
        encoded = encoded + Scheme[unencoded[i]];
    }
    return encoded;
}


int main (int argc, char* argv[]) {
  vector<int> table;
  table =  buildFrequencyTable(argv[1]);
  map<char , string> Dict;
  priority_queue<Node*,vector<Node*>> heap;
  for(int i = 0 ; i < table.size() ; ++i){
      if(table[i] != 0){
          Node* tmp = new Node(char(i),table[i]);
          cout << tmp-> frequency << "" << tmp->character << endl;
          heap.push(tmp);
    }
  }
  Node* huffmanTree = new Node();
  huffmanTree = buildTree(heap);
  generateCode(huffmanTree, Dict , "");    
  string file = generateNewString(argv[1]);
  string encoded = encodeString(file,Dict);
  writeNewFile("compressed.txt",encoded);
}