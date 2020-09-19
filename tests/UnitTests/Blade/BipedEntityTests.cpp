#include <gtest/gtest.h>
#include <Utils/FieldOffset.h>
#include <bld_system.h>
#include <Entities/BipedEntity.h>


TEST(BipedEntityTests, SizeOf)
{
    EXPECT_EQ(sizeof(B_BipedEntity), 0x1224);
}

TEST(BipedEntityTests, Fields)
{
    EXPECT_OFFSET_EQ(B_BipedEntity, unknown0370, 0x0370);
}
