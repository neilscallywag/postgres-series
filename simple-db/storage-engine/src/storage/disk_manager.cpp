#include "storage/disk_manager.h"
#include <stdexcept>

DiskManager::DiskManager(const std::string &filename) {
  // open file for read/write, create if missing
  db_file_.open(filename,
                std::fstream::in | std::fstream::out | std::fstream::binary);

  if (!db_file_.is_open()) {
    // file doesn't exist -- create it
    db_file_.clear();
    db_file_.open(filename, std::fstream::out | std::fstream::binary);
    db_file_.close();
    db_file_.open(filename,
                  std::fstream::in | std::fstream::out | std::fstream::binary);
  }
}

DiskManager::~DiskManager() {
  if (db_file_.is_open()) {
    db_file_.close();
  }
}

void DiskManager::WritePage(uint32_t page_id, Page *page) {
  size_t offset = page_id * PAGE_SIZE;

  // Move write pointer
  db_file_.seekp(offset);

  // Write raw page bytes
  db_file_.write(reinterpret_cast<char *>(page), PAGE_SIZE);

  db_file_.flush();
}

void DiskManager::ReadPage(uint32_t page_id, Page *page) {
  size_t offset = page_id * PAGE_SIZE;

  db_file_.seekg(offset);

  db_file_.read(reinterpret_cast<char *>(page), PAGE_SIZE);

  std::streamsize bytes_read = db_file_.gcount();

  if (bytes_read < PAGE_SIZE) {
    // Zero-fill unread portion (or entire page)
    memset(page, 0, PAGE_SIZE);
  }
}
