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

#ifndef __EFL_ASSIST_EDITFIELD_H__
#define __EFL_ASSIST_EDITFIELD_H__

#include <Elementary.h>
#include <stdbool.h>
#ifndef DESKTOP
	#include <tizen.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef Ea_Editfield_Type
 *
 * Editfield Entry types.
 *
 * @see ea_editfield_add()
 */
typedef enum
{
   EA_EDITFIELD_SINGLELINE,
   EA_EDITFIELD_MULTILINE,
   EA_EDITFIELD_SCROLL_SINGLELINE,
   EA_EDITFIELD_SCROLL_MULTILINE,
   EA_EDITFIELD_SCROLL_SINGLELINE_PASSWORD,
   EA_EDITFIELD_SEARCHBAR
} Ea_Editfield_Type;

/**
 * @brief Add an elementary entry widget with clear button and rename icon.
 *
 * @details Add an elementary entry widget with some special features.
 *          The first is clear button that will delete all characters in current entry widget.
 *          The second is rename icon which symbolizes the editable field in dialogue view.
 *          Visible state of each clear button and rename icon will be changed automatically.
 *          This API will return an elementary entry widget, and some elm_entry APIs can change
 *          the internal state of this widget. For maintaining the original states of entry widget,
 *          following signal emitting is needed :
 *
 *          1. For maintaining visible state of rename icon :
 *                 "elm,state,rename,visible", "elm,state,rename,hidden"
 *
 *          2. For maintaining visible state of clear button :
 *                 "elm,state,clear,visible", "elm,state,clear,hidden"
 *
 *          3. For maintaining scrollable state of entry :
 *                 "elm,state,scroll,enabled", "elm,state,scroll,disabled"
 *
 *
 * @param [in] parent The parent widget object
 * @param [in] type Types for supporting different entry widget modes.
 *
 * @return elementary entry widget
 *
 */
Evas_Object *ea_editfield_add(Evas_Object *parent, Ea_Editfield_Type type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __EFL_ASSIST_EDITFIELD_H__ */
