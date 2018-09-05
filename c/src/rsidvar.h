// VariantKey
//
// rsidvar.h
//
// @category   Libraries
// @author     Nicola Asuni <nicola.asuni@genomicsplc.com>
// @copyright  2017-2018 GENOMICS plc
// @license    MIT (see LICENSE)
// @link       https://github.com/genomicsplc/variantkey
//
// LICENSE
//
// Copyright (c) 2017-2018 GENOMICS plc
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/**
 * @file rsidvar.h
 * @brief Functions to read VariantKey-rsID binary files.
 *
 * The functions provided here allows fast search for rsID and VariantKey values from binary files
 * made of adjacent constant-length binary blocks sorted in ascending order.
 *
 * rsvk.bin:
 * Lookup table to retrieve VariantKey from rsID.
 * This binary file can be generated by the `resources/tools/rsvk.sh' script from a TSV file.
 * This can also be in *Apache Arrow File* format with a single *RecordBatch*, or *Feather* format.
 * The first column must contain the rsID sorted in ascending order.
 *
 * vkrs.bin:
 * Lookup table to retrieve rsID from VariantKey.
 * This binary file can be generated by the `resources/tools/vkrs.sh' script from a TSV file.
 * This can also be in *Apache Arrow File* format with a single *RecordBatch*, or *Feather* format.
 * The first column must contain the VariantKey sorted in ascending order.
 */

#ifndef RSIDVAR_H
#define RSIDVAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "binsearch.h"
#include "variantkey.h"

/**
 * Struct containing the RSVK or VKRS memory mapped file column info.
 */
typedef struct rsidvar_cols_t
{
    const uint64_t *vk;  //!< Pointer to the VariantKey column.
    const uint32_t *rs;  //!< Pointer to the rsID column.
    uint64_t nrows;      //!< Number of rows.
} rsidvar_cols_t;

/**
 * Memory map the VKRS binary file.
 *
 * @param file  Path to the file to map.
 * @param mf    Structure containing the memory mapped file.
 * @param cvr   Structure containing the pointers to the VKRS memory mapped file columns.
 *
 * @return Returns the memory-mapped file descriptors.
 */
void mmap_vkrs_file(const char *file, mmfile_t *mf, rsidvar_cols_t *cvr);

/**
 * Memory map the RSVK binary file.
 *
 * @param file  Path to the file to map.
 * @param mf    Structure containing the memory mapped file.
 * @param crv   Structure containing the pointers to the RSVK memory mapped file columns.
 *
 * @return Returns the memory-mapped file descriptors.
 */
void mmap_rsvk_file(const char *file, mmfile_t *mf, rsidvar_cols_t *crv);

/**
 * Search for the specified rsID and returns the first occurrence of VariantKey in the RV file.
 *
 * @param crv       Structure containing the pointers to the RSVK memory mapped file columns (rsvk.bin).
 * @param first     Pointer to the first element of the range to search (min value = 0).
 *                  This will hold the position of the first record found.
 * @param last      Last element of the range to search (max value = nitems - 1).
 * @param rsid      rsID to search.
 *
 * @return VariantKey data or zero data if not found
 */
uint64_t find_rv_variantkey_by_rsid(rsidvar_cols_t crv, uint64_t *first, uint64_t last, uint32_t rsid);

/**
 * Get the next VariantKey for the specified rsID in the RV file.
 * This function should be used after find_rv_variantkey_by_rsid.
 * This function can be called in a loop to get all VariantKeys that are associated with the same rsID (if any).
 *
 * @param crv       Structure containing the pointers to the RSVK memory mapped file columns (rsvk.bin).
 * @param pos       Pointer to the current item. This will hold the position of the next record.
 * @param last      Last element of the range to search (max value = nitems - 1).
 * @param rsid      rsID to search.
 *
 * @return VariantKey data or zero data if not found
 */
uint64_t get_next_rv_variantkey_by_rsid(rsidvar_cols_t crv, uint64_t *pos, uint64_t last, uint32_t rsid);

/**
 * Search for the specified VariantKey and returns the first occurrence of rsID in the VR file.
 *
 * @param cvr       Structure containing the pointers to the VKRS memory mapped file columns (vkrs.bin).
 * @param first     Pointer to the first element of the range to search (min value = 0).
 *                  This will hold the position of the first record found.
 * @param last      Last element of the range to search (max value = nitems - 1).
 * @param vk        VariantKey.
 *
 * @return rsID or 0 if not found
 */
uint32_t find_vr_rsid_by_variantkey(rsidvar_cols_t cvr, uint64_t *first, uint64_t last, uint64_t vk);

/**
 * Search for the specified CHROM-POS range and returns the first occurrence of rsID in the VR file.
 *
 * @param cvr       Structure containing the pointers to the VKRS memory mapped file columns (vkrs.bin).
 * @param first     Pointer to the first element of the range to search (min value = 0).
 * @param last      Pointer to the last element of the range to search (max value = nitems - 1).
 * @param chrom     Chromosome encoded number.
 * @param pos_min   Start reference position, with the first base having position 0.
 * @param pos_max   End reference position, with the first base having position 0.
 *
 * @return rsID
 */
uint32_t find_vr_chrompos_range(rsidvar_cols_t cvr, uint64_t *first, uint64_t *last, uint8_t chrom, uint32_t pos_min, uint32_t pos_max);

#ifdef __cplusplus
}
#endif

#endif  // RSIDVAR_H
