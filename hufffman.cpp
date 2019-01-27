#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>


//A Tree node
struct Node
{
	char character;
	int freqency;
	Node *left, *right;
};

//Function to allocate a new tree node
Node* getNode(char character, int freqency, Node* left, Node* right)
{
	Node* node = new Node();

	node->character = character;
	node->freqency = freqency;
	node->left = left;
	node->right = right;

	return node;
}

//Comparison object to be used to order the heap
struct compare
{
	bool operator()(Node* l, Node* r){
		return l->freqency > r->freqency;
	}
};

void Encode(Node* root, std::string input_string,
			std::unordered_map<char, std::string> &huffmanCode)
{
	if (root == nullptr) return;

	if (!root->left && !root->right) {
		huffmanCode[root->character] = input_string;
	}
	Encode(root->left, input_string + "0", huffmanCode);
	Encode(root->right, input_string + "1", huffmanCode);
}

void Decode(Node* root, int &index, std::string input_string)
{
	if (root == nullptr) {
		return;
	}
	if (!root->left && !root->right){
		std::cout << root->character;
		return;
	}
	index++;
	if (input_string[index] =='0')
		Decode(root->left, index, input_string);
	else
		Decode(root->right, index, input_string);
}

Node* BuildHuffmanTree(std::unordered_map<char, int> freq)
{
	std::priority_queue<Node*, std::vector<Node*>, compare> pq;

	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1){
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		int sum = left->freqency + right->freqency;
		pq.push(getNode('\0', sum, left, right));
	}

	Node* root = pq.top();
	return root;
}

std::string ReadTextFile(const std::string& filename)
{
    std::string text {};
    std::ifstream file;
    file.open (filename, std::ios::out);
    if (file.is_open())
    {
        std::string line;
        while ( getline (file,line)){
          text.append(line);
          text.append("\n");
        } 
    }
    file.close();
    return text;
}

//Huffman coding algorithm
int main()
{
	std::string filename {"example.txt"};
	std::string input_text {};
	input_text=ReadTextFile(filename);
	
	//count frequency 
	std::unordered_map<char, int> characters_frequency;
	for (char single_char: input_text) {
		characters_frequency[single_char]++;
	}

	//make Huffmantree
	Node* root  {BuildHuffmanTree(characters_frequency)};

	//make dictionary 
	std::unordered_map<char, std::string> Huffman_dictionary;
	Encode(root, "", Huffman_dictionary);
	std::cout << "Huffman Dictionary: " << std::endl;
	for (auto pair: Huffman_dictionary) {
		std::cout<<std::setw(2)<< std::left<<pair.first<<":"<<pair.second<<std::endl;
	}

	//input text
	std::cout <<std::endl<<"Original string:"<<std::endl<<input_text<<std::endl;

	//encoded string
	std::string EncodedString {};
	for (char single_char: input_text) {
		EncodedString += Huffman_dictionary[single_char];
	}
	std::cout<<"Encoded string:"<<std::endl;
	std::cout<< EncodedString <<std::endl;

	//decode the encoded string
	int index = -1;
	std::cout<<std::endl<<"Decoded string:"<<std::endl;
	while (index < (int)EncodedString.size() - 2) {
		Decode(root, index, EncodedString);
	}

	return 0;
}