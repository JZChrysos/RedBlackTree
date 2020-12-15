#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std;
struct Node{
	int data;
	bool red; // keeps track of color
	Node * left;
	Node * right;
	Node * parent; // we need to keep track of the parent now
};
void balance(Node*& head, Node*& node){
	//if the new node is the root, make it black
	if(node == head){
		node->red = false;
		//cout << "Case 1" << endl;
	}
	//if its parent is black, keep it red
	else if(node->parent->red == false){
		//cout << "Case 2" << endl;
	}
	//if its parent is red, depends on the uncle (the uncle may be null, but if the parent is red then there must be a grandparent).
	else{
		//dad
		Node * dad = node->parent;
		//grandpa
		Node * grandpa = node->parent->parent;
		//uncle
		Node * uncle;
		if(grandpa->left == dad){
			uncle = grandpa->right;
		}
		else{
			uncle = grandpa->left;
		}
		//cases for uncle
		if(uncle != NULL && uncle->red == true){
			dad->red = false;
			uncle->red = false;
			grandpa->red = true;
			balance(head, grandpa);
			//cout << "Case 3" << endl;
		}
		else{ //Worst case. The tricky part is that every affected node must have relationships updated, and the order is important.
			if(grandpa->left == dad && dad->right == node){
				grandpa->left = node;
				node->parent = grandpa;
				dad->parent = node;
				dad->right = node->left;
				if(node->left != NULL){
				node->left->parent = dad;
				}
				node->left = dad;
				balance(head, dad);
				//cout << "Case 4" << endl;
			}
			else if(grandpa->right == dad && dad->left == node){
				grandpa->right = node;
				node->parent = grandpa;
				dad->parent = node;
				dad->left = node->right;
				if(node->right != NULL){
				node->right->parent = dad;
				}
				node->right = dad;
				balance(head, dad);
				//cout << "Case 4" << endl;
			}	
			else if(grandpa->left == dad && dad->left == node){
				if(grandpa == head){
					head = dad;
				}
				else{
					if(grandpa->parent->right == grandpa){
						grandpa->parent->right = dad;
					}
					else{
						grandpa->parent->left = dad;
					}
				}
				dad->parent = grandpa->parent;
				grandpa->parent = dad;
				grandpa->left = dad->right;
				if(dad->right != NULL){
					dad->right->parent = grandpa;
				}
				dad->right = grandpa;
				dad->red = false;
				grandpa->red = true;
				//cout << "Case 5" << endl;
			}
			else if(grandpa->right == dad && dad->right == node){
				if(grandpa == head){
					head = dad;
				}
				else{
					if(grandpa->parent->right == grandpa){
						grandpa->parent->right = dad;
					}
					else{
						grandpa->parent->left = dad;
					}
				}
				dad->parent = grandpa->parent;
				grandpa->parent = dad;
				grandpa->right = dad->left;
				if(dad->left != NULL){
					dad->left->parent = grandpa;
				}
				dad->left = grandpa;
				dad->red = false;
				grandpa->red = true;
				//cout << "Case 5" << endl;
			}
		}
	}
}
void add(Node*& head, Node*& node, Node*& parentnode, char side, int num){
	if(node == NULL){
		Node * newnode = new Node();
		newnode->data = num;
		newnode->red = true; // new nodes are red by default
		newnode->parent = parentnode; //makes the node we came from to be the parent
		if(side == 'L'){
		parentnode->left = newnode;
		}
		else if (side == 'R'){
		parentnode->right = newnode;
		}
		node = newnode;
		balance(head, newnode); //balance every new node after adding it
	}/*
	else if(head->left == NULL && head->right == NULL){
		Node * newnode = new Node();
		if (head->data > num){
			head->left = newnode;
			newnode->data = num;
			newnode->parent = head; //parent
			newnode->red = true; //color	
		}
		else{
			head->right = newnode;
			newnode->data = num;
			newnode->parent = head; //parent
			newnode->red = true; //color
		}
		balance(newnode); //balance every node after adding it
	}*/
	else{
		if(node->data > num){
			add(head, node->left, node, 'L', num);
		}
		else{
			add(head, node->right, node, 'R', num);
		}
	}
}
Node* rem(Node*& head, int num){
	if(head == NULL){
		return head;
	}
	else if(head->data == num){
		if(head->left != NULL && head->right != NULL){
			Node * current = head->right;
			if(current->left == NULL){
				current->left = head->left;
				return current;
			}
			else{
			while(current->left->left != NULL){
				current = current->left;
			}
			Node * newhead = current->left;
			current->left = newhead->right;
			newhead->left = head->left;
			newhead->right = head->right;
			return newhead;
			}
		}
		else if(head->left == NULL && head->right != NULL){
			return head->right;
		}
		else if(head->left != NULL && head->right == NULL){
			return head->left;
		}
		else{
			return NULL;
		}
	}
	else{
		if(head->data > num){
			head->left = rem(head->left, num);
		}
		if(head->data < num){
			head->right = rem(head->right, num);
		}
		return head;
	}
}
bool search(Node*& head, int num){
	if (head == NULL){
		return false;
	}
	else if (head->data > num){
		return search(head->left, num);
	}
	else if (head->data < num){
		return search(head->right, num);
	}
	else{ 
		return true;
	}
}
void vis(Node * head, int depth){ // I think this is kinda clever
	if(head != NULL){
	vis(head->right, depth+1);
       	cout << endl;
	if(head->red){
		cout << string(depth * 5, ' ')<< "\033[1;31m" << head->data << "\033[0m" << endl;
	}
	else{
		cout << string(depth * 5, ' ')<< head->data << endl;
	}
	vis(head->left, depth+1);
	}
}
int main(){
	//create tree
	Node * head = new Node();
	head = rem(head, 0);
	Node * prehead = NULL;
	//enter loop
	bool quit = false;
	while(quit == false){
	cout << "What would you like to do? Add (A), or read a File (F)? or Quit (Q): ";
	char input;
	cin >> input;
	if (input == 'A'){
		cout << "Enter some numbers, then enter 0 when you're done: " << endl;
		bool done = false;
		while(done == false){
		int num;
		cin >> num;
		if (num == 0){
			done = true;
		}
		else if (num != 0){
			add(head, head, prehead, 'N', num);
			cout << string(20, '=');
			vis(head,0);
			cout << string(20, '=');
			cout << endl;
		}
		}
			/*cout << endl << "Enter a space separated list of integers, and I will add them: ";
			char nums[100];
			cin.getline(nums, 100, '\n');
			int current = 0;
			for(int i=0; i < strlen(nums); i++){
				if(nums[i] != ' '){
					current = current * 10 + (nums[i] - '0');
				}
				else{
					add(head, current);
					current = 0;
				}
			}
			add(head,current);*/
		}
	else if (input == 'F'){ // stole this idea from Pranav (I also used this in heap)
			cin.get();
			cout << "Enter the file you would like to open, in this program there is a file called 'Numbers.txt'\n";
                	char file[20];
                	cin.getline(file, 80, '\n');
                        FILE* pfile = NULL;
                        char line[1000];
                        // opens the file with the given name from user
                        pfile = fopen(file, "r");
                        // alerts if file is unreadable
                        if (pfile == NULL)
                        {
                            cout << "Error in opening the file.";
                            return 0;
                        }

                        // goes through each line, and stores whats beween the spaces in the integer array
                        int index = 0;
                        while (fgets(line, 1000, pfile)) {
                            char* buffer = new char[20];
                            memset(buffer, '\0', 20);
                            int pos = 0;
                            while (buffer != NULL) {
                                buffer = strchr(line + pos, ' ');
                                if (buffer != NULL) {
                                    char lineBuffer[20];
                                    memset(lineBuffer, '\0', 20);
                                    strncpy(lineBuffer, line + pos, sizeof(line - buffer));
                                    add(head, head, prehead, 'N', stoi(lineBuffer)); // thing gets added here
                                    pos = (buffer-line) + 1;
                                }
             		    }
			}
			vis(head,0);
			cout << endl;
	}

	else if (input == 'R'){
		cout << endl << "What int would you like to delete? ";
		int delint;
		cin >> delint;
		head = rem(head,delint);	
	}
	else if (input == 'V'){
		vis(head, 0);
		cout << endl;
	}
	else if (input == 'S'){
		cout << endl << "What int would you like to search for? ";
		int num;
		cin >> num;
		if (search(head, num) == false){
			cout << "The tree does not contain this integer." << endl;
		}
		else {
			cout << "The tree contains this integer!" << endl;
		}
	}
	else{
		quit = true;
	}
	}
	return 0;
}
