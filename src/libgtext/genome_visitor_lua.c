/*
  Copyright (c) 2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#include "lauxlib.h"
#include "gtlua.h"
#include "libgtext/gff3_visitor.h"
#include "libgtext/genome_visitor_lua.h"

static int gff3_visitor_lua_new(lua_State *L)
{
  GenomeVisitor **gv;
  Env *env = get_env_from_registry(L);
  assert(L);
  /* construct object */
  gv = lua_newuserdata(L, sizeof (GenomeVisitor**));
  *gv = gff3_visitor_new(NULL, env);
  assert(*gv);
  luaL_getmetatable(L, GENOME_VISITOR_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

static int genome_visitor_lua_delete(lua_State *L)
{
  GenomeVisitor **gv = check_genome_visitor(L, 1);
  Env *env;
  env = get_env_from_registry(L);
  genome_visitor_delete(*gv, env);
  return 0;
}

static const struct luaL_Reg genome_visitor_lib_f [] = {
  { "gff3_visitor", gff3_visitor_lua_new },
  { NULL, NULL }
};

int luaopen_genome_visitor(lua_State *L)
{
  assert(L);
  luaL_newmetatable(L, GENOME_VISITOR_METATABLE);
  /* metatable.__index = metatable */
  lua_pushvalue(L, -1); /* duplicate the metatable */
  lua_setfield(L, -2, "__index");
  /* set its _gc field */
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, genome_visitor_lua_delete);
  lua_settable(L, -3);
  /* register functions */
  luaL_register(L, "gt", genome_visitor_lib_f);
  return 1;
}
