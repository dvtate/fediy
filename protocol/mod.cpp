


class Ctx {

};


struct User {
    const char* m_server; 
    const char* m_name;
};

struct Req {
    const char* m_path;
    const char* m_body; // null = get request
    User* m_user;       // null = unauthenticated
    const char* m_headers;
};

// 
extern "C" {
    void request_handler(const Req& request, ) {}

    void start() { }

    void stop() { }

    void setup() { } 
};