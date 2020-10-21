//#pragma once
//#include <new>
//#include <cassert>
//
//
//namespace
//{
//    // utility
//    std::new_handler get_new_handler(void)
//    {
//        std::new_handler handler = std::set_new_handler(0);
//        std::set_new_handler(handler);
//
//        return handler;
//    }
//
//    //// custom allocation scheme goes here!
//    //void* allocate(std::size_t pAmount)
//    //{
//
//    //}
//
//    //void deallocate(void* pMemory)
//    //{
//
//    //}
//
//    // custom allocation scheme goes here!
//    const std::size_t allocation_limit = 1073741824; // 1G
//    std::size_t totalAllocation = 0;
//
//    void* allocate(std::size_t pAmount)
//    {
//        // make sure we're within bounds
//        assert(totalAllocation + pAmount < allocation_limit);
//
//        // over allocate to store size
//        void* mem = std::malloc(pAmount + sizeof(std::size_t));
//        if (!mem)
//            return 0;
//
//        // track amount, return remainder
//        totalAllocation += pAmount;
//        *static_cast<std::size_t*>(mem) = pAmount;
//
//        return static_cast<char*>(mem) + sizeof(std::size_t);
//    }
//
//    void deallocate(void* pMemory)
//    {
//        // get original block
//        void* mem = static_cast<char*>(pMemory) - sizeof(std::size_t);
//
//        // track amount
//        //std::size_t amount = *static_cast<std::size_t*>(mem);
//        std::size_t pAmount = *static_cast<std::size_t*>(mem);
//        totalAllocation -= pAmount;
//
//        // free
//        std::free(mem);
//    }
//
//
//    // allocate with throw, properly
//    void* allocate_throw(std::size_t pAmount)
//    {
//        void* result = allocate(pAmount);
//
//        while (!result)
//        {
//            // call failure handler
//            std::new_handler handler = get_new_handler();
//            if (!handler)
//            {
//                throw std::bad_alloc();
//            }
//
//            handler();
//
//            // try again
//            result = allocate(pAmount);
//        }
//
//        return result;
//    }
//}
//
//void* operator new(std::size_t pAmount) throw(std::bad_alloc)
//{
//    return allocate_throw(pAmount);
//}
//
//void* operator new[](std::size_t pAmount) throw(std::bad_alloc)
//{
//    return allocate_throw(pAmount);
//}
//
//void* operator new(std::size_t pAmount, const std::nothrow_t&) throw()
//{
//    return allocate(pAmount);
//}
//
//void* operator new[](std::size_t pAmount, const std::nothrow_t&) throw()
//{
//    return allocate(pAmount);
//}
//
//void operator delete(void* pMemory) throw()
//{
//    deallocate(pMemory);
//}
//
//void operator delete[](void* pMemory) throw()
//{
//    deallocate(pMemory);
//}
//
//void operator delete(void* pMemory, const std::nothrow_t&) throw()
//{
//    deallocate(pMemory);
//}
//
//void operator delete[](void* pMemory, const std::nothrow_t&) throw()
//{
//    deallocate(pMemory);
//}