<rules>
# Project Core Rules & Language Settings

## 1. Language & Output Control (CRITICAL)
* All outputs (responses, documentation, error explanations, code comments, commit messages) MUST be in Japanese.
* When asking the user for permission to execute a command, you MUST briefly explain its purpose and content in Japanese before asking for confirmation.
* Define the "project root" as the nearest ancestor directory containing `.git`, `Dockerfile`, or `docker-compose.yml`. If none exist, use the current directory.
* Save all generated artifacts and documents in the `docs/` folder directly under the project root, strictly using the naming convention: `YYYYMMDD_topic_name.md`.
* IMPORTANT CONTEXT SAVING: After generating and writing a document, ONLY output the completion status and the file path in the chat. DO NOT output the full content of the document in the chat window.

## 2. Workflow Orchestration
* **Plan Node Default**: Enter plan mode for ANY non-trivial task (3+ steps or architectural decisions). If requirements are ambiguous, use the AskUserQuestion tool to interview the user BEFORE planning. If something goes sideways, STOP and re-plan immediately. Write detailed specs upfront to reduce ambiguity.
* **Subagent Strategy**: Use subagents liberally to keep the main context window clean. Offload research, exploration, and parallel analysis to subagents. For complex problems, throw more compute at it via subagents. One task per subagent for focused execution.
* **Self-Improvement Loop**: After ANY correction from the user: update `tasks/lessons.md` with the pattern. Write rules for yourself that prevent the same mistake. Ruthlessly iterate on these lessons until the mistake rate drops. Review lessons at session start for the relevant project.
* **Verification Before Done**: Never mark a task complete without proving it works. Diff behavior between main and your changes when relevant. Ask yourself: "Would a staff engineer approve this?" Run tests, check logs, demonstrate correctness.
* **Demand Elegance (Balanced)**: For non-trivial changes: pause and ask "is there a more elegant way?". If a fix feels hacky: "Knowing everything I know now, implement the elegant solution". Skip this for simple, obvious fixes – don't over-engineer. Challenge your own work before presenting it.
* **Autonomous Bug Fixing**: When given a bug report: just fix it. Don't ask for hand-holding. Point at logs, errors, failing tests – then resolve them. Zero context switching required from the user. Go fix failing CI tests without being told how.

## 3. Task Management
When executing tasks, strictly follow these steps:
1. **Plan First**: Write phase-wise gated plan to `tasks/todo.md` with checkable items.
2. **Verify Plan**: Check in before starting implementation.
3. **Track Progress**: Mark items complete as you go.
4. **Explain Changes**: High-level summary at each step (in Japanese).
5. **Document Results**: Add review section to `tasks/todo.md`.
6. **Capture Lessons**: Update `tasks/lessons.md` after corrections.

## 4. Core Principles
* **Simplicity First**: Make every change as simple as possible. Impact minimal code.
* **Security First**: NEVER hardcode API keys, passwords, or tokens. Always use environment variables (`.env`) and follow secure coding practices.
* **Meaningful Tests**: Write tests covering happy paths, error paths, and boundary values. Do not write tautological (always passing) tests.
* **No Laziness**: Find root causes. No temporary fixes. Senior developer standards.
* **Minimal Impact**: Changes should only touch what's necessary. Avoid introducing bugs.
* **YAGNI (You Aren't Gonna Need It)**: Treat implementation of features not included in the Acceptance Criteria as defects. Do not write code or tests for unrequested features.
* **Deletion Priority**: Before writing new code, always consider if the problem can be solved by refactoring or deleting existing code.

## 5. Project Specific Commands (Essential)
When executing tests, builds, or running the application, strictly use the following commands:
* **Install**: `npm install` (or your specific install command)
* **Build**: `npm run build` (or your specific build command)
* **Test**: `npm run test` (or your specific test command)
* **Lint**: `npm run lint` (or your specific lint command)
</rules>