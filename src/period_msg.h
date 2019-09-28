#ifndef PERIOD_MSG_H
#define PERIOD_MSG_H

#include "common_header.h"
#include "struct.h"
#include "send_que.h"

bool PeriodMsg_SetCmd(const FpgaRegBaseAddr* reg, const Fh228PeriodicMsgCmd* cmds, const int count);
void PeriodMsg_SetFpgaEnable(const FpgaRegBaseAddr *reg, const int msgNo, const bool enable);

#endif
