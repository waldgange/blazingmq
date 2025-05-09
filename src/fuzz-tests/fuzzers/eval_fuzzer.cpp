#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <bmqeval_simpleevaluator.h>
#include <bmqeval_simpleevaluatorparser.hpp>
#include <bmqeval_simpleevaluatorscanner.h>

#include <bdlma_localsequentialallocator.h>

using namespace BloombergLP;
using namespace bsl;
using namespace BloombergLP::bmqeval;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::string fuzz_input(reinterpret_cast<const char *>(data), size);

  bdlma::LocalSequentialAllocator<2048> localAllocator;
  CompilationContext compilationContext(&localAllocator);
  SimpleEvaluator evaluator;
  try {
    evaluator.compile(fuzz_input, compilationContext);
  } catch (...) {
  }

  return 0;
}
