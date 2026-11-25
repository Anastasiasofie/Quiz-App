#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#include "../includes/quiz.h"
#include "../includes/data.h"
#include "../includes/ui.h"


static int geprueft = 0;
static int fehler = 0;


#define CHECK(expr) do { \
    geprueft++; \
    if (!(expr)) { \
        fehler++; \
        printf("Fehler in %s, Zeile %d: %s\n", __FILE__, __LINE__, #expr); \
    } \
} while (0)

#define F_IN "t_ui_in.txt"
#define F_OUT "t_ui_out.txt"
#define F_err "t_ui_err.txt"

static int saved_out = -1;
static int saved_err = -1;
static char out_buf[1024];
static char err_buf[1024];

//Tastatur simulieren
static void simulate_input(const char *input)
{
    FILE *f = fopen(F_IN, "w");
    fputs(input, f);
    fclose(f);
    freopen(F_IN, "r", stdin);

    fflush(stdout);
    fflush(stderr);
    saved_out = dup(fileno(stdout));
    saved_err = dup(fileno(stderr));
    FILE *out_file = fopen(F_OUT, "w");
    FILE *err_file = fopen(F_err, "w");
    dup2(fileno(out_file), fileno(stdout));
    dup2(fileno(err_file), fileno(stderr));
    fclose(out_file);
    fclose(err_file);
}

static void lies(const char *filename, char *buffer, size_t size)
{
    FILE *f = fopen(filename, "r");
    size_t len = f ? fread(buffer, 1, size - 1, f) : 0;
    buffer[len] = '\0';
    if (f) fclose(f);
}

//Funktion zum Wiederherstellen der Standardausgabe und -fehlerausgabe
static void stop(void)
{
    fflush(stdout);
    fflush(stderr);
    dup2(saved_out, fileno(stdout));
    dup2(saved_err, fileno(stderr));
    close(saved_out);
    close(saved_err);
    lies(F_OUT, out_buf, sizeof(out_buf));
    lies(F_err, err_buf, sizeof(err_buf));
}

static int anzahl_vorkommen(const char *str, const char *sub)
{
    int count = 0;
    for (const char *p = str; (p = strstr(p, sub)); p += strlen(sub)) {
        count++;
    }
    return count;
}

static int enthaelt(const char *str, const char *sub)
{
    return strstr(str, sub) != NULL;
}

static Frage beispiel_frage(void)
{
    Frage f;
    memset(&f, 0, sizeof(f));
    strcpy(f.thema, "Beispielthema");
    strcpy(f.frage, "Beispielfrage?");
    strcpy(f.antworten[0], "Antwort A");
    strcpy(f.antworten[1], "Antwort B");
    strcpy(f.antworten[2], "Antwort C");
    strcpy(f.antworten[3], "Antwort D");
    f.richtige_antwort = 1; // Antwort B ist richtig
    return f;
}
