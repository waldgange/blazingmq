#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <mqbcmd_messages.h>
#include <mqbcmd_parseutil.h>

using namespace BloombergLP;
using namespace bsl;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::string fuzz_input(reinterpret_cast<const char *>(data), size);
  bsl::string sample_p2 = fuzz_input;

  bsl::string error;
  mqbcmd::Command actual;
  mqbcmd::ParseUtil::parse(&actual, &error, sample_p2);

  return 0;
}
