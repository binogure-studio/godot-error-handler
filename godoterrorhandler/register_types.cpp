#include "globals.h"
#include "object_type_db.h"
#include "register_types.h"

#include "godoterrorhandler.h"

void register_godoterrorhandler_types() {
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotErrorHandler", GodotErrorHandler::get_singleton()));

  ObjectTypeDB::register_virtual_type<GodotErrorHandler>();
}

void unregister_godoterrorhandler_types() {
  GodotErrorHandler::reset_singleton();
}
