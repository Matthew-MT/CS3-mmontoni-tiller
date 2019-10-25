#define SFML_STATIC

#ifndef BST_HPP
#define BST_HPP

#include <functional>
#include <optional>
#include <vector>

using namespace std;

template<class DataType> class BSTElem {
private:
    DataType stored;
    unsigned int count = 1;
    BSTElem<DataType>* leftChild = nullptr, * rightChild = nullptr;
    bool accessible = false;
public:
    BSTElem(DataType init, unsigned int initCount = 1) {
        stored = init;
        accessible = true;
        count = initCount;
    }
    BSTElem() {}
    ~BSTElem() {
        if (leftChild != nullptr) delete leftChild;
        if (rightChild != nullptr) delete rightChild;
    }

    void setLeft(BSTElem<DataType>* ptr) {leftChild = ptr;}
    void setRight(BSTElem<DataType>* ptr) {rightChild = ptr;}
    void incrementCount() {count++;}
    void setCount(unsigned int num) {count = num;}

    void setData(DataType* data, bool resetCount = true) {
        stored = data;
        accessible = true;
        if (resetCount) count = 1;
    }

    BSTElem<DataType>* left() {return leftChild;}
    BSTElem<DataType>* right() {return rightChild;}
    DataType getData() {
        if (accessible) return stored;
        else throw "Could not access data.\n";
    }
    unsigned int getCount() {return count;}
};

