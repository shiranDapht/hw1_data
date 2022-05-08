#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "exceptions.h"
#include <iostream>
#include <memory>
#include <assert.h>


using std::shared_ptr;

/*generic AVL_Tree - under the assumption that the data is a shared_ptr*/
template<class T, class Compare>
class AVLTree
{
private:
    struct Node
    {
        T data;
        int height;
        struct Node* left;
        struct Node* right;
        // Node* mostLeft; 
        struct Node* father;

    };
    int size_of_tree;
    Node* root;
    Node* max;
    Node* mostLeft;
    Compare compare; //will be used to compare the data (if the first is bigger)


    /*operation for Node*/
    Node* createNode(const T& data)
    {
        Node* new_node = new Node;
        new_node->height = 0;
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->father = nullptr;
        //new_node->mostLeft=new_node;
        new_node->data = data;
        return new_node;
    }



    Node* insertNode(Node* new_node, Node* root, Node* father)
    {
        /*insert like in a binary tree before fixing by rolling*/
        if (root == nullptr)
        {
            new_node->father = father;
            return new_node;
        }
        Node* current = root;
        //the data of the node and of current can not be equall
        if (compare(getNodeData(current), getNodeData(new_node)) > 0)
        {
            current->left = insertNode(new_node, current->left, current);
        }
        else if (compare(getNodeData(new_node), getNodeData(current)) > 0)
        {
            current->right = insertNode(new_node, current->right, current);
        }
        /*balance the tree- fix by rolling,
        will only get here and enter the if-condition if a balance was changed
        after adding a node to the tree, will correct recursively until the root*/
        if (calculateBalance(root) == 2) //This is the node that the recursion treats as a root 
        {//and is not necessarily the true root of the whole tree, at the end it will be 
            if (calculateBalance(root->left) >= 0)
            {
                return rollLeftLeft(root);
            }
            else //BF(root->left)=-1
            {
                return rollLeftRight(root);
            }
        }
        else if (calculateBalance(root) == -2)
        {
            if (calculateBalance(root->right) <= 0)
            {
                return rollRightRight(root);
            }
            else //BF(root->right)=1
            {
                return rollRightLeft(root);
            }
        }
        /*correct height*/
        int right_height = calculateHeight(root->right);
        int left_height = calculateHeight(root->left);
        int max = (right_height > left_height) ? right_height : left_height;
        root->height = max + 1;
        //fixMostLeft(new_node);   
        return root;
    }

    //roll after,may not be father-check
    Node* deleteNode(Node* father, Node* to_remove)
    {
        if (to_remove == nullptr)
        {
            return nullptr;
        }
        /*remove the node*/
        if (compare(father->data, to_remove->data) == 0) //to remove == father
        {
            if (father->right == nullptr && father->left == nullptr) // a leaf
            {
                father->data = nullptr;
                delete father;
                return nullptr;
            }
            else if (father->left == nullptr) //we remove someone who has a right son
            {
                Node* right_son = father->right;
                father->data = nullptr; //gonna get the son's data
                father->data = right_son->data;
                father->right = deleteNode(father->right, father);//remove the son
            }
            else if (father->right == nullptr)  //we remove someone who has a left son
            {
                Node* left_son = father->left;
                father->data = nullptr; //gonna get the son's data
                father->data = left_son->data;
                father->left = deleteNode(father->left, father);//remove the son
            }
            else  //we want to remove someone who has a left son and a right son
            {
                Node* replacment = getMostLeft(father->right);//a son who fit in order
                father->data = nullptr;                     //to renew the tree balance
                father->data = replacment->data;
                father->right = deleteNode(father->right, father);
            }
        }
        else if (compare(father->data, to_remove->data) > 0) //father->data > to_remove->data
        {                                                //we remove a left son
            father->left = deleteNode(father->left, to_remove); //will enter the if
        }                                        //"father==to_remove" and delete properly
        else  //we remove a right son
        {
            father->right = deleteNode(father->right, to_remove);
        }
        /*fix height*/
        int left_height = calculateHeight(father->left);
        int right_height = calculateHeight(father->right);
        int maximum = (left_height > right_height) ? left_height : right_height;
        father->height = maximum + 1;
        /*balance the tree*/
        if (calculateBalance(father) == 2)
        {
            if (calculateBalance(father->left) >= 0)
            {
                return rollLeftLeft(father);
            }
            else
            {
                return rollLeftRight(father);
            }
        }
        else if (calculateBalance(father) == -2)
        {
            if (calculateBalance(father->right) == 1)
            {
                return rollRightLeft(father);
            }
            else
            {
                return rollRightRight(father);
            }
        }
        return father;
    }


