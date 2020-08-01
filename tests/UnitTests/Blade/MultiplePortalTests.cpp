#include <gtest/gtest.h>
#include <Utils/FieldOffset.h>
#include <bld_system.h>
#include "Surface.h"


TEST(MultiplePortalSurfaceTests, SizeOf)
{
    EXPECT_EQ(sizeof(B_MultiplePortalSurface), 0xB0);
}

TEST(MultiplePortalSurfaceTests, Fields)
{
    EXPECT_OFFSET_EQ(B_MultiplePortalSurface, normal, 0x004);
    EXPECT_OFFSET_EQ(B_MultiplePortalSurface, p, 0x01C);
    EXPECT_OFFSET_EQ(B_MultiplePortalSurface, unknown024, 0x024);
    EXPECT_OFFSET_EQ(B_MultiplePortalSurface, unknown028, 0x028);
}
