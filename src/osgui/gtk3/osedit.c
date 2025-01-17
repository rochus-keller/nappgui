/*
 * NAppGUI Cross-platform C SDK
 * 2015-2022 Francisco Garcia Collado
 * MIT Licence
 * https://nappgui.com/en/legal/license.html
 *
 * File: osedit.c
 *
 */

/* Operating System edit box */

#include "osedit.h"
#include "osedit.inl"
#include "osgui.inl"
#include "osglobals.inl"
#include "oscontrol.inl"
#include "ospanel.inl"
#include "oswindow.inl"
#include "cassert.h"
#include "color.h"
#include "event.h"
#include "font.h"
#include "heap.h"
#include "ptr.h"
#include "strings.h"
#include "unicode.h"

#if !defined(__GTK3__)
#error This file is only for GTK Toolkit
#endif

struct _osedit_t 
{
    OSControl control;
    uint32_t fstyle;
    uint32_t fsize;
    uint32_t fsize_render;
    bool_t launch_event;
    GtkWidget *tview;
    edit_flag_t flags;
    GtkCssProvider *color;
    GtkCssProvider *bgcolor;
    GtkCssProvider *font;
    color_t ccolor;
    Listener *OnFilter;
    Listener *OnChange;
    Listener *OnFocus;
};

/*---------------------------------------------------------------------------*/

static gboolean i_OnDraw(GtkWidget *widget, cairo_t *cr, OSEdit *edit)
{
    /* It seems gtk_widget_get_preferred_size don't compute font changes
     * until draw is made */
    edit->fsize_render = edit->fsize;
    return FALSE;
}

/*---------------------------------------------------------------------------*/

static gboolean i_DrawBackground(GtkWidget *widget, cairo_t *cr, OSEdit *edit)
{
    GtkStyleContext *ctx = osglobals_entry_context();
    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_height(widget);
    cassert_no_null(edit);

    if (edit->bgcolor != NULL)
        gtk_style_context_add_provider(ctx, GTK_STYLE_PROVIDER(edit->bgcolor), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_style_context_save(ctx);
    gtk_style_context_set_state(ctx, gtk_widget_get_state_flags(widget));
    gtk_render_background(ctx, cr, 0, 0, w, h);
    gtk_render_frame(ctx, cr, 0, 0, w, h);
    gtk_style_context_restore(ctx);
    edit->fsize_render = edit->fsize;

    if (edit->bgcolor != NULL)
        gtk_style_context_remove_provider(ctx, GTK_STYLE_PROVIDER(edit->bgcolor));

    return FALSE;
}

/*---------------------------------------------------------------------------*/

static gchar *i_text(OSEdit *edit, bool_t *allocated)
{
    if (edit->tview != NULL)
    {
        GtkTextIter st, end;
        GtkTextBuffer *tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit->tview));
        gtk_text_buffer_get_start_iter(tbuf, &st);
        gtk_text_buffer_get_end_iter(tbuf, &end);
        *allocated = TRUE;
        return gtk_text_buffer_get_text(tbuf, &st, &end, FALSE);
    }
    else
    {
       *allocated = FALSE;
       return (gchar*)gtk_entry_get_text(GTK_ENTRY(edit->control.widget));
    }
}

/*---------------------------------------------------------------------------*/

static void i_OnChange(GtkEditable *editable, OSEdit *edit)
{
    cassert_no_null(edit);
    if (edit->launch_event == TRUE && gtk_widget_is_sensitive(edit->control.widget) && edit->OnFilter != NULL)
    {
        EvText params;
        EvTextFilter result;
        bool_t allocated;
        params.text = (const char_t*)i_text(edit, &allocated);
        params.cpos = (uint32_t)gtk_editable_get_position(editable);
        result.apply = FALSE;
        result.text[0] = '\0';
        result.cpos = UINT32_MAX;
        listener_event(edit->OnFilter, ekEVTXTFILTER, edit, &params, &result, OSEdit, EvText, EvTextFilter);
        if (allocated)
            g_free((gchar*)params.text);

        if (result.apply == TRUE)
        {
            bool_t prev = edit->launch_event;
            edit->launch_event = FALSE;
            osedit_text(edit, result.text);
            edit->launch_event = prev;
        }

        if (result.cpos != UINT32_MAX)
            gtk_editable_set_position(editable, (gint)result.cpos);
        else
            gtk_editable_set_position(editable, (gint)params.cpos);
    }
}

