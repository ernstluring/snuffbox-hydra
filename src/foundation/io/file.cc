#include "foundation/io/file.h"
#include "foundation/memory/memory.h"
#include "foundation/auxiliary/pointer_math.h"

namespace snuffbox
{
  namespace foundation
  {
    //--------------------------------------------------------------------------
    File::File() :
      is_ok_(false),
      buffer_(nullptr),
      length_(0)
    {

    }

    //--------------------------------------------------------------------------
    File::File(const Path& path, FileOpenMode mode) :
      is_ok_(false),
      buffer_(nullptr),
      length_(0)
    {
      stream_ = std::fstream(
        path.ToString().c_str(),
        FileFlagsToOpenMode(static_cast<FileFlags>(mode)));

      is_ok_ = stream_.is_open();

      if (is_ok_ == false)
      {
        return;
      }

      length_ = stream_.tellg();
      stream_.seekg(std::ios_base::beg);
    }

    //--------------------------------------------------------------------------
    uint8_t* File::ReadBuffer(size_t* length, bool is_string)
    {
      if (length == nullptr)
      {
        return nullptr;
      }

      if (is_ok_ == false)
      {
        *length = 0;
        return nullptr;
      }

      size_t len = length_ + (is_string == true ? 1 : 0);

      if (buffer_ == nullptr)
      {
        buffer_ = reinterpret_cast<uint8_t*>(Memory::Allocate(len));

        stream_.seekg(std::ios_base::beg);
        stream_.read(reinterpret_cast<char*>(buffer_), length_);
        
        if (is_string == true)
        {
          void* block = PointerMath::Offset(buffer_, length_);
          memset(block, '\0', sizeof(uint8_t));
        }
      }

      *length = len;
      return buffer_;
    }

    //--------------------------------------------------------------------------
    bool File::is_ok() const
    {
      return is_ok_;
    }

    //--------------------------------------------------------------------------
    size_t File::length() const
    {
      return length_;
    }

    //--------------------------------------------------------------------------
    File::~File()
    {
      if (is_ok_ == false)
      {
        return;
      }

      if (buffer_ != nullptr)
      {
        Memory::Deallocate(buffer_);
        buffer_ = nullptr;
      }

      stream_.close();
      is_ok_ = false;
    }

    //--------------------------------------------------------------------------
    std::ios::openmode File::FileFlagsToOpenMode(FileFlags flags)
    {
      std::ios::openmode mode = 0;

      if ((flags & FileFlags::kRead) == FileFlags::kRead)
      {
        mode |= std::ios::in;
      }

      if ((flags & FileFlags::kWrite) == FileFlags::kWrite)
      {
        mode |= std::ios::out;
      }

      mode |= std::ios::binary | std::ios::ate;

      return mode;
    }
  }
}