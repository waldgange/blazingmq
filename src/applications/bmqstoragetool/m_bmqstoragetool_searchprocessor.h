// Copyright 2014-2023 Bloomberg Finance L.P.
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

// bmqstoragetool
#include <m_bmqstoragetool_commandprocessor.h>

// MQB
#include <mqbs_datafileiterator.h>
#include <mqbs_filestoreprotocol.h>
#include <mqbs_journalfileiterator.h>
#include <mqbs_mappedfiledescriptor.h>
// #include <mqbu_storagekey.h>

// BDE
#include <bsls_keyword.h>

namespace BloombergLP {
namespace m_bmqstoragetool {

// =====================
// class SearchProcessor
// =====================

class SearchParameters {
  public:
    SearchParameters();
    SearchParameters(bslma::Allocator* allocator);
    bsl::vector<bsl::string> searchGuids;
};

class SearchProcessor : public CommandProcessor {

  private:
    // DATA
    bsl::string d_dataFile;

    bsl::string d_journalFile;

    mqbs::MappedFileDescriptor d_dataFd;

    mqbs::MappedFileDescriptor d_journalFd;

    mqbs::DataFileIterator d_dataFileIter;

    mqbs::JournalFileIterator d_journalFileIter;

    SearchParameters d_searchParameters;

  public:
    // CREATORS
    SearchProcessor();
    SearchProcessor(bslma::Allocator* allocator);
    SearchProcessor(bsl::string& journalFile, bslma::Allocator* allocator);
    SearchProcessor(mqbs::JournalFileIterator& journalFileIter, SearchParameters& params, bslma::Allocator* allocator);
    ~SearchProcessor();

    void process(bsl::ostream& ostream) BSLS_KEYWORD_OVERRIDE;

    // TODO: remove
    mqbs::JournalFileIterator& getJournalFileIter() { return d_journalFileIter;}
};

}  // close package namespace
}  // close enterprise namespace
