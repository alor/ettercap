
/* $Id: ec_conf.h,v 1.3 2003/09/18 22:15:01 alor Exp $ */

#ifndef EC_CONF_H
#define EC_CONF_H


struct conf_entry {
   char *name;
   int *value;
};

struct conf_section {
   char *title;
   struct conf_entry *entries;
};


/* exported functions */

extern void load_conf(void);
extern void conf_dissectors(void);

#endif

/* EOF */

// vim:ts=3:expandtab

