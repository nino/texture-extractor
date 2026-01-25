# Texture extractor

This is in the Nino-is-figuring-out-how-Qt-works stage. No features are
implemented.

## Building

You need to have Qt version 6 installed. You can open the project in Qt Creator
and just kinda hit "Run" or you can run

```sh
cmake -Bbuild .
cmake --build build
```

Then the executable should live in the `build` directory.

## Testing

The project includes comprehensive tests for all GUI components using Qt Test framework.

### Running Tests

To build and run all tests:

```sh
# Build the project with tests
cmake -Bbuild .
cmake --build build

# Run all tests
cd build
ctest --output-on-failure
```

Or run individual test executables:

```sh
# From the build directory
./tests/test_EditableRectItem
./tests/test_SourceImageView
./tests/test_ExtractedView
./tests/test_DocumentWindow
./tests/test_MainWindow
./tests/test_Integration
```

### Test Coverage

The test suite includes:

- **Unit Tests:**
  - `test_EditableRectItem`: Tests for the editable rectangle with corner handles
    - Construction and initialization
    - Bounding box calculations
    - Handle rendering and hit detection
    - Corner dragging and resizing
    - Scale-aware behavior

  - `test_SourceImageView`: Tests for the custom graphics view with pan and zoom
    - Pan functionality (right-click drag)
    - Zoom functionality (mouse wheel)
    - Drag mode management
    - Transform calculations

  - `test_ExtractedView`: Tests for the extracted texture view
    - Basic construction and properties
    - QGraphicsView inheritance
    - Scene integration

  - `test_DocumentWindow`: Tests for document windows and photo views
    - Window creation and management
    - Title setting and retrieval
    - PhotoView layout and components
    - Image loading pipeline

  - `test_MainWindow`: Tests for the main application window
    - Menu and action creation
    - File opening functionality
    - Drag and drop support
    - Multiple document handling

- **Integration Tests:**
  - `test_Integration`: End-to-end workflow testing
    - Complete application workflow
    - Component interactions
    - Multi-document scenarios
    - View interactions with graphics items

### Test Helpers

The tests use a shared test helper library (`test_helpers.h/cpp`) that provides:
- Test image creation utilities
- Mouse event simulation
- Wheel event simulation
- Drag operation simulation
- Fuzzy comparison for floating-point values

### Requirements

Tests require Qt Test module (Qt::Test), which is automatically included when building the project.

## Coding Standards

This project follows specific coding standards to maintain consistency and clarity. See [CLAUDE.md](CLAUDE.md) for full details.

### Key Rules

**Member Access**: All member variable and member function access MUST use explicit `this->` prefix.

```cpp
// ✅ CORRECT
void MyClass::updateName(const QString& name) {
    this->name = name;
    this->refreshUI();
}

// ❌ INCORRECT
void MyClass::updateName(const QString& name) {
    name = name;  // Bug: shadows parameter!
    refreshUI();  // Not clear this is a member
}
```

**Headers**: Use `.hpp` extension and `#pragma once` for all C++ headers.
