#include <gtest/gtest.h>
#include "storage/page.h"

TEST(PageTest, BasicReadWrite) {
    Page page;

    const char* msg = "hello world";
    page.Write(0, msg, 11);

    char buffer[20];
    page.Read(0, buffer, 11);

    buffer[11] = '\0';
    ASSERT_STREQ(buffer, msg);
}
