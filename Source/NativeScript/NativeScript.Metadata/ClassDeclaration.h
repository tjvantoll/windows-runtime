#pragma once

#include <vector>
#include <memory>

#include "BaseClassDeclaration.h"
#include "InterfaceDeclaration.h"
#include "IteratorRange.h"

namespace NativeScript {
namespace Metadata {

enum class ClassType {
    Uninstantiable = 0x00,
    Instantiable = 0x01,
    Subclassable = 0x02
};

class ClassDeclaration final : public BaseClassDeclaration {
public:
    typedef BaseClassDeclaration Base;

    explicit ClassDeclaration(IMetaDataImport2*, mdTypeDef);

    static std::unique_ptr<InterfaceDeclaration> declaringInterfaceForMethod(const MethodDeclaration&, size_t* outIndex);

    std::wstring baseFullName() const;

    ClassType classType() const;

    IteratorRange<MethodIterator> initializers() const;

private:
    static std::unique_ptr<InterfaceDeclaration> declaringInterfaceForInstanceInitializer(const MethodDeclaration&, size_t*);

    std::vector<MethodDeclaration> _initializers;
};

}
}
