#include <iostream>
using namespace std;

struct Node
{
	Node(Node* parent = nullptr)		
	{									
		this->parent = parent;
	}

	int tag = 0;

	Node* parent = nullptr;
	Node* child = nullptr;
	Node* brother = nullptr;
};

struct Tree
{
	Node* root = nullptr;
	Node** leaves = nullptr;			 
	Node** tops = nullptr;		//branches (inner nodes 0, -1, -2, --- -5)
	int amount_of_leaves = 0;		
	int amount_of_tops = 0;	
};


//-------Functions----------
void Newick_format(Tree*& Trees, int& number_of_trees);				//reshape input into the tree
void Prepare_tree(Tree& tree);										//create tree
void Fill_lists(Node* curent_node, Node** leaves_list, Node** tops_list, int& top_place, int& leaf_place);	//fill the tree 

void Create_array(int**& arr, const int& x, const int& y);		//array for comparison of trees

int Position(Node**& leaf_list, int& max_size, int& tag);		//search for leaf position

void Compare_leaves(Node**& leaf_list_1, Node**& leaf_list_2, int& leaves_count_1, int& leaves_count_2, int**& arr);		//compare leaves from both trees 
void Compare_leaf_top(Node**& leaf_list_1, Node**& leaf_list_2, int& leaves_count_1, int& leaves_count_2, bool change_coord, int**& arr); //compare nodes with children )
void Compare_tops(Tree& tree_1, Tree& tree_2, int**& arr);	//return max value


void Fill_children_list(Node**& child_list, Node*& subtree, int& children_count);
int Compare_children(Tree& tree_1, Tree& tree_2, Node*& subtree_1, Node*& subtree_2, int& max, bool second_to_first, int**& arr);
void Permutations(Tree& tree_1, Tree& tree_2, Node**& children_list_1, Node**& children_list_2,
	int& right, int& shorter_list_size, int& max, int** arr, bool second_child_bigger, int left = 0);

int Max(Tree& tree_1, Tree& tree_2, Node*& subtree_1, Node*& subtree_2, int**& arr);	//search for max value

void Delete_tree(Node*& node);		
//-----------------


//------------------------------------------------
void Newick_format(Tree*& Trees, int& number_of_trees)
{
	cin >> number_of_trees;

	Trees = new Tree[number_of_trees];
	Node* ptr_to_tree = nullptr;

	char input = 0;
	int parent_tag = 0;

	for (int i = 0; i < number_of_trees; i++)
	{
		ptr_to_tree = new Node;
		input = 0;
		parent_tag = 0;		//parent label 0>=

		while (input != ';')
		{
			cin >> input;

			if (input == '(')	
			{
				ptr_to_tree->tag = parent_tag;
				parent_tag--;						

				Trees[i].amount_of_tops++;	

				ptr_to_tree->child = new Node(ptr_to_tree);	
				ptr_to_tree = ptr_to_tree->child;	
			}
			else if (input == ',')
			{
				ptr_to_tree->brother = new Node(ptr_to_tree->parent);		
				ptr_to_tree = ptr_to_tree->brother;			
			}
			else if (input == ')')
			{
				ptr_to_tree = ptr_to_tree->parent;
			}
			else if (input != ';') //if number
			{
				//if the number is multi-digit
				if (ptr_to_tree->tag == 0)	//if there is a number then we assign it as a label to the current node we are in
					Trees[i].amount_of_leaves++;

				ptr_to_tree->tag *= 10;				
				ptr_to_tree->tag += input - '0';	//change char to int 
			}
		}//while
		Trees[i].root = ptr_to_tree; //assign the tree to the root 
	}
}

void Prepare_tree(Tree& tree)
{
	Node** leafs = new Node * [tree.amount_of_leaves];
	Node** tops = new Node * [tree.amount_of_tops];

	tree.amount_of_tops = 0;		
	tree.amount_of_leaves = 0;

	Fill_lists(tree.root, leafs, tops, tree.amount_of_tops, tree.amount_of_leaves);

	tree.leaves = leafs;			
	tree.tops = tops;
}

//top_place - node	top_list - array of nodes
void Fill_lists(Node* current_node, Node** leaves_list, Node** top_list, int& top_place, int& leaf_place)
{
	if (current_node)
	{
		if (current_node->tag > 0)	//if >0 then leaves
		{
			leaves_list[leaf_place] = current_node;
			leaf_place++;
		}
		else		//if < 0 then nodes  from -5 to 0
		{
			top_list[top_place] = current_node;
			top_place++;
		}
		Fill_lists(current_node->child, leaves_list, top_list, top_place, leaf_place);	
		Fill_lists(current_node->brother, leaves_list, top_list, top_place, leaf_place);
	}
}
//------------------------------------------------


