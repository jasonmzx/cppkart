#include <mutex>
#include <queue>

template<typename T>
class TSQueue {
public:
    void Push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(value);
    }

    bool TryPop(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty()) {
            return false;
        }
        value = queue.front();
        queue.pop();
        return true;
    }

private:
    std::queue<T> queue;
    mutable std::mutex mtx;
};