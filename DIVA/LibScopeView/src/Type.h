//===-- LibScopeView/Type.h -------------------------------------*- C++ -*-===//
///
/// Copyright (c) 2017 by Sony Interactive Entertainment Inc.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///
//===----------------------------------------------------------------------===//
///
/// Definitions of Type and its subclasses.
///
//===----------------------------------------------------------------------===//

#ifndef SCOPEVIEWTYPE_H
#define SCOPEVIEWTYPE_H

#include "Object.h"

namespace LibScopeView {

/// \brief Class to represent a DWARF Type object.
class Type : public Element {
public:
  Type() : Type(SV_Type) {}

  /// \brief Return true if Obj is an instance of Type.
  static bool classof(const Object *Obj) {
    return SV_Type <= Obj->getKind() && Obj->getKind() <= SV_TypeSubrange;
  }

protected:
  Type(ObjectKind K);

private:
  // Flags specifying various properties of the Type.
  enum TypeAttributes {
    IsBaseType,
    IsConstType,
    IsImportedModule,
    IsImportedDeclaration,
    IsInheritance,
    IsPointerType,
    IsPointerMemberType,
    IsReferenceType,
    IsRvalueReferenceType,
    IsRestrictType,
    IsTemplateTypeParam,
    IsTemplateValueParam,
    IsTemplateTemplateParam,
    IsUnspecifiedType,
    IsVolatileType,
    IncludeInPrint,
    TypeAttributesSize
  };
  std::bitset<TypeAttributesSize> TypeAttributesFlags;

public:
  /// \brief Work out and set the full name for the type.
  void formulateTypeName(const PrintSettings &Settings);

  bool getIsBaseType() const { return TypeAttributesFlags[IsBaseType]; }
  void setIsBaseType() { TypeAttributesFlags.set(IsBaseType); }
  bool getIsConstType() const { return TypeAttributesFlags[IsConstType]; }
  void setIsConstType() { TypeAttributesFlags.set(IsConstType); }
  bool getIsImportedDeclaration() const {
    return TypeAttributesFlags[IsImportedDeclaration];
  }
  void setIsImportedDeclaration() {
    TypeAttributesFlags.set(IsImportedDeclaration);
  }

  bool getIsImportedModule() const {
    return TypeAttributesFlags[IsImportedModule];
  }
  void setIsImportedModule() { TypeAttributesFlags.set(IsImportedModule); }

  bool getIsInheritance() const { return TypeAttributesFlags[IsInheritance]; }
  void setIsInheritance() { TypeAttributesFlags.set(IsInheritance); }
  bool getIsPointerType() const { return TypeAttributesFlags[IsPointerType]; }
  void setIsPointerType() { TypeAttributesFlags.set(IsPointerType); }
  bool getIsPointerMemberType() const {
    return TypeAttributesFlags[IsPointerMemberType];
  }
  void setIsPointerMemberType() {
    TypeAttributesFlags.set(IsPointerMemberType);
  }
  bool getIsReferenceType() const {
    return TypeAttributesFlags[IsReferenceType];
  }
  void setIsReferenceType() { TypeAttributesFlags.set(IsReferenceType); }
  bool getIsRestrictType() const { return TypeAttributesFlags[IsRestrictType]; }
  void setIsRestrictType() { TypeAttributesFlags.set(IsRestrictType); }
  bool getIsRvalueReferenceType() const {
    return TypeAttributesFlags[IsRvalueReferenceType];
  }
  void setIsRvalueReferenceType() {
    TypeAttributesFlags.set(IsRvalueReferenceType);
  }
  bool getIsTemplateType() const {
    return TypeAttributesFlags[IsTemplateTypeParam];
  }
  void setIsTemplateType() { TypeAttributesFlags.set(IsTemplateTypeParam); }

  bool getIsTemplateValue() const {
    return TypeAttributesFlags[IsTemplateValueParam];
  }
  void setIsTemplateValue() { TypeAttributesFlags.set(IsTemplateValueParam); }

  bool getIsTemplateTemplate() const {
    return TypeAttributesFlags[IsTemplateTemplateParam];
  }
  void setIsTemplateTemplate() {
    TypeAttributesFlags.set(IsTemplateTemplateParam);
  }

  bool getIsUnspecifiedType() const {
    return TypeAttributesFlags[IsUnspecifiedType];
  }
  void setIsUnspecifiedType() { TypeAttributesFlags.set(IsUnspecifiedType); }
  bool getIsVolatileType() const { return TypeAttributesFlags[IsVolatileType]; }
  void setIsVolatileType() { TypeAttributesFlags.set(IsVolatileType); }
  bool getIncludeInPrint() const { return TypeAttributesFlags[IncludeInPrint]; }
  void setIncludeInPrint() { TypeAttributesFlags.set(IncludeInPrint); }

public:
  // Functions to be implemented by derived classes.

