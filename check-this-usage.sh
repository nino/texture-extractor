#!/bin/bash
# Script to check for missing explicit this-> usage in C++ member functions
# Uses clang-query to find implicit this usage

set -e

BUILD_DIR="${BUILD_DIR:-build}"

if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
    echo "Error: compile_commands.json not found in $BUILD_DIR"
    echo "Please run: cmake -B$BUILD_DIR -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    exit 1
fi

echo "Checking for implicit this-> usage..."
echo "======================================="

# Find all source files in src/
SRC_FILES=$(find src -name "*.cpp" 2>/dev/null)

if [ -z "$SRC_FILES" ]; then
    echo "No source files found in src/"
    exit 0
fi

# Create a temporary clang-query file
QUERY_FILE=$(mktemp)
cat > "$QUERY_FILE" << 'QUERY'
set output diag
match memberExpr(
  hasObjectExpression(ignoringImpCasts(cxxThisExpr())),
  unless(hasObjectExpression(materializeTemporaryExpr())),
  unless(hasAncestor(cxxDependentScopeMemberExpr()))
)
QUERY

# Run clang-query on each file
FOUND_ISSUES=0
for file in $SRC_FILES; do
    OUTPUT=$(clang-query -p="$BUILD_DIR" "$file" -f="$QUERY_FILE" 2>&1 | grep -v "Skipping" | grep -v "^$" || true)
    if [ -n "$OUTPUT" ]; then
        echo "Issues in $file:"
        echo "$OUTPUT"
        echo ""
        FOUND_ISSUES=1
    fi
done

rm -f "$QUERY_FILE"

if [ $FOUND_ISSUES -eq 0 ]; then
    echo "✓ All member accesses use explicit this->"
    exit 0
else
    echo "✗ Found member accesses without explicit this->"
    echo "Please add 'this->' prefix to all member accesses"
    exit 1
fi
