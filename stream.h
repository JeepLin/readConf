// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/

#ifndef _ZERO_COPY_STREAM_IMPL_LITE_H__
#define _ZERO_COPY_STREAM_IMPL_LITE_H__

#include <string>
#include <iosfwd>
#include "common.h"

using namespace std;

class ZeroCopyOutputStream {
public:
	inline ZeroCopyOutputStream() {}
	virtual ~ZeroCopyOutputStream(){};
	virtual bool Next(void** data, int* size) = 0;
	virtual void BackUp(int count) = 0;

	virtual int ByteCount() const = 0;
//private:
//	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ZeroCopyOutputStream);
};

class StringOutputStream : public ZeroCopyOutputStream
{
		public:
				StringOutputStream(string* target);
				~StringOutputStream();

				bool Next(void** data, int* size);
				void BackUp(int count);
				int ByteCount() const;
		private:
				static const int kMinimumSize = 16;
				string* target_;
};

class FileOutputStream : public ZeroCopyOutputStream {
public:
	// Creates a stream that writes to the given Unix file descriptor.
	// If a block_size is given, it specifies the size of the buffers
	// that should be returned by Next().  Otherwise, a reasonable default
	// is used.
	FileOutputStream(int file_descriptor,
								int block_size = -1);
	~FileOutputStream();

	// Writes all pending data to the underlying stream.  Returns false if a
	// write error occurred on the underlying stream.  (The underlying
	// stream itself is not necessarily flushed.)
	bool Flush();

	// Call SetOwnsCopyingStream(true) to tell the FileOutputStream to
	// delete the underlying CopyingOutputStream when it is destroyed.
	void SetOwnsCopyingStream(bool value) { owns_copying_stream_ = value; }

	// implements ZeroCopyOutputStream ---------------------------------
	bool Next(void** data, int* size);
	void BackUp(int count);
	int ByteCount() const;

private:
	// Write the current buffer, if it is present.
	bool WriteBuffer();
	// Insures that buffer_ is not NULL.
	void AllocateBufferIfNeeded();
	// Frees the buffer.
	void FreeBuffer();

	// The underlying copying stream.
	//CopyingOutputStream* copying_stream_;
	//CopyingFileOutputStream *copying_stream_;
	bool owns_copying_stream_;
	// True if we have seen a permenant error from the underlying stream.
	bool failed_;

	// The current position of copying_stream_, relative to the point where
	// we started writing.
	int position_;

	// Data is written from this buffer.  It may be NULL if no buffer is
	// currently in use.  Otherwise, it points to an array of size buffer_size_.
	scoped_array<unsigned int> buffer_;
	const int buffer_size_;

	// Number of valid bytes currently in the buffer (i.e. the size last
	// returned by Next()).  When BackUp() is called, we just reduce this.
	// 0 <= buffer_used_ <= buffer_size_.
	int buffer_used_;

	//CopyingFileOutputStream *copying_stream_;
	const int file_;
	bool close_on_delete_;
	bool is_closed_;
	// The errno of the I/O error, if one has occurred.  Otherwise, zero.
	int errno_;

	bool Close();
	void SetCloseOnDelete(bool value) { close_on_delete_ = value; }
	int GetErrno() { return errno_; }

	// implements CopyingOutputStream --------------------------------
	bool Write(const void* buffer, int size);

	int close_no_eintr(int fd);

};


#endif  // GOOGLE_PROTOBUF_IO_ZERO_COPY_STREAM_IMPL_LITE_H__
