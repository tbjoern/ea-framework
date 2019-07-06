#include "gtest/gtest.h"
#include <Instance.hpp>
#include <Individual.hpp>

using namespace eaframework;

namespace {

TEST(InstanceTest, ReadsStartAssignmentFromFile) {
    auto assignment = read_start_assignment("data/test.assignment");

    Individual expected;
    expected.bit_vector = std::vector<unsigned short>{0, 0, 1, 1};

    ASSERT_EQ(assignment->bit_vector.size(), 4);
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(assignment->bit_vector[i], expected.bit_vector[i]);
    }
}

}
