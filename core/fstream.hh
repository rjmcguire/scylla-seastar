/*
 * This file is open source software, licensed to you under the terms
 * of the Apache License, Version 2.0 (the "License").  See the NOTICE file
 * distributed with this work for additional information regarding copyright
 * ownership.  You may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (C) 2015 Cloudius Systems, Ltd.
 */

#pragma once

// File <-> streams adapters
//
// Seastar files are block-based due to the reliance on DMA - you must read
// on sector boundaries.  The adapters in this file provide a byte stream
// interface to files, while retaining the zero-copy characteristics of
// seastar files.

#include "file.hh"
#include "iostream.hh"
#include "shared_ptr.hh"


/// Data structure describing options for opening a file input stream
struct file_input_stream_options {
    uint64_t offset = 0;          ///< File offset at which to start reading
    size_t buffer_size = 8192;    ///< I/O buffer size
    unsigned read_ahead = 0;      ///< Number of extra read-ahead operations
};

// Create an input_stream for a given file, with the specified options.
// Multiple fibers of execution (continuations) may safely open
// multiple input streams concurrently for the same file.
input_stream<char> make_file_input_stream(
        file file,
        file_input_stream_options options = {});

// Create an input_stream for reading starting at a given position of the
// given file. Multiple fibers of execution (continuations) may safely open
// multiple input streams concurrently for the same file.
input_stream<char> make_file_input_stream(
        file file, uint64_t offset,
        uint64_t buffer_size = file_input_stream_options().buffer_size);

struct file_output_stream_options {
    unsigned buffer_size = 8192;
    unsigned preallocation_size = 1024*1024; // 1MB
    unsigned write_behind = 1; ///< Number of buffers to write in parallel
};

// Create an output_stream for writing starting at the position zero of a
// newly created file.
// NOTE: flush() should be the last thing to be called on a file output stream.
output_stream<char> make_file_output_stream(
        file file,
        uint64_t buffer_size = 8192);

/// Create an output_stream for writing starting at the position zero of a
/// newly created file.
/// NOTE: flush() should be the last thing to be called on a file output stream.
output_stream<char> make_file_output_stream(
        file file,
        file_output_stream_options options);

