---
marp: true
math: katex
html: true
size: 4:3
paginate: true
style: |
  section.special {
    background: #f8f7d3ff;
    color: black;
  }
  section {
    font-size: 25pt;
  }
  h2 {
    font-size: 35pt;
    /* text-shadow: 1px 1px 0px #000000;*/
    color: #333;
    background-color: #e1bee7;  /* Yellow background to highlight */
    padding: 0.2em 0.4em;       /* Optional padding for better appearance */
    border-radius: 0.2em;       /* Optional rounded corners */
  }
  h3 {
    font-size: 30pt;
    text-shadow: 1px 1px 0px #000000;
    color: #333;
    background-color: #f6dffaff;  /* Yellow background to highlight */
    padding: 0.2em 0.4em;         /* Optional padding for better appearance */
    border-radius: 0.2em;         /* Optional rounded corners */
  }
  strong {
    text-shadow: 1px 1px 0px #000000;
  }
  @media print {
    strong {
      text-shadow: none !important;
      -webkit-text-stroke: 0.6px rgba(0,0,0,0.35);
      text-stroke: 0.6px rgba(0,0,0,0.35); /* ignored by many, harmless */
    }
  }
  img[alt~="center"] {
    display: block;
    margin: 0 auto;
  }
  img[alt~="outline"] {
    border: 2px solid #388bee;
  }
  .columns {
    display: flex;
    gap: 2rem;
  }
  .column {
    flex: 1;
  }
---

# New Tools & Reused Old Tools

---


## New Tools for AI

- The GUI days are gone, the CUI days are back.
- We don't click buttons anymore; instead, we use CLI to interact with AI actively.
- With AI, it's like working with multiple software engineers at the same time, and we need a CLI (not a GUI) for this goal.

---

### Old Tools are back

- tmux is widely used with Claude code
- vim (nvim) is widely used to give inputs quickly and effectively.
- Git worktree is used for parallel working with AI (isolation among branches).
- VS Code Multi-root Workspace is used for managing multiple workspaces (by AI).

---

## New Approaches of using AI

Prompt: What should the AI do?
Context: What should the AI know?
Harness: How should the AI operate?

Prompt → instructions
Context → information
Harness → orchestration

---

### Harness Engineering

- With the Anthropy source code leaks, we can guess why Anthropic LLM gives better answers.
- It can orchestrate additional tools to get the answers better and quicker.
- In other words, they know how to solve problems using AI effectively, and it is called "Harness Engineering".

---

### The Future of AI Engineering

- It's likely to be using RAG for Harness Engineering.
- It's likely to use local LLM & RAG & Ontology for (relatively) simple and secure work.
- We will purchase LLM/AI computing power when we need higher-level design & architecture.

---

### We need to understand how LLM works

- There are many (amazingly good) resources on the web.
- Anyone can learn how to build an LLM from scratch.
- Building and using LLM can be common sense very soon.
- Building and using RAG can be the most fundamental skills for problem solvers/system builders.

---

## Building the 2nd Brain

- It's not about the tools, it's about how you use your brain to solve problems.
- It's a good idea to start with notes and pens to understand how you learn the best
- We started with the idea I & C (Invite & Control) when we use AI.
- Now, its' ACC (accelerate)

---

### ACC

- Automate
    - Don't do things manually when you can do automate.
    - Ask LLM how to automate
    - Master script languages such as shell/python/ruby for automation

---

- Context & Connect
    - Connect ideas, concepts, patterns, or anything.
    - The connection should be context based
    
So, when we build our 2nd brain, we need to build to make ACC possible using the system.
    
---

### The most important tools for 2nd Brain (ACC approach)

- Git/GitHub
    - You should be able to revert anything back.
- Editor
    - You should be abe to add/delete/manipulate the content as quickly, effectively, and easily as possible.
    - nvim is gaining more and more popularity.
    - any tool is OK as long as (1) it's light and fast and (2) powerful enough to do complicated things.

---

### Then try these

- Obsidian
    - Text based (markdown and YAML)
    - The markdown/YAML file is database content
    - Powerful 3rd party tools (plugins)
    - JavaScript (Electron) based, so you can make your own plugins easily
    - Cross-platform
    - Customaizable for any purpose you need
    - ...

---

- NVIM (neo vim)
    - Light & Fast
    - Based on the good old vim
    - Powerful 3rd party tools (plugins)
    - Lua based, so you can make your own plugins easily
    - Cross-platform
    - Customizatble for any purpose you need
    - ...
    
---

- UNIX Shell script
    - For Linux users, it's built-in
    - For Mac users, it's built-in
    - For windows users, use WLS2
    
Anything can (should be) be automized using anytools (Git/Obsidian/NVIM/Shell ... with AI)

---

## New AI Tools

- LLM Wiki (LLM makes WIki for you)
    - AI legendary Karpathy
    - https://gist.github.com/karpathy/442a6bf555914893e9891c11519de94f
- Mem Palace (AI memory system)
    - Actress Yoyovich (Resident evil actor) created this
    - https://github.com/MemPalace/mempalace
    
---

### Old Ideas but Important

- MCP
- RAG
- Ontology
...

You can learn anything with AI as long as you don't know about them.
