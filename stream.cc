// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include "stream.h"
#include "common.h"
#include <errno.h>
#include <iostream>

using namespace std;

static const int kDefaultBlockSize = 8192;

StringOutputStream::StringOutputStream(string* target)
		: target_(target) 
{
}

StringOutputStream::~StringOutputStream() 
{
}

bool StringOutputStream::Next(void** data, int* size) {
		int old_size = target_->size();

		// Grow the string.
		if (old_size < target_->capacity()) {
				// Resize the string to match its capacity, since we can get away
				// without a memory allocation this way.
				target_->resize(target_->capacity());
		} else {
				// Size has reached capacity, so double the size.  Also make sure
				// that the new size is at least kMinimumSize.
				target_->resize(max(old_size*2,kMinimumSize+ 0));
		}
		*data= (target_->empty() ? NULL : &*target_->begin())+ old_size;
		//*data = string_as_array(target_) + old_size;
		*size = target_->size() - old_size;
		return true;
}

void StringOutputStream::BackUp(int count) {
	//	GOOGLE_CHECK_GE(count, 0);
	//	GOOGLE_CHECK_LE(count, target_->size());
		target_->resize(target_->size() - count);
}

int StringOutputStream::ByteCount() const {
		return target_->size();
}

// ===================================================================

FileOutputStream::FileOutputStream(int file_descriptor, int block_size)
:		file_(file_descriptor),
		close_on_delete_(false),
		is_closed_(false),
		errno_(0),
		owns_copying_stream_(false),
		failed_(false),
		position_(0),
		buffer_size_(block_size > 0 ? block_size : kDefaultBlockSize),
		buffer_used_(0) {
		}

FileOutputStream::~FileOutputStream() {
		WriteBuffer();
		if (owns_copying_stream_) {
				if (close_on_delete_) {
					if (!Close()) {
						JEEP_ERROR_CHECK(0,"close() failed: ")
					}
			}
		}
}

int FileOutputStream::close_no_eintr(int fd) {
		int result;
		do {
				result = close(fd);
		} while (result < 0 && errno == EINTR);
		return result;
}

bool FileOutputStream::Close() {

		JEEP_ERROR_CHECK(!is_closed_,"close() failed: ")
		
		is_closed_ = true;
		if (close_no_eintr(file_) != 0) {
				// The docs on close() do not specify whether a file descriptor is still
				// open after close() fails with EIO.  However, the glibc source code
				// seems to indicate that it is not.
				errno_ = errno;
				return false;
		}

		return true;
}

bool FileOutputStream::Write(
				const void* buffer, int size) {
		//GOOGLE_CHECK(!is_closed_);
		JEEP_ERROR_CHECK(!is_closed_,"close() failed: ")
		int total_written = 0;

		const unsigned int* buffer_base = reinterpret_cast<const unsigned int*>(buffer);

		while (total_written < size) {
				int bytes;
				do {
						bytes = write(file_, buffer_base + total_written, size - total_written);
				} while (bytes < 0 && errno == EINTR);

				if (bytes <= 0) {
						// Write error.
						if (bytes < 0) {
								errno_ = errno;
						}
						return false;
				}
				total_written += bytes;
		}

		return true;
}



bool FileOutputStream::Flush() {
		return WriteBuffer();
}

bool FileOutputStream::Next(void** data, int* size) {
		if (buffer_used_ == buffer_size_) {
				if (!WriteBuffer()) return false;
		}

		AllocateBufferIfNeeded();

		*data = buffer_.get() + buffer_used_;
		*size = buffer_size_ - buffer_used_;
		buffer_used_ = buffer_size_;
		return true;
}

void FileOutputStream::BackUp(int count) {
		/*
		GOOGLE_CHECK_GE(count, 0);
		GOOGLE_CHECK_EQ(buffer_used_, buffer_size_)
				<< " BackUp() can only be called after Next().";
		GOOGLE_CHECK_LE(count, buffer_used_)
				<< " Can't back up over more bytes than were returned by the last call"
				" to Next().";
		*/
		JEEP_ERROR_CHECK(count!=0,"BackUp ")
		JEEP_ERROR_CHECK(buffer_used_== buffer_size_,"BackUp ")
		JEEP_ERROR_CHECK(count== buffer_used_,"BackUp ")
		buffer_used_ -= count;
}

int FileOutputStream::ByteCount() const {
		return position_ + buffer_used_;
}

bool FileOutputStream::WriteBuffer() {
		if (failed_) {
				// Already failed on a previous write.
				return false;
		}

		if (buffer_used_ == 0) return true;

		if (Write(buffer_.get(), buffer_used_)) {
				position_ += buffer_used_;
				buffer_used_ = 0;
				return true;
		} else {
				failed_ = true;
				FreeBuffer();
				return false;
		}
}

void FileOutputStream::AllocateBufferIfNeeded() {
		if (buffer_ == NULL) {
				buffer_.reset(new unsigned int[buffer_size_]);
		}
}

void FileOutputStream::FreeBuffer() {
		buffer_used_ = 0;
		buffer_.reset();
}
