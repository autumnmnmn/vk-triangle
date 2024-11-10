#ifndef GENERAL_BASICS
#define GENERAL_BASICS

namespace basic {

    template<typename T>
    struct forgetfulQueue {
        uint32_t length;
        uint32_t occupancy;
        uint32_t front_index;
        T *data = nullptr;

        void allocate(uint32_t newLength) {
            if (data == nullptr) {
                free(data);
            }
            length = newLength;
            occupancy = 0;
            front_index = 0;
            data = (T*) malloc(length * sizeof(T));
        }

        void free() {
            free(data);
        }

        void enqueue(T datum) {
            // TODO
        }
    };

}

#endif

