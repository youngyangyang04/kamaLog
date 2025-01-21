#include "CurrentThread.h"

namespace CurrentThread
{
      thread_local int t_cachedTid = 0; // 在源文件中定义线程局部变量
    void cacheTid()
    {
        if (t_cachedTid == 0)
        {
            t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid)); // Ensure syscall and SYS_gettid are defined
        }
    }
}