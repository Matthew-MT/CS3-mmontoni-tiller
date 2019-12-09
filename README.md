# CS3-mmontoni-till

CS3 Project repo. Contains all projects for [CSCI-250-09.19.19].

## 1: Falling apart
- Solved using vector template class and sort/reverse functions (50 pts)
- Wrote and tested 3 independent test cases (10*3 = 30 pts)
- Screenshot of Kattis submission page in project folder (10 pts)
- Total points: 100

## 2: Babelfish
- Solved using unordered_map template class. Also used std::regex. Kattis accepted (50 pts)
- Wrote and tested 3 independent test cases (10*3 = 30 pts)
- Screenshot is in the file screenshot.jpg (10 pts)
- Total points: 100

## 3: CD
- Solved using unordered_map template class and binary_search. Kattis accepted (20+30 = 50 pts)
- Wrote and tested 3 independent test cases (10*3 = 30 pts)
- Screenshot is in the file screenshot.jpg (10 pts)
- Total points: 100

## 4: Putovanje
- Solved using vector template class and pointer manipulation. Kattis accepted (20+30 = 50 pts)
- Wrote and tested 3 independent test cases (10*3 = 30 pts)
- Screenshot is in the file screenshot.jpg (10 pts)
- Total points: 100

## 5: ICPC Tutorial
- Solved using lambda functions. No template classes necessary. Kattis accepted (50 pts)
- Wrote and tested 3 independent test cases (10*3 = 30 pts)
- Screenshot is in the file screenshot.jpg (10 pts)
- Total points: 100

## 6: Binary Search Tree
Note: All code in the folder bst is mine, except for the code in bst/sfml. bst/sfml is the library I used to make the interface work.
- Main file: bst.cpp
    - Windows-runnable executable: bst.exe
- Implemented BST on my own, including all relevant methods (40 pts)
    - File with implementation is in bst.hpp
- Provided a prompt to input a file for the program to read into the tree (20 pts)
    - Used sample file csi.txt
- Provided menu options for the user to display the words in all three BST traversal techniques, to the screen (10 pts)
- Provided a menu option to look up a word and display how many times it has been found in the file (10 pts)
- Provided an option to delete a word from the tree (10 pts)
- Provided a menu option to print the top ten words in terms of frequency to a file specified by the user (10 pts)
- Other data:
    - Nodes are initialized to 1
    - Punctuation is ignored
    - Provided a menu option to exit program
    - Total points: 100

## Extra Credit: Where's My Internet
Note: Used own implementation of Graph.
- Solved with minor tweaks to DFS algorithm. Kattis accepted (65 pts)
- Wrote and tested 3 independent test cases (3*10 = 30 pts)
- Screenshot is in the file screenshot.jpg (5 pts)
- Total points: 100

## 7: Single Source, Shortest Path, Non-Negative Weights
Note: I'm not too sure how I feel about using the algorithm provided directly in the notebook to solve this problem.
Just feels too easy. It took only one try to get it to work, versus my own implementation.
- Solved using Dijkstra's algorithm from CS3 notebook. Kattis accepted (65 pts)
- Wrote and tested 3 independent test cases (3*10 = 30 pts)
- Screenshot is in the file screenshot.jpg (5 pts)
- Total points: 100

## 8: Minimum Spanning Tree
Note: Attempt at old solution is in folder `old_solns`.
- Solved using Kruskal's algorithm. Kattis accepted (85 pts)
- Wrote and tested 3 independent test cases (3*5 = 15 pts)
- Screenshot is in the file screenshot.jpg (0 pts)
- Total points: 100

## Final Project
Note: I attempted to create a GUI to go with this project, but I wasn't able to get it working on time.
Instead what I have created is a mish mash of different programs to try and reliably test the given functions.
### Important: Times are displayed as CPU cycles, not seconds.
### Also important: Due to difficulties, timeComplexity.txt is hand-sorted.
- Tested 100k, 500k, & 1m random values (5 pts)
- Tested 9 different sorting algorithms, gathered full data from 6 of them (6*5 = 30 pts)
- Program tests each sort algorithm 10 times (15 pts)
- Tested 3 different search algorithms (5*3 = 15 pts)
- Program tests each search algorithm 10 times (10 pts)
- Sorted algorithm running times are in the file timeComplexity.txt, sorted manually (5 pts)
- No bonus points
- Total points: 80