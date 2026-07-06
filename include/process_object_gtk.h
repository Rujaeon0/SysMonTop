#ifndef PROCESS_OBJECT_H
#define PROCESS_OBJECT_H
#include <glib-object.h>
#include "process.h"
#define PROCESS_TYPE_OBJECT (process_object_get_type())

G_DECLARE_FINAL_TYPE(ProcessObject, process_object , PROCESS, OBJECT, GObject)

struct _ProcessObject{
    GObject parent_instance;
    mem process;
};

ProcessObject *process_object_new(void);

#endif