# Coordination Documentation Index
**Complete Guide to DataViz Development Coordination Files**

## Overview

This document serves as an index to all coordination and developer guides for the DataViz project. Use this to navigate the documentation structure and find the guide you need.

---

## Quick Navigation

### ?? Global & Organization
- **[GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)** - Project-wide organization, timeline, all teams
  - Project overview and team structure
  - Full data flow architecture
  - Build and CMake structure
  - Development workflow and Git strategy
  - Communication protocols
  - Performance targets and risk management

### ?? C2V Team (Code-to-Visualization Analysis)
- **[C2V_COORDINATION.md](./C2V_COORDINATION.md)** - C2V team synchronization
  - Team structure and responsibilities
  - Detailed task breakdown for Dev 1 & 2
  - Integration points and handoffs
  - Data exchange formats
  - Testing strategy
  - Timeline and milestones

- **[C2V_DEVELOPER_1_FRONTEND.md](./C2V_DEVELOPER_1_FRONTEND.md)** - Frontend/AST Developer
  - AST Parser implementation
  - Structure Extractor with self-referential detection
  - Unit tests and CMake setup
  - Weeks 1-3 detailed tasks
  - Testing checklist and debugging tips

- **[C2V_DEVELOPER_2_BACKEND.md](./C2V_DEVELOPER_2_BACKEND.md)** - Backend/Instrumentation Developer
  - Execution Logger with JSON serialization
  - State Mapper for visualization generation
  - LLVM Instrumentation integration
  - VisualizationState model
  - Weeks 2-4 detailed tasks
- Full pipeline testing strategy

### ?? UI Team (User Interface Application)
- **[UI_COORDINATION.md](./UI_COORDINATION.md)** - UI team synchronization
  - Team structure and responsibilities
  - Parallel development timeline
  - Component layout and signal flow
  - Data interfaces between components
  - CMake dependency chain
  - Testing and integration strategy

- **[UI_DEVELOPER_1_RENDERING.md](./UI_DEVELOPER_1_RENDERING.md)** - Rendering/Visualization Developer
  - Graphviz rendering engine integration
  - Qt interactive graph widget
  - Animation playback controller
  - Zoom, pan, and selection interactions
  - Weeks 4-6 detailed tasks
  - Export functionality (SVG, PNG)

- **[UI_DEVELOPER_2_FRAMEWORK.md](./UI_DEVELOPER_2_FRAMEWORK.md)** - Framework/Application Developer
  - Qt main window design
  - Control panel widget
  - Properties panel for node/edge info
  - Output console for logging
  - File dialogs and project I/O
  - Weeks 4-6 detailed tasks
  - Pipeline integration

### ?? V2C Team (Visualization-to-Code Synthesis)
- **[V2C_GUIDE.md](./V2C_GUIDE.md)** - Complete V2C team guide
  - AST Manipulation and code generation
  - API mapping for user structures
  - Code formatting and rewriting
  - Bidirectional visualization mapping
  - Weeks 5-7 focus

- **[V2C_MEMBER_1_CODE_GENERATION.md](./V2C_MEMBER_1_CODE_GENERATION.md)** - Code Generation Developer
  - Synthesis engine fundamentals
  - AST Manipulation techniques
  - Code generation from visual operations
  - API mapping service
  - Detailed code generation examples

---

## Document Type Legend

| Type | Purpose | Audience |
|------|---------|----------|
| **COORDINATION** | Team synchronization | Team leads, developers on that team |
| **DEVELOPER** | Individual tasks & implementation | Specific developer |
| **GUIDE** | Comprehensive overview | All team members |

---

## By Role

### If You're a C2V Developer...
1. Start: [C2V_COORDINATION.md](./C2V_COORDINATION.md)
2. Your role: [C2V_DEVELOPER_1_FRONTEND.md](./C2V_DEVELOPER_1_FRONTEND.md) or [C2V_DEVELOPER_2_BACKEND.md](./C2V_DEVELOPER_2_BACKEND.md)
3. Context: [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)

