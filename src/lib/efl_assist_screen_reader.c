#include "efl_assist.h"
#include "efl_assist_private.h"
#include <Ecore_X.h>
#include <vconf.h>
#include <tts.h>
#define UNAVAILABLE_TEXT "Screen reader is unavailable during using this application. You can press home or back key to go back to home screen."

static tts_h tts = NULL;

static void _tts_shutdown(void)
{
   int ret = 0;
   if (tts)
     {
        /* check current state */
        tts_state_e state;
        tts_get_state(tts, &state);
        if (state == TTS_STATE_PLAYING || state == TTS_STATE_PAUSED)
          {
             ret = tts_stop(tts);
             if (TTS_ERROR_NONE != ret)
               {
                  fprintf(stderr, "Fail to stop handle : result(%d)", ret);
                  return;
               }
          }

        /* it is possible to try to shutdown before the state is ready,
           because tts_prepare(); works Asynchronously. see elm_modapi_init(): */
        if (state == TTS_STATE_READY)
          {
             ret = tts_unprepare(tts);
             if (TTS_ERROR_NONE != ret)
               {
                  fprintf(stderr, "Fail to unprepare handle : result(%d)", ret);
                  return;
               }

             ret = tts_unset_state_changed_cb(tts);
             if (TTS_ERROR_NONE != ret)
               {
                  fprintf(stderr, "Fail to set callback : result(%d)", ret);
                  return;
               }
          }

        ret = tts_destroy(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to destroy handle : result(%d)", ret);
             return;
          }

        if (tts) tts = NULL;
     }
}

void _tts_state_changed_cb(tts_h tts, tts_state_e previous, tts_state_e current, void* data)
{
   int ret = 0;
   int u_id = 0;

   if (TTS_STATE_CREATED == previous && TTS_STATE_READY == current)
     {
        ret = tts_add_text(tts, UNAVAILABLE_TEXT, NULL, TTS_VOICE_TYPE_AUTO,
                           TTS_SPEED_AUTO, &u_id);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to add kept text : ret(%d)\n", ret);
          }

        ret = tts_play(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to play TTS : ret(%d)\n", ret);
          }
     }
}

static void _tts_init(void)
{
   int ret = 0;

   ret = tts_create(&tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to get handle : result(%d)", ret);
        return;
     }

   ret = tts_set_state_changed_cb(tts, _tts_state_changed_cb, NULL);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to set callback : result(%d)", ret);
        return;
     }

   ret = tts_set_mode(tts, TTS_MODE_SCREEN_READER);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to set mode : result(%d)", ret);
        return;
     }

   ret = tts_prepare(tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to prepare handle : result(%d)", ret);
        return;
     }
}

static void _timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
   Ecore_X_Window xwin;
   unsigned int val;

   xwin = elm_win_xwindow_get(data);
   if (!xwin) return;

   evas_object_del(obj);

   val = 2;
   ecore_x_window_prop_card32_set
     (xwin, ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL, &val, 1);

   _tts_shutdown();
}

EAPI Eina_Bool
ea_screen_reader_support_set(Evas_Object *win, Eina_Bool support)
{
   Ecore_X_Window xwin;
   unsigned int val;
   int tts_val;
   Evas_Object *base;
   Evas_Object *body;
   Evas_Object *popup;

   if (vconf_get_bool(VCONFKEY_SETAPPL_ACCESSIBILITY_TTS, &tts_val) != 0)
     return EINA_FALSE;

   if (!tts_val) return EINA_FALSE;

   if (!win) return EINA_FALSE;

   xwin = elm_win_xwindow_get(win);
   if (!xwin) return EINA_FALSE;

   if (support)
     {
        val = 0;
        elm_config_access_set(EINA_TRUE);

        ecore_x_window_prop_card32_set
          (xwin, ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL, &val, 1);
     }
   else
     {
        elm_config_access_set(EINA_FALSE);

        popup = elm_popup_add(win);
        evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_object_text_set(popup, UNAVAILABLE_TEXT);
        elm_popup_timeout_set(popup, 12.0);
        evas_object_smart_callback_add(popup, "timeout", _timeout_cb, win);

        _tts_init();
        evas_object_show(popup);
     }

   return EINA_TRUE;
}

EAPI Eina_Bool
ea_screen_reader_support_get()
{
   return elm_config_access_get();
}
