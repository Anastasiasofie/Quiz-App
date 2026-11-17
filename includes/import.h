#ifndef IMPORT_H
#define IMPORT_H


FILE *open_csv(const char *dateiname);

int parse_csv_line(void);

int validate_question(void);

int import_csv(void);

#endif