/*---------------------------------------------------------------------------*/

static gboolean i_OnPressed(GtkWidget *widget, GdkEventButton *event, OSEdit *edit)
{
    cassert_no_null(edit);
    cassert_no_null(event);
    cassert_unref(edit->control.widget == widget, widget);
    cassert(edit->tview == NULL);
    if (event->button == 1 && BIT_TEST(edit->flags, ekEDAUTOSEL) == TRUE)
    {
        gtk_editable_select_region(GTK_EDITABLE(edit->control.widget), 0, -1);
        gtk_widget_grab_focus(edit->control.widget);
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*/

OSEdit *osedit_create(const edit_flag_t flags)
{
    OSEdit *edit = heap_new0(OSEdit);
    Font *font = _osgui_create_default_font();
    GtkWidget *widget;
    edit->flags = flags;

    switch (edit_type(flags)) {
    case ekEDSING:
        widget = gtk_entry_new();
        gtk_entry_set_width_chars(GTK_ENTRY(widget), 0);
        _oscontrol_widget_font(widget, "entry", font, &edit->font);
//        _oscontrol_set_css(widget, "entry {padding-left:4px;padding-right:4px;padding-top:3px;padding-bottom:3px;}");
//        _oscontrol_set_css(widget, "entry {border-bottom-left-radius:4px;border-bottom-right-radius:4px;border-top-left-radius:4px;border-top-right-radius:4px;}");
        g_signal_connect(G_OBJECT(widget), "draw", G_CALLBACK(i_OnDraw), (gpointer)edit);
        g_signal_connect(G_OBJECT(widget), "changed", G_CALLBACK(i_OnChange), (gpointer)edit);
        g_signal_connect(G_OBJECT(widget), "button-press-event", G_CALLBACK(i_OnPressed), (gpointer)edit);
        break;

    case ekEDMULT:
    {
        GtkBorder padding;
        String *css;
        edit->tview = gtk_text_view_new();
        _oscontrol_widget_font(edit->tview, "textview", font, &edit->font);
        gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(edit->tview), FALSE);
        widget = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(widget), edit->tview);
        g_signal_connect(G_OBJECT(edit->tview), "draw", G_CALLBACK(i_DrawBackground), (gpointer)edit);
        gtk_widget_show(edit->tview);
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(edit->tview), GTK_WRAP_WORD_CHAR);
        osglobals_register_entry(&padding);
        css = str_printf("textview {background-image:none;background-color:transparent;padding-right:%dpx;padding-bottom:%dpx;padding-left:%dpx;padding-top:%dpx;}", padding.right, padding.bottom, padding.left, padding.top);
        _oscontrol_set_css(edit->tview, tc(css));
        _oscontrol_set_css(edit->tview, "textview text {background-image:none;background-color:transparent;}");
        str_destroy(&css);
        break;
    }

    cassert_default();
    }

    edit->fsize = (uint32_t)font_size(font);
    edit->fsize_render = edit->fsize;
    edit->ccolor = kCOLOR_TRANSPARENT;
    edit->launch_event = TRUE;
    _oscontrol_init(&edit->control, ekGUI_COMPONENT_EDITBOX, widget, edit->tview ? edit->tview : widget, TRUE);
    font_destroy(&font);
    return edit;
}

/*---------------------------------------------------------------------------*/

void osedit_destroy(OSEdit **edit)
{
    cassert_no_null(edit);
    cassert_no_null(*edit);

    if ((*edit)->tview != NULL)
    {
        // Is destroyed by scrolled window
        //g_object_unref((*edit)->tview);
        if ((*edit)->bgcolor != NULL)
            g_object_unref((*edit)->bgcolor);
    }

    listener_destroy(&(*edit)->OnFilter);
    listener_destroy(&(*edit)->OnChange);
    listener_destroy(&(*edit)->OnFocus);
    _oscontrol_destroy(*(OSControl**)edit);
    heap_delete(edit, OSEdit);
}

