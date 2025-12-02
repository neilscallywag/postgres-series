#pragma once
#include <cstdint>
#include <cstring>

/**
 * @brief The fixed size (in bytes) of a database page.
 *
 * Most database systems use 4 KB pages. This constant defines
 * the size of the in-memory page buffer. Every page written or read
 * through DiskManager will always be exactly PAGE_SIZE bytes.
 */
static const uint32_t PAGE_SIZE = 4096; // 4 KB

/**
 * @brief Represents a raw, fixed-size page of memory used by the storage
 * engine.
 *
 * This is the lowest-level storage abstraction in the database. A Page stores
 * exactly PAGE_SIZE bytes and provides raw byte-level access with no notion of:
 *
 *  - tuples/records
 *  - free space
 *  - slots
 *  - schema
 *  - variable-length fields
 *
 * Higher-level page formats (such as slotted pages) will build on top of
 * this container. The Page class is responsible only for:
 *
 *  - Zero-initializing new pages
 *  - Providing safe byte-level read/write operations
 *  - Serving as the in-memory representation of on-disk pages
 *
 * The DiskManager serializes/deserializes Page objects to/from the database
 * file.
 */
class Page {
public:
  /**
   * @brief Construct a new Page and initialize all bytes to zero.
   *
   * Newly created pages should always be zeroed to avoid undefined behavior
   * and to provide deterministic behavior before any data is written.
   */
  Page() { memset(data_, 0, PAGE_SIZE); }

  /**
   * @brief Write raw bytes into the page at a given offset.
   *
   * @param offset   The byte offset inside the page to begin writing.
   * @param src      Pointer to the memory buffer to copy from.
   * @param size     Number of bytes to write.
   *
   * This is a low-level operation used by higher abstractions.
   * No bounds checking is performed here for performance reasons.
   *
   * This method performs a memcpy:
   *   memcpy(data_ + offset, src, size);
   */
  void Write(uint32_t offset, const void *src, uint32_t size) {
    memcpy(data_ + offset, src, size);
  }

  /**
   * @brief Read raw bytes from the page into the provided buffer.
   *
   * @param offset   The byte offset in the page to begin reading.
   * @param dest     Pointer to the output buffer to copy into.
   * @param size     Number of bytes to read.
   *
   * This method performs:
   *   memcpy(dest, data_ + offset, size);
   */
  void Read(uint32_t offset, void *dest, uint32_t size) const {
    memcpy(dest, data_ + offset, size);
  }

private:
  /**
   * @brief Fixed-size byte array storing the page content.
   *
   * This array is embedded directly inside the Page object
   * to avoid heap allocation and maximize cache locality.
   */
  uint8_t data_[PAGE_SIZE];
};
