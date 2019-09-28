/**
 * @file
 * @brief 工程通用头文件在此文件中添加
 * @attention 请勿将你自己的一些模块文件, 添加到这个文件中, 防止造成其它模块编译不过
 */
#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

// 在此添加标准C语言头文件...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 在此添加VxWorks系统头文件...
#include <vxworks.h>
#include <semLib.h>
#include <pciConfigLib.h>

// 在此添加第三方库头文件...
#include "banana/ba_base_types.h"
#include "banana/ba_inet.h"
#include "banana/ba_mutex.h"
#include "banana/ba_register.h"
#include "banana/ba_task.h"
#include "banana/ba_dma.h"
#include "chaos/cs_buf_writer.h"
#include "chaos/cs_fast_buf_splitter.h"
#include "chaos/cs_linked_hash_map.h"
#include "chaos/cs_pingpong_buf.h"
#include "chaos/cs_ring_buf.h"
#include "chaos/cs_utils.h"
#include "elog4c/elog4c.h"

// 在此添加工程通用头文件...
#include "fh228/fh228_def.h"
#include "fh228/fh228_api.h"


#endif
