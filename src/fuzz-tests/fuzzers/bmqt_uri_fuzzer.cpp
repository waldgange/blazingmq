#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <bsl_string.h>
#include <bmqt_uri.h>

using namespace BloombergLP;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    bsl::string fuzz_input(reinterpret_cast<const char *>(data), size);

    bmqt::UriParser::initialize();
    bmqt::Uri uri;
    bsl::string error;
    int rc = bmqt::UriParser::parse(&uri, &error, fuzz_input);

    // Check it
    bool valid = uri.isValid();
    if (valid) {
        // read out some values
    }

    return 0;
}