    Node* getMaxNode(Node* node)const
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->right == nullptr)
        {
            return node;
        }
        return getMaxNode(node->right);
    }

    Node* getMostLeft(Node* node)const
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->left == nullptr)
        {
            return node;
        }
        return getMostLeft(node->left);
    }

    int calculateBalance(Node* node) const
    {
        return calculateHeight(node->left) - calculateHeight(node->right);
    }

    int calculateHeight(const Node* node) const
    {
        if (node == nullptr)
        {
            return -1;
        }
        else
        {
            return node->height;
            // int left=0,right=0;
            // left=calculateHeight(node->left);
            // right=calculateHeight(node->right);
            // int max=(left>right)?left:right;
            // return 1+max; 
        }
    }

    T& getNodeData(Node* node) const
    {
        assert(node != nullptr); //make sure node isn't null before calling this function
        return node->data;
    }


    /*roll function*/
    Node* rollLeftLeft(Node* node)
    {   //rotate
        Node* temp = node->left;
        node->left->father = node->father;
        node->father = temp;
        if (temp->right != nullptr)
        {
            temp->right->father = node;
        }
        node->left = node->father->right;
        node->father->right = node;
        //update height
        int right_height = calculateHeight(node->right);
        int left_height = calculateHeight(node->left);
        int max_height = (right_height > left_height) ? right_height : left_height;
        node->height = max_height + 1;
        if (node->father != nullptr)
        {
            int father_right_height = calculateHeight(node->father->right);
            int father_left_height = calculateHeight(node->father->left);
            int father_max_height = (father_right_height > father_left_height) ?
                father_right_height : father_left_height;
            node->father->height = father_max_height + 1;
        }
        return node->father;
    }
    Node* rollRightRight(Node* node)
    {
        //rotate
        Node* temp = node->right;
        node->right->father = node->father;
        node->father = temp;
        node->right = node->father->left;
        if (node->right != nullptr)
        {
            node->right->father = node;
        }
        node->father->left = node;
        //update heights
        int right_height = calculateHeight(node->right);
        int left_height = calculateHeight(node->left);
        int max_height = (right_height > left_height) ? right_height : left_height;
        node->height = max_height + 1;
        if (node->father != nullptr)
        {
            int father_right_height = calculateHeight(node->father->right);
            int father_left_height = calculateHeight(node->father->left);
            int father_max_height = (father_right_height > father_left_height) ?
                father_right_height : father_left_height;
            node->father->height = father_max_height + 1;
        }
        return node->father;
    }

    //combination of a single left rotation followed by a single right rotation
    Node* rollRightLeft(Node* node)
    {
        node->right = rollLeftLeft(node->right);
        return rollRightRight(node);
    }


    //combination of a single right rotation followed by a single left rotation
    Node* rollLeftRight(Node* node)
    {
        node->left = rollRightRight(node->left);
        return rollLeftLeft(node);
    }

    Node* findNode(Node* node, const T& data)
    {
        if (node == nullptr)
        {
            return nullptr; //the data is not in the tree
        }
        if (compare(node->data, data) == 0) //node->data==data   
        {
            return node;
        }
        if (compare(node->data, data) > 0) // node->data>data
        {
            return findNode(node->left, data);
        }
        if (compare(node->data, data) < 0) //node->data<data
        {
            return findNode(node->right, data);
        }
        return nullptr;
    }

    /*helper function for AVLTree*/
    Node* convertSortedArrayToTreeAux(T* sorted_array, int start, int end, Node* father)
    {
        if (start > end)
        {
            return nullptr;
        }
        int middle = (start + end) / 2;
        Node* new_element_in_tree = createNode(sorted_array[middle]);
        new_element_in_tree->father = father;
        new_element_in_tree->right = convertSortedArrayToTreeAux(sorted_array, middle + 1, end, new_element_in_tree);
        new_element_in_tree->left = convertSortedArrayToTreeAux(sorted_array, start, middle - 1, new_element_in_tree);
        int right_height = calculateHeight(new_element_in_tree->right);
        int left_height = calculateHeight(new_element_in_tree->left);
        int max_height = (right_height > left_height) ? right_height : left_height;
        new_element_in_tree->height = max_height + 1;
        return new_element_in_tree;
    }

    void emptyTreeAux(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }
        if (node->right != nullptr)
        {
            emptyTreeAux(node->right);
        }
        if (node->left != nullptr)
        {
            emptyTreeAux(node->left);
        }
        node->data = nullptr;
        delete node;
        return;
    }




