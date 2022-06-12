#include <iostream>
#include <thread>
#include <memory>
#include <optional>
#include <mutex>
#include <list>

using namespace std;

/* MultiThreading example
 * https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/
 *
 */
typedef void(*fptr)(thread&);

class thread_wrapper
{
public:
    thread_wrapper(thread&t):_impl{t}{}
    thread_wrapper(thread&t, fptr func):handler{func},_impl{t}{}

    ~thread_wrapper()
    {
        if(handler) {
            (*handler)(_impl);
        } else {
            defReset(_impl);
        }
    }

    std::optional<fptr> handler;

    static void defReset(thread& t)
    {
        if (t.joinable())
        {
          cout << "detach\n";
          t.detach();//default beh.
        }
    }

// Other methods
private:
    std::thread& _impl;
};

std::mutex mu;

using namespace std;
void LaunchRocket()
{
    lock_guard<mutex> lock(mu);
    cout << "Thread " << this_thread::get_id() << ", Launching Rocket\n";
}

int main()
{
    cout << "Hello World!" << endl;
    auto threadGuard = [](auto& th){
        if (th.joinable())
        {
            cout << "join\n";
            th.join();
        }
    };

    thread t0(LaunchRocket);
    thread t1(LaunchRocket);
    auto a = thread_wrapper(t0, threadGuard);
    auto a2 = thread_wrapper(t1);

    {//mutex
        std::list<int> l;
        std::mutex m;
        {
            std::lock_guard guard(m);
            l.push_back(42);
        }

    }

    return 0;
}
