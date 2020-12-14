#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std;
struct Node{
	int data;
	Node * left;
	Node * right;
};
void add(Node*& head, int num){
	if(head == NULL){
		Node * newnode = new Node();
		newnode->data = num;
		head = newnode;
	}
	else if(head->left == NULL && head->right == NULL){
		Node * newnode = new Node();
		if (head->data > num){
			head->left = newnode;
			newnode->data = num;	
		}
		else{
			head->right = newnode;
			newnode->data = num;
		}
	}
	else{
		if(head->data > num){
			add(head->left, num);
		}
		else{
			add(head->right, num);
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
	cout << string(depth * 5, ' ')<< head->data << endl;
	vis(head->left, depth+1);
	}
}
int main(){
	//create tree
	Node * head = new Node();
	head = rem(head, 0);
	//enter loop
	bool quit = false;
	while(quit == false){
	cout << "What would you like to do? Add (A), Remove (R), Search (S), or Visualize (V)? or Quit (Q): ";
	char input;
	cin >> input;
	if (input == 'A'){
		cout << endl << "How would you like to add, File (F) or User input (U)?  ";
		char input2;
		cin >> input2;
		cin.get();
		if (input2== 'U'){
			/*cout << "Enter some numbers, then enter 0 when you're done: " << endl;
			bool done = false;
			while(done == false){
			int num;
			cin >> num;
			if (num == 0){
				done = true;
			}
			else if (num != 0){
			add(head,num);
			}
			}*/
			cout << endl << "Enter a space separated list of integers, and I will add them: ";
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
			add(head,current);
		}
		else if (input2 == 'F'){ // stole this idea from Pranav (I also used this in heap)
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
                                    add(head, stoi(lineBuffer)); // thing gets added here
                                    pos = (buffer-line) + 1;
                                }
                                }
                        }
		}
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
