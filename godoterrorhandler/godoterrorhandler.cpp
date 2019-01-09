#include "godoterrorhandler.h"

GodotErrorHandler *GodotErrorHandler::singleton = NULL;

GodotErrorHandler::GodotErrorHandler() {

	eh.errfunc = _err_handler;
	eh.userdata = this;
	add_error_handler(&eh);

  singleton = this;
}

GodotErrorHandler::~GodotErrorHandler() {

	remove_error_handler(&eh);
  singleton = NULL;
}

GodotErrorHandler *GodotErrorHandler::get_singleton() {
  if (GodotErrorHandler::singleton == NULL) {
    GodotErrorHandler::singleton = new GodotErrorHandler();
  }

  return GodotErrorHandler::singleton;
}

void GodotErrorHandler::_err_handler(void *ud, const char *p_func, const char *p_file, int p_line, const char *p_err, const char *p_descr, ErrorHandlerType p_type) {

	GodotErrorHandler *gdh = (GodotErrorHandler *)ud;

	Dictionary errorObject;

	errorObject["error"] = p_err;
	errorObject["error_descr"] = p_descr;
	errorObject["source_file"] = p_file;
	errorObject["source_line"] = p_line;
	errorObject["source_func"] = p_func;
	errorObject["warning"] = p_type == ERR_HANDLER_WARNING;
	Array cstack;

	Vector<ScriptLanguage::StackInfo> si;

	for (int i = 0; i < ScriptServer::get_language_count(); i++) {
		si = ScriptServer::get_language(i)->debug_get_current_stack_info();
		if (si.size())
			break;
	}

	cstack.resize(si.size() * 2);
	for (int i = 0; i < si.size(); i++) {
		String path;
		int line = 0;
		if (si[i].script.is_valid()) {
			path = si[i].script->get_path();
			line = si[i].line;
		}
		cstack[i * 2 + 0] = path;
		cstack[i * 2 + 1] = line;
	}

	errorObject["callstack"] = cstack;

  gdh->handle_error(errorObject);
}

void GodotErrorHandler::handle_error(Dictionary errorObject) {
  emit_signal("error_threw", errorObject);
}


void GodotErrorHandler::reset_singleton() {
  delete GodotErrorHandler::singleton;

  GodotErrorHandler::singleton = NULL;
}

void GodotErrorHandler::_bind_methods() {
  ADD_SIGNAL(MethodInfo("error_threw", PropertyInfo(Variant::DICTIONARY, "errorObject")));
}
