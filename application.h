
#ifndef APPLICATION_H

#define APPLICATION_H


#include "BBLibc.h"

typedef struct {
        void *unknownFuncs[5];
        int  (*destroy)(int); /*0x005B8E00*/
        boolean (*start)(void);
        void (*wait_for_event)(void);/*0x005B8DD3*/
        void (*end)(void);
        void *unknownFuncs1[3];
        void (*mark_level_to_load)(char *);
        void *unknownFuncs2[1];
        void (*load_level)(char *);
        void *unknownFuncs3[17];
} application_methods_t;


typedef struct {
        application_methods_t *methods;
        char unknownFields1[1540];
        BBLibc_name_t mapName;
        char unknownFields2[428];
        void *module;
        char unknownFields3[16];
} application_t;


#ifndef BLD_EXT_FUNCS

extern int application_destroy(application_t *self, int a);
extern boolean application_start(application_t *self);
extern void application_wait_for_event(application_t *self);
extern void application_end(application_t *self);
extern void application_load_level_script(application_t *self, char *script);
extern application_t* application_init(
        application_t *self, void *module, int nCmdShow, char *cmdLine
);
extern application_t *application_init2(
        application_t *self, void *module, int nCmdShow, char *cmdLine,
        void *unknown
);

#endif

#endif /* APPLICATION_H */

 