// x-number of leaves and nodes in one tree, y-number of leaves and nodes in the second tree
void Create_array(int**& arr, const int& x, const int& y)		
{
	arr = new int* [x];				

	for (int i = 0; i < x; ++i)
	{
		arr[i] = new int[y];		
		for (int j = 0; j < y; j++)
			arr[i][j] = 0;		 
	}
}


//------------------------------------------------
//number of leaves - max_size
int Position(Node**& leaf_list, int& max_size, int& tag)
{
	if (tag < 1)		
		return max_size + (tag * (-1));		

	int position = 0;

	while (leaf_list[position]->tag != tag)		
		position++;

	return position;
}

//leaves1 - number of leaves of one tree , leaves2 - number of leaves of the second tree
void Compare_leaves(Node**& leaf_list_1, Node**& leaf_list_2, int& leaves_count_1, int& leaves_count_2, int**& arr)
{
	for (int i = 0; i < leaves_count_1; i++)
	{
		for (int j = 0; j < leaves_count_2; j++)
		{
			if (leaf_list_1[i]->tag == leaf_list_2[j]->tag)
			{
				arr[i][j] = 1;
				break;
			}
		}
	}
}


void Compare_leaf_top(Node**& leaf_list_1, Node**& leaf_list_2, int& leaves_count_1, int& leaves_count_2, bool change_coord, int**& arr)
{
	Node* ptr = nullptr;	
	int p = 0;

	for (int i = 0; i < leaves_count_1; i++)						 
	{
		ptr = leaf_list_1[i];										//enter the leaf list of the first tree and check its position in the second tree
		p = Position(leaf_list_2, leaves_count_2, ptr->tag);		//return the leaf position from the second tree
	
		while (ptr->parent)			
		{
			ptr = ptr->parent;		

			if (!change_coord)
				arr[leaves_count_1 + (ptr->tag * (-1))][p] = 1;		
			// take a leaf, enter the parent, select 1, enter the parent again, and so on back to the beginning - 0 
			else
				arr[p][leaves_count_1 + (ptr->tag * (-1))] = 1;
		}
	}
}


void Compare_tops(Tree& tree_1, Tree& tree_2, int**& arr)
{
	for (int i = tree_1.amount_of_leaves; i < (tree_1.amount_of_leaves + tree_1.amount_of_tops); i++)
	{
		for (int j = tree_2.amount_of_leaves; j < (tree_2.amount_of_leaves + tree_2.amount_of_tops); j++)
		{
			arr[i][j] = -1;
		}
	}

	cout << tree_1.amount_of_leaves - Max(tree_1, tree_2, tree_1.tops[0], tree_2.tops[0], arr) << endl;
}
//------------------------------------------------


//------------------------------------------------
void Fill_children_list(Node**& child_list, Node*& subtree, int& children_count)
{
	Node* ptr_to_tree = subtree->child;

	for (int i = 0; i < children_count; i++)
	{
		child_list[i] = ptr_to_tree;

		if (ptr_to_tree)
			ptr_to_tree = ptr_to_tree->brother;
	}
}


int Max(Tree& tree_1, Tree& tree_2, Node*& subtree_1, Node*& subtree_2, int**& arr)
{

	int max = 0;				
	int children_count_1 = 0;	
	int children_count_2 = 0;

	
	int y = Position(tree_1.leaves, tree_1.amount_of_leaves, subtree_1->tag);		
	int x = Position(tree_2.leaves, tree_2.amount_of_leaves, subtree_2->tag);		

	if (arr[y][x] != -1) return arr[y][x];		

	children_count_1 = Compare_children(tree_1, tree_2, subtree_1, subtree_2, max, false, arr);		
	children_count_2 = Compare_children(tree_1, tree_2, subtree_1, subtree_2, max, true, arr);		

	Node** child_1 = new Node * [children_count_1];
	Node** child_2 = new Node * [children_count_2];

	Fill_children_list(child_1, subtree_1, children_count_1);
	Fill_children_list(child_2, subtree_2, children_count_2);

	if (children_count_1 < children_count_2) // if it is not square matrix
		Permutations(tree_2, tree_1, child_2, child_1, children_count_2, children_count_1, max, arr, true);		//permute bigger tree 
	else
		Permutations(tree_1, tree_2, child_1, child_2, children_count_1, children_count_2, max, arr, false);

	delete[] child_1;
	delete[] child_2;

	arr[y][x] = max;

	return max;
}


