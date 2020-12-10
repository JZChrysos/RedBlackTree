#include <iostream>
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
void rem(Node*& head, int num){
	if(head == NULL){
		cout << "That number isn't in here, sorry!" << endl;
	}
	else if(head->data == num){
		if(head->left != NULL && head->right != NULL){
			Node * current = head->right;
			while(current->left != NULL){
				current = current->left;
			}
			Node * temp = current->right;
			current->left = head->left;
			current->right = head->right;
			head = current;
			current = temp;
			delete current;
		}
		else if(head->left == NULL && head->right != NULL){
			head = head->right;
			delete head;
		}
		else if(head->left != NULL && head->right == NULL){
			head = head->left;
			delete head;
		}
		else{
			delete head;
		}
	}
	else{
		if(head->data > num){
			rem(head->left, num);
		}
		if(head->data < num){
			rem(head->right, num);
		}
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
void vis(Node * head){
	if(head != NULL){
	vis(head->left);
	cout << head->data;
	vis(head->right);
	}
}
int main(){
	//create tree
	Node * head = new Node();
	//enter loop
	bool quit = false;
	while(quit == false){
	cout << "What would you like to do? Add (A), Remove (R), Search (S), or Visualize (V)? (or QUIT): ";
	char input;
	cin >> input;
	if (input == 'A'){
		cout << endl << "How would you like to add, File (F) or User input (U)?  ";
		char input2;
		cin >> input2;
		if (input2== 'U'){
			cout << "Enter some numbers, then enter 0 when you're done: " << endl;
			bool done = false;
			while(done == false){
			int num;
			if (num == 0){
				done = true;
			}
			else{
			cin >> num;
			add(head,num);
			}
			}
			/*cout << endl << "Enter a space separated list of integers, and I will add them: ";
			char nums[100];
			cin >> nums;
			for(int i=0; nums[i] != '\n'; i++){
				int current = 0;
				if(nums[i] != ' '){
					current = current * 10 + (nums[i] - '0');
				}
				else{
					add(head, current);
					current = 0;
				}
			}*/
		}
		else{
			cout << "I didn't implement this yet, sorry!";
		}
	}
	else if (input == 'R'){
		cout << endl << "What int would you like to delete? ";
		int delint;
		cin >> delint;
		rem(head,delint);	
	}
	else if (input == 'V'){
		vis(head);
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
		cout << "Want to quit? (Y/N): ";
		char qint;
		cin >> qint;
		if (qint == 'Y'){
			quit = true;
		}
	}
	}
	return 0;
}
