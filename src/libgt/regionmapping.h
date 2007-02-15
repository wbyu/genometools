/*
  Copyright (c) 2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#ifndef REGIONMAPPING_H
#define REGIONMAPPING_H

#include "str.h"

/* maps a sequence-region to a sequence file */
typedef struct RegionMapping RegionMapping;

RegionMapping* regionmapping_new(Str *mapping_filename, Error*);
/* returns a new reference */
Str*           regionmapping_map(RegionMapping*, const char *sequence_region,
                                 Error*);
void           regionmapping_free(RegionMapping*);

#endif
