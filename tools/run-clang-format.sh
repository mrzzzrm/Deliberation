#!/bin/bash
find Source -iname "*.cpp" -o -iname "*.hpp" -o -iname "*.h"  -o -iname "*.inl" | xargs -I{} sh -c "clang-format -i -style=file '{}'"
