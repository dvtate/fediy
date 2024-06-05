#include <openssl/sha.h>

#include "DB.hpp"

#include "App.hpp"

DB::DB(const std::string& db_file):
    m_db(db_file,SQLite::OPEN_FULLMUTEX | SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE ),
    m_get_user_query(m_db, "SELECT username, name, hashedPassword, email, locale, joinTs, about FROM Users WHERE username = ?"),
    m_add_user_query(m_db, "INSERT INTO Users (username, isAdmin, name, hashedPassword, email, locale, joinTs, about) VALUES (?, ?, ?, ?, ?, ?, ?)"),
    m_get_peer_query(m_db, "SELECT * FROM Peers WHERE domain = ?"),
    m_add_peer_query(m_db, "INSERT INTO Peers (domain, connectTs, bearerToken, symKey, pubkey, tokenExpireTs) VALUES (?, ?, ?, ?, ?, ?)")
{
}

template<class T>
struct CleanupRoutine {
    T m_action;
    explicit CleanupRoutine(T f): m_action(std::move(f)) {}
    ~CleanupRoutine() { m_action(); }
};

class DBQueryThreadSafety {
    SQLite::Statement& m_stmt;
    std::mutex& m_mtx;
public:
    DBQueryThreadSafety(
        SQLite::Statement& statement,
        std::mutex& mutex
    ): m_stmt(statement), m_mtx(mutex) {
        mutex.lock();
    }
    ~DBQueryThreadSafety() {
        m_stmt.clearBindings();
        m_stmt.reset();
        m_mtx.unlock();
    }
};

std::shared_ptr<LocalUser> DB::get_user(const std::string& username, std::string password) {
    // Hash provided password
    unsigned char hashed_password[129];
    password += g_app->m_config.m_salt;
    unsigned char* hp = SHA512((unsigned char*) password.c_str(), password.size(), hashed_password);

    // Thread safety
    DBQueryThreadSafety lock{m_get_user_query, m_mtx};

    // Fetch relevant user from database
    m_get_user_query.bindNoCopy(0, username);
    if (!m_get_user_query.executeStep()) // no match
        return nullptr;

    // Verify that password is correct
    const auto pw_col = m_get_user_query.getColumn(2);
    if (pw_col.getBytes() < 128) {
        LOG_ERR("DB: hashedPassword column shorter than expected");
        return nullptr;
    }
    auto db_hp = (unsigned char*) pw_col.getBlob();
    for (int i = 0; i < 128; i++)
        if (hp[i] != db_hp[i])
            return nullptr; // wrong pw

    // Make user object
    return std::make_shared<LocalUser>(
        m_get_user_query.getColumn(2).getString(),  // name
        username,
        m_get_user_query.getColumn(1).getInt() != 0, // isAdmin
        m_get_user_query.getColumn(4).getString(),  // email
        m_get_user_query.getColumn(5).getString(),  // locale
        m_get_user_query.getColumn(6).getUInt(),  // join_ts
        m_get_user_query.getColumn(7).getString()   // about
    );
}

std::shared_ptr<LocalUser> DB::get_user(const std::string& username) {
    // Thread safety
    DBQueryThreadSafety lock{m_get_user_query, m_mtx};

    // Fetch user from db
    m_get_user_query.bindNoCopy(0, username);
    if (!m_get_user_query.executeStep()) // no match
        return nullptr;
    return std::make_shared<LocalUser>(
        m_get_user_query.getColumn(2).getString(),  // name
        username,
        m_get_user_query.getColumn(1).getInt() != 0, // isAdmin
        m_get_user_query.getColumn(4).getString(),  // email
        m_get_user_query.getColumn(5).getString(),  // locale
        m_get_user_query.getColumn(6).getUInt(),  // join_ts
        m_get_user_query.getColumn(7).getString()   // about
    );
}

bool DB::add_user(const LocalUser& user, std::string password) {
    // Hash provided password
    unsigned char hashed_password[129];
    password += g_app->m_config.m_salt;
    unsigned char* hp = SHA512((unsigned char*) password.c_str(), password.size(), hashed_password);

    // Thread safety
    auto& q = m_add_user_query;
    DBQueryThreadSafety lock{q, m_mtx};

    q.bindNoCopy(0, user.m_username);
    q.bind(1, (int32_t)user.m_is_admin);
    q.bindNoCopy(2, user.get_name());
    q.bindNoCopy(3, )
    return true;
}