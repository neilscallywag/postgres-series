#include <gtest/gtest.h>
#include "storage/disk_manager.h"
#include "storage/page.h"
#include <filesystem>

TEST(DiskManagerTest, WriteAndReadPage) {
    const std::string db_file = "test.db";

    // Cleanup from previous runs
    if (std::filesystem::exists(db_file)) {
        std::filesystem::remove(db_file);
    }

    DiskManager dm(db_file);

    // Create a page and write data into it
    Page write_page;
    const char* msg = "hello disk";
    write_page.Write(0, msg, 10);

    dm.WritePage(0, &write_page);

    // Load the page back
    Page read_page;
    dm.ReadPage(0, &read_page);

    char buffer[20];
    read_page.Read(0, buffer, 10);
    buffer[10] = '\0';

    ASSERT_STREQ(buffer, msg);
}
