#include <string>
#include <deque>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>

/*!
    This class is used to construct a decision tree that results in an optimal lookup time

    path rules
    - components separated by /'s
    - parameter path coponents start with a :
    - wildcards end with *

*/

// TODO replace this with a more manual routing solution similar to express


template <class T>
class PathRouteMapper {
    struct PathRouteTreeNode {
        T* m_value;
        std::map<std::string, PathRouteTreeNode*> m_children;
        PathRouteTreeNode(T* value = nullptr): m_value(value) {}
    };

    PathRouteTreeNode m_root;

public:

    void add(const std::string& path, T* new_value) {
        PathRouteTreeNode* n = &m_root;

        std::map<std::string, T*> m;

        // Iterate over each path element
        std::stringstream ss{path};
        while (ss.good()) {
            std::string substr;
            std::getline(ss, substr, '/');
            // if (substr == "")
            //     continue;
            if (n->m_children.contains(substr)) {
                n = n->m_children[substr];
            } else {
                auto* n2 = new PathRouteTreeNode();
                n->m_children.emplace(std::move(substr), n2);
                n = n2;
            }
        }
        n->m_value = new_value;

    }


    T* get(const char* str, const size_t len) {

    }
};
