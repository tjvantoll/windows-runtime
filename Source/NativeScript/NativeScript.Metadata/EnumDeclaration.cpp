#include "pch.h"
#include "EnumDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

EnumDeclaration::MemberIterator::MemberIterator(ComPtr<IMetaDataImport2> metadata, mdTypeDef token, ULONG index)
    : _metadata{metadata}
      , _token{token}
      , _currentIndex{index}
      , _enumerator{nullptr} {

    ASSERT_SUCCESS(_metadata->EnumFields(&_enumerator, _token, nullptr, 0, nullptr));
    ASSERT_SUCCESS(_metadata->ResetEnum(_enumerator, _currentIndex));
}

EnumDeclaration::MemberIterator::MemberIterator(const MemberIterator&& other)
    : _metadata{move(other._metadata)}
      , _token{move(other._token)}
      , _currentIndex{move(other._currentIndex)}
      , _enumerator{move(other._enumerator)} {

    other._enumerator = nullptr;
}

EnumDeclaration::MemberIterator& EnumDeclaration::MemberIterator::operator=(const MemberIterator&& other) {
    _metadata = move(other._metadata);
    _token = move(other._token);
    _currentIndex = move(other._currentIndex);
    _enumerator = move(other._enumerator);

    other._enumerator = nullptr;

    return *this;
}

EnumDeclaration::MemberIterator::~MemberIterator() {
    _metadata->CloseEnum(_enumerator);
    _enumerator = nullptr;
}

EnumMemberDeclaration EnumDeclaration::MemberIterator::operator*() const {
    mdFieldDef field{0};

    ASSERT_SUCCESS(_metadata->EnumFields(&_enumerator, _token, &field, 1, nullptr));
    ASSERT_SUCCESS(_metadata->ResetEnum(_enumerator, _currentIndex));

    return EnumMemberDeclaration(_metadata, field);
}

EnumDeclaration::MemberIterator& EnumDeclaration::MemberIterator::operator++() {
    ASSERT_SUCCESS(_metadata->ResetEnum(_enumerator, ++_currentIndex));
    return *this;
}

EnumDeclaration::EnumDeclaration(ComPtr<IMetaDataImport2> metadata, mdTypeDef token)
    : Base(metadata, token) {
}

// Skips COR_ENUM_FIELD field
size_t EnumDeclaration::size() const {
    ULONG size{0};

    HCORENUM enumerator{nullptr};
    ASSERT_SUCCESS(_metadata->EnumFields(&enumerator, _token, nullptr, 0, nullptr));
    ASSERT_SUCCESS(_metadata->CountEnum(enumerator, &size));
    _metadata->CloseEnum(enumerator);

    return size - 1;
}

EnumDeclaration::MemberIterator EnumDeclaration::begin() const {
    return MemberIterator(_metadata, _token, 1);
}

EnumDeclaration::MemberIterator EnumDeclaration::end() const {
    return MemberIterator(_metadata, _token, size() + 1);
}

bool operator==(const EnumDeclaration::MemberIterator& left, const EnumDeclaration::MemberIterator& right) {
    return left._currentIndex == right._currentIndex;
}

bool operator!=(const EnumDeclaration::MemberIterator& left, const EnumDeclaration::MemberIterator& right) {
    return left._currentIndex != right._currentIndex;
}

}
}