int Compare_children(Tree& tree_1, Tree& tree_2, Node*& subtree_1, Node*& subtree_2, int& max, bool second_to_first, int**& arr)
{
	Node* ptr_to_tree = nullptr;

	if (second_to_first)
		ptr_to_tree = subtree_2->child;
	else
		ptr_to_tree = subtree_1->child;		

	int children_count = 0;
	int tmp = 0;		

	while (ptr_to_tree != nullptr)
	{
		children_count++;

		if (second_to_first)
			tmp = Max(tree_1, tree_2, subtree_1, ptr_to_tree, arr);		
		else
			tmp = Max(tree_1, tree_2, ptr_to_tree, subtree_2, arr);		

		if (tmp > max)			
			max = tmp;

		ptr_to_tree = ptr_to_tree->brother;	
	}

	return children_count;
}

//right - size of the bigger tree
//left - current position
void Permutations(Tree& tree_1, Tree& tree_2, Node**& children_list_1, Node**& children_list_2,
	int& right, int& shorter_list_size, int& max, int** arr, bool second_child_is_bigger, int left){
	if (left != shorter_list_size)
	{
		for (int i = left; i < right; i++)		
		{
			swap(children_list_1[left], children_list_1[i]);		//to obtain all possible permutations
			Permutations(tree_1, tree_2, children_list_1, children_list_2, right, shorter_list_size, max, arr, second_child_is_bigger, left + 1);
			swap(children_list_1[left], children_list_1[i]);		
		}
	}
	else
		
	{
		int value_of_permutation = 0;		

		for (int i = 0; i < shorter_list_size; i++)
		{
			if (second_child_is_bigger) {
				int y = Position(tree_2.leaves, tree_2.amount_of_leaves, children_list_2[i]->tag);
				int x = Position(tree_1.leaves, tree_1.amount_of_leaves, children_list_1[i]->tag);

				value_of_permutation += arr[y][x];
			}
			else {
				int x = Position(tree_2.leaves, tree_2.amount_of_leaves, children_list_2[i]->tag);		
				int y = Position(tree_1.leaves, tree_1.amount_of_leaves, children_list_1[i]->tag);

				value_of_permutation += arr[x][y];
			}
		}
		if (value_of_permutation > max)
			max = value_of_permutation;
	}
}

//------------------------------------------------
void Delete_tree(Node*& node)
{
	if (node != nullptr)
	{
		Delete_tree(node->child);
		Delete_tree(node->brother);
		delete node;
		node = nullptr;
	}
}


int main()

{
	Tree* Trees = nullptr;
	int** arr = nullptr;
	int number_of_trees = 0;

	Newick_format(Trees, number_of_trees);

	for (int i = 0; i < number_of_trees; i++)
	{
		Prepare_tree(Trees[i]);
	}
	for (int i = 0; i < number_of_trees; i++)
	{
		for (int j = 1 + i; j < number_of_trees; j++)
		{
			Create_array(arr, /*x*/Trees[i].amount_of_leaves + Trees[i].amount_of_tops, /*y*/Trees[j].amount_of_leaves + Trees[j].amount_of_tops);

			Compare_leaves(Trees[i].leaves, Trees[j].leaves, Trees[i].amount_of_leaves, Trees[j].amount_of_leaves, arr);

			Compare_leaf_top(Trees[i].leaves, Trees[j].leaves, Trees[i].amount_of_leaves, Trees[j].amount_of_leaves, false, arr);

			Compare_leaf_top(Trees[j].leaves, Trees[i].leaves, Trees[j].amount_of_leaves, Trees[i].amount_of_leaves, true, arr);

			Compare_tops(Trees[i], Trees[j], arr);

			for (int ii = 0; ii < Trees[i].amount_of_leaves + Trees[i].amount_of_tops; ii++)
			{
				delete[] arr[ii];
			}
			delete[] arr;
		}
	}
	for (int i = 0; i < number_of_trees; i++)
	{
		Delete_tree(Trees[i].root);
		delete[] Trees[i].leaves;
		delete[] Trees[i].tops;
	}
	delete[] Trees;
	return 0;
}
