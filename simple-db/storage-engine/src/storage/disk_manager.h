#pragma once

#include "storage/page.h"
#include <fstream>
#include <string>

/**
 * @brief Responsible for durable storage of fixed-size pages on disk.
 *
 * DiskManager abstracts the database file format. It provides:
 *
 *   - Binary read/write access to page-sized blocks (PAGE_SIZE bytes)
 *   - Automatic file growth when writing pages past the current end-of-file
 *   - Zero-filled pages when reading from uninitialized regions
 *   - Stable mapping between page_id -> file offset
 *
 * The DiskManager itself does NOT:
 *
 *   - Cache pages in memory (BufferPoolManager handles that)
 *   - Track free pages or allocation (that will be a higher-level component)
 *   - Perform any concurrency control or locking
 *
 * ## File Layout
 * Pages are stored sequentially in the database file:
 *
 *   offset = page_id * PAGE_SIZE
 *
 * Every page write is exactly PAGE_SIZE bytes. If a write occurs beyond the
 * current end-of-file, the file is automatically extended.
 *
 * ## Error Handling
 * Unwritten pages (i.e., holes beyond EOF) are returned as zero-filled pages.
 */
class DiskManager {
public:
  /**
   * @brief Open or create a database file.
   *
   * If the file does not exist, it is created. The file is opened in binary
   * read/write mode. No buffering is used — operations are performed directly
   * on the file stream.
   */
  DiskManager(const std::string &filename);

  /// @brief Close the underlying file stream.
  ~DiskManager();

  /**
   * @brief Write one page to disk.
   *
   * @param page_id The zero-based page index.
   * @param page    The Page containing data to write.
   *
   * Writes exactly PAGE_SIZE bytes. Writing to page_ids beyond EOF will
   * cause the on-disk file to expand.
   */
  void WritePage(uint32_t page_id, Page *page);

  /**
   * @brief Read a page from disk.
   *
   * @param page_id The zero-based page index.
   * @param page    Output buffer to store the read data.
   *
   * If the requested page lies beyond EOF, the output page is fully
   * zero-filled. If only part of the page can be read, the remainder
   * is zero-filled.
   */
  void ReadPage(uint32_t page_id, Page *page);

private:
  std::fstream db_file_; ///< File stream for the database file
};
