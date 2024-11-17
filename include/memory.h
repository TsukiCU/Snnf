#ifndef _CUDA_MEMORY_H
#define _CUDA_MEMORY_H

#include "common.h"
#include "noncopyable.h"

namespace cuda
{

/*
 * @brief : Create a memory block regardless of its location.
 * @todo  : Concurrency (urgent), memory alignment and log.
 */
class MemoryBuffer {
public:
    MemoryBuffer(uint32_t size);
    MemoryBuffer(void* data, uint32_t size);
    ~MemoryBuffer();

    void* data() const { return data_; }
    uint32_t get_size() const { return size_; }

    void copy_from(const MemoryBuffer& src);
    void copy_to(MemoryBuffer& dst) const;

    static MemoryBuffer create_from_existing(void* data, uint32_t size) {
        MemoryBuffer buffer(size);
        buffer.data_ = data;
        return buffer;
    }

private:
    void* data_;
    uint32_t size_;

    void allocate();
    void deallocate();
};

} // cuda

#endif // _CUDA_MEMORY_H