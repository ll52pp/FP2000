/**
 * @file
 * @brief ����ͨ��ͷ�ļ��ڴ��ļ������
 * @attention �������Լ���һЩģ���ļ�, ��ӵ�����ļ���, ��ֹ�������ģ����벻��
 */
#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

// �ڴ���ӱ�׼C����ͷ�ļ�...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// �ڴ����VxWorksϵͳͷ�ļ�...
#include <vxworks.h>
#include <semLib.h>
#include <pciConfigLib.h>

// �ڴ���ӵ�������ͷ�ļ�...
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

// �ڴ���ӹ���ͨ��ͷ�ļ�...
#include "fh228/fh228_def.h"
#include "fh228/fh228_api.h"


#endif
