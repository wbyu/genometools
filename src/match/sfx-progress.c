/*
  Copyright (c) 2007 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdio.h>
#include <time.h>
#include "core/ma_api.h"
#include "sfx-progress.h"

struct Sfxprogress
{
  bool showprogressbar;
  clock_t startclock, overalltime;
  const char *eventdescription;
};

Sfxprogress *sfxprogress_new(const char *event)
{
  Sfxprogress *sfxprogress;

  sfxprogress = gt_malloc(sizeof (Sfxprogress));
  if (event == NULL)
  {
    sfxprogress->showprogressbar = true;
    sfxprogress->startclock = sfxprogress->overalltime = 0;
    sfxprogress->eventdescription = NULL;
  } else
  {
    sfxprogress->showprogressbar = false;
    sfxprogress->startclock = clock();
    sfxprogress->overalltime = 0;
    sfxprogress->eventdescription = event;
  }
  return sfxprogress;
}

void sfxprogress_deliverthetime(FILE *fp,Sfxprogress *sfxprogress,
                                const char *newevent)
{
  if (sfxprogress->showprogressbar)
  {
    return;
  } else
  {
    clock_t stopclock;

    stopclock = clock();
    fprintf(fp,"# TIME %s %.2f\n",sfxprogress->eventdescription,
             (double) (stopclock-sfxprogress->startclock)/
             (double) CLOCKS_PER_SEC);
    (void) fflush(fp);
    sfxprogress->overalltime += (stopclock - sfxprogress->startclock);
    if (newevent == NULL)
    {
      fprintf(fp,"# TIME overall %.2f\n",
                  (double) sfxprogress->overalltime/(double) CLOCKS_PER_SEC);
      (void) fflush(fp);
    } else
    {
      sfxprogress->startclock = stopclock;
      sfxprogress->eventdescription = newevent;
    }
  }
}

bool sfxprogress_withbar(const Sfxprogress *sfxprogress)
{
  return sfxprogress->showprogressbar;
}
