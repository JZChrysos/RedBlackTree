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
Node * sib(Node * node){
	if(node->parent == NULL){
		return NULL;
	}
	else if(node->parent->right == node){
		return node->parent->left;
	}
	else{
		return node->parent->right; 
	}
}
void delbalance(Node*& head, Node*& P, Node*& S){// node is the place where a removed node used to be (kinda). This function only deals with the case where the removed node and replacement are both black, since all other cases are easy.
	//Case 1: node is the new head
	if(P == NULL){
		cout << "Case 1";
		return;
	}
	Node * node;
	if(P->right == S){
		node = P->left;
	}
	else{
		node = P->right;
	}
	//Case 2: sib(node) is red
	if(S != NULL && S->red == true){ //sibling can't be null anyways
		P->red = true;
		S->red = false;
		if(P != head){
			S->parent = P->parent;
			if(P->parent->right == P){
				P->parent->right = S;
			}
			else{
				P->parent->left = S;
			}
		}
		else{
			S->parent = NULL;
			head = S;
		}
		P->parent = S;
		if(P->right == node){
			P->left = S->right;
			if(S->right != NULL){
				S->right->parent = P;
			}
			S->right = P;
		}
		else{
			P->right = S->left;
			if(S->left != NULL){
				S->left->parent = P;
			}
			S->left = P;
		}
		cout << "Ran case 2";
		delbalance(head, node->parent, S); // recurrence, will be passed to a later case
	}
	//Case 3: P,X,Y black
	else if(P->red == false && (S->right == NULL || S->right->red == false) && (S->left == NULL || S->left->red == false)){
		S->red = true;
		Node * Aunt = sib(P);
		if(P->parent != NULL){
			delbalance(head, P->parent, Aunt);
		}
		cout << "Ran case 3";
	}
	//Case 4: P Red, X,Y black
	else if(P->red == true && (S->right == NULL || S->right->red == false) && (S->left == NULL || S->left->red == false)){
		S->red = true;
		P->red = false;
		cout << "Ran case 4";
	}
	//Case 5: X red, Y black
	else if(P->red == true && P->right == node && S->right->red == true && (S->left == NULL || S->left->red == false)){
		S->red = true;
		S->right->red = false;
		Node * X = S->right;
		P->left = X;
		X->parent = P;
		S->right = X->left;
		if(X->left != NULL){
			X->left->parent = S;
		}
		X->left = S;
		S->parent = X;
		delbalance(head, node->parent, S);
		cout << "Ran case 5";
	}
	else if(P->red == true && P->left == node && S->left->red == true && (S->right == NULL || S->right->red == false)){
		S->red = true;
		S->left->red = false;
		Node * X = S->left;
		P->right = X;
		X->parent = P;
		S->left = X->right;
		if(X->right != NULL){
			X->right->parent = S;
		}
		X->right = S;
		S->parent = X;
		delbalance(head, node->parent, S);	
		cout << "Ran case 5";
	}
	//Case 6: X black, Y red
	else if(P->left == node && S->right->red == true){
		bool pisred = P->red;
		P->red = S->red;
		S->red = pisred;
		S->right->red = false;
		Node * G = P->parent;
		if(P == head){
			S->parent = NULL;
			head = S;
		}
		else{
			S->parent = G;
			if(G->right == P){
				G->right = S;
			}	
			else{
				G->left = S;
			}
		}
		P->parent = S;
		P->right = S->left;
		if(S->left != NULL){
			S->left->parent = P;
		}
		S->left = P;
		cout << "Ran case 6";
	}
	else if(P->right == node && S->left->red == true){
		bool pisred = P->red;
		P->red = S->red;
		S->red = pisred;
		S->left->red = false;
		Node * G = P->parent;
		if(P == head){
			S->parent = NULL;
			head = S;
		}
		else{
			S->parent = G;
			if(G->right == P){
				G->right = S;
			}	
			else{
				G->left = S;
			}
		}
		P->parent = S;
		P->left = S->right;
		if(S->right != NULL){
			S->right->parent = P;
		}
		S->right = P;
		cout << "Ran case 6";
	}

}
void del(Node*& head, Node*& node, int num){ //starting at node, removes the value num, updates the tree (head may change)
	if(node == NULL){
		cout << "Not found, sorry king :(" << endl;
	}
	else if (node->data == num){ // here node must be deleted
		if(node->left != NULL && node->right != NULL){
			Node * current = node->right;
			while(current->left != NULL){
				current = current->left;
			}
			Node * P;
			if(current != node->right){
				P = current->parent;
			}
			else{
				P = current;
			}
			Node * S = sib(current);
			//determine if balancing needs to be done, and fixes colors if not
			bool balance = false;
			if(current != node->right){
				if(current->red == false && current->right == NULL){
					balance = true;
				}
				else if(current->red == false && current->right->red == true){
					balance = true;
				}
				else if(current->red == false && current->right != NULL && current->right->red == true){
					current->right->red = false;
				}
				if(current->right != NULL){
					current->right->parent = current->parent;
					current->parent->left = current->right;
				}
				else{
					current->parent->left = NULL;
				}
				current->red = node->red;
				current->right = node->right;
				node->right->parent = current;
				current->left = node->left;
				node->left->parent = current;
			}
			else{
				if(node->red == false && current->red == false){
					balance = true;
				}
				else if(node->red == false && current->red == true){
					current->red = false;
				}
				else if(node->red == true){
					current->red = true;
					if(current->right != NULL){
						current->right->red = false;
					}
					else{
						balance = true;
					}
				}
				current->left = node->left;
				node->left->parent = current;
			}
			if(node != head){
				current->parent = node->parent;
				if(node->parent->right == node){
					node->parent->right = current;
				}
				else if(node->parent->left == node){
					node->parent->left = current;
				}
			}
			else{
				current->parent = NULL;
				current->red = false; // root must be black
				head = current;
			}
			if(balance == true){
				delbalance(head, P, S);
			}
		}
		else if(node->left == NULL && node->right != NULL){
			// determine if balancing needs to be done
			bool balance = false;
			if(node->red == false && node->right->red == false){
				balance = true;
			}
			else if(node->red == false && node->right->red == true){
				node->right->red = false;
			}
			if(node != head){
				node->right->parent = node->parent;
				if(node->parent->right == node){
					node->parent->right = node->right;
				}
				else{
					node->parent->left = node->right;
				}
			}
			else{
				node->right->parent = NULL;
				node->right->red = false; // root must be black
				head = node->right;
			}
			if(balance == true){
				Node * dummy = NULL;
				delbalance(head, node, dummy);
			}
		}
		else if(node->left != NULL && node->right == NULL){
			// determine if balancing needs to be done
			bool balance = false;
			if(node->red == false && node->left->red == false){
				balance = true;
			}
			else if(node->red == false && node->left->red == true){
				node->left->red = false;
			}
			if(node != head){
				node->left->parent = node->parent;
				if(node->parent->right == node){
					node->parent->right = node->left;
				}
				else{
					node->parent->left = node->left;
				}
			}
			else{
				node->left->parent = NULL;
				head = node->left;
			}
			if(balance == true){
				Node * dummy = NULL;
				delbalance(head, node, dummy);
			}
		}
		else if(node->left == NULL && node->right == NULL){
			if(node != head){
				bool balance = false;
				if(node->red == false){
					balance = true;
				}
				Node * P = node->parent;
				Node * S = sib(node);
				if(P->right == node){
					P->right = NULL;
				}
				else{
					P->left = NULL;
				}
				if(balance == true){
					delbalance(head, P, S);
				}
			}
			else{
				head = NULL;
			}
		}
	}
	else if(node->data > num){
		del(head, node->left, num);
	}
	else if(node->data < num){
		del(head, node->right, num);
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
	cout << "What would you like to do? Add (A), read a File (F), delete (D), or search (S)? or Quit (Q): ";
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

	else if (input == 'D'){
		cout << endl << "What int would you like to delete? ";
		int delint;
		cin >> delint;
		del(head,head,delint);
		vis(head,0);	
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
