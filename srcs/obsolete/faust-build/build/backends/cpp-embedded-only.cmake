# this file may be used to select different backends
# it's always read by the default makefile target
# values are among: 
#    OFF       don't include the backend
#    OFF       embed the backend in the faust compiler
#              embed the backend in the faust static library
#              embed the backend in the faust dynamic library
#              embed the backend in the faust wasm library

set ( CPP_BACKEND    COMPILER                       CACHE STRING  "Include CPP backend"       FORCE )
set ( C_BACKEND      OFF                            CACHE STRING  "Include C backend"         FORCE )
set ( CODEBOX_BACKEND      OFF                            CACHE STRING  "Include Codebox backend"         FORCE )
set ( CMAJOR_BACKEND OFF                            CACHE STRING  "Include Cmajor backend"    FORCE )
set ( CSHARP_BACKEND OFF                            CACHE STRING  "Include CSharp backend"    FORCE )
set ( DLANG_BACKEND  OFF                            CACHE STRING  "Include Dlang backend"     FORCE )
set ( FIR_BACKEND    OFF                            CACHE STRING  "Include FIR backend"       FORCE )
set ( INTERP_BACKEND OFF                            CACHE STRING  "Include Interpreter backend" FORCE )
set ( JAVA_BACKEND   OFF                            CACHE STRING  "Include JAVA backend"      FORCE )
set ( JAX_BACKEND    OFF                            CACHE STRING  "Include JAX backend"       FORCE )
set ( JULIA_BACKEND  OFF                            CACHE STRING  "Include Julia backend"     FORCE )
set ( JSFX_BACKEND   OFF                            CACHE STRING  "Include JSFX backend"     FORCE )
set ( LLVM_BACKEND                                  CACHE STRING  "Include LLVM backend"      FORCE )
set ( OLDCPP_BACKEND OFF                            CACHE STRING  "Include old CPP backend"   FORCE )
set ( RUST_BACKEND   OFF                            CACHE STRING  "Include Rust backend"      FORCE )
# Template is deactivated 
set ( TEMPLATE_BACKEND   OFF    CACHE STRING  "Include Template backend"  FORCE )
set ( VHDL_BACKEND   OFF                            CACHE STRING  "Include VHDL backend"      FORCE )
set ( WASM_BACKEND   OFF                            CACHE STRING  "Include WASM backend"      FORCE )