### If You're a UI Developer...
1. Start: [UI_COORDINATION.md](./UI_COORDINATION.md)
2. Your role: [UI_DEVELOPER_1_RENDERING.md](./UI_DEVELOPER_1_RENDERING.md) or [UI_DEVELOPER_2_FRAMEWORK.md](./UI_DEVELOPER_2_FRAMEWORK.md)
3. Context: [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)

### If You're a V2C Developer...
1. Start: [V2C_GUIDE.md](./V2C_GUIDE.md)
2. Your role: [V2C_MEMBER_1_CODE_GENERATION.md](./V2C_MEMBER_1_CODE_GENERATION.md) or find V2C_MEMBER_2 guide
3. Context: [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)

### If You're a Tech Lead...
1. Overview: [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)
2. Each team: [C2V_COORDINATION.md](./C2V_COORDINATION.md), [UI_COORDINATION.md](./UI_COORDINATION.md), [V2C_GUIDE.md](./V2C_GUIDE.md)
3. Individual: Developer guides as needed

---

## Key Information by Topic

### Project Timeline
- **Weeks 1-3**: C2V Phase 1 (C2V Dev 1 AST parsing)
- **Weeks 2-4**: C2V Phase 2 (C2V Dev 2 instrumentation)
- **Weeks 4-6**: UI Phase (Parallel: UI Dev 1 rendering + UI Dev 2 framework)
- **Weeks 5-7**: V2C Phase (Code generation and rewriting)
- **Weeks 6-8**: Integration, testing, polish

