#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

class fixed_thread_pool{
public:
    explicit fixed_thread_pool(size_t thread_count):data_(std::make_shared<data>()) {
        for (size_t i = 0; i < thread_count; ++i){
            std::thread([data = data_]{
                std::unique_lock<std::mutex> lk(data->mtx_);
                for(;;){
                    if(!data->tasks_.empty()){
                        auto current = std::move(data->tasks_.front());
                        data->tasks_.pop();
                        lk.unlock();
                        current();
                        lk.lock();
                    }else if(data->stop_){
                        break;
                    }else{
                        data->cond_.wait(lk);
                    }
                }
            }).detach();
        }
    }

    fixed_thread_pool() = default;
    fixed_thread_pool(fixed_thread_pool&&) = default;

    ~fixed_thread_pool(){
        if(data_){
            {
                std::lock_guard<std::mutex> lk(data_->mtx_);
                data_->stop_ = true;
            }
            data_->cond_.notify_all();
        }
    }

    template<typename F, typename... Args>
    void add(F&& f, Args&&... args){
        {
            std::lock_guard<std::mutex> lk(data_->mtx_);
            data_->tasks_.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        }
        data_->cond_.notify_one();
    }

    bool working(){
        return !data_->tasks_.empty();
    }


private:
    struct data
    {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool stop_ = false;
        std::queue<std::function<void()>> tasks_;
    };
    std::shared_ptr<data> data_;
};