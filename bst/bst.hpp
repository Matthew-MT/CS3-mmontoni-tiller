#define SFML_STATIC

#ifndef BST_HPP
#define BST_HPP

#include <functional>
#include <optional>
#include <vector>

using namespace std;

/**
 * Class that represents a single BST element.
 * Contains a value for data, count, left child pointer, and right child pointer.
 */
template<class DataType = string> class BSTElem {
private:
    DataType stored; // Data stored by this element
    unsigned int count = 1; // The number of times the data has been encountered
    BSTElem<DataType>* leftChild = nullptr, * rightChild = nullptr; // Pointers to the left and right children
    bool accessible = false; // Note: this is only to make sure access of the data is restricted until it is initialized
public:
    /**
     * Initialization constructor.
     */
    BSTElem(DataType init, unsigned int initCount = 1) {
        stored = init;
        accessible = true;
        count = initCount;
    }
    /**
     * Default constructor.
     */
    BSTElem() {}
    /**
     * Destructor. Deletes children.
     */
    ~BSTElem() {
        if (leftChild != nullptr) delete leftChild;
        if (rightChild != nullptr) delete rightChild;
    }

    /**
     * Public methods for accessing and setting members of the element.
     */
    void setLeft(BSTElem<DataType>* ptr) {leftChild = ptr;}
    void setRight(BSTElem<DataType>* ptr) {rightChild = ptr;}
    void incrementCount() {count++;}
    void setCount(unsigned int num) {count = num;}

    void setData(DataType* data, bool resetCount = true) {
        stored = data;
        accessible = true;
        if (resetCount) count = 1;
    }

    /**
     * Child access.
     */
    BSTElem<DataType>* left() {return leftChild;}
    BSTElem<DataType>* right() {return rightChild;}

    /**
     * Return the data stored in this element.
     */
    DataType getData() {
        if (accessible) return stored;
        else throw "Could not access data.\n";
    }

    /**
     * Return the number of times this element has been encountered.
     */
    unsigned int getCount() {return count;}
};

/**
 * Main binary search tree implementation.
 * Contains all the necessary methods for manipulating the tree,
 * including element lookup, element deletion, tree traversal, etc.
 */
