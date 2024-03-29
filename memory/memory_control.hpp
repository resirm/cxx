#ifndef MEMORY_CONTROL_HPP
#define MEMORY_CONTROL_HPP

#include <iostream>

template<int blkSize_, int totalSize_>
class MemCtl{
private:
    typedef char byte;
    struct Block_{
        Block_* next_;
        byte data_[blkSize_-sizeof(Block_*)];
    };
    struct MemCtlBlk_{
        MemCtlBlk_* next_;
        Block_ data_[totalSize_/blkSize_];
    };
    Block_* freeNodeHeader_;
    MemCtlBlk_* memCtlBlkHeader_;
    int mBlkCnt;
    int blkCnt;

public:
    MemCtl(): freeNodeHeader_(nullptr), memCtlBlkHeader_(nullptr), mBlkCnt(0), blkCnt(0){

    }

    ~MemCtl(){
        auto p = memCtlBlkHeader_;
        while(p != nullptr){
            std::cout << "delete memCtlBlk" << std::endl;
            auto t = p;
            p = p->next_;
            delete t;
        }
    }

    void* malloc(){
        if(freeNodeHeader_ == nullptr){
            int cnt = totalSize_/blkSize_;
            MemCtlBlk_* newMem = new MemCtlBlk_();
            ++mBlkCnt;
            blkCnt += cnt;
            for(int i = 0; i < cnt-1; ++i){
                newMem->data_[i].next_ = &newMem->data_[i+1];
            }
            newMem->data_[cnt-1].next_ = nullptr;
            newMem->next_ = nullptr;
            freeNodeHeader_ = &newMem->data_[0];
            if(memCtlBlkHeader_ == nullptr){
                std::cout << "First memCtlBlk" << std::endl;
                memCtlBlkHeader_ = newMem;
            }else{
                std::cout << "New memCtlBlk" << std::endl;
                newMem->next_ = memCtlBlkHeader_;
                memCtlBlkHeader_ = newMem;
            }
        }

        void* newBlock = static_cast<void*>(freeNodeHeader_);
        freeNodeHeader_ = freeNodeHeader_->next_;
        return newBlock;
    }

    void free(void* p){
        Block_* usedBlock = static_cast<Block_*>(p);
        usedBlock->next_ = freeNodeHeader_;
        freeNodeHeader_ = usedBlock;
    }

    void info(){
        std::cout << "memCtlBlk number: " << mBlkCnt << "\nBlock_ number: " << blkCnt << std::endl;
    }
};

#endif // !MEMORY_CONTROL_HPP