/*---------------------------------------------------------------------------*/

void osedit_OnFilter(OSEdit *edit, Listener *listener)
{
    cassert_no_null(edit);
    listener_update(&edit->OnFilter, listener);
}

/*---------------------------------------------------------------------------*/

void osedit_OnChange(OSEdit *edit, Listener *listener)
{
    cassert_no_null(edit);
    listener_update(&edit->OnChange, listener);
}

/*---------------------------------------------------------------------------*/

void osedit_OnFocus(OSEdit *edit, Listener *listener)
{
    cassert_no_null(edit);
    listener_update(&edit->OnFocus, listener);
}

/*---------------------------------------------------------------------------*/

void osedit_text(OSEdit *edit, const char_t *text)
{
    cassert_no_null(edit);
    edit->launch_event = FALSE;
    if (edit->tview != NULL)
    {
        GtkTextBuffer *tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit->tview));
        String *markup = NULL;
        cassert(edit_type(edit->flags) == ekEDMULT);
        if (edit->fstyle & ekFSTRIKEOUT)
        {
            if (edit->fstyle & ekFUNDERLINE)
                markup = str_printf("<u><s>%s</s></u>", text);
            else
                markup = str_printf("<s>%s</s>", text);
        }
        else if (edit->fstyle & ekFUNDERLINE)
        {
            markup = str_printf("<u>%s</u>", text);
        }

        if (markup != NULL)
        {
            GtkTextIter iter;
            gtk_text_buffer_set_text(tbuf, "", -1);
            gtk_text_buffer_get_start_iter(tbuf, &iter);
#if GTK_CHECK_VERSION(3, 16, 0)
            gtk_text_buffer_insert_markup(tbuf, &iter, tc(markup), str_len(markup));
#endif
            str_destroy(&markup);
        }
        else
        {
            gtk_text_buffer_set_text(tbuf, (const gchar*)text, -1);
        }
    }
    else
    {
        cassert(edit_type(edit->flags) == ekEDSING);
        gtk_entry_set_text(GTK_ENTRY(edit->control.widget), (const gchar*)text);
    }

    edit->launch_event = TRUE;
}

/*---------------------------------------------------------------------------*/

void osedit_tooltip(OSEdit *edit, const char_t *text)
{
    unref(edit);
    unref(text);
    cassert_msg(FALSE, "Not implemented");
}

/*---------------------------------------------------------------------------*/

void osedit_font(OSEdit *edit, const Font *font)
{
    cassert_no_null(edit);
    edit->fstyle = font_style(font);
    cassert(!(edit->fstyle & ekFPOINTS));
    edit->fsize = (uint32_t)font_size(font);
    if (edit->tview != NULL)
    {
        cassert(edit_type(edit->flags) == ekEDMULT);
        _oscontrol_remove_provider(edit->tview, edit->font);
        _oscontrol_widget_font(edit->tview, "textview", font, &edit->font);
    }
    else
    {
        cassert(edit_type(edit->flags) == ekEDSING);
        _oscontrol_remove_provider(edit->control.widget, edit->font);
        _oscontrol_widget_font(edit->control.widget, "entry", font, &edit->font);
    }
}

/*---------------------------------------------------------------------------*/

void osedit_align(OSEdit *edit, const align_t align)
{
    cassert_no_null(edit);
    if (edit->tview != NULL)
    {
        cassert_msg(align == ekLEFT, "Not implemented");
    }
    else
    {
        gfloat a = 0.f;
        cassert(edit_type(edit->flags) == ekEDSING);
        switch (align) {
        case ekLEFT:
            a = 0.f;
            break;
        case ekCENTER:
        case ekJUSTIFY:
            a = .5f;
            break;
        case ekRIGHT:
            a = .99f;
            break;
        cassert_default();
        }

        gtk_entry_set_alignment(GTK_ENTRY(edit->control.widget), a);
    }
}

