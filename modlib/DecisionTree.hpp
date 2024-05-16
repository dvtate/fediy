


/*!
    This class is used to construct a decision tree that results in an optimal lookup time

    path rules
    - components separated by /'s
    - parameter path coponents start with a :
    - wildcards end with *

*/
template <class T>
class PathRouteMapBuilder {
    std::deque<std::string> m_paths;
    std::deque<T> m_targets;

};

template <class T>
class PathRouteMapper {
    class PathNode {
    };
public:
    PathRouteMapper(PathRouteMapBuilder<T> builder) {
        // TODO
    }

    std::optional<T&> get(const std::string& path) {

    }
};
