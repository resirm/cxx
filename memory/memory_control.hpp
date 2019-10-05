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

public:
    MemCtl(): freeNodeHeader_(nullptr), memCtlBlkHeader_(nullptr){

    }

    void* malloc(){
        if(freeNodeHeader_ == nullptr){
            int cnt = totalSize_/blkSize_;
            MemCtlBlk_* newMem = new MemCtlBlk_();
            for(int i = 0; i < cnt-1; ++i){
                newMem->data_[i].next_ = &newMem->data_[i+1];
            }
            newMem->data_[cnt-1].next_ = nullptr;
            freeNodeHeader_ = &newMem->data_[0];
            if(memCtlBlkHeader_ == nullptr){
                std::cout << "First memCtlBlk" << std::endl;
                memCtlBlkHeader_ = newMem;
                memCtlBlkHeader_->next_ = nullptr;
            }else{
                std::cout << "New memCtlBlk" << std::endl;
                memCtlBlkHeader_->next_ = newMem;
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
};

#endif // !MEMORY_CONTROL_HPP