#include "LogStream.h"
#include <algorithm>

static const char digits[] = "9876543210123456789";

template <typename T>
void LogStream::formatInteger(T num)
{
    if (buffer_.avail() >= kMaxNumberSize)
    {
        char *start = buffer_.current();
        char *cur = start;
        static const char *zero = digits + 9;
        bool negative = (num < 0); // 判断num是否为负数
        do
        {
            int remainder = static_cast<int>(num % 10);
            (*cur++) = zero[remainder];
            num /= 10;
        } while (num != 0);
        if (negative)
        {
            *cur++ = '-';
        }
        *cur = '\0';
        std::reverse(start, cur);
        int length = static_cast<int>(cur - start);
        buffer_.add(length);
    }
}
// 重载输出流运算符<<，用于将布尔值写入缓冲区
LogStream &LogStream::operator<<(bool express) {
    buffer_.append(express ? "true" : "false", express ? 4 : 5);
    return *this;
}

// 重载输出流运算符<<，用于将短整型写入缓冲区
LogStream &LogStream::operator<<(short number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将无符号短整型写入缓冲区
LogStream &LogStream::operator<<(unsigned short number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将整型写入缓冲区
LogStream &LogStream::operator<<(int number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将无符号整型写入缓冲区
LogStream &LogStream::operator<<(unsigned int number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将长整型写入缓冲区
LogStream &LogStream::operator<<(long number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将无符号长整型写入缓冲区
LogStream &LogStream::operator<<(unsigned long number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将长长整型写入缓冲区
LogStream &LogStream::operator<<(long long number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将无符号长长整型写入缓冲区
LogStream &LogStream::operator<<(unsigned long long number) {
    formatInteger(number);
    return *this;
}

// 重载输出流运算符<<，用于将浮点数写入缓冲区
LogStream &LogStream::operator<<(float number) {
    *this<<static_cast<double>(number);
    return *this;
}

// 重载输出流运算符<<，用于将双精度浮点数写入缓冲区
LogStream &LogStream::operator<<(double number) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.12g", number);
    buffer_.append(buffer, strlen(buffer));
    return *this;
}

// 重载输出流运算符<<，用于将字符写入缓冲区
LogStream &LogStream::operator<<(char str) {
    buffer_.append(&str, 1);
    return *this;
}

// 重载输出流运算符<<，用于将C风格字符串写入缓冲区
LogStream &LogStream::operator<<(const char *str) {
    buffer_.append(str, strlen(str));
    return *this;
}

// 重载输出流运算符<<，用于将无符号字符指针写入缓冲区
LogStream &LogStream::operator<<(const unsigned char *str) {
    buffer_.append(reinterpret_cast<const char*>(str), strlen(reinterpret_cast<const char*>(str)));
    return *this;
}

// 重载输出流运算符<<，用于将std::string对象写入缓冲区
LogStream &LogStream::operator<<(const std::string &str) {
    buffer_.append(str.c_str(), str.size());
    return *this;
}

LogStream& LogStream::operator<<(const GeneralTemplate& g)
{
    buffer_.append(g.data_, g.len_);
    return *this;
}