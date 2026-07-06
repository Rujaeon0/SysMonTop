#include "process_object_gtk.h"

G_DEFINE_TYPE(ProcessObject, process_object, G_TYPE_OBJECT)

static void process_object_class_init(ProcessObjectClass *klass){

}

static void process_object_init(ProcessObject *self){

}

ProcessObject *
process_object_new(void){
    return g_object_new(PROCESS_TYPE_OBJECT, NULL);
}