public:
    AVLTree() :size_of_tree(0), root(nullptr), max(nullptr), mostLeft(nullptr), compare()
    {}
    ~AVLTree()
    {
        if (size_of_tree > 0)
        {
            emptyTree();
        }
        size_of_tree = 0;
        root = nullptr;
        max = nullptr;
        mostLeft = nullptr;
    }

    int getSizeOfTree() const
    {
        return this->size_of_tree;
    }


    AVLTree(const AVLTree<T, Compare>& tree) = default;

    /*operation on the AVL*/
    void insert(const T& data)
    {
        Node* node = findNode(root, data);
        if (node != nullptr)
        {
            throw DataAlreadyExist();
        }
        Node* new_node = createNode(data);
        root = insertNode(new_node, root, nullptr);
        size_of_tree++;
        max = getMaxNode(root);
        mostLeft = getMostLeft(root);

    }


    void remove(const T& data)
    {
        if (checkIfEmpty() == true)
        {
            return;
        }
        Node* to_delete = findNode(root, data);
        if (to_delete == nullptr)
        {
            throw DataDoesntExist();
        }
        root = deleteNode(root, to_delete);
        size_of_tree = size_of_tree - 1;
        max = getMaxNode(root);
        mostLeft = getMostLeft(root);
        return;
    }


    T& find(const T& data)
    {
        Node* node = findNode(root, data);
        if (node == nullptr)
        {
            throw DataDoesntExist();
        }
        return node->data;
    }


    int getHeight()const
    {
        if (root == nullptr)
        {
            return 0;
        }
        return root->height;
    }

    void emptyTree()
    {
        emptyTreeAux(this->root);
        this->size_of_tree = 0;
        this->root = nullptr;
        max = nullptr;
        mostLeft = nullptr;
    }

    bool checkIfEmpty()const
    {
        return (size_of_tree == 0) ? true : false;
    }

    T& getDataOfMax()
    {
        return  getNodeData(max);
    }
    const T& getDataOfMax()const
    {
        return getNodeData(max);
    }

    /*function which work like an iterator on AVL*/
    Node* getFirst()
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        return mostLeft;
    }

    Node* getRoot() //added by me
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        return root;
    }

    T& getMinData()
    {
        return getNodeData(mostLeft);
    }


    Node* getNext(Node* current)
    {
        if (current == nullptr)
        {
            return nullptr;
        }
        if (current == max) //reached the end of the tree
        {
            return nullptr;
        }
        if (current->right == nullptr) //there is no right son->go up
        {
            Node* last = current;
            current = current->father;
            while (current != nullptr && current->right == last) //&& current->father!=null
            { //we came from a right son so we need to go up until we reach the end
              //of the right sub tree we were in 
                last = current;
                current = current->father;
            }
        }
        else
        { //current->right!=nullptr -> we can go right
            current = current->right;
            while (current->left != nullptr)// we went over the left side and the root of                     
            {      //a sub_tree and need to do in order of the right side->go left
                current = current->left;
            }
        }
        return current;
    }

    int doInorder(int n, T* data_arr) //fill array - start from root of tree
    {
        assert(n > 0);
        if (n > size_of_tree) //the num of the groups is smaller than the requested                  
        {                    //amount of groups to go through
            return -1;
        }
        Node* current = getFirst();
        data_arr[0] = getNodeData(current);
        int counter = 1;
        while (counter < n)
        {
            current = getNext(current);
            data_arr[counter] = getNodeData(current);
            counter++;
        }
        return 0;
    }

    int sizeOfTreeSpecific(const T& MinId_employee, const T& MaxId_employee) //  start from specific Node in tree,
        //return num of nodes in tree - from start_node to end_node
    {
        bool is_inserted = false;
        Node* current = findNode(root,MinId_employee);
        
        if (current == nullptr)
        {
            insert(MinId_employee);
            is_inserted = true;
            current = findNode(root, MinId_employee);
            current = getNext(current);
        }
        
        if (current == nullptr )//|| end_node == nullptr
        {
            if (is_inserted)
            {
                remove(MinId_employee);
            }
            return 0; //the tree is'nt empty, therefore will not be
        }
        int counter = 0;
        while (current != nullptr && compare(MaxId_employee, getNodeData(current))>=0 )
        {
            current = getNext(current);
            counter= counter+1;
        }
        if (is_inserted)
        {
            remove(MinId_employee);
        }
        return counter; //end_node!=current, so end node should count - we start count from 1
    }

    int doInorderSpecific(int n, T* data_arr, const T& MinId_employee 
        , const T& MaxId_employee) //fill array - start from specific Node in tree
        //till end_node
    {
        assert(n > 0);
        if (n > size_of_tree)                  
        {                    
            return -1;
        }
        
        Node* current = findNode(root, MinId_employee);
        bool is_inserted = false;
        if (current == nullptr)
        {
            insert(MinId_employee);
            is_inserted = true;
            current = findNode(root, MinId_employee);
            current = getNext(current);
            
        }
        if (current == nullptr)
        {
            if (is_inserted)
            {
                remove(MinId_employee);
            }
            return -1;
        }
        data_arr[0] = getNodeData(current);
        int counter = 1;
        while (counter < n) 
        {
            current = getNext(current);
            if (current == nullptr)
            {
                if (is_inserted)
                {
                    remove(MinId_employee);
                }
                return -1;
            }
            data_arr[counter] = getNodeData(current);
            counter++;
        }
        if (is_inserted)
        {
            remove(MinId_employee);
        }
        return 0;
    }

    //it's on the user to call this function with valid parameters and a sorted array
    void convertSortedArrayToAvlTree(T* sorted_array, int size)
    {
        assert(size > 0 && sorted_array != nullptr);
        root = convertSortedArrayToTreeAux(sorted_array, 0, size - 1, nullptr);
        mostLeft = getMostLeft(root);
        size_of_tree = size;
        max = getMaxNode(root);
        return;
    }
   
};

#endif 