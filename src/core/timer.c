/*
  Copyright (c) 2006-2008 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2006-2008 Center for Bioinformatics, University of Hamburg

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

#include <sys/time.h>
#include "core/ma.h"
#include "core/timer.h"
#include "core/unused_api.h"
#include "core/xposix.h"

typedef enum {
  TIMER_RUNNING,
  TIMER_STOPPED
} Timerstate;

struct GtTimer {
  struct timeval gstart_tv,
                 start_tv,
                 stop_tv;
  struct rusage  gstart_ru,
                 start_ru,
                 stop_ru;
  Timerstate state;
  const char *statedesc;
  bool has_desc;
};

GtTimer* gt_timer_new(void)
{
  GtTimer *t = gt_malloc(sizeof *t);
  t->state = TIMER_RUNNING;
  t->statedesc = NULL;
  t->has_desc = false;
  return t;
}

GtTimer* gt_timer_new_with_progress_description(const char* desc)
{
  GtTimer *t = gt_timer_new();
  t->statedesc = desc;
  t->has_desc = true;
  return t;
}

void gt_timer_start(GtTimer *t)
{
  gt_assert(t);
  gettimeofday(&t->gstart_tv, NULL);
  gettimeofday(&t->start_tv, NULL);
  gt_xgetrusage(RUSAGE_SELF, &t->start_ru);
  t->state = TIMER_RUNNING;
}

void gt_timer_stop(GtTimer *t)
{
  gt_assert(t);
  if (t->state == TIMER_RUNNING) {
    gettimeofday(&t->stop_tv, NULL);
    gt_xgetrusage(RUSAGE_SELF, &t->stop_ru);
    t->state = TIMER_STOPPED;
  }
}

static int timeval_subtract(struct timeval *result,
                            struct timeval *x,
                            struct timeval *y)
{
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;
  return x->tv_sec < y->tv_sec;
}

GT_UNUSED
static int timeval_add(struct timeval *result,
                       const struct timeval *x,
                       const struct timeval *y)
{
  result->tv_sec = x->tv_sec + y->tv_sec;
  result->tv_usec = x->tv_usec + y->tv_usec;
  while (result->tv_usec > 1000000) {
    result->tv_usec -= 1000000;
    result->tv_sec++;
  }
  return 0;
}

void gt_timer_show_formatted(GtTimer *t, const char *fmt, FILE *fp)
{
  struct timeval elapsed_tv;
  if (t->state == TIMER_RUNNING)
    gt_timer_stop(t);
  gt_assert(t->state == TIMER_STOPPED);
  timeval_subtract(&elapsed_tv, &t->stop_tv, &t->gstart_tv);
  fprintf(fp, fmt,
          (long)(elapsed_tv.tv_sec),
          (long)(elapsed_tv.tv_usec),
          (long)(t->stop_ru.ru_utime.tv_sec - t->stop_ru.ru_utime.tv_sec),
          (long)(t->stop_ru.ru_stime.tv_sec - t->stop_ru.ru_stime.tv_sec));
}

void gt_timer_show(GtTimer *t, FILE *fp)
{
  gt_timer_show_formatted(t, "%ld.%06lds real %lds user %lds system\n", fp);
}

void gt_timer_show_progress(GtTimer *t, const char *desc, FILE *fp)
{
  struct timeval elapsed_tv;
  gettimeofday(&t->stop_tv, NULL);
  timeval_subtract(&elapsed_tv, &t->stop_tv, &t->start_tv);
  fprintf(fp,"# TIME %s %ld.%02ld\n",
          t->statedesc,
          (long)(elapsed_tv.tv_sec),
          (long)(elapsed_tv.tv_usec));
  t->statedesc = desc;
  gettimeofday(&t->start_tv, NULL);
}

void gt_timer_show_progress_final(GtTimer *t, FILE *fp)
{
  struct timeval elapsed_tv;
  gt_timer_stop(t);
  timeval_subtract(&elapsed_tv, &t->stop_tv, &t->gstart_tv);
  fprintf(fp,"# TIME overall %ld.%02ld\n",
          (long)(elapsed_tv.tv_sec),
          (long)(elapsed_tv.tv_usec));
}

void gt_timer_delete(GtTimer *t)
{
  if (!t) return;
  gt_free(t);
}
