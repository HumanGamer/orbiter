// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#define OAPI_IMPLEMENTATION

#include "Script.h"

const char *path = ".";
const char *libname = "LuaInline";

oapi::ScriptInterface::ScriptInterface (Orbiter *pOrbiter)
{
	orbiter = pOrbiter;
	hLib = NULL;
}

HINSTANCE oapi::ScriptInterface::LoadInterpreterLib ()
{
	hLib = orbiter->LoadModule (path, libname);
	return hLib;
}

INTERPRETERHANDLE oapi::ScriptInterface::NewInterpreter ()
{
	if (!hLib && !LoadInterpreterLib()) return 0;
	INTERPRETERHANDLE(*proc)() = (INTERPRETERHANDLE(*)())GetProcAddress (hLib, "opcNewInterpreter");
	if (proc) {
		INTERPRETERHANDLE hInterp = proc();
		return hInterp;
	}
	return 0;
}

int oapi::ScriptInterface::DelInterpreter (INTERPRETERHANDLE hInterp)
{
	if (!hLib && !LoadInterpreterLib()) return 0;
	int(*proc)(INTERPRETERHANDLE) = (int(*)(INTERPRETERHANDLE))GetProcAddress (hLib, "opcDelInterpreter");
	if (proc) return proc(hInterp);
	else return 3;
}

INTERPRETERHANDLE oapi::ScriptInterface::RunInterpreter (const char *cmd)
{
	if (!hLib && !LoadInterpreterLib()) return NULL;
	INTERPRETERHANDLE(*proc)(const char*) = (INTERPRETERHANDLE(*)(const char*))GetProcAddress (hLib, "opcRunInterpreter");
	INTERPRETERHANDLE hInterp = NULL;
	if (proc) hInterp = proc(cmd);
	return hInterp;
}

bool oapi::ScriptInterface::ExecScriptCmd (INTERPRETERHANDLE hInterp, const char *cmd)
{
	if (!hLib && !LoadInterpreterLib()) return false;
	bool(*proc)(INTERPRETERHANDLE,const char*) = (bool(*)(INTERPRETERHANDLE,const char*))GetProcAddress (hLib, "opcExecScriptCmd");
	if (proc) return proc(hInterp, cmd);
	else      return false;
}

bool oapi::ScriptInterface::AsyncScriptCmd (INTERPRETERHANDLE hInterp, const char *cmd)
{
	if (!hLib && !LoadInterpreterLib()) return false;
	bool(*proc)(INTERPRETERHANDLE,const char*) = (bool(*)(INTERPRETERHANDLE,const char*))GetProcAddress (hLib, "opcAsyncScriptCmd");
	if (proc) return proc(hInterp, cmd);
	else      return false;
}

lua_State *oapi::ScriptInterface::GetLua (INTERPRETERHANDLE hInterp)
{
	if (!hLib && !LoadInterpreterLib()) return NULL;
	lua_State*(*proc)(INTERPRETERHANDLE)=(lua_State*(*)(INTERPRETERHANDLE))GetProcAddress(hLib, "opcGetLua");
	if (proc) return proc(hInterp);
	else      return NULL;
}
