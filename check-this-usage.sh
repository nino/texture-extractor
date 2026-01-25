#!/bin/bash
# Script to check for missing explicit this-> usage in C++ member functions
# Simple source-based approach

set -e

echo "Checking for implicit this-> usage..."
echo "======================================="

# Find all source files in src/
SRC_FILES=$(find src -name "*.cpp" 2>/dev/null)

if [ -z "$SRC_FILES" ]; then
    echo "No source files found in src/"
    exit 0
fi

FOUND_ISSUES=0

# Read the list of member names from headers
# This is a simplified check - look for common patterns of implicit member access

for file in $SRC_FILES; do
    # Look for lines that might be implicit member access
    # Pattern: lines with identifier followed by = or -> or ( that aren't preceded by this->
    # This is a heuristic and may have false positives/negatives
    
    # Skip lines with "this->" already
    # Look for patterns like: memberVar = ... or memberFunc(...) at start of statement
    VIOLATIONS=$(grep -n '^[[:space:]]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*[=(]' "$file" | \
                grep -v 'this->' | \
                grep -v '^[[:space:]]*//' | \
                grep -v '^[[:space:]]*/\*' | \
                grep -v '.*:.*:' | \
                head -20 || true)
    
    if [ -n "$VIOLATIONS" ]; then
        echo "Potential issues in $file (first 20):"
        echo "$VIOLATIONS"
        echo ""
        FOUND_ISSUES=1
    fi
done

if [ $FOUND_ISSUES -eq 0 ]; then
    echo "✓ All member accesses appear to use explicit this->"
    echo "(Note: This is a simplified check - manual review recommended)"
    exit 0
else
    echo "✗ Found potential member accesses without explicit this->"
    echo "Please review the above lines and add 'this->' prefix where appropriate"
    echo "(Note: This check may have false positives)"
    exit 1
fi
