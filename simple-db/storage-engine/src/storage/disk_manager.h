#pragma once

#include <string>
#include <fstream>
#include "storage/page.h"

/**
 * @brief Manages on-disk storage of database pages.
 *
 * DiskManager is responsible for:
 *   - Creating/opening a database file
 *   - Writing fixed-size pages (PAGE_SIZE bytes)
 *   - Reading pages from file
 *   - Ensuring the file grows as new pages are written
 *
 * DiskManager does NOT cache pages—that is handled by the BufferPoolManager.
 * DiskManager simply provides durable, page-based I/O.
 */
class DiskManager {
public:

    /**
     * @brief Construct a DiskManager for a given database file.
     *
     * If the file does not exist, it will be created. The file is opened in
     * binary read/write mode.
     *
     * @param filename The path to the database file.
     */
    DiskManager(const std::string& filename);

    /**
     * @brief Destructor: closes the underlying file stream.
     */
    ~DiskManager();

    /**
     * @brief Write a page to disk at the given page ID.
     *
     * @param page_id  The zero-based page index.
     * @param page     Pointer to the in-memory Page to write.
     *
     * The file will be automatically grown if writing past the current end.
     */
    void WritePage(uint32_t page_id, Page* page);

    /**
     * @brief Read a page from disk into the given Page object.
     *
     * @param page_id  The zero-based page index.
     * @param page     Pointer to the destination Page object.
     *
     * If reading past EOF, the output page is zero-filled.
     */
    void ReadPage(uint32_t page_id, Page* page);

private:
    std::fstream db_file_; ///< Underlying file stream for the database file
};
