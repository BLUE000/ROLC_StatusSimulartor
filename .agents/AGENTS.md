# Project Rules for ROLC_StatusSimulator

## WIP_STATE.md Management Rule
- `WIP_STATE.md` MUST ALWAYS start with the exact notice header:
```markdown
# 注意
このファイルは現在状態のみを保持する。
履歴を残してはならない。
完了済み作業を残してはならない。
常に最新状態へ置き換えること。
過去の情報が残っていたら過去の情報は削除してください。
```
- NEVER include historical logs, completed task history, or old milestones in `WIP_STATE.md`.
- ONLY keep current active status and upcoming next steps.

## Memo and Reference Rule
- NEVER rely on memory or assumptions when checking data structures, class trees, or specifications. ALWAYS inspect local reference files (e.g. `tmp/` memos or authoritative test files) first.
- Keep temporary/personal reference notes in `tmp/` (git-ignored). DO NOT add temporary workspace notes to official V-model specification documents (`docs/`) unless explicitly requested by the user.