/*---------------------------------------------------------------------------*/

void osedit_passmode(OSEdit *edit, const bool_t passmode)
{
    cassert_no_null(edit);
    if (edit->tview != NULL)
    {
        cassert_msg(FALSE, "Not implemented");
    }
    else
    {
        gtk_entry_set_visibility(GTK_ENTRY(edit->control.widget), (gboolean)!passmode);
    }
}

/*---------------------------------------------------------------------------*/

void osedit_editable(OSEdit *edit, const bool_t is_editable)
{
    cassert_no_null(edit);
    if (edit->tview != NULL)
    {
        cassert(edit_type(edit->flags) == ekEDMULT);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(edit->tview), (gboolean)is_editable);
    }
    else
    {
        cassert(edit_type(edit->flags) == ekEDSING);
        gtk_editable_set_editable(GTK_EDITABLE(edit->control.widget), (gboolean)is_editable);
    }
}

/*---------------------------------------------------------------------------*/

void osedit_autoselect(OSEdit *edit, const bool_t autoselect)
{
    cassert_no_null(edit);
    if (autoselect == TRUE)
        BIT_SET(edit->flags, ekEDAUTOSEL);
    else
        BIT_CLEAR(edit->flags, ekEDAUTOSEL);
}

/*---------------------------------------------------------------------------*/

static void i_set_color(OSEdit *edit, const color_t color)
{
    GtkWidget *widget = NULL;
    const char_t *type = NULL;
    cassert_no_null(edit);
    if (edit->tview != NULL)
    {
        cassert(edit_type(edit->flags) == ekEDMULT);
        widget = edit->tview;
        type = "textview";
    }
    else
    {
        cassert(edit_type(edit->flags) == ekEDSING);
        widget = edit->control.widget;
        type = "entry";
    }

    if (edit->color != NULL)
    {
        _oscontrol_remove_provider(widget, edit->color);
        edit->color = NULL;
    }

    if (color != kCOLOR_TRANSPARENT)
        _oscontrol_widget_color(widget, type, color, &edit->color);
}

/*---------------------------------------------------------------------------*/

void osedit_color(OSEdit *edit, const color_t color)
{
    i_set_color(edit, color);
    edit->ccolor = color;
}

/*---------------------------------------------------------------------------*/

void osedit_bgcolor(OSEdit *edit, const color_t color)
{
    cassert_no_null(edit);
    if (edit->tview != NULL)
    {
        cassert(edit_type(edit->flags) == ekEDMULT);
        if (edit->bgcolor != NULL)
        {
            g_object_unref(edit->bgcolor);
            edit->bgcolor = NULL;
        }

        if (color != kCOLOR_TRANSPARENT)
        {
            char_t html[16];
            String *css;
            color_to_html(color, html, sizeof(html));
            css = str_printf("entry {background-color:%s;background-image:none;}", html);
            edit->bgcolor = gtk_css_provider_new();
            gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(edit->bgcolor), (gchar*)tc(css), -1, NULL);
            str_destroy(&css);
        }
    }
    else
    {
        cassert(edit_type(edit->flags) == ekEDSING);
        if (edit->bgcolor != NULL)
        {
            _oscontrol_remove_provider(edit->control.widget, edit->bgcolor);
            edit->bgcolor = NULL;
        }

        if (color != kCOLOR_TRANSPARENT)
            _oscontrol_widget_bg_color(edit->control.widget, "entry", color, &edit->bgcolor);
    }
}

/*---------------------------------------------------------------------------*/

void osedit_bounds(const OSEdit *edit, const real32_t refwidth, const uint32_t lines, real32_t *width, real32_t *height)
{
    GtkRequisition s;
    cassert_no_null(edit);
    cassert_no_null(width);
    cassert_no_null(height);
    cassert_unref(lines == 1, lines);
    if (edit->tview != NULL)
    {
        gtk_widget_set_size_request(edit->tview, -1, -1);
        gtk_widget_get_preferred_size(edit->tview, &s, NULL);
    }
    else
    {
        gtk_widget_set_size_request(edit->control.widget, -1, -1);
        gtk_widget_get_preferred_size(edit->control.widget, &s, NULL);
    }

    *width = refwidth;
    *height = (real32_t)s.height + edit->fsize - edit->fsize_render;
}

