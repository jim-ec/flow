
add_library(flow INTERFACE)
target_include_directories(flow INTERFACE .)
target_sources(flow INTERFACE
    flow/Chain.h
    flow/Generate.h
    flow/Elements.h
    flow/ElementsReferenced.h
    flow/Filter.h
    flow/Flatten.h
    flow/Flow.h
    flow/Fold.h
    flow/Fuse.h
    flow/Inspect.h
    flow/SequenceIterator.h
    flow/Map.h
    flow/Zip.h
    flow/Stride.h
    flow/Take.h
    flow/Cycle.h
    Flow/Maybe.h
    flow/details.h
)