  /// \brief Process the values for a DW_TAG_enumerator.
  virtual const std::string &getValue() const;
  virtual void setValue(const std::string & /*Value*/) {}

  bool getIsPrintedAsObject() const override;
  /// \brief Returns a text representation of this DIVA Object.
  std::string getAsText(const PrintSettings &Settings) const override;
  /// \brief Returns a YAML representation of this DIVA Object.
  std::string getAsYAML() const override;

private:
  // DW_AT_byte_size for PrimitiveType.
  unsigned ByteSize;

public:
  unsigned getByteSize() const;
  void setByteSize(unsigned Size);
};

/// \brief Class to represent DW_TAG_typedef_type
class TypeDefinition : public Type {
public:
  TypeDefinition() : Type(SV_TypeDefinition) {}

  /// \brief Return true if Obj is an instance of TypeDefinition.
  static bool classof(const Object *Obj) {
    return Obj->getKind() == SV_TypeDefinition;
  }

  bool getIsPrintedAsObject() const override { return true; }
  /// \brief Returns a text representation of this DIVA Object.
  std::string getAsText(const PrintSettings &Settings) const override;
  /// \brief Returns a YAML representation of this DIVA Object.
  std::string getAsYAML() const override;
};

/// \brief Class to represent a DW_TAG_enumerator
class TypeEnumerator : public Type {
public:
  TypeEnumerator() : Type(SV_TypeEnumerator), ValueRef(nullptr) {}

  /// Return true if Obj is an instance of TypeEnumerator.
  static bool classof(const Object *Obj) {
    return Obj->getKind() == SV_TypeEnumerator;
  }

private:
  StringPoolRef ValueRef; // Enumerator value.

public:
  /// \brief Process the values for a DW_TAG_enumerator.
  const std::string &getValue() const override;
  void setValue(const std::string &Value) override;

  bool getIsPrintedAsObject() const override { return false; }
  /// \brief Returns a text representation of this DIVA Object.
  std::string getAsText(const PrintSettings &Settings) const override;
  /// \brief Returns a YAML representation of this DIVA Object.
  std::string getAsYAML() const override;
};

/// \brief Class to represent DW_TAG_imported_module /
/// DW_TAG_imported_declaration
class TypeImport : public Type {
public:
  TypeImport()
      : Type(SV_TypeImport), InheritanceAccess(AccessSpecifier::Unspecified) {}

  /// \brief Return true if Obj is an instance of TypeImport.
  static bool classof(const Object *Obj) {
    return Obj->getKind() == SV_TypeImport;
  }

  /// \brief Access specifier, only valid for inheritance.
  AccessSpecifier getInheritanceAccess() const;
  void setInheritanceAccess(AccessSpecifier Access);

private:
  AccessSpecifier InheritanceAccess;

public:
  bool getIsPrintedAsObject() const override;

  /// \brief Returns a text representation of this DIVA Object.
  std::string getAsText(const PrintSettings &Settings) const override;
  /// \brief Returns a YAML representation of this DIVA Object.
  std::string getAsYAML() const override;

private:
  virtual std::string getInheritanceAsText(const PrintSettings &Settings) const;
  virtual std::string getUsingAsText(const PrintSettings &Settings) const;
  // Gets a YAML representation of DIVA Object as an Inheritance attribute.
  virtual std::string getInheritanceAsYAML() const;
  virtual std::string getUsingAsYAML() const;
};

/// \brief Class to represent a DWARF Template parameter holder.
///
/// Parameters can be values, types or templates.
class TypeTemplateParam : public Type {
public:
  TypeTemplateParam() : Type(SV_TypeTemplateParam), ValueRef(nullptr) {}

  /// \brief Return true if Obj is an instance of TypeParam.
  static bool classof(const Object *Obj) {
    return Obj->getKind() == SV_TypeTemplateParam;
  }

private:
  StringPoolRef ValueRef; // Value in case of value or template parameters.

public:
  /// \brief Template parameter value
  const std::string &getValue() const override;
  void setValue(const std::string &Value) override;

  bool getIsPrintedAsObject() const override;

  /// \brief Returns a text representation of this DIVA Object.
  std::string getAsText(const PrintSettings &Settings) const override;
  /// \brief Returns a YAML representation of this DIVA Object.
  std::string getAsYAML() const override;
};

/// \brief Class to represent a DW_TAG_subrange_type
class TypeSubrange : public Type {
public:
  TypeSubrange() : Type(SV_TypeSubrange) {}
  ~TypeSubrange();

  /// Return true if Obj is an instance of TypeSubrange.
  static bool classof(const Object *Obj) {
    return Obj->getKind() == SV_TypeSubrange;
  }
};

} // namespace LibScopeView

#endif // SCOPEVIEWTYPE_H