template<class DataType = string> class BST {
public:
    enum Behavior {left, right, increment, at};
    enum Traversal {preorder, inorder, postorder};
private:
    BSTElem<DataType>* root = nullptr;
    unsigned int height = 0, elements = 0;
    function<typename BST<DataType>::Behavior(BSTElem<DataType>* parent, DataType data)> pushBehavior, searchFnc;

    pair<BSTElem<DataType>*, BSTElem<DataType>*> leftTraverse(BSTElem<DataType>* parent) {
        if (parent == nullptr) return {nullptr, nullptr};
        else {
            //cout << "Here once\n";
            if (parent->left() == nullptr) return {nullptr, parent};
            else {
                //cout << "Here twice\n";
                pair<BSTElem<DataType>*, BSTElem<DataType>*> result = leftTraverse(parent->left());
                if (result.first == nullptr) result.first = parent;
                return result;
            }
        }
    }

    BSTElem<DataType>* insElem(BSTElem<DataType>* parent, DataType toBeAdded, unsigned int elemH = 0) {
        if (!(bool)pushBehavior) throw "Unspecified behavior function. Please set a behavior function.";
        if (parent == nullptr) {
            if (elemH > height) height = elemH;
            return new BSTElem<DataType>(toBeAdded);
        } else {
            BST<DataType>::Behavior result = pushBehavior(parent, toBeAdded);
            if (result == at) return new BSTElem<DataType>(toBeAdded);
            else if (result == left) parent->setLeft(insElem(parent->left(), toBeAdded, elemH + 1));
            else if (result == right) parent->setRight(insElem(parent->right(), toBeAdded, elemH + 1));
            else if (result == increment) parent->incrementCount();
        }
        return parent;
    }

    BSTElem<DataType>* delElem(BSTElem<DataType>* ptr) {
        cout << "Deleting...\n";
        if (ptr != nullptr) cout << "Element: " << ptr->getData() << '\n';
        if (ptr == nullptr) return nullptr;
        else if (ptr->left() != nullptr && ptr->right() != nullptr) {
            cout << "Has both children...\n";
            pair<BSTElem<DataType>*, BSTElem<DataType>*> temp = this->leftTraverse(ptr->right());
            if (temp.second != nullptr) {
                cout << "Second exists...\n";
                if ((temp.second)->right() != nullptr) {
                    cout << "Second has right child...\n";
                    BSTElem<DataType>* dummy = new BSTElem<DataType>("DUMMY");
                    dummy->setLeft((temp.second)->left());
                    dummy->setRight((temp.second)->right());

                    if (temp.first != nullptr) {
                        (temp.first)->setLeft(this->delElem(dummy));
                        (temp.second)->setRight(temp.first);
                    } else (temp.second)->setRight(this->delElem(dummy));
                    (temp.second)->setLeft(ptr->left());

                    ptr->setLeft(nullptr);
                    ptr->setRight(nullptr);
                    delete ptr;
                    return ptr = temp.second;
                } else {
                    cout << "Second has no children...\n";
                    if (temp.first != nullptr) {
                        (temp.first)->setLeft(nullptr);
                        (temp.second)->setRight(temp.first);
                    } else (temp.second)->setRight(ptr->right());
                    (temp.second)->setLeft(ptr->left());

                    ptr->setLeft(nullptr);
                    ptr->setRight(nullptr);
                    delete ptr;
                    return ptr = temp.second;
                }
            } else throw "Deletion errors detected.\n";
        } else if (ptr->left() != nullptr) {
            cout << "Has left only...\n";
            BSTElem<DataType>* elem = ptr->left();
            ptr->setLeft(nullptr);
            delete ptr;
            return ptr = elem;
        } else if (ptr->right() != nullptr) {
            cout << "Has right only...\n";
            BSTElem<DataType>* elem = ptr->right();
            cout << "Here 1...\n";
            ptr->setRight(nullptr);
            cout << "Here 2...\n";
            delete ptr;
            cout << "Here 3...\n";
            cout << (ptr == nullptr ? "nullptr" : "bad") << '\n';
            cout << (elem != nullptr ? elem->getData() : "elem is nullptr too") << '\n';
            return ptr = elem;
            /*pair<BSTElem<DataType>*, BSTElem<DataType>*> temp = this->leftTraverse(ptr->right());
            if (temp.first != nullptr) {
                BSTElem<DataType>* dummy = new BSTElem<DataType>("DUMMY");
                dummy->setRight((temp.second)->right());

                if (temp.first != nullptr) {
                    (temp.first)->setLeft(delElem(dummy));
                    (temp.second)->setRight(temp.first);
                } else (temp.second)->setRight(delElem(dummy));
            }
            ptr->setRight(nullptr);
            delete ptr;
            return ptr = temp.second;*/
        } else {
            cout << "Has no children...\n";
            delete ptr;
            return ptr = nullptr;
        }
        /*if (ptr == nullptr) return nullptr;
        else if (ptr->left() != nullptr && ptr->right() != nullptr) {
            cout << "Start both\n";
            pair<BSTElem<DataType>*, BSTElem<DataType>*> temp = this->leftTraverse(ptr->right());
            cout << (temp.first)->getData() << ' ' << (temp.second)->getData() << '\n';
            if (temp.second != nullptr && (temp.second)->right() != nullptr) {
                BSTElem<DataType>* elem = temp.second;
                temp.second = new BSTElem<DataType>(); //create dummy element
                (temp.second)->setRight(elem->right());
                elem->setLeft(ptr->left());
                elem->setRight(ptr->right());
                ptr->setLeft(nullptr);
                ptr->setRight(nullptr);
                delElem(ptr);
                if (temp.first != nullptr) (temp.first)->setLeft(delElem(temp.second));
                else {
                    elem->setRight(delElem(temp.second));
                    return elem;
                }
                return elem;
            } else if (temp.second != nullptr) return delElem(temp.second);
            else return nullptr;
        } else if (ptr->left() != nullptr) {
            cout << "Start left\n";
            pair<BSTElem<DataType>*, BSTElem<DataType>*> temp = this->leftTraverse(ptr->left());
            if ((temp.second)->right() != nullptr) {
                BSTElem<DataType>* elem = temp.second;
                temp.second = new BSTElem<DataType>(); //create dummy element
                (temp.second)->setRight(elem->right());
                elem->setLeft(ptr->left());
                elem->setRight(nullptr);
                ptr->setLeft(nullptr);
                delElem(ptr);
                if (temp.first != nullptr) (temp.first)->setLeft(delElem(temp.second));
                else elem->setLeft(delElem(temp.second));
                return elem;
            } else {
                if (temp.first != nullptr) (temp.first)->setLeft(nullptr);
                (temp.second)->setLeft(ptr->left());
                return temp.second;
            }
        } else if (ptr->right() != nullptr) {
            cout << "Start right\n";
            pair<BSTElem<DataType>*, BSTElem<DataType>*> temp = this->leftTraverse(ptr->right());
            if ((temp.second)->right() != nullptr) {
                BSTElem<DataType>* elem = temp.second;
                temp.second = new BSTElem<DataType>(); //create dummy element
                (temp.second)->setRight(elem->right());
                elem->setLeft(nullptr);
                elem->setRight(ptr->right());
                ptr->setRight(nullptr);
                delElem(ptr);
                if (temp.first != nullptr) (temp.first)->setLeft(delElem(temp.second));
                else elem->setRight(delElem(temp.second));
                return elem;
            } else {
                if (temp.first != nullptr) (temp.first)->setLeft(nullptr);
                (temp.second)->setRight(ptr->right());
                return temp.second;
            }
        } else {
            delete ptr;
            return nullptr;
        }*/
    }

    vector<BSTElem<DataType>*>& preTrav(BSTElem<DataType>* ptr, vector<BSTElem<DataType>*>& curVect) {
        curVect.push_back(ptr);
        if (ptr->left() != nullptr) preTrav(ptr->left(), curVect);
        if (ptr->right() != nullptr) preTrav(ptr->right(), curVect);
        return curVect;
    }
    vector<BSTElem<DataType>*>& inTrav(BSTElem<DataType>* ptr, vector<BSTElem<DataType>*>& curVect) {
        if (ptr->left() != nullptr) inTrav(ptr->left(), curVect);
        curVect.push_back(ptr);
        if (ptr->right() != nullptr) inTrav(ptr->right(), curVect);
        return curVect;
    }
    vector<BSTElem<DataType>*>& postTrav(BSTElem<DataType>* ptr, vector<BSTElem<DataType>*>& curVect) {
        if (ptr->left() != nullptr) postTrav(ptr->left(), curVect);
        if (ptr->right() != nullptr) postTrav(ptr->right(), curVect);
        curVect.push_back(ptr);
        return curVect;
    }

    pair<BSTElem<DataType>*, BSTElem<DataType>*> searchElem(BSTElem<DataType>* parent, DataType toBeFound) {
        if (parent == nullptr) return {nullptr, nullptr};
        BST<DataType>::Behavior result = searchFnc(parent, toBeFound);
        if (result == at) return {parent, nullptr};
        else if (result == left) {
            pair<BSTElem<DataType>*, BSTElem<DataType>*> returned = searchElem(parent->left(), toBeFound);
            if (returned.second == nullptr && returned.first != nullptr) returned.second == parent;
            return returned; 
        } else if (result == right) {
            pair<BSTElem<DataType>*, BSTElem<DataType>*> returned = searchElem(parent->right(), toBeFound);
            if (returned.second == nullptr && returned.first != nullptr) returned.second == parent;
            return returned; 
        } else return {nullptr, nullptr};
    }

    unsigned int getHeightToBottom(BSTElem<DataType>* parent, unsigned int prev = 0) {
        if (parent == nullptr) return prev;
        else {
            unsigned int
                leftRes = getHeight(parent->left(), prev + 1),
                rightRes = getHeight(parent->right(), prev + 1);
            return (leftRes > rightRes ? leftRes : rightRes);
        }
    }
public:
    BST(const vector<DataType>& init = {}) {
        this->setInsertFnc([](BSTElem<DataType>* parent, DataType data) -> BST<DataType>::Behavior {
            if (parent == nullptr) return at;
            DataType parentData = parent->getData();
            if (data > parentData) return right;
            else if (data < parentData) return left;
            else if (data == parentData) return increment;
            else throw "Push function errors detected.";
        });
        this->setSearchFnc([](BSTElem<DataType>* parent, DataType data) -> BST<DataType>::Behavior {
            if (parent == nullptr) return at;
            try {cout << "In here: " << parent->getData() << '\n';} catch (string err) {cout << "ERROR";return at;}
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

    BSTElem<DataType>* searchFor(DataType item) {return (this->searchElem(root, item)).first;}

    vector<BSTElem<DataType>*> traverse(Traversal ofType) {
        vector<BSTElem<DataType>*> result;
        if (ofType == preorder) preTrav(root, result);
        else if (ofType == inorder) inTrav(root, result);
        else if (ofType == postorder) postTrav(root, result);
        return result;
    }

    const vector<BSTElem<DataType>*>& preorderTraversal() {
        vector<BSTElem<DataType>*> result;
        return preTrav(root, result);
    }

    const vector<BSTElem<DataType>*>& inorderTraversal() {
        vector<BSTElem<DataType>*> result;
        return inTrav(root, result);
    }

    const vector<BSTElem<DataType>*>& postorderTraversal() {
        vector<BSTElem<DataType>*> result;
        return postTrav(root, result);
    }

    bool deleteElement(DataType toBeDeleted) {
        pair<BSTElem<DataType>*, BSTElem<DataType>*> place = this->searchElem(root, toBeDeleted);
        if (place.first == nullptr) return false;
        else {
            if (place.second != nullptr) {
                bool leftSide = (place.second)->left() == place.first;
                BSTElem<DataType>* temp = delElem(place.first);
                if (leftSide) (place.second)->setLeft(temp);
                else (place.second)->setRight(temp);
                return (bool)temp;
            } else if (root == place.first) return (bool)(root = delElem(place.first));
            else throw "Deletion errors detected.\n";
            /*cout
                << "Now has: " << temp->getData()
                << "\nLeft child: " << (temp->left() != nullptr ? temp->left()->getData() : "nullptr")
                << "\nRight child: " << (temp->right() != nullptr ? temp->right()->getData() : "nullptr") << '\n';
            return (bool)temp;*/
        }
    }

    void insertElement(DataType toBeAdded) {root = insElem(root, toBeAdded);}

    unsigned int getHeight() {return height = getHeightToBottom(root);}

    void setInsertFnc(function<typename BST<DataType>::Behavior(BSTElem<DataType>* parent, DataType data)> fnc) {pushBehavior = fnc;}
    void setSearchFnc(function<typename BST<DataType>::Behavior(BSTElem<DataType>* parent, DataType data)> fnc) {searchFnc = fnc;}
};

#endif