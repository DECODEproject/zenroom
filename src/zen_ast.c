/*  Zenroom (DECODE project)
 *
 *  (c) Copyright 2017-2018 Dyne.org foundation
 *  designed, written and maintained by Denis Roio <jaromil@dyne.org>
 *
 * This source code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Public License as published
 * by the Free Software Foundation; either version 3 of the License,
 * or (at your option) any later version.
 *
 * This source code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * Please refer to the GNU Public License for more details.
 *
 * You should have received a copy of the GNU Public License along with
 * this source code; if not, write to:
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <jutils.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <lua_functions.h>

#include <zenroom.h>
#include <zen_memory.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// from lualibs_detected (generated by make embed-lua)
extern zen_extension_t zen_extensions[];


// prototypes from lua_modules.c
extern int zen_exec_extension(lua_State *L, zen_extension_t *p);
extern int zen_require_override(lua_State *L, const int restricted);

extern int lua_cjson_safe_new(lua_State *l);

extern void zen_add_io(lua_State *L);

// prototypes from zen_memory.c
extern zen_mem_t *libc_memory_init();
extern void *zen_memory_manager(void *ud, void *ptr, size_t osize, size_t nsize);

// prototypes from lua_functions.c
extern void zen_setenv(lua_State *L, char *key, char *val);

// prototype from lpeglabel/lptree.c
int luaopen_lpeglabel (lua_State *L);

zenroom_t *ast_init(char *script) {
	lua_State *L = NULL;
	zen_mem_t *mem = NULL;
	mem = libc_memory_init();
	L = lua_newstate(zen_memory_manager, mem);
	if(!L) {
		error(L,"%s: %s", __func__, "lua state creation failed");
		return NULL;
	}
	// create the zenroom_t global context
	zenroom_t *Z = system_alloc(sizeof(zenroom_t));
	Z->lua = L;
	Z->mem = mem;
	Z->stdout_buf = NULL;
	Z->stdout_pos = 0;
	Z->stdout_len = 0;
	Z->stderr_buf = NULL;
	Z->stderr_pos = 0;
	Z->stderr_len = 0;
	Z->userdata = NULL;
	//Set zenroom context as a global in lua
	//this will be freed on lua_close
	lua_pushlightuserdata(L, Z);
	lua_setglobal(L, "_Z");
	// open all standard lua libraries
	luaL_openlibs(L);
	// open lpeglabel

	luaL_requiref(L, "lpeg", luaopen_lpeglabel, 1);
	luaL_requiref(L, "json", lua_cjson_safe_new, 1);

	// load our own openlibs and extensions
	zen_add_io(L);
	// second arg is restriction
	zen_require_override(L, 0);
	// save script CODE in Lua context
	zen_setenv(L,"CODE",(char*)script);

	return(Z);
}

int ast_parse(zenroom_t *Z) {
	zen_extension_t *p;
	for (p = zen_extensions;
	     p->name != NULL; ++p) {
		if (strcasecmp(p->name, "ast") == 0)
			return zen_exec_extension(Z->lua,p);
	}
	return 0;
}

void ast_teardown(zenroom_t *Z) {
	void *mem = Z->mem;
	if(Z->lua) lua_close((lua_State*)Z->lua);
	if(mem) system_free(mem);
	system_free(Z);
}

// implementation exposed as public function
int zenroom_parse_ast(char *script, int verbosity,
                      char *stdout_buf, size_t stdout_len,
                      char *stderr_buf, size_t stderr_len) {
	zenroom_t *Z = ast_init(script);
	if(!Z) {
		error(NULL, "%s: initialisation failed.", __func__);
		return 1; }
	lua_State *L = Z->lua;
	if(!L) {
		error(L, "%s: initialisation failed.", __func__);
		return 1; }

	int return_code = 1; // return error by default
	if(!script) {
		error(L, "NULL string as script for zenroom_exec()");
		exit(1); }
	set_debug(verbosity);

	// setup stdout and stderr buffers
	Z->stdout_buf = stdout_buf;
	Z->stdout_len = stdout_len;
	Z->stderr_buf = stderr_buf;
	Z->stderr_len = stderr_len;

	int r;
	notice(L,"Parsing AST of script");
	r = ast_parse(Z);

	if(r) {
#ifdef __EMSCRIPTEN__
		EM_ASM({Module.exec_error();});
#endif
		//		error(r);
		error(L, "Error detected. Parsing aborted.");

		ast_teardown(Z);
		return(1);
	}
	return_code = 0; // return success

#ifdef __EMSCRIPTEN__
	EM_ASM({Module.exec_ok();});
#endif

	act(L, "AST parser completed.");
	ast_teardown(Z);
	return(return_code);
}
