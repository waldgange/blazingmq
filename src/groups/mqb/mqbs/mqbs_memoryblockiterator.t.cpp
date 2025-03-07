// Copyright 2015-2023 Bloomberg Finance L.P.
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// mqbs_memoryblockiterator.t.cpp                                     -*-C++-*-
#include <mqbs_memoryblockiterator.h>

// MQB
#include <mqbs_memoryblock.h>

// BDE
#include <bsls_types.h>

// TEST DRIVER
#include <bmqtst_testhelper.h>

// CONVENIENCE
using namespace BloombergLP;
using namespace bsl;

// ============================================================================
//                                    TESTS
// ----------------------------------------------------------------------------
static void test1_breathingTest()
// ------------------------------------------------------------------------
// BREATHING TEST
//
// Testing:
//   Basic functionality of protocol structs.
// ------------------------------------------------------------------------
{
    bmqtst::TestHelper::printTestName("breathingTest");

    mqbs::MemoryBlockIterator obj;

    // Default iterator
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.position(), 0U);
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), static_cast<const mqbs::MemoryBlock*>(0));

    char                dummy = 1;
    bsls::Types::Uint64 size  = 10000;  // 1MB
    mqbs::MemoryBlock   block(&dummy, size);

    // Reset iterator
    obj.reset(&block, 0, size, true);
    BMQTST_ASSERT_EQ(obj.isForwardIterator(), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 0U);
    BMQTST_ASSERT_EQ(obj.remaining(), size);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    // Advance
    BMQTST_ASSERT_EQ(obj.advance(1000), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 1000U);
    BMQTST_ASSERT_EQ(obj.remaining(), size - 1000);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    BMQTST_ASSERT_EQ(obj.advance(2000), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 3000U);
    BMQTST_ASSERT_EQ(obj.remaining(), size - 3000);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    BMQTST_ASSERT_EQ(obj.advance(9000), false);  // exceed
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.position(), 3000U);  // old position
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    obj.clear();
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.position(), 0U);
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), static_cast<const mqbs::MemoryBlock*>(0));
}

static void test2_reverseIteration()
{
    bmqtst::TestHelper::printTestName("reverseIteration");

    char                dummy = 1;
    bsls::Types::Uint64 size  = 10000;
    mqbs::MemoryBlock   block(&dummy, size);

    // Create iterator
    mqbs::MemoryBlockIterator obj(&block, size, size - 1000, false);

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), false);
    BMQTST_ASSERT_EQ(obj.advance(2000), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 8000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 7000U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    BMQTST_ASSERT_EQ(obj.advance(5000), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 3000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 2000U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    BMQTST_ASSERT_EQ(obj.advance(2000), true);  // success
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.position(), 1000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    BMQTST_ASSERT_EQ(obj.advance(1), false);
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.position(), 1000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), &block);
}

static void test3_bidirectionalIteration()
{
    bmqtst::TestHelper::printTestName("bi-directional Iteration");

    char                dummy = 1;
    bsls::Types::Uint64 size  = 10000;  // 1MB
    mqbs::MemoryBlock   block(&dummy, size);

    // Create objator.
    mqbs::MemoryBlockIterator obj(&block,
                                  0,     // position
                                  8000,  // remaining
                                  true);

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.remaining(), 8000U);
    BMQTST_ASSERT_EQ(obj.position(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    obj.flipDirection();

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), false);
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.position(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    obj.flipDirection();

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), true);
    BMQTST_ASSERT_EQ(obj.advance(1000), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 1000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 7000U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    obj.flipDirection();

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), false);
    BMQTST_ASSERT_EQ(obj.advance(500), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 500U);
    BMQTST_ASSERT_EQ(obj.remaining(), 500U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    obj.flipDirection();

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), true);
    BMQTST_ASSERT_EQ(obj.advance(7500), true);
    BMQTST_ASSERT_EQ(obj.atEnd(), true);
    BMQTST_ASSERT_EQ(obj.position(), 8000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 0U);
    BMQTST_ASSERT_EQ(obj.block(), &block);

    obj.flipDirection();

    BMQTST_ASSERT_EQ(obj.isForwardIterator(), false);
    BMQTST_ASSERT_EQ(obj.atEnd(), false);
    BMQTST_ASSERT_EQ(obj.position(), 8000U);
    BMQTST_ASSERT_EQ(obj.remaining(), 8000U);
    BMQTST_ASSERT_EQ(obj.block(), &block);
}

// ============================================================================
//                                 MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    TEST_PROLOG(bmqtst::TestHelper::e_DEFAULT);

    switch (_testCase) {
    case 0:
    case 3: test3_bidirectionalIteration(); break;
    case 2: test2_reverseIteration(); break;
    case 1: test1_breathingTest(); break;
    default: {
        cerr << "WARNING: CASE '" << _testCase << "' NOT FOUND." << endl;
        bmqtst::TestHelperUtil::testStatus() = -1;
    } break;
    }

    TEST_EPILOG(bmqtst::TestHelper::e_CHECK_DEF_GBL_ALLOC);
}
