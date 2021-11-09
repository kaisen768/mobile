#ifndef _ATCOMMAND_H_
#define _ATCOMMAND_H_

#include "atchannel.h"

int atcommand_indentification(struct atchannel_s *atchannel);
int atcommand_sysinfoex(struct atchannel_s *atchannel);
int atcommand_unconnect(struct atchannel_s *atchannel);
int atcommand_monsc(struct atchannel_s *atchannel);
int atcommand_sim_signal(struct atchannel_s *atchannel);

int atcommand_ledctrl_set(struct atchannel_s *atchannel);
int atcommand_ledctrl_get(struct atchannel_s *atchannel);

int atcommand_cpin_set(struct atchannel_s *atchannel);
int atcommand_cpin_get(struct atchannel_s *atchannel);

int atcommand_ipcfl_set(struct atchannel_s *atchannel);
int atcommand_ipcfl_get(struct atchannel_s *atchannel);

int atcommand_simswitch_set(struct atchannel_s *atchannel, int card);
int atcommand_simswitch_get(struct atchannel_s *atchannel);

int atcommand_eCall_set(struct atchannel_s *atchannel);
int atcommand_eCall_get(struct atchannel_s *atchannel);

int atcommand_network_set(struct atchannel_s *atchannel);
int atcommand_network_get(struct atchannel_s *atchannel);

int atcommand_netmode_set(struct atchannel_s *atchannel);
int atcommand_netmode_get(struct atchannel_s *atchannel);

#endif
