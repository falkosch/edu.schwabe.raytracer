# Backlog

Open work items live in `.backlog/*.md`, one topic per file (kebab-case names). Implemented work moves to
`.backlog/DONE/` under the same file name, so an open item and its done parts pair up by name. Don't delete finished
items anymore: `DONE/` replaces the older habit of removing them. Don't recreate items deleted before `DONE/` existed.

## When is something done

- Done means implemented and verified in the code: read or grep the sources, check the tests, check `git log`. A
  decision that was made and applied (e.g. a compiler flag pinned) also counts.
- Markers inside an item (`~~strikethrough~~`, "DONE", status tables) are hints, not proof. Verify them.
- A decision that leaves a follow-up open stays open (e.g. "the SSA optimizer stays off, re-check on newer MSVC").
- Gaps found while verifying (a missing test, a leftover commented-out include) go into the open file as open
  points. Don't count that part as done.

## Cleaning up

- **Whole item done:** `git mv .backlog/<name>.md .backlog/DONE/<name>.md`, then add the header line and the
  resolution/commits section described below.
- **Part of an item done:** carve it out.
    - Move the done sections verbatim into `DONE/<name>.md`.
    - In the open file, change only what the removal forces: a short pointer to `DONE/<name>.md` (usually in the
      Motivation), renumbered lists and sections, stale status tables or strikethrough rows dropped. Don't reword
      the open parts.
    - Keep evidence that open work still needs in the open file, even if it is a finished finding: measurements,
      repros and benchmark tables behind an open or revisitable decision.
- **`DONE/<name>.md` already exists:** append the new done parts to it. Never create a second done file for the
  same item.
- **Cross-references:** fix references in other backlog files ("Related backlog items", "see `<name>.md`") and in
  `CLAUDE.md` files that pointed at moved content.

## Done file layout

Match the existing files in `DONE/`:

```markdown
# <Original title> (done parts)

Carved out of `.backlog/<name>.md`. <One sentence on what stays open there.>

## <Done section, moved verbatim>

### Resolution

<How it was fixed, if the moved text only describes the problem.>

## Commits

- `<short hash>` <commit subject>
```

- Drop "(done parts)" and the "stays open" sentence when the whole item moved.
- Take hashes and subjects from `git log`, not from memory.

## Conventions

- Plain Markdown, LF line endings, wrapped at 120 columns.
- Commit backlog changes separately from code, as `docs(backlog): ...`, and only when asked.
