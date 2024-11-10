#ifndef GENERAL_MATH
#define GENERAL_MATH

namespace math {
    template <typename T>
    struct vec2 {
        T x, y;

        template <typename U>
        explicit operator vec2<U>() const {
            return { static_cast<U>(x), static_cast<U>(y) };
        }
    };

    template <typename S>
    struct vec3 {
        S x;
        S y;
    };

    typedef vec2<uint32_t> vec2u;
    typedef vec2<int32_t> vec2i;
    typedef vec2<float> vec2f;
    typedef vec2<double> vec2d;


}

#endif

