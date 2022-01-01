# Maximum agreement subtree problem
This project was made for _Algorithms and Data Structures_ course at University of Technology in Gdańsk.

**Instructions:**
The exercise is to implement an algorithm that reads a set of **n** rooted trees in the _NEWICK_ format, and then for each of **n(n-1)/2** pairs of trees will calculate the minimum number of leaves whose removal from both trees will make them isomorphic. Here are two examples of trees, each containing 10 identical leaves labeled from 1 to 10:
(4,(8,1,(5,3)),(9,2,(10,(7,6))));
(10,(8,(9,(5,4)),(6,2,3)),(7,1));

The trees should be saved in the computer's memory in the form of general trees, using one pointer per son, one per brother and one per parent.

**How to use the program:**
Input
-> The value of n denotes the size of the set of n rooted trees in _NEWICK_ format
Example:
4
(1,5,(10,6,3),(2,(8,7)),(9,4));
((7,(3,(4,9,(1,2)))),8,(5,(10,6)));
(7,((6,(9,5),(8,3)),(1,(2,10,4))));
(7,(8,3,4,6,1),(9,5,10),2);

Output
-> **n(n-1)/2** comparisons of input trees (each with each). The order of comparison: 1-2, 1-3, ..., 1-n, 2-3, 2-4, ..., 2-n, ..., (n-1) -n.
Example:
5
7
6
6
6
6
