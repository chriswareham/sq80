/*
 * Copyright (c) 2021 Chris Wareham <chris@chriswareham.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "main.h"
#include "xmlparser.h"

typedef enum {
    STATE_START,
    STATE_SQ80,
    STATE_NAME,
    STATE_TYPE,
    STATE_PARAM,
    STATE_FINISH
} ParserState;

typedef struct {
    ParserState state;
    Patch *patch;
} ParserData;

static void start_element(GMarkupParseContext *, const gchar *, const gchar **, const gchar **, gpointer, GError **);
static void end_element(GMarkupParseContext *, const gchar *, gpointer, GError **);
static void text(GMarkupParseContext *, const gchar *, gsize, gpointer, GError **);
static char *get_line(FILE *);

gboolean
xmlparser_write(const gchar *filename, Patch *patch)
{
    FILE *fp;
    gint i;

    if (!(fp = fopen(filename, "w"))) {
        return FALSE;
    }

    fprintf(fp, "<sq80>\n");
    fprintf(fp, "<name>%s</name>\n", patch->name);
    fprintf(fp, "<type>%s</type>\n", patch->type);
    for (i = 0; i < PARAMETER_COUNT; i++) {
        fprintf(fp, "<param id=\"%d\" value=\"%d\"/>\n", i, patch->parameters[i]);
    }
    fprintf(fp, "</sq80>\n");

    fclose(fp);

    return TRUE;
}

Patch *
xmlparser_read(const gchar *filename, GError **error)
{
    FILE *fp;
    ParserData data;
    GMarkupParser parser;
    GMarkupParseContext *context;
    gboolean status;
    gchar *buf;

    if (!(fp = fopen(filename, "r"))) {
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "%s", strerror(errno));
        return NULL;
    }

    data.state = STATE_START;
    data.patch = g_malloc0(sizeof(Patch));

    parser.start_element = start_element;
    parser.end_element = end_element;
    parser.text = text;
    parser.passthrough = NULL;
    parser.error = NULL;

    context = g_markup_parse_context_new(&parser, 0, &data, NULL);

    status = FALSE;
    while ((buf = get_line(fp))) {
        status = g_markup_parse_context_parse(context, buf, strlen(buf), error);
        g_free(buf);
        if (!status) {
            if (data.patch->name) {
                g_free(data.patch->name);
            }
            if (data.patch->type) {
                g_free(data.patch->type);
            }
            g_free(data.patch);
            data.patch = NULL;
            break;
        }
    }

    g_markup_parse_context_free(context);

    fclose(fp);

    return data.patch;
}

static void
start_element(GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
    ParserData *pd = data;
    gint i, id, value;
    gchar *ptr;

    switch (pd->state) {
    case STATE_START:
        if (strcmp(element_name, "sq80") == 0) {
            pd->state = STATE_SQ80;
        } else {
            g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "invalid first element '%s'", element_name);
        }
        break;
    case STATE_SQ80:
        if (strcmp(element_name, "name") == 0) {
            pd->state = STATE_NAME;
        } else if (strcmp(element_name, "type") == 0) {
            pd->state = STATE_TYPE;
        } else if (strcmp(element_name, "param") == 0) {
            pd->state = STATE_PARAM;
            id = -1;
            value = -1;
            for (i = 0; attribute_names[i] && attribute_values[i]; i++) {
                if (strcmp(attribute_names[i], "id") == 0) {
                    id = strtol(attribute_values[i], &ptr, 10);
                    if (attribute_values[i][0] == '\0' || *ptr != '\0') {
                        id = -1;
                    }
                } else if (strcmp(attribute_names[i], "value") == 0) {
                    value = strtol(attribute_values[i], &ptr, 10);
                    if (attribute_values[i][0] == '\0' || *ptr != '\0') {
                        value = -1;
                    }
                } else {
                    g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ATTRIBUTE, "unknown attribute '%s'", element_name);
                    break;
                }
            }
            if (id < 0 || id > 35 || value < 0 || value > 127) {
                g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "bad parameter value");
            } else {
                pd->patch->parameters[id] = value;
            }
        } else {
            g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "invalid element '%s'", element_name);
        }
        break;
    case STATE_NAME:
    case STATE_TYPE:
    case STATE_PARAM:
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "invalid element '%s'", element_name);
        break;
    case STATE_FINISH:
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "element '%s' after last valid element", element_name);
        break;
    default:
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ELEMENT, "unknown element '%s'", element_name);
        break;
    }
}

static void
end_element(GMarkupParseContext *context, const gchar *element_name, gpointer data, GError **error)
{
    ParserData *d = data;

    switch (d->state) {
    case STATE_START:
    case STATE_FINISH:
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "end element '%s' before first valid element", element_name);
        break;
    case STATE_SQ80:
        d->state = STATE_FINISH;
        break;
    case STATE_NAME:
    case STATE_TYPE:
    case STATE_PARAM:
        d->state = STATE_SQ80;
        break;
    default:
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "unknown parser state");
        break;
    }
}

static void
text(GMarkupParseContext *context, const gchar *text, gsize len, gpointer data, GError **error)
{
    ParserData *pd = data;

    switch (pd->state) {
    case STATE_NAME:
        pd->patch->name = g_malloc0(len + 1);
        memcpy(pd->patch->name, text, len);
        break;
    case STATE_TYPE:
        pd->patch->type = g_malloc0(len + 1);
        memcpy(pd->patch->type, text, len);
        break;
    case STATE_SQ80:
        if (len) {
            g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "content '%.*s' not allowed in element 'sq80'", (int)len, text);
        }
        break;
    case STATE_PARAM:
        if (len) {
            g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "content '%.*s' not allowed in element 'param'", (int)len, text);
        }
        break;
    default:
        g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT, "unknown parser state");
        break;
    }
}

static gchar *
get_line(FILE *fp)
{
    gchar *line, *ptr, buf[BUFSIZ];
    gint n;

    if ((line = g_malloc(BUFSIZ)) == NULL) {
        return NULL;
    }

    if ((fgets(line, BUFSIZ, fp)) == NULL) {
        g_free(line);
        return NULL;
    }

    while (line[strlen(line) - 1] != '\n') {
        if (feof(fp)) {
            break;
        }
        fgets(buf, BUFSIZ, fp);
        if ((ptr = g_realloc(line, strlen(line) + BUFSIZ)) == NULL) {
            g_free(line);
            return NULL;
        }
        line = ptr;
        ptr = line + strlen(line);
        strcpy(ptr, buf);
    }

    for (n = strlen(line) - 1; isspace(line[n]); n--) {
        line[n] = '\0';
    }

    return line;
}
