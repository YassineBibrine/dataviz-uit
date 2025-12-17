# DataViz UIT - Quick Reference

## Quick Start

```bash
# First time build
.\build.bat -Clean

# Regular build
.\build.bat

# Build and run
.\build.bat -Run

# Just run (after building)
.\run.bat
```

## Project Status ?

- [x] Qt properly configured and detected
- [x] All compilation errors fixed
- [x] Application builds successfully
- [x] Qt dependencies auto-deployed
- [x] Application launches and runs
- [x] Graphviz integration working

## File Locations

- **Executable:** `build\bin\Release\dataviz-uit.exe`
- **Source Code:** `src\`
- **Build Files:** `build\`

## Key Technologies

- **C++17** with Visual Studio 2022
- **Qt 6.10.0** (MSVC 2022 64-bit)
- **CMake 3.16+** with Visual Studio Generator
- **Graphviz** (optional, for advanced layouts)

## Troubleshooting

### Build fails?
```bash
.\build.bat -Clean
```

### Qt not found?
- Verify Qt is at: `C:/Qt/6.10.0/msvc2022_64`
- Or edit CMakeLists.txt line 10-16 with your Qt path

### Missing DLLs?
- Run from `build\bin\Release\` directory
- Or re-run: `cd build\bin\Release && windeployqt dataviz-uit.exe`

## Development Workflow

1. Edit code in `src\`
2. Run `.\build.bat`
3. Test with `.\run.bat`
4. Repeat!

## Need Help?

- See: `README.md` for full documentation
- See: `BUILD_FIXES.md` for technical details

---
**Last Updated:** Project successfully builds and runs!
