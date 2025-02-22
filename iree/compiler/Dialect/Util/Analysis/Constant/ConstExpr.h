// Copyright 2021 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_COMPILER_DIALECT_IREE_UTIL_ANALYSIS_CONSTANT_CONST_EXPR_H_
#define IREE_COMPILER_DIALECT_IREE_UTIL_ANALYSIS_CONSTANT_CONST_EXPR_H_

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "mlir/IR/Operation.h"
#include "mlir/Support/LLVM.h"

namespace mlir {
namespace iree_compiler {
namespace IREE {
namespace Util {

// Analyzes an entire module to determine all operations/values that are
// purely derived from constants or immutable data and builds a
// dependency tree.
//
// Modifying any of the analyzed operations invalidates this analysis.
class ConstExprAnalysis {
 public:
  struct ConstValueInfo;
  explicit ConstExprAnalysis(Operation *rootOp);

  void print(raw_ostream &os) const;
  void dump() const;

  // Returns const-expr info for an operation (or nullptr if unknown).
  const ConstValueInfo *lookup(Value queryValue) const {
    return constInfoMap.lookup(queryValue);
  }

  // Return const-expr info for an operation (or nullptr if unknown). Presently,
  // an operation's results will either all be const-expr or not, so we just
  // check the first. 0-result ops cannot be const-expr.
  const ConstValueInfo *lookup(Operation *queryOp) const {
    if (queryOp->getNumResults() == 0) return nullptr;
    return lookup(queryOp->getResult(0));
  }

  // Returns true if the given value is only derived from immutable inputs.
  // Note that this only returns true for derived values. Direct use of
  // existing constants returns false.
  bool isConstExprValue(Value queryValue) const {
    ConstValueInfo *found = constInfoMap.lookup(queryValue);
    if (!found) return false;
    return found->state == ConstValueInfo::CONSTANT && !found->isRoot;
  }

  // Returns whether the given operation is considered const-expr. Presently,
  // an operation's results will either all be const-expr or not, so we just
  // check the first. 0-result ops cannot be const-expr.
  bool isConstExprOperation(Operation *queryOp) const {
    if (queryOp->getNumResults() == 0) return false;
    return isConstExprValue(queryOp->getResult(0));
  }

  // Populates a set, in arbitrary order, of all const-expr ops in the
  // program. This includes root ops.
  void populateConstExprOperations(llvm::DenseSet<Operation *> &ops) const {
    for (auto it : constInfoMap) {
      ConstValueInfo *info = it.second;
      if (info->state == ConstValueInfo::CONSTANT) {
        Operation *definingOp = info->constValue.getDefiningOp();
        assert(definingOp && "const-expr values must have a defining op");
        ops.insert(definingOp);
      }
    }
  }

  // Map of a root value in the program that should be considered constant
  // to the operation that defines the constant. Two cases:
  //   LoadGlobalOp.result -> GlobalOp
  //   ConstantOp.result -> ConstantOp
  // Entries can come from the whole program.
  using ConstRootMap = llvm::DenseMap<Value, Operation *>;

  // Information about a Value that is has been analyzed.
  struct ConstValueInfo {
    ConstValueInfo(Value constValue) : constValue(constValue) {}

    // UNKNOWN: Not all producers have been validated.
    // CONSTANT: Producers have all been validated as constants.
    // NON_CONSTANT: The op is not eligible to be treated as a constant or
    //   one or more producers is non constant.
    enum State { UNKNOWN, CONSTANT, NON_CONSTANT };
    State state = UNKNOWN;

    // The presumed constant value.
    Value constValue;

    // Root values (in ConstRootMap) that this value (indirectly) derives from.
    SmallPtrSet<Value, 4> roots;

    // Direct producers that feed into this constant value.
    SmallPtrSet<ConstValueInfo *, 8> producers;

    // Whether this is a root.
    bool isRoot = false;

    // Whether this is a const-expr value.
    bool isConstExpr() const { return state == CONSTANT; }

    Operation *getOperation() const {
      Operation *ret = constValue.getDefiningOp();
      assert(ret && "const-expr must have a defining op");
      return ret;
    }
  };

 private:
  // Expands the frontier to include all results of a given op in an UNKNOWN
  // state. This also checks that all of its operands are known, adding
  // them recusrively if not.
  void expandToOp(Operation *op);

  // Add a new info record for a value to analyze for const-ness.
  ConstValueInfo *addInfo(Value constValue);

  // Map of a root value in the program that should be considered constant
  // to the operation that defines the constant. Two cases:
  //   LoadGlobalOp.result -> GlobalOp
  //   ConstantOp.result -> ConstantOp
  // Entries can come from the whole program.
  llvm::DenseMap<Value, Operation *> constantRoots;

  // Map of analyzed value to corresponding info struct.
  llvm::DenseMap<Value, ConstValueInfo *> constInfoMap;

  // Allocated ConstValueInfo structs (to preserve pointer stability).
  llvm::SmallVector<std::unique_ptr<ConstValueInfo>> allocedConstInfos;

  // Worklist of const value info structs which need more resolution.
  using ConstValueWorklist = llvm::SmallVector<ConstValueInfo *>;
  ConstValueWorklist worklist;
};

inline raw_ostream &operator<<(raw_ostream &os,
                               const ConstExprAnalysis &analysis) {
  analysis.print(os);
  return os;
}

}  // namespace Util
}  // namespace IREE
}  // namespace iree_compiler
}  // namespace mlir

#endif  // IREE_COMPILER_DIALECT_IREE_UTIL_ANALYSIS_CONSTANT_CONST_EXPR_H_
