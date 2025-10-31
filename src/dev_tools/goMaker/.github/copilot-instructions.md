# AI Agent Instructions

## 1. Environment & Commands

### Package Management (ZERO TOLERANCE)
- **YARN ONLY** - Never use `npm` or `npx`
- **fish shell** - Generate commands for fish, not bash
- All commands run from repo root: `yarn start`, `yarn build`, `yarn test`, `yarn lint`

### Critical Workflow
- **DO NOT RUN YARN COMMANDS** - Never run `yarn lint`, `yarn test`, `yarn start`, or any other yarn commands unless explicitly requested by the user
- **EXCEPTION**: Validation commands (`yarn lint && yarn test`) are acceptable after major architectural changes for verification (at the end of the entire plan)
- **Read file contents first** before editing - files change between requests, never assume previous knowledge is current
- **After backend changes**: Run `wails generate module` to update TypeScript bindings
- **File deletion**: Use `rm -f` for files, `rm -R` for folders, ask confirmation, then explicitly remove from context memory
- **Context updates**: When user renames/refactors methods or structures, immediately update understanding and mental model

### Step-by-Step Mode (Alternative Rules of Engagement)
When I say "We want to go into step-by-step" mode, switch to these rules:

🚫 **Never Run:**
- `yarn lint`
- `yarn test`
- `yarn start`

🛑 **Stop Between Steps:**
- Never run amok or jump ahead
- Stop after each step for your review and approval
- Wait for you to say "go ahead" before proceeding

📋 **Planning Process:**
- Show you what I want to do WITHOUT modifying code first
- Explain WHY I want to make each change
- Wait for your approval before making any code changes
- One step at a time with your review

🔒 **PERSISTENCE RULE (CRITICAL):**
- **ONCE IN STEP-BY-STEP MODE, STAY THERE INDEFINITELY**
- Do NOT fall back to normal mode unless explicitly told "exit step-by-step mode"
- Step-by-step mode persists across multiple requests and conversations
- Every action must be approved individually, no exceptions

### Design Mode (Discussion + Analysis)
When I say "Let's go into design mode" or similar, switch to these rules:

📋 **Lightweight Discussion:**
- NO full codebase scans or comprehensive exploration upfront
- NO telling user about current code state unless specifically asked
- Discussion-focused mode for architectural analysis and planning

🔍 **Just-in-Time File Reading:**
- Before discussing ANY specific file, component, or implementation detail, ALWAYS read that file first
- Check file contents immediately before referencing to ensure accuracy
- Never assume file contents based on previous knowledge - files change between requests
- Only read files when discussion requires specific implementation details

💭 **Discussion + Analysis:**
- Architectural analysis and conceptual discussion
- NO code modifications, builds, tests, or implementations
- Focus on design decisions, trade-offs, and approaches
- Reference actual code patterns only when files have been read

🎯 **Focus Areas:**
- Architecture decisions and trade-offs
- Implementation approaches and design patterns
- Problem analysis and technical feasibility
- Requirements clarification and solution evaluation

📋 **Output Format:**
- Bullet points and structured analysis
- Pros/cons of different approaches
- Clear recommendations with reasoning
- Questions to clarify requirements

🔒 **NO MODIFICATION RULE:**
- **ONCE IN DESIGN MODE, STAY THERE INDEFINITELY**
- Must explicitly exit design mode before making any code changes
- Do NOT fall back to normal mode unless explicitly told "exit design mode"
- Design mode persists across multiple requests and conversations
- If asked to implement something in design mode, respond: "Still in design mode - should I exit design mode and implement this?"

### Mode Switching Rules (CRITICAL)
- **MUTUALLY EXCLUSIVE**: You can NEVER be in both design mode and step-by-step mode simultaneously
- **Only two ways to exit each mode:**
  1. Explicit command: "exit [mode-name] mode"
  2. Command to enter the other mode: "go into [other-mode] mode"
- **Mode persistence**: Once in a mode, stay there across all requests and conversations until explicitly changed

### VS Code Problems Server Reset
When VS Code shows stale errors for deleted files or incorrect TypeScript diagnostics:
```bash
# Method 1: Restart TypeScript language server (most common)
Cmd+Shift+P → "TypeScript: Restart TS Server"

# Method 2: Reload VS Code window (nuclear option)
Cmd+Shift+P → "Developer: Reload Window"

# Method 3: Clear workspace state and reload
Cmd+Shift+P → "Developer: Reset Workspace State" → "Developer: Reload Window"
```

## 2. Codebase Architecture

### Wails Structure
- **Go backend** (`app/`): API handlers, business logic, viewConfig
- **React frontend** (`frontend/src/`): Facet-based views, Mantine UI
- **Auto-generated models**: TypeScript types from Go structs (`frontend/wailsjs/go/models.ts`)
- **Direct CGO bridge**: JS ↔ Go function calls in same process

### Code Generation Systems
1. **Wails bridge**: Auto-generates frontend/backend bindings
2. **Our codegen**: Protects manual code within `// EXISTING_CODE` pairs
3. **Design docs**: See `./design/` folder for architectural guidance

### Data Flow Architecture
- **Streaming/decoupled fetching**: Pages appear immediately, data streams in progressively
- **Reference**: store, facet, and concrete collection types in Go packages
- **Collection/Store/Page** pattern with auto-generated types like `monitors.MonitorsPage`

