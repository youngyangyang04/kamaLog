#include "LogFile.h"
LogFile::LogFile(const std::string &basename,
                 off_t rollsize,
                 int flushInterval,
                 int checkEveryN ) : basename_(basename),
                                           rollsize_(rollsize),
                                           flushInterval_(flushInterval),
                                           checkEveryN_(checkEveryN),
                                           startOfPeriod_(0),
                                           lastRoll_(0),
                                           lastFlush_(0)
{
    // 重新启动时，可能没有log文件，因此在构建logFile对象，直接调用rollfile()创建一个新的log文件
    rollFile();
}
LogFile::~LogFile() = default;
void LogFile::append(const char *data, int len)
{
    std::lock_guard<std::mutex> lg(mutex_);
    appendInlock(data, len);
}
void LogFile::flush()
{
    file_->flush();
}
// 滚动日志
bool LogFile::rollFile()
{
    time_t now = 0;
    std::string filename = getLogFileName(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if (now > lastRoll_)
    {
        lastFlush_ = now;
        lastRoll_ = now;
        startOfPeriod_ = start;
        // 让file_指向一个名为filename的文件，相当于新建了一个文件，但是rollfile一次就会创建一共file对象去将数据写到日志文件中
        file_.reset(new FileUtil(filename));
        return true;
    }
    return false;
}
// 日志格式basename+now+".log"
std::string LogFile::getLogFileName(const std::string &basename, time_t *now)
{
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL); // 获取当前时间
    localtime_r(now, &tm);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S", &tm);

    filename += timebuf;
    filename += ".log";
    return filename;
}
void LogFile::appendInlock(const char *data, int len)
{
    file_->append(data, len);

    time_t now = time(NULL); // 当前时间
    ++count_;

    // 1. 判断是否需要滚动日志
    if (file_->writtenBytes() > rollsize_)
    {
        rollFile();
    }
    else if (count_ >= checkEveryN_) // 达到写入次数阈值后，进行检查
    {
        count_ = 0;

        // 基于时间周期滚动日志
        time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
        if (thisPeriod != startOfPeriod_)
        {
            rollFile();
        }
    }

    // 2. 判断是否需要刷新日志（独立的刷新逻辑）
    if (now - lastFlush_ > flushInterval_)
    {
        lastFlush_ = now;
        file_->flush();
    }
}

