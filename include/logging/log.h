#ifndef __LOG__H__
#define __LOG__H__

#include <ctime>
#include <string>
#include <vector>


static std::string GetTime() {
    const char* fmt = "%Y-%m-%d %H:%M:%S";
    time_t t = time(nullptr);

    char time_str[64] = {0};
    (void)strftime(time_str, sizeof(time_str), fmt, localtime(&t));

    return time_str;
}


static std::string GetFileName(const std::string& file) {
#ifndef WIN32
    std::string pattern = "/";
#else
    std::string pattern = "\\";
#endif

    std::string::size_type pos;
    std::string result;
    
    pos = file.rfind(pattern);
    if (pos < file.size()) {
        result = file.substr(pos + pattern.size());
    }

    return result;
}


#define LOGI(format, ...) \
    (void)fprintf(stderr,"[INFO]%s [%s:%d] " format "\n", GetTime().data(), __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(format, ...) \
    (void)fprintf(stderr,"[ERROR]%s [%s:%d] " format "\n", GetTime().data(), __FILE__, __LINE__, ##__VA_ARGS__)


#endif //!__LOG__H__