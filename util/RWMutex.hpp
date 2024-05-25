#pragma once

#include <atomic>
#include <mutex>


struct RWMutex {
    std::mutex m_mtx;
    std::atomic<unsigned short> m_readers{0};

    void read_lock() {
        m_mtx.lock();
        m_readers++;
        m_mtx.unlock();
    }

    void read_unlock() {
        m_readers--;
    }

    void write_lock() {
        m_mtx.lock();
        while (m_readers > 0); // spinlock is kinda ugly but eh
    }
    void write_unlock() {
        m_mtx.unlock();
    }

    struct LockForWrite {
        RWMutex& m_mtx;
        explicit LockForWrite(RWMutex& mtx): m_mtx(mtx) {
            mtx.write_lock();
        }
        ~LockForWrite() {
            m_mtx.write_unlock();
        }
    };

    struct LockForRead {
        RWMutex& m_mtx;
        explicit LockForRead(RWMutex& mtx): m_mtx(mtx) {
            mtx.read_lock();
        }
        ~LockForRead() {
            m_mtx.read_unlock();
        }
    };
};
