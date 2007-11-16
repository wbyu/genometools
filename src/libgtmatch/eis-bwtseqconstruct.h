/*
  Copyright (c) 2007 Thomas Jahns <Thomas.Jahns@gmx.net>

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
#ifndef EIS_BWTSEQCONSTRUCT_H
#define EIS_BWTSEQCONSTRUCT_H

/**
 * \file eis-bwtseqconstruct.h
 * Interface definitions for construction of an indexed representation of the
 * BWT of a sequence as presented by Manzini and Ferragina (Compressed
 * Representations of Sequences and Full-Text Indexes, 2006)
 * \author Thomas Jahns <Thomas.Jahns@gmx.net>
 */

#include "libgtmatch/sarr-def.h"
#include "libgtmatch/eis-suffixerator-interface.h"

/**
 * \brief Loads (or creates if necessary) an encoded indexed sequence
 * object of the BWT transform.
 * @param params a struct holding parameter information for index construction
 * @param sa Suffixarray data structure to build BWT index from
 * @param env genometools reference for core functions
 * @return reference to new BWT sequence object
 */
extern BWTSeq *
availBWTSeqFromSA(const struct bwtParam *params, Suffixarray *sa,
                  Seqpos totalLen, Env *env);

/**
 * \brief Loads an encoded indexed sequence object of the
 * BWT transform.
 * @param params a struct holding parameter information for index construction
 * @param sa Suffixarray data structure to build BWT index from
 * @param env genometools reference for core functions
 * @return reference to new BWT sequence object
 */
extern BWTSeq *
loadBWTSeqForSA(const struct bwtParam *params, Suffixarray *sa,
                Seqpos totalLen, Env *env);

/**
 * \brief Creates or loads an encoded indexed sequence object of the
 * BWT transform.
 * @param params a struct holding parameter information for index construction
 * @param si Suffixerator interface to read data for BWT index from
 * @param projectName base file name for index written (should be the
 * same as the one sa was read from
 * @param env genometools reference for core functions
 * @return reference to new BWT sequence object
 */
extern BWTSeq *
createBWTSeqFromSfxI(const struct bwtParam *params, sfxInterface *si,
                     Seqpos totalLen, Env *env);

extern BWTSeq *
createBWTSeqFromSA(const struct bwtParam *params, Suffixarray *sa,
                   Seqpos totalLen, Env *env);

#endif
