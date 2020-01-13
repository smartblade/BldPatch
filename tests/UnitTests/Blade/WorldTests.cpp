#include <gtest/gtest.h>
#include <Utils/FieldOffset.h>
#include <bld_system.h>
#include "BWorld.h"


TEST(WorldTests, Fields)
{
    EXPECT_OFFSET_EQ(B_World, world, 0x0018);
    EXPECT_OFFSET_EQ(B_World, nextEntitySuffix, 0x1824);
    EXPECT_OFFSET_EQ(B_World, unknown1828, 0x1828);
    EXPECT_OFFSET_EQ(B_World, world_file_name, 0x185C);
    EXPECT_OFFSET_EQ(B_World, unknown187C, 0x187C);
    EXPECT_OFFSET_EQ(B_World, unknown1880, 0x1880);
    EXPECT_OFFSET_EQ(B_World, atmospheres, 0x18AC);
    EXPECT_OFFSET_EQ(B_World, sectors, 0x18C4);
    EXPECT_OFFSET_EQ(B_World, unknown18F8, 0x18F8);
    EXPECT_OFFSET_EQ(B_World, unknown18FC, 0x18FC);
    EXPECT_OFFSET_EQ(B_World, unknown1914, 0x1914);
    EXPECT_OFFSET_EQ(B_World, unknown191C, 0x191C);
    EXPECT_OFFSET_EQ(B_World, lights, 0x1920);
    EXPECT_OFFSET_EQ(B_World, initial_point_position, 0x1954);
    EXPECT_OFFSET_EQ(B_World, initial_point_orientation, 0x196C);
}