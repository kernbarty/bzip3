/* bzip3 - A spiritual successor to BZip2.
 * Copyright (C) 2022 Kamila Szewczyk
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BZIP3_H
#define BZIP3_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Minimum and maximum block sizes supported by bzip3. */
#define BZ3_MIN_BLOCK_SIZE (65 * 1024)
#define BZ3_MAX_BLOCK_SIZE (511 * 1024 * 1024)

/* A sensible default block size for general-purpose use (8 MiB).
 * Balances compression ratio and memory usage reasonably well for
 * most files I work with. Increase for large, highly compressible data. */
#define BZ3_DEFAULT_BLOCK_SIZE (8 * 1024 * 1024)

/* Error codes returned by bzip3 functions. */
#define BZ3_OK             0
#define BZ3_ERR_OUT_OF_BOUNDS -1
#define BZ3_ERR_BWT        -2
#define BZ3_ERR_CRC        -3
#define BZ3_ERR_MALFORMED  -4
#define BZ3_ERR_TRUNCATED  -5
#define BZ3_ERR_DATA       -6
#define BZ3_ERR_INIT       -7

/**
 * Opaque state structure for bzip3 stream operations.
 * Allocate with bz3_new() and free with bz3_free().
 */
struct bz3_state;

/**
 * @brief Allocate and initialise a new bzip3 state.
 *
 * @param block_size  Compression block size in bytes. Must be between
 *                    BZ3_MIN_BLOCK_SIZE and BZ3_MAX_BLOCK_SIZE.
 *                    Use BZ3_DEFAULT_BLOCK_SIZE if unsure.
 * @return Pointer to the newly allocated state, or NULL on failure.
 */
struct bz3_state * bz3_new(int32_t block_size);

/**
 * @brief Free a bzip3 state previously allocated with bz3_new().
 *
 * @param state  State to free. May be NULL (no-op).
 */
void bz3_free(struct bz3_state * state);

/**
 * @brief Retrieve the last error code stored in the state.
 *
 * @param state  bzip3 state.
 * @return One of the BZ3_ERR_* constants, or BZ3_OK if no error.
 */
int8_t bz3_last_error(struct bz3_state * state);

/**
 * @brief Convert an error code to a human-readable string.
 *
 * @param err  Error code (one of the BZ3_ERR_* constants).
 * @return Pointer to a static string describing the error.
 */
const char * bz3_strerror(int8_t err);

/**
 * @brief Return the recommended size for the working buffer.
 *
 * The working buffer passed to bz3_compress_block() / bz3_decompress_block()
 * must be at least this many bytes.
 *
 * @param block_size  The block size used when creating the state.
 * @return Required buffer size in bytes.
 */
size_t bz3_bound(size_t block_size);

/**
 * @brief Compress a single block of data.
 *
 * @param state       Initialised bzip3 state.
 * @param buffer      Working buffer of at least bz3_bound(block_size) bytes.
 *                    On entry the first `in_size` bytes contain the input.
 *                    On success the comp
