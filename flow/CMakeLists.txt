
add_library(flow INTERFACE)
target_include_directories(flow INTERFACE .)
target_sources(flow INTERFACE
    flow/Chain.h
    flow/Cofold.h
    flow/Deref.h
    flow/Elements.h
    flow/Enumerate.h
    flow/Filter.h
    flow/Flatten.h
    flow/Flow.h
    flow/Fuse.h
    flow/Inspect.h
    flow/Iterator.h
    flow/Map.h
    flow/Merge.h
    flow/RefElements.h
    flow/Stride.h
    flow/Successors.h
    flow/Take.h
    flow/TypeTraits.h
)