### Key Patterns
- **DataFacet enum**: Use `types.DataFacet.*` values, never custom strings
- **ViewStateKey**: `{ viewName: string, tabName: types.DataFacet }`
- **Imports**: From `@models`, `@components`, `@utils`, `@hooks`
- **No React imports** (implicitly available)
- **Use `Log` from `@utils`** instead of console.log (invisible in Wails)

## 3. Development Principles

### Code Quality (CRITICAL)
- **No over-engineering**: Simple, boring code that works beats complex "elegant" solutions
- **STOP and THINK**: Ask "What's the simplest solution?" before coding
- **If solution has >3 moving parts**, it's probably over-engineered
- **No `any` in TypeScript** - always use specific types
- **No comments in production code** - only for TODO items

### Discovery-Driven Development
- **Implementation may reveal better solutions**: Be open to pivoting when simpler approaches emerge
- **Redundancy elimination**: When multiple approaches exist, prefer removing redundant functionality over maintaining both
- **Document architectural decisions**: When discovering superior patterns, explain why the change improves the system
- **Progressive understanding**: Build context incrementally through targeted investigation rather than broad scanning

### Collaboration Protocol
- **Ask early, ask often**: When complexity starts creeping in, stop and discuss
- **Own mistakes**: Don't blame "someone" - broken code is my responsibility
- **Use existing utilities first** - check `@utils` before creating new ones
- **Stop conditions**: Test failures, lint errors, unclear requirements - stop and report

### Systematic Removal Protocol
When removing functionality from the codebase, follow dependency order to prevent broken references:
1. **Interfaces first**: Remove method signatures from interfaces/contracts
2. **Templates**: Update code generation templates to stop producing the functionality
3. **Regenerate**: Run code generation to update all generated files
4. **App layer**: Remove from main application API methods
5. **Frontend**: Update React components and hooks
6. **Bindings**: Regenerate Wails TypeScript bindings
7. **Tests**: Remove or update test cases
8. **Validation**: Run full test suite to confirm clean removal

### Validation Protocol
- **After major architectural changes**: Always run `yarn lint && yarn test` for comprehensive validation
- **Green build requirement**: All changes must pass linting and full test suite before completion
- **Test-driven confidence**: Use test results to confirm architectural changes work as intended

### React Timing Issues Protocol (CRITICAL - 50% of bugs are timing-related)
- **ASSUMPTION**: Any unexpected UI behavior is likely a React timing/async issue
- **IMMEDIATE RESPONSE**: Add comprehensive logging using `Log` from `@utils` throughout the entire data flow
- **Required logging points**: Action start → State change → API call → Data received → Component re-render → Final result
- **Timing scenarios to suspect**:
  - Data appears/disappears unexpectedly (optimistic updates vs. real data)
  - Navigation happens but selection is wrong (page change vs. data loading)
  - Actions work sometimes but not others (race conditions)
  - State seems "one step behind" (useEffect dependency issues)
- **Debug pattern**: Log every async boundary with timestamps and data snapshots
- **Never assume component state is synchronous** - always consider useEffect timing and data loading phases

### Race Condition Prevention
- **Sequential over parallel**: Avoid Promise.all() with state-modifying operations
- **Common scenarios**: Multiple API calls updating same backend state, parallel component store calls
- **When in doubt**: Use await chains instead of parallel operations

## 4. Critical Technical Details

### Header Actions Contract
- **Backend**: Every facet config defines `HeaderActions []string` (never nil, use `[]` if empty)
- **Data-table facets**: Must include `export` in HeaderActions
- **Frontend**: Assume `config.headerActions` is always array - no null checks, only length checks
- **Action alignment**: Backend identifiers must match frontend `ActionType` values from `useActions()`

### Component Usage
```tsx
<BaseTab
  data={pageData?.monitors || []}
  columns={getColumns(getCurrentDataFacet())}
  viewStateKey={viewStateKey}
  loading={pageData?.state === types.StoreState.FETCHING || false}
  error={error}
  onSubmit={handleSubmit}
  onDelete={handleDelete}
/>
```

### View Architecture Pattern
```
views/[viewname]/
├── [ViewName].tsx     # Main component: Imports, Hooks, Handlers, Render
├── facets.ts          # DataFacet configurations and routing
├── columns.ts         # Table column definitions per facet
└── index.ts           # Exports
```

### Code Placement Rules
- **Go files**: Package declarations first, proper import grouping
- **TypeScript**: Imports grouped properly, respect class/function boundaries
- **Include sufficient context** in oldString for precise placement
- **Never insert before package lines** or between import statements

### File Structure References
- `app/`: Go backend, API handlers (`api_*.go`), business logic, viewConfig
- `frontend/src/views/`: Facet-based React views and renderers
- `frontend/src/components/`: UI components (BaseTab, Table, etc.)
- `frontend/wailsjs/go/models.ts`: Auto-generated TypeScript types
- `pkg/`: Go packages for backend functionality

### Development Commands
```bash
yarn start              # Wails dev mode
yarn build             # Production build  
yarn test              # All tests (Go + TypeScript)
yarn lint              # Lint Go and TypeScript
yarn test-go           # Go backend tests only
yarn test-tsx          # Frontend tests only

# Go build commands - redirect output to /tmp
go build -o /tmp/app .              # Build main app to /tmp
go build -o /tmp/pkg ./pkg/...      # Build packages to /tmp
```