template<class DataType = string> class BST {
public:
    /**
     * Enumerators for function behavior.
     */
    enum Behavior {left, right, increment, at};
    enum Traversal {preorder, inorder, postorder};
private:
    BSTElem<DataType>* root = nullptr; // Pointer to root element
    unsigned int height = 0, elements = 0; // Extra data
    function<typename BST<DataType>::Behavior(BSTElem<DataType>* parent, DataType data)> pushBehavior, searchFnc;
    // Funcions for searching and adding elements to the tree

    /**
     * Traverses down the left nodes of the given parent until it reaches a nullptr,
     * the returns the last and the second to last element found along that path.
     */
    pair<BSTElem<DataType>*, BSTElem<DataType>*> leftTraverse(BSTElem<DataType>* parent) {
        if (parent == nullptr) return {nullptr, nullptr};
        else {
            if (parent->left() == nullptr) return {nullptr, parent};
            else {
                pair<BSTElem<DataType>*, BSTElem<DataType>*> result = this->leftTraverse(parent->left());
                if (result.first == nullptr) result.first = parent;
                return result;
            }
        }
    }

    /**
     * Insert an element, using the specified insertion behavior function.
     */
    BSTElem<DataType>* insElem(BSTElem<DataType>* parent, DataType toBeAdded, unsigned int elemH = 0) {
        if (!(bool)pushBehavior) throw "Unspecified behavior function. Please set a behavior function.";
        if (parent == nullptr) {
            if (elemH > height) height = elemH;
            return new BSTElem<DataType>(toBeAdded); // Note: if parent is nullptr, then there are no children with which to operate
            // Therefore, insertion is the only option
        } else {
            BST<DataType>::Behavior result = pushBehavior(parent, toBeAdded);
            if (result == at) {
                delete parent; // Note: insertion function may cause overwriting of leaf nodes
                return new BSTElem<DataType>(toBeAdded);
            } else if (result == left) parent->setLeft(this->insElem(parent->left(), toBeAdded, elemH + 1));
            else if (result == right) parent->setRight(this->insElem(parent->right(), toBeAdded, elemH + 1));
            else if (result == increment) parent->incrementCount();
        }
        return parent; // Finally, return current ptr back to calling function if no elements are inserted
    }

    /**
     * Huge function to delete a single node.
     * Checks whether the node has left or right children, then returns a ptr to the replacement element.
     */
    BSTElem<DataType>* delElem(BSTElem<DataType>* ptr) {
        if (ptr == nullptr) return nullptr; // Can't do anything with a nullptr
        else if (ptr->left() != nullptr && ptr->right() != nullptr) { // Case: has both children
            pair<BSTElem<DataType>*, BSTElem<DataType>*> temp = this->leftTraverse(ptr->right());
            // Left traverse the right child to find a replacement element
            if (temp.second != nullptr) { // Case: replacement element has right child
                if ((temp.second)->right() != nullptr) {
                    BSTElem<DataType>* dummy = new BSTElem<DataType>("DUMMY"); // Dummy element
                    dummy->setLeft((temp.second)->left());
                    dummy->setRight((temp.second)->right());

                    if (temp.first != nullptr) { // Case: replacement element has different parent than the ptr
                        (temp.first)->setLeft(this->delElem(dummy));
                        (temp.second)->setRight(temp.first);
                    } else (temp.second)->setRight(this->delElem(dummy)); // Case: replacement element's parent is the ptr
                    (temp.second)->setLeft(ptr->left());

                    ptr->setLeft(nullptr);
                    ptr->setRight(nullptr);
                    delete ptr;
                    return ptr = temp.second;
                } else { // Case: replacement element does not have right child
                    if (temp.first != nullptr) { // Case: replacement element has different parent than the ptr
                        (temp.first)->setLeft(nullptr);
                        (temp.second)->setRight(temp.first);
                    } else (temp.second)->setRight(ptr->right()); // Case: replacement element's parent is the ptr
                    (temp.second)->setLeft(ptr->left());

                    ptr->setLeft(nullptr);
                    ptr->setRight(nullptr);
                    delete ptr;
                    return ptr = temp.second;
                }
            } else throw "Deletion errors detected.\n";
        } else if (ptr->left() != nullptr) { // Case: has left child but not right child
            BSTElem<DataType>* elem = ptr->left();
            ptr->setLeft(nullptr);
            delete ptr;
            return ptr = elem;
        } else if (ptr->right() != nullptr) { // Case: has right child but not left child
            BSTElem<DataType>* elem = ptr->right();
            ptr->setRight(nullptr);
            delete ptr;
            return ptr = elem;
        } else { // Case: no children
            delete ptr;
            return ptr = nullptr;
        }
    }

    /**
     * Return a vector of elements using the preorder traversal method.
     */
    vector<BSTElem<DataType>*>& preTrav(BSTElem<DataType>* ptr, vector<BSTElem<DataType>*>& curVect) {
        curVect.push_back(ptr);
        if (ptr->left() != nullptr) this->preTrav(ptr->left(), curVect);
        if (ptr->right() != nullptr) this->preTrav(ptr->right(), curVect);
        return curVect;
    }
    
    /**
     * Return a vector of elements using the inorder traversal method.
     */
    vector<BSTElem<DataType>*>& inTrav(BSTElem<DataType>* ptr, vector<BSTElem<DataType>*>& curVect) {
        if (ptr->left() != nullptr) this->inTrav(ptr->left(), curVect);
        curVect.push_back(ptr);
        if (ptr->right() != nullptr) this->inTrav(ptr->right(), curVect);
        return curVect;
    }
    
    /**
     * Return a vector of elements using the postorder traversal method.
     */
    vector<BSTElem<DataType>*>& postTrav(BSTElem<DataType>* ptr, vector<BSTElem<DataType>*>& curVect) {
        if (ptr->left() != nullptr) this->postTrav(ptr->left(), curVect);
        if (ptr->right() != nullptr) this->postTrav(ptr->right(), curVect);
        curVect.push_back(ptr);
        return curVect;
    }

    /**
     * Search for a specific element and return a pointer to the node.
     */
    pair<BSTElem<DataType>*, BSTElem<DataType>*> searchElem(BSTElem<DataType>* parent, DataType toBeFound) {
        if (parent == nullptr) return {nullptr, nullptr};
        BST<DataType>::Behavior result = searchFnc(parent, toBeFound);
        if (result == at) return {parent, nullptr};
        else if (result == left) {
            pair<BSTElem<DataType>*, BSTElem<DataType>*> returned = this->searchElem(parent->left(), toBeFound);
            if (returned.second == nullptr && returned.first != nullptr) returned.second = parent;
            return returned; 
        } else if (result == right) {
            pair<BSTElem<DataType>*, BSTElem<DataType>*> returned = this->searchElem(parent->right(), toBeFound);
            if (returned.second == nullptr && returned.first != nullptr) returned.second = parent;
            return returned; 
        } else return {nullptr, nullptr};
    }

    /**
     * Return the height of the tree to the bottom.
     */
    unsigned int getHeightToBottom(BSTElem<DataType>* parent, unsigned int prev = 0) {
        if (parent == nullptr) return prev;
        else {
            unsigned int
                leftRes = this->getHeightToBottom(parent->left(), prev + 1),
                rightRes = this->getHeightToBottom(parent->right(), prev + 1);
            return (leftRes > rightRes ? leftRes : rightRes);
        }
    }
public:
    /**
     * Default constructor, takes an optional vector of initiation data to initialize the tree with.
     */
    BST(const vector<DataType>& init = {}) {
        // Initialize insertion function to default
        this->setInsertFnc([](BSTElem<DataType>* parent, DataType data) -> BST<DataType>::Behavior {
            if (parent == nullptr) return at;
            DataType parentData = parent->getData();
            if (data > parentData) return right;
            else if (data < parentData) return left;
            else if (data == parentData) return increment;
            else throw "Push function errors detected.";
        });
        // Initialize search function to default
        this->setSearchFnc([](BSTElem<DataType>* parent, DataType data) -> BST<DataType>::Behavior {
            if (parent == nullptr) return at;
            DataType parentData;
            try {parentData = parent->getData();} catch (string err) {cout << "ERROR";return at;}
            if (data > parentData) return right;
            else if (data < parentData) return left;
            else if (data == parentData) return at;
            else throw "Search function errors detected.";
        });
        for (int i = 0; i < init.size(); i++) this->insertElement(init.at(i));
    }

    ~BST() {
        delete root;
        /**
         * Note:
         * Becase each element automatically deletes its children,
         * there is no need to traverse the entire tree to free its memory.
         * The only thing you need to do is delete root.
         */
    }

    /**
     * Public element search method.
     */
    BSTElem<DataType>* searchFor(DataType item) {return (this->searchElem(root, item)).first;}

    /**
     * Public generic traversal function.
     */
    vector<BSTElem<DataType>*> traverse(Traversal ofType) {
        vector<BSTElem<DataType>*> result;
        if (ofType == preorder) preTrav(root, result);
        else if (ofType == inorder) inTrav(root, result);
        else if (ofType == postorder) postTrav(root, result);
        return result;
    }

    /**
     * Public preorder traversal method.
     */
    const vector<BSTElem<DataType>*>& preorderTraversal() {
        vector<BSTElem<DataType>*> result;
        return preTrav(root, result);
    }

    /**
     * Public inorder traversal method.
     */
    const vector<BSTElem<DataType>*>& inorderTraversal() {
        vector<BSTElem<DataType>*> result;
        return inTrav(root, result);
    }

    /**
     * Public postorder traversal method.
     */
    const vector<BSTElem<DataType>*>& postorderTraversal() {
        vector<BSTElem<DataType>*> result;
        return postTrav(root, result);
    }

    /**
     * Public function to delete a node.
     */
    bool deleteElement(DataType toBeDeleted) {
        pair<BSTElem<DataType>*, BSTElem<DataType>*> place = this->searchElem(root, toBeDeleted); // Acquire a pointer to the node if it exists
        if (place.first == nullptr) return false; // Return false if element not found
        else {
            if (place.second != nullptr) { // Make sure the element's parent node is maintained
                bool leftSide = (place.second)->left() == place.first;
                BSTElem<DataType>* temp = this->delElem(place.first);
                if (leftSide) (place.second)->setLeft(temp);
                else (place.second)->setRight(temp);
                return (bool)temp;
            } else if (root == place.first) return (bool)(root = this->delElem(place.first));
            else throw "Deletion errors detected.\n";
        }
    }

    /**
     * Public insertion method.
     */
    void insertElement(DataType toBeAdded) {root = this->insElem(root, toBeAdded);}

    /**
     * Get the height of the tree from the root.
     */
    unsigned int getHeight() {return height = getHeightToBottom(root);}

    /**
     * Function setter for the insertion behavior function.
     */
    void setInsertFnc(function<typename BST<DataType>::Behavior(BSTElem<DataType>* parent, DataType data)> fnc) {pushBehavior = fnc;}

    /**
     * Function setter for the search behavior function.
     */
    void setSearchFnc(function<typename BST<DataType>::Behavior(BSTElem<DataType>* parent, DataType data)> fnc) {searchFnc = fnc;}
};

#endif