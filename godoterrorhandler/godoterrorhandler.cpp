#include "godoterrorhandler.h"

GodotErrorHandler *GodotErrorHandler::singleton = nullptr;

GodotErrorHandler::GodotErrorHandler() {

	eh.errfunc = _err_handler;
	eh.userdata = this;
	add_error_handler(&eh);

  singleton = this;
}

GodotErrorHandler::~GodotErrorHandler() {

	remove_error_handler(&eh);
  singleton = nullptr;
}

GodotErrorHandler *GodotErrorHandler::get_singleton() {
  if (GodotErrorHandler::singleton == nullptr) {
    GodotErrorHandler::singleton = new GodotErrorHandler();
  }

  return GodotErrorHandler::singleton;
}

void GodotErrorHandler::_err_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, bool p_editor_notify, ErrorHandlerType p_type) {

	GodotErrorHandler *gdh = (GodotErrorHandler *)p_self;

	Dictionary errorObject;

	errorObject["error"] = p_error;
	errorObject["error_descr"] = p_errorexp;
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
		cstack[i * 2 + 0] = si[i].file;
		cstack[i * 2 + 1] = si[i].line;
	}

	errorObject["callstack"] = cstack;

  gdh->handle_error(errorObject);
}

void GodotErrorHandler::handle_error(Dictionary errorObject) {
  emit_signal(SNAME("error_threw"), errorObject);
}


void GodotErrorHandler::reset_singleton() {
  delete GodotErrorHandler::singleton;

  GodotErrorHandler::singleton = nullptr;
}

void GodotErrorHandler::_bind_methods() {
	ADD_SIGNAL(MethodInfo("error_threw", PropertyInfo(Variant::DICTIONARY, "errorObject")));
}
