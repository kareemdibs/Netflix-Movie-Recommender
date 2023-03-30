#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
public:
    class Iterator
    {
    public:
        Iterator() : m_isValid(false) {} // default constructor, invalid iterator

        Iterator(typename std::vector<ValueType>::iterator it, typename std::vector<ValueType>::const_iterator end)
            : m_valueIt(it), m_end(end), m_isValid(true) {} // constructor for a valid iterator

        bool is_valid() const {
            return m_isValid;
        }

        ValueType& get_value() const {
            return *m_valueIt; // dereference the value iterator
        }

        void advance() {
            if (!m_isValid) {
                return; // iterator is invalid, return without advancing
            }

            ++m_valueIt; // advance the value iterator

            if (m_valueIt == m_end) { // reached the end of the value vector
                m_isValid = false; // invalidate the iterator
            }
        }

    private:
        bool m_isValid;
        typename std::vector<ValueType>::iterator m_valueIt; // iterator for the vector of values
        typename std::vector<ValueType>::const_iterator m_end; // marks the end of the value vector
    };

    // constructor
    TreeMultimap() : m_root(nullptr) {}

    // destructor
    ~TreeMultimap()
    {
        m_root = deleteNodes(m_root); // recursively delete nodes starting from the root
    }

    // insert a new key-value pair into the multimap
    void insert(const std::string& key, const ValueType& value) {
        if (m_root == nullptr) { // tree is empty, create a new root node
            m_root = new Node(key, value);
            return;
        }

        Node* nodePtr = m_root;

        while (true) {
            if (key == nodePtr->m_key) { // found an existing key, append the value to its vector
                nodePtr->m_values.push_back(value);
                return;
            }
            if (key < nodePtr->m_key) { // go to the left child
                if (nodePtr->left == nullptr) {
                    nodePtr->left = new Node(key, value);
                    return;
                }
                nodePtr = nodePtr->left;
                /*if (nodePtr->left != nullptr) {
                    nodePtr = nodePtr->left;
                }
                else {
                    nodePtr->left = new Node(key, value);
                    return;
                }*/
            }
            else if (key > nodePtr->m_key) { // go to the right child
                if (nodePtr->right == nullptr) {
                    nodePtr->right = new Node(key, value);
                    return;
                }
                nodePtr = nodePtr->right;
                /*if (nodePtr->right != nullptr) {
                    nodePtr = nodePtr->right;
                }
                else
                {
                    nodePtr->right = new Node(key, value);
                    return;
                }*/
            }
        }
    }

    // return an iterator pointing to the first value associated with the given key
    // if the key is not found, return an invalid iterator
    Iterator find(const KeyType& key) const {
        Node* find_nodePtr = this->m_root;

        while (find_nodePtr != nullptr) {
            if (key == find_nodePtr->m_key) { // found the key
                return Iterator(find_nodePtr->m_values.begin(), find_nodePtr->m_values.end());
            }
            
            if (key < find_nodePtr->m_key) { // go to the left child
                find_nodePtr = find_nodePtr->left;
            }
            else if (key > find_nodePtr->m_key) { // go to the right child
                find_nodePtr = find_nodePtr->right;
            }
        }

        return Iterator(); // return an invalid iterator
    }

private:
    // a node in the multimap's binary search tree
    struct Node
    {
        KeyType m_key;
        std::vector<ValueType> m_values; // vector of values associated with the key
        Node* left;
        Node* right;

        // constructor for a node
        Node(const KeyType& key, ValueType value) : m_key(key), left(nullptr), right(nullptr)
        {
            m_values.push_back(value);
        }
    };

    // deletes all nodes in the BST
    Node* deleteNodes(Node* toBeDeleted) {
        if (toBeDeleted == nullptr) {
            return nullptr;
        }
        deleteNodes(toBeDeleted->left);
        deleteNodes(toBeDeleted->right);
        delete toBeDeleted;
        return nullptr;
    }

    Node* m_root;
};

#endif // TREEMULTIMAP_INCLUDED