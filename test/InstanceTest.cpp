#include "gtest/gtest.h"
#include <Instance.hpp>
#include <Individual.hpp>
#include <Graph.hpp>

using namespace eaframework;

namespace {

TEST(Instance, ReadsStartAssignment) {
    auto assignment = read_start_assignment("data/test.assignment");

    Individual expected;
    expected.bit_vector = std::vector<Bit>{0, 0, 1, 1};

    ASSERT_EQ(assignment->bit_vector.size(), 4);
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(assignment->bit_vector[i], expected.bit_vector[i]);
    }
}

TEST(Instance, ReadsEmptyAssignment) {
    auto assignment = read_start_assignment("data/InstanceTest/empty.assignment");

    Individual expected;
    expected.bit_vector = std::vector<Bit>{0, 0, 0, 0};

    ASSERT_EQ(assignment->bit_vector.size(), 4);
}

TEST(Instance, ReadsInstanceWithStartAssignment) {
    auto instance = read_instance("data/test.txt", true);

    ASSERT_EQ(instance.graph->node_count(), 4);

    ASSERT_EQ(instance.start_assignment->bit_vector.size(), 4);
    EXPECT_EQ(instance.start_assignment->bit_vector[2], BIT_ONE);
    EXPECT_EQ(instance.start_assignment->bit_vector[3], BIT_ONE);
}

TEST(Instance, ReadsInstanceWithoutStartAssignment) {
    auto instance = read_instance("data/test.txt", false);

    ASSERT_EQ(instance.graph->node_count(), 4);

    ASSERT_EQ(instance.start_assignment->bit_vector.size(), 4);
    EXPECT_EQ(instance.start_assignment->bit_vector[2], BIT_ZERO);
    EXPECT_EQ(instance.start_assignment->bit_vector[3], BIT_ZERO);
}


}
