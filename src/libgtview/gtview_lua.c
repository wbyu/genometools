/*
  Copyright (c) 2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#include <assert.h>
#include "libgtview/diagram_lua.h"
#include "libgtview/feature_index_lua.h"
#include "libgtview/feature_stream_lua.h"
#include "libgtview/feature_visitor_lua.h"
#include "libgtview/gtview_lua.h"
#include "libgtview/render_lua.h"

int luaopen_gtview(lua_State *L)
{
  assert(L);
  luaopen_diagram(L);
  luaopen_feature_index(L);
  luaopen_feature_stream(L);
  luaopen_feature_visitor(L);
  luaopen_render(L);
  return 1;
}
