/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef __EFL_ASSIST_PRIVATE_H__
#define __EFL_ASSIST_PRIVATE_H__

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "efl-assist"
#define __CONSTRUCTOR__ __attribute__ ((constructor))
#define __DESTRUCTOR__ __attribute__ ((destructor))

/* eina magic types */
#define EA_MAGIC_NONE 0x87657890
#define EA_MAGIC_CUTLINK 0x78908765

typedef unsigned int ea_magic;
#define EA_MAGIC                ea_magic __magic

#define EA_MAGIC_SET(d, m)      (d)->__magic = (m)
#define EA_MAGIC_CHECK(d, m)    ((d) && ((d)->__magic == (m)))
#define EA_MAGIC_FAIL(d, m, fn) \
		_ea_magic_fail((d), (d) ? (d)->__magic : 0, (m), (fn));

void _ea_magic_fail(const void *d, ea_magic m,
		    ea_magic req_m, const char *fname);

#define CRITICAL(...) EINA_LOG_DOM_CRIT(_ea.ea_log_dom, __VA_ARGS__)
#define ERR(...)      EINA_LOG_DOM_ERR(_ea.ea_log_dom, __VA_ARGS__)
#define WRN(...)      EINA_LOG_DOM_WARN(_ea.ea_log_dom, __VA_ARGS__)
#define INF(...)      EINA_LOG_DOM_INFO(_ea.ea_log_dom, __VA_ARGS__)
#define DBG(...)      EINA_LOG_DOM_DBG(_ea.ea_log_dom, __VA_ARGS__)

typedef struct _Ea Ea;
typedef struct _Ea_Event_Mgr Ea_Event_Mgr;

struct _Ea
{
   Eina_List *event_mgrs;
   int ea_log_dom;
};

extern Ea _ea;

void ea_event_mgr_clear(Ea_Event_Mgr *event_mgr);

#ifdef __cplusplus
}
#endif

#endif /* __EFL_ASSIST_PRIVATE_H__ */