See: [GLOBAL_COORDINATION.md - Phase Breakdown](./GLOBAL_COORDINATION.md#phase-breakdown)

### Data Structures
- **DataStructureInfo** - Structure metadata
- **ExecutionEvent** - Runtime execution snapshot
- **VisualizationState** - Animation frame
- **RenderedGraph** - Layout with coordinates

See: [GLOBAL_COORDINATION.md - Data Flow](./GLOBAL_COORDINATION.md#data-flow-architecture)

### Testing Strategy
- Unit tests by component
- Integration tests by phase
- System tests for full pipeline
- CI/CD with GitHub Actions

See: [GLOBAL_COORDINATION.md - Testing](./GLOBAL_COORDINATION.md#testing-strategy)

### Build System
- CMake 3.21+
- LLVM/Clang 14+
- Qt6 Core/Gui/Widgets/OpenGL
- Graphviz
- nlohmann/json

See: [GLOBAL_COORDINATION.md - Build & CMake](./GLOBAL_COORDINATION.md#build--cmake-structure)

---

## File Organization in Docs

```
docs/
?? GLOBAL_COORDINATION.md       ? Start here!
?? INDEX.md    ? This file
?
?? C2V_COORDINATION.md
?? C2V_DEVELOPER_1_FRONTEND.md
?? C2V_DEVELOPER_2_BACKEND.md
?
?? UI_COORDINATION.md
?? UI_DEVELOPER_1_RENDERING.md
?? UI_DEVELOPER_2_FRAMEWORK.md
?
?? V2C_GUIDE.md
?? V2C_MEMBER_1_CODE_GENERATION.md
?? (V2C_MEMBER_2_*.md - to be created)
```

---

## Document Features

Each developer guide typically includes:

? **Deliverables Overview**
- What you will build
- Timeline for each phase

? **Detailed Tasks**
- Complete code examples
- Header and implementation files
- CMake configuration

? **Testing**
- Unit test examples
- Testing checklist
- Debugging tips

? **Integration**
- Data handoff from other teams
- Signal/slot connections
- Shared structures

? **Performance**
- Target metrics
- Optimization techniques
- Profiling guidance

? **Timeline**
- Week-by-week milestones
- Status tracking table

---

## How to Use These Documents

### First Day
1. Clone repository
2. Read [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)
3. Read your team's coordination guide
4. Read your role-specific developer guide

### Daily
- Reference your developer guide for implementation
- Check testing checklist
- Use debugging tips section

### Weekly
- Review team coordination for dependencies
- Check timeline progress
- Verify data structure compatibility

### Before Integration
- Verify your deliverables against checklist
- Run all unit tests
- Review integration points
- Check with other team

---

## Common Questions

### "What should I implement this week?"
? See your **developer guide** timeline section

### "What data do I receive from Team X?"
? See **COORDINATION** documents - Integration Points section

### "How do my components connect to the UI?"
? See **UI_COORDINATION.md** - Component Layout section

### "What's the project timeline?"
? See **GLOBAL_COORDINATION.md** - Phase Breakdown section

### "How do we handle merge conflicts?"
? See **GLOBAL_COORDINATION.md** - Git strategy section

### "What's the build order?"
? See **GLOBAL_COORDINATION.md** - CMake structure section

---

## Document Maintenance

### When to Update
- After each milestone (end of week)
- When adding team members
- Before phase transitions
- If design decisions change

### Who Should Update
- Team leads (for team coordination)
- Tech lead (for global coordination)
- Individual developers (for their guides)

### Update Process
1. Make changes to document
2. Review with team
3. Commit with message: `[DOCS] Update X coordination`
4. Notify related teams of changes

---

## Integration Checkpoints

### End of Week 1
? C2V Dev 1 delivers ASTParser
- See: [C2V_COORDINATION.md - Handoff 1](./C2V_COORDINATION.md#handoff-1-dev-1--dev-2-end-of-week-1)

### End of Week 2
? C2V Dev 1 delivers StructureExtractor
? UI Dev 1 starts GraphvizRenderer
- See: [C2V_COORDINATION.md - Handoff 2](./C2V_COORDINATION.md#handoff-2-dev-1--dev-2-end-of-week-2)

### End of Week 4
? C2V pipeline complete
? UI Dev 1 delivers GraphWidget
? V2C Dev 1 starts code generation
- See: [GLOBAL_COORDINATION.md - Phase 2](./GLOBAL_COORDINATION.md#phase-2-ui-rendering-weeks-4-6)

### End of Week 6
? UI application complete
? C2V + UI integrated
? V2C Dev 2 starts source rewriting
- See: [GLOBAL_COORDINATION.md - Phase 3](./GLOBAL_COORDINATION.md#phase-3-v2c-synthesis-weeks-5-7)

### End of Week 8
? Complete system working
? All integration tests passing
? Ready for release

---

## Communication Channels

As specified in [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md#communication--meetings):

### Daily (Async)
- Team Slack/Discord channels
- Morning standup updates

### Weekly
- Friday 2 PM UTC - All teams sync
- 30 minutes
- Status + blockers + dependencies

### Bi-Weekly
- Wednesdays 3 PM UTC
- Architecture review
- Tech leads + team leads

---

## Resources

### External Documentation
- LLVM: https://llvm.org/
- Clang: https://clang.llvm.org/
- Qt6: https://doc.qt.io/qt-6/
- Graphviz: https://graphviz.org/
- CMake: https://cmake.org/

### Internal Repository
- Source: https://github.com/YassineBibrine/dataviz-uit
- Branch: `main` (stable)
- Development: Feature branches per team

---

## Summary

This index helps you find:
- **Your specific tasks** - Developer guides
- **Team coordination** - Coordination guides
- **Project context** - Global coordination
- **Integration points** - Relevant sections in each

**Start with [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md), then dive into your team and role-specific guides!**

---

**Document Version**: 1.0  
**Last Updated**: [Current Date]  
**Status**: ? Active

---

## Next Steps

1. **Read**: [GLOBAL_COORDINATION.md](./GLOBAL_COORDINATION.md)
2. **Find**: Your team's coordination guide
3. **Deep Dive**: Your developer guide
4. **Implement**: Follow detailed tasks
5. **Test**: Use provided test examples
6. **Integrate**: Coordinate with other teams

Welcome to DataViz! ??
