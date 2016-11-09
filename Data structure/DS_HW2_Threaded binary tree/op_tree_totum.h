#include<iostream>
using namespace std;

class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);

private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();

private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if (p != NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl == 0 && p->left != NULL) gothrough(p->left);
	if (p->is_threadr == 0 && p->right != NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s

	if (root == NULL)  //empty tree
	{
		node* newNode = new node();
		newNode->number = s;

		root = newNode;
		root->left = head;
		root->right = tail;
		head->right = root;
		tail->left = root;

		return;
	}
	node *p = root;
	while (true)
	{
		if (s < p->number && !(p->is_threadl))  //turn left
		{
			p = p->left;
		}
		else if (s > p->number && !(p->is_threadr)) //turn right
		{
			p = p->right;
		}
		else
		{
			break;
		}
	} //end while

	/*    insert    */
	if (s < p->number) //add newNode to left of p
	{
		node* newNode = new node();
		newNode->number = s;
		newNode->left = p->left;
		if (newNode->left == head)
		{
			head->right = newNode;
		}

		newNode->right = p;
		p->left = newNode;
		p->is_threadl = 0;
	}

	if (s > p->number) //add newNode to right of p
	{
		node* newNode = new node();
		newNode->number = s;
		newNode->right = p->right;
		if (newNode->right == tail)
		{
			tail->left = newNode;
		}
		newNode->left = p;

		p->right = newNode;
		p->is_threadr = 0;
	}
}

void op_tree_totum::deletion(int s){
	//TODO: fill in the code to do the deletion of the node with number s
	if (root == NULL)
	{
		return;
	}

	node* deleteNode;
	node* parent = NULL;

	/* delete head */
	if (s == root->number) 
	{
		if (root->is_threadl && root->is_threadr)
		{
			delete root;
			root = NULL;
			head->right = tail;
			tail->left = head;
			return;
		}
		else
		{
			deleteNode = root;
		}
	}

	/* the node to be deleted is not head node */
	else
	{
		//to find the parent of the node to be deleted
		parent = root;
		while (true)
		{
			if (s < parent->number)
			{
				if (!(parent->is_threadl) && parent->left->number != s)
				{
					parent = parent->left;
				}
				else
				{
					break;
				}
			}
			else if (s > parent->number)
			{
				if (!(parent->is_threadr) && parent->right->number != s)
				{
					parent = parent->right;
				}
				else
			    {
					break;
				}
			}
			else
			{
				break; //never execute
			}
		}



		//found the node to be deleted and save it in the deleteNode
		if (!(parent->is_threadl) && s == parent->left->number)
		{
			deleteNode = parent->left;
		}
		else if (!(parent->is_threadr) && s == parent->right->number)
		{
			deleteNode = parent->right;
		}
		else
		{
			//not found the node to be deleted
			return;
		}
	}



	/* Mainly three circumstances we need to consider about */

	//first : the deleted node has left child
	if (!(deleteNode->is_threadl))  
	{
		node* parent = deleteNode->left;
		if (parent->is_threadr)
		{
			deleteNode->number = parent->number;
			deleteNode->left = parent->left;
			if (deleteNode->left == head)
			{
				head->right = deleteNode;
			}
			deleteNode->is_threadl = parent->is_threadl;  //0 or 1 both might happen
			delete parent;
		}
		else
		{
			while (!(parent->right->is_threadr))
			{
				parent = parent->right;
			}
			node* temp = parent->right;
			deleteNode->number = temp->number;
			parent->right = temp->right;
			if (parent->right == tail)
			{
				tail->left = parent;
			}
			parent->is_threadr = 1;
			delete temp;
		}
	}
	//Second : the deleted node has right child
	else if (!(deleteNode->is_threadr)) 
	{
		node* parent = deleteNode->right;
		if (parent->is_threadl)
		{
			deleteNode->number = parent->number;
			deleteNode->right = parent->right;
			if (deleteNode->right == tail)
			{
				tail->left = deleteNode;
			}
			deleteNode->is_threadr = parent->is_threadr;
		}
		else
		{
			while (!(parent->left->is_threadl))
			{
				parent = parent->left;
			}

			node* temp = parent->left;
			deleteNode->number = temp->number;
			parent->left = temp->left;
			if (parent->left == head)
			{
				head->right = parent;
			}
			parent->is_threadl = 1;
			delete temp;
		}
	}

	//Third : the deleted node is a child node
	else   
	{
		if (parent->left == deleteNode)
		{
			parent->left = deleteNode->left;
			if (parent->left == head)
			{
				head->right = parent;
			}
			parent->is_threadl = 1;
		}
		else// if (parent->right == deleteNode)
		{
			parent->right = deleteNode->right;
			if (parent->right == tail)
			{
				tail->left = parent;
			}
			parent->is_threadr = 1;
		}
		delete deleteNode;
	}
}

void op_tree_totum::inorder_run(){
	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recurtion 
	node* p = head->right;
	while (p != tail)
	{
		printf("%d ", p->number);
		if (p->is_threadr)
		{
			p = p->right;
		}
		else
		{
			p = p->right;
			while (!(p->is_threadl))
			{
				p = p->left;
			}
		}
	}
}

void op_tree_totum::reverseorder_run(){
	//TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recurtion 
	node* p = tail->left;
	while (head != p)
	{
		printf("%d ", p->number);
		if (p->is_threadl)
		{
			p = p->left;
		}
		else
		{
			p = p->left;
			while (!(p->is_threadr))
			{
				p = p->right;
			}
		}
	}
}

int op_tree_totum::size(){
	return num;
}