/*---------------------------------------------------------------------------*/

void osedit_attach(OSEdit *edit, OSPanel *panel)
{
    _ospanel_attach_control(panel, (OSControl*)edit);
}

/*---------------------------------------------------------------------------*/

void osedit_detach(OSEdit *edit, OSPanel *panel)
{
    _ospanel_detach_control(panel, (OSControl*)edit);
}

/*---------------------------------------------------------------------------*/

void osedit_visible(OSEdit *edit, const bool_t is_visible)
{
    _oscontrol_set_visible((OSControl*)edit, is_visible);
}

/*---------------------------------------------------------------------------*/

void osedit_enabled(OSEdit *edit, const bool_t is_enabled)
{
    _oscontrol_set_enabled((OSControl*)edit, is_enabled);
    i_set_color(edit, is_enabled ? edit->ccolor : UINT32_MAX);
}

/*---------------------------------------------------------------------------*/

void osedit_size(const OSEdit *edit, real32_t *width, real32_t *height)
{
    _oscontrol_get_size((const OSControl*)edit, width, height);
}

/*---------------------------------------------------------------------------*/

void osedit_origin(const OSEdit *edit, real32_t *x, real32_t *y)
{
    _oscontrol_get_origin((const OSControl*)edit, x, y);
}

/*---------------------------------------------------------------------------*/

void osedit_frame(OSEdit *edit, const real32_t x, const real32_t y, const real32_t width, const real32_t height)
{
    _oscontrol_set_frame((OSControl*)edit, x, y, width, height);
}

/*---------------------------------------------------------------------------*/

void _osedit_set_focus(OSEdit *edit)
{
    cassert_no_null(edit);
    if (edit->OnFocus != NULL)
    {
        bool_t params = TRUE;
        listener_event(edit->OnFocus, ekEVFOCUS, edit, &params, NULL, OSEdit, bool_t, void);
    }
}

/*---------------------------------------------------------------------------*/

void _osedit_unset_focus(OSEdit *edit)
{
    cassert_no_null(edit);
    if (edit->launch_event == TRUE && gtk_widget_is_sensitive(edit->control.widget) && edit->OnChange != NULL)
    {
        EvText params;
        bool_t allocated;
        params.text = (const char_t*)i_text(edit, &allocated);
        listener_event(edit->OnChange, ekEVTXTCHANGE, edit, &params, NULL, OSEdit, EvText, void);
        if (allocated)
            g_free((gchar*)params.text);
    }

    if (edit->OnFocus != NULL)
    {
        bool_t params = FALSE;
        listener_event(edit->OnFocus, ekEVFOCUS, edit, &params, NULL, OSEdit, bool_t, void);
    }

    if (edit->tview == NULL)
    {
        gtk_editable_select_region(GTK_EDITABLE(edit->control.widget), -1, -1);
    }
    else
    {
        GtkTextIter iter;
        GtkTextBuffer *tbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit->tview));
        gtk_text_buffer_get_start_iter(tbuf, &iter);
        gtk_text_buffer_select_range(tbuf, &iter, &iter);
    }
}

/*---------------------------------------------------------------------------*/

void _osedit_detach_and_destroy(OSEdit **edit, OSPanel *panel)
{
    cassert_no_null(edit);
    osedit_detach(*edit, panel);
    osedit_destroy(edit);
}

/*---------------------------------------------------------------------------*/

bool_t _osedit_autoselect(const OSEdit *edit)
{
    cassert_no_null(edit);
    return BIT_TEST(edit->flags, ekEDAUTOSEL);
}

/*---------------------------------------------------------------------------*/

GtkWidget *_osedit_focus(OSEdit *edit)
{
    cassert_no_null(edit);
    if (edit->tview != NULL)
        return edit->tview;
    return edit->control.widget;
}
