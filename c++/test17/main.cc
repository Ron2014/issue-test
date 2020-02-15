#include <iostream>
using namespace std;

class EEBits {
private:
    unsigned char indices[32];  // 混淆器
    bool bits[32];  // 数值位

public:
    EEBits() {
        reindex();
    }
    // bits[*]
    // * 为 右移2位的 unsigned char，前面空 2 个 0
    // 0011 1111 = 1 + 2 + 4 + 8 + 16 + 32 = 63
    // bits 应该有 64 个元素才对；
    // 或者 ~indices[*] 最大为 0111 1100，最小为 0000 0000
    // indices[*] 最小为 1000 0011，最大为 1111 1111
    inline bool& operator [](int index) {
        return bits[(unsigned char)(~indices[index]) >> 2];
    }
    inline bool operator [](int index) const {
        return bits[(unsigned char)(~indices[index]) >> 2];
    }

private:
    void reindex() {
        // 初始化 indices
        // 0000 0000  0000 0100
        // 0000 1000  0000 1100
        // ...
        // 0111 1000  0111 1100
        for (int i = 0; i < 32; ++i) {
            indices[i] = i << 2;
        }
        // 对 indices 乱序
        for (int i = 0; i < 48; ++i) {
            int a = std::rand() & 31;
            int b = std::rand() & 31;
            unsigned char tmp = indices[a];
            indices[a] = indices[b];
            indices[b] = tmp;
        }
        // 对 indices 取反
        // 1000 0011  1000 0111
        // 1000 1011  1000 1111
        // ...
        // 1111 1011  1111 1111
        for (int i = 0; i < 32; ++i) {
            indices[i] = ~indices[i];
        }
    }
};

class UIntBits {
private:
    EEBits bits;

public:
    UIntBits() {
    }

    // 对 this 调用 = 操作符
    /**
     * UIntBits a(123);
     * 相当于
     * UIntBits a = 123;
    */
    UIntBits(unsigned int value) {
        this->operator = (value);
    }

    // unsigned int ------> bool array
    // bits[*] = false ------ bit 为 0
    // bits[*] = true ------ bit 为 1
    // 先看赋值
    // bit[*] 走了 [] 运算符重载，所以 i 会转换成 xi
    // 最终 value 对应的 bit 被打乱顺序，存在 bits.bits 里
    inline UIntBits& operator = (unsigned int value) {
        for (int i = 0; i < 32; ++i) {
            bits[i] = (value & (1 << i)) != 0;
        }
        return *this;
    }

    // bit array ------> unsigned int
    // bit[*] 走了 [] 运算符重载，所以 i 会转换成 xi
    // 反过来，通过 unsigned int() 强制转换，却能读到正确的值。
    inline operator unsigned int() const {
        unsigned int result = 0;
        for (int i = 0; i < 32; ++i) {
            if (bits[i])
                result |= (1 << i);
        }
        return result;
    }

    /**
     * COPY UIntBits：bit array ------> bit array
     * value.bits[*] ------ i 经过 value.indices 变成 ix ，取值
     * bits[*] ------ i 经过 bits.indices 变成 iy，赋值
    */
    inline UIntBits& operator = (const UIntBits& value) {
        if (&value != this) {
            for (int i = 0; i < 32; ++i) {
                bits[i] = value.bits[i];
            }
        }
        return *this;
    }

    // x 为 0 或 1
    // 对于内存布局/字节来讲：
    // xxxx xxxx  xxxx xxxx  xxxx xxxx  x--- ----
    // ---- ---x  xxxx xxxx  xxxx xxxx  xxxx xxxx
    // 对于小端数值来讲：
    // 字节：0000 000x  xxxx xxxx  xxxx xxxx  xxxx xxxx
    // 数值：xxxxxxxx xxxxxxxx xxxxxxxx 0000000x
    inline UIntBits& operator <<= (unsigned int value) {
        // 小端
        // 低位字节移动到高位上
        for (int i = 32 - (int)value - 1; i > -1; --i) {
            bits[i + value] = bits[i];
        }
        for (int i = 0; i < (int)value; ++i) {
            bits[i] = false;
        }
        return *this;
    }

    inline UIntBits& operator >>= (unsigned int value) {
        for (int i = 0; i < 32 - (int)value; ++i) {
            bits[i] = bits[i + value];
        }
        for (int i = 32 - 1; i >= 32 - (int)value; --i) {
            bits[i] = false;
        }
        return *this;
    }

    // by unsigned int
    // UIntBits << unsigned int ------> UIntBits <<= unsigned int
    inline UIntBits operator << (unsigned int value) {
        UIntBits result = *this;
        result <<= value;
        return result;
    }

    inline UIntBits operator >> (unsigned int value) {
        UIntBits result = *this;
        result >>= value;
        return result;
    }

    // by UIntBits
    // UIntBits <<= UIntBits ------> UIntBits <<= unsigned int
    inline UIntBits& operator <<= (const UIntBits& value) {
        this->operator <<= (value.operator unsigned int());
        return *this;
    }

    inline UIntBits& operator >>= (const UIntBits& value) {
        this->operator >>= (value.operator unsigned int());
        return *this;
    }

    // by UIntBits
    // UIntBits << UIntBits ------> UIntBits <<= UIntBits
    inline UIntBits operator << (const UIntBits& value) {
        UIntBits result = *this;
        result <<= value;
        return result;
    }

    inline UIntBits operator >> (const UIntBits& value) {
        UIntBits result = *this;
        result >>= value;
        return result;
    }

    inline UIntBits operator ~ () {
        UIntBits result = *this;
        for (int i = 0; i < 32; ++i) {
            result.bits[i] = !result.bits[i];
        }
        return result;
    }

    inline UIntBits& operator ^= (const UIntBits& value) {
        for (int i = 0; i < 32; ++i) {
            if (value.bits[i])
                bits[i] = !bits[i];
        }
        return *this;
    }

    inline UIntBits operator ^ (const UIntBits& value) {
        UIntBits result = *this;
        result ^= value;
        return result;
    }

    inline UIntBits operator ^ (unsigned int value) {
        UIntBits result = *this;
        result ^= value;
        return result;
    }

    inline UIntBits& operator |= (const UIntBits& value) {
        for (int i = 0; i < 32; ++i) {
            if (value.bits[i])
                bits[i] = true;
        }
        return *this;
    }

    inline UIntBits operator | (unsigned int value) {
        UIntBits result = *this;
        result |= value;
        return result;
    }

    inline UIntBits operator | (const UIntBits& value) {
        UIntBits result = *this;
        result |= value;
        return result;
    }
};
