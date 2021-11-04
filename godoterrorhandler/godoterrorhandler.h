#ifndef GODOTERRORHANDLER_H
#define GODOTERRORHANDLER_H

#include <inttypes.h>

#include "core/error/error_macros.h"
#include "core/object/object.h"
#include "core/object/script_language.h"
#include "core/variant/dictionary.h"

class GodotErrorHandler : public Object
{
  GDCLASS(GodotErrorHandler, Object);

  static GodotErrorHandler *singleton;
	static void _err_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, bool p_editor_notify, ErrorHandlerType p_type);
  
	ErrorHandlerList eh;

protected:
  static void _bind_methods();

  struct ErrorObject {

    String source_file;
    String source_func;
    int source_line;
    String error;
    String error_descr;
    bool warning;
    Array callstack;
  };

public:
  static GodotErrorHandler *get_singleton();
  static void reset_singleton();
  GodotErrorHandler();
  ~GodotErrorHandler();

  void handle_error(Dictionary errorObject);
};
#endif // GODOTERRORHANDLER_H
