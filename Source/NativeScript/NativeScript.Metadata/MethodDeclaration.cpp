#include "pch.h"
#include <codecvt>
#include "MethodDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;
using namespace Microsoft::WRL;

const wchar_t* OVERLOAD_ATTRIBUTE_W{L"Windows.Foundation.Metadata.OverloadAttribute"};
const wchar_t* DEFAULT_OVERLOAD_ATTRIBUTE_W{L"Windows.Foundation.Metadata.DefaultOverloadAttribute"};

// TODO
namespace {

vector<ParameterDeclaration> makeParameterDeclarations(IMetaDataImport2* metadata, mdMethodDef token) {
    HCORENUM enumerator{nullptr};
    ULONG count{0};
    array<mdParamDef, 1024> tokens;

    ASSERT_SUCCESS(metadata->EnumParams(&enumerator, token, tokens.data(), tokens.size(), &count));
    ASSERT(count < tokens.size() - 1);
    metadata->CloseEnum(enumerator);

    vector<ParameterDeclaration> result;
    for (size_t i = 0; i < count; ++i) {
        result.emplace_back(metadata, tokens[i]);
    }

    return result;
}

}

MethodDeclaration::MethodDeclaration(IMetaDataImport2* metadata, mdMethodDef token)
    : Base()
    , _metadata{metadata}
    , _token{token}
    , _parameters(makeParameterDeclarations(metadata, token)) {

    ASSERT(token != mdMethodDefNil);
}

bool MethodDeclaration::isExported() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    if (!(IsMdPublic(methodFlags) || IsMdFamily(methodFlags) || IsMdFamORAssem(methodFlags))) {
        return false;
    }

    if (IsMdSpecialName(methodFlags)) {
        return false;
    }

    return true;
}

bool MethodDeclaration::isStatic() const {
    DWORD methodFlags{0};
    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nullptr, 0, nullptr, &methodFlags, nullptr, nullptr, nullptr, nullptr));

    return IsMdStatic(methodFlags) != 0;
}

wstring MethodDeclaration::name() const {
    identifier nameData;
    ULONG nameLength{0};

    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, nullptr, nameData.data(), nameData.size(), &nameLength, nullptr, nullptr, nullptr, nullptr, nullptr));

    wstring result{nameData.data(), nameLength - 1};
    return result;
}

wstring MethodDeclaration::fullName() const {
    mdTypeDef parentToken{mdTypeDefNil};

    ASSERT_SUCCESS(_metadata->GetMethodProps(_token, &parentToken, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr));

    identifier parentFullNameData;
    ULONG parentFullNameDataLength{0};

    ASSERT_SUCCESS(_metadata->GetTypeDefProps(parentToken, parentFullNameData.data(), parentFullNameData.size(), &parentFullNameDataLength, nullptr, nullptr));

    wstring fullName{parentFullNameData.data(), parentFullNameDataLength - 1};
    fullName.append(L".").append(name());
    return fullName;
}

IteratorRange<MethodDeclaration::ParameterIterator> MethodDeclaration::parameters() const {
    return IteratorRange<ParameterIterator>(_parameters.begin(), _parameters.end());
}

wstring MethodDeclaration::overloadName() const {
    const uint8_t* data{nullptr};
    HRESULT getAttributeResult{_metadata->GetCustomAttributeByName(_token, OVERLOAD_ATTRIBUTE_W, reinterpret_cast<const void**>(&data), nullptr)};

    ASSERT_SUCCESS(getAttributeResult);

    if (getAttributeResult == S_FALSE) {
        return wstring();
    }

    // Skip prolog
    data += 2;

    // If it's null
    if (*data == UINT8_MAX) {
        return wstring();
    }

    // Read size and advance
    ULONG size{CorSigUncompressData(data)};

    // TODO
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring name{converter.from_bytes(reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data) + size)};
    return name;
}

bool MethodDeclaration::isDefaultOverload() const {
    HRESULT getAttributeResult{_metadata->GetCustomAttributeByName(_token, DEFAULT_OVERLOAD_ATTRIBUTE_W, nullptr, nullptr)};

    ASSERT_SUCCESS(getAttributeResult);

    return getAttributeResult == S_OK;
}
}
}
