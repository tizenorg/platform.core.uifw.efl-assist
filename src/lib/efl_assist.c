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

#include "efl_assist.h"
#include "efl_assist_private.h"

/*===========================================================================*
 *                                 Local                                     *
 *===========================================================================*/

Ea _ea;

static void
ea_init(void)
{
   memset(&_ea, 0x00, sizeof(_ea));

   _ea.ea_log_dom = eina_log_domain_register("efl-assist",
                                             EINA_COLOR_LIGHTBLUE);
   if (!_ea.ea_log_dom)
     {
        EINA_LOG_ERR("could not register efl-assist log domain");
        _ea.ea_log_dom = EINA_LOG_DOMAIN_GLOBAL;
     }
}

static void
ea_shutdown(void)
{
   Eina_List *l;
   Ea_Event_Mgr *event_mgr;

   //Remove Event Managers
   EINA_LIST_FOREACH(_ea.event_mgrs, l, event_mgr)
     ea_event_mgr_clear(event_mgr);
   _ea.event_mgrs = eina_list_free(_ea.event_mgrs);

   if ((_ea.ea_log_dom > - 1) && (_ea.ea_log_dom != EINA_LOG_DOMAIN_GLOBAL))
     {
        eina_log_domain_unregister(_ea.ea_log_dom);
        _ea.ea_log_dom = -1;
     }
}

static const char *
_magic_string_get(ea_magic m)
{
	switch (m) {
	case EA_MAGIC_NONE:
		return "None (Freed Object)";

	case EA_MAGIC_CUTLINK:
		return "cutlink";

	default:
		return "<UNKNOWN>";
     }
}

__CONSTRUCTOR__ static void
ea_mod_init(void)
{
	ea_init();

	DBG("loaded");
}

__DESTRUCTOR__ static void
ea_mod_shutdown(void)
{
	DBG("unloaded");

	ea_shutdown();
}


/*===========================================================================*
 *                                Global                                     *
 *===========================================================================*/

void
_ea_magic_fail(const void *d, ea_magic m, ea_magic req_m, const char *fname)
{
	ERR("\n*** MAGIC FAIL (%s) ***\n", fname);

	if (!d)
		ERR("  Input handle pointer is NULL!");
	else if (m == EA_MAGIC_NONE)
		ERR("  Input handle has already been freed!");
	else if (m != req_m)
		ERR("  Input handle is wrong type\n"
		    "    Expected: %08x - %s\n"
		    "    Supplied: %08x - %s",
		    (unsigned int)req_m, _magic_string_get(req_m),
		    (unsigned int)m, _magic_string_get(m));

   if (getenv("EA_ERROR_ABORT")) abort();
}

/*===========================================================================*
 *                                  API                                      *
 *===========================================================================*/

