add_mlir_library(IREEInputDialect
  InputDialect.cpp
  InputOps.cpp

  ADDITIONAL_HEADER_DIRS
  ${IREE_DIALECTS_SOURCE_DIR}/include

  DEPENDS
  IREEInputIncGen

  LINK_LIBS PUBLIC
  MLIRArithmetic
  MLIRIR
  MLIRSideEffectInterfaces
)

iree_dialects_target_includes(IREEInputDialect)
