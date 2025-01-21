#include <cstring>
#include "FileUtil.h"

FileUtil::FileUtil(std::string &file_name) : file_(::fopen(file_name.c_str(), "ae")),
                                             writtenBytes_(0)
{
    // 将file_缓冲区设置为本地缓冲降低io次数。
    ::setbuffer(file_, buffer_, sizeof(buffer_));
}
FileUtil::~FileUtil()
{
    if (file_)
    {
        ::fclose(file_);
    }
}
// 向文件写入数据
void FileUtil::append(const char *data, size_t len)
{
    size_t writen = 0;
    while (writen != len)
    {
        size_t remain = len - writen;
        size_t n = write(data + writen, remain);
        if (n != remain)
        {
            // 错误判断
            int err = ferror(file_);
            if (err)
            {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
                clearerr(file_); //  清除文件指针的错误标志
                break;
            }
        }
        writen += n;
    }
    writtenBytes_ += writen;
}

void FileUtil::flush()
{
    ::fflush(file_);
}
// 真正向文件写入数据
size_t FileUtil::write(const char *data, size_t len)
{
    // 没用选择线程安全的fwrite()为性能考虑。
   return  ::fwrite_unlocked(data, 1, len, file_);
}