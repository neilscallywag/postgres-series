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

TEST(DiskManagerTest, AutoExtendFileOnWrite) {
    const std::string db_file = "test2.db";

    if (std::filesystem::exists(db_file)) {
        std::filesystem::remove(db_file);
    }

    DiskManager dm(db_file);

    Page page;
    const char* msg = "extended";
    page.Write(0, msg, 8);

    // Write to page 5 (which doesn't exist yet)
    dm.WritePage(5, &page);

    // Now read it back
    Page loaded;
    dm.ReadPage(5, &loaded);

    char buffer[20];
    loaded.Read(0, buffer, 8);
    buffer[8] = '\0';

    ASSERT_STREQ(buffer, msg);
}

TEST(DiskManagerTest, ReadUninitializedPageReturnsZeroes) {
    const std::string db_file = "test3.db";

    if (std::filesystem::exists(db_file)) {
        std::filesystem::remove(db_file);
    }

    DiskManager dm(db_file);

    Page page;
    dm.ReadPage(2, &page); // never written

    uint8_t buffer[PAGE_SIZE];
    memset(buffer, 0, PAGE_SIZE);

    // Compare the entire page with zeros
    uint8_t temp[PAGE_SIZE];
    page.Read(0, temp, PAGE_SIZE);

    ASSERT_EQ(memcmp(buffer, temp, PAGE_SIZE), 0);
}

TEST(DiskManagerTest, MultiplePageReadWrite) {
    const std::string db_file = "test4.db";

    if (std::filesystem::exists(db_file)) {
        std::filesystem::remove(db_file);
    }

    DiskManager dm(db_file);

    // Create different patterns
    Page p1, p2, p3;
    p1.Write(0, "AAAA", 4);
    p2.Write(0, "BBBB", 4);
    p3.Write(0, "CCCC", 4);

    dm.WritePage(0, &p1);
    dm.WritePage(1, &p2);
    dm.WritePage(2, &p3);

    Page r1, r2, r3;
    dm.ReadPage(0, &r1);
    dm.ReadPage(1, &r2);
    dm.ReadPage(2, &r3);

    char b1[5], b2[5], b3[5];

    r1.Read(0, b1, 4); b1[4] = '\0';
    r2.Read(0, b2, 4); b2[4] = '\0';
    r3.Read(0, b3, 4); b3[4] = '\0';

    ASSERT_STREQ(b1, "AAAA");
    ASSERT_STREQ(b2, "BBBB");
    ASSERT_STREQ(b3, "CCCC");
}

