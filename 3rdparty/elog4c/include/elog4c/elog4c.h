#ifndef ELOG_4_C_H_1882D7F5_EC1F_4FAD_8209_ABE6C287A16F
#define ELOG_4_C_H_1882D7F5_EC1F_4FAD_8209_ABE6C287A16F

#if defined(_WIN32)
#include <stdarg.h>
#include <string.h>
#if defined(WIN_DRIVER) || defined(_NTDDK_) || defined(_WDF_H_) || defined(WDFAPI)
#include <wdm.h>
#include <ntstrsafe.h>
#define elog_print_impl DbgPrint
#define vsnprintf RtlStringCchVPrintfA
#else
#include <stdio.h>
#define elog_print_impl printf
#endif
#elif defined(__VXWORKS__)
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define elog_print_impl printf
#elif defined(LINUX_DRIVER) || defined(module_init)
#include <linux/init.h>
#include <linux/module.h>
#define elog_print_impl(format, arg...) printk(KERN_DEBUG format, ## arg)
#else
// 目前暂未实现此平台功能, 你需要自定义宏elog_print_impl来实现printf一样的打印功能
#error "elog4c: sorroy, elog4c is not support on your platform"
#endif


#define ELOG_FILTER_TAG_MAX_LEN 32  ///< 日志标签的最大长度
#define ELOG_MAX_LOG_STRING_LEN 512 ///< 日志内容的最大长度

// 你可以在自己的工程中定义宏ELOG_OUTPUT_LEVEL来过滤不同等级的日志
#ifndef ELOG_OUTPUT_LEVEL
#define ELOG_OUTPUT_LEVEL ELOG_LVL_VERBOSE
#endif

// 你可以在自己的工程中定义宏ELOG_OUTPUT_TAG来过滤不同标签的日志
#ifndef ELOG_OUTPUT_TAG
#define ELOG_OUTPUT_TAG ""
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ELOG_LVL_ASSERT = 0,
    ELOG_LVL_ERROR,
    ELOG_LVL_WARN,
    ELOG_LVL_INFO,
    ELOG_LVL_DEBUG,
    ELOG_LVL_VERBOSE,
    ELOG_LVL_COUNT,
} elog_level;

static const char* _elog_utils_get_filename(const char *path)
{
    int i, j = 0;
    for (i = 0; path[i]; i++)
    {
        if ((path[i] == '\\') || (path[i] == '/'))
        {
            j = i;
        }
    }
    return &path[j + 1];
}

static void elog_print(const elog_level level, const char *tag, const char *file, const char *func, const long line, const char *format, ...)
{
    const char levels[] = {'A', 'E', 'W', 'I', 'D', 'V'};
    char content[ELOG_MAX_LOG_STRING_LEN] = {0};
    va_list args;

    if (level > ELOG_OUTPUT_LEVEL)
    {
        return;
    }

    if (!strstr(tag, ELOG_OUTPUT_TAG))
    {
        return;
    }

    va_start(args, format);
//用以4996警告被忽略
#if defined(_WIN32)
#pragma warning(push)//保存编译器状态
#pragma warning(disable:4996)
#endif
    vsnprintf(content, ELOG_MAX_LOG_STRING_LEN, format, args);
#if defined(_WIN32)
#pragma warning(pop)//恢复编译器状态
#endif

    va_end(args);

    elog_print_impl("%c[%s][%s:%ld] %s\n", levels[level], tag, _elog_utils_get_filename(file), line, content);
}

#if defined(WIN32) || defined(_WIN32)
#define elog_a(tag, fmt, ...) elog_print(ELOG_LVL_ASSERT, tag, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define elog_e(tag, fmt, ...) elog_print(ELOG_LVL_ERROR, tag, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define elog_w(tag, fmt, ...) elog_print(ELOG_LVL_WARN, tag, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define elog_i(tag, fmt, ...) elog_print(ELOG_LVL_INFO, tag, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define elog_d(tag, fmt, ...) elog_print(ELOG_LVL_DEBUG, tag, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define elog_v(tag, fmt, ...) elog_print(ELOG_LVL_VERBOSE, tag, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#elif defined(__VXWORKS__) || defined(LINUX_DRIVER) || defined(module_init)
#define elog_a(tag, fmt, args...) elog_print(ELOG_LVL_ASSERT, tag, __FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#define elog_e(tag, fmt, args...) elog_print(ELOG_LVL_ERROR, tag, __FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#define elog_w(tag, fmt, args...) elog_print(ELOG_LVL_WARN, tag, __FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#define elog_i(tag, fmt, args...) elog_print(ELOG_LVL_INFO, tag, __FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#define elog_d(tag, fmt, args...) elog_print(ELOG_LVL_DEBUG, tag, __FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#define elog_v(tag, fmt, args...) elog_print(ELOG_LVL_VERBOSE, tag, __FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#else
#error "elog4c can not run on your platform"
#endif

#ifdef __cplusplus
}
#endif

#endif
