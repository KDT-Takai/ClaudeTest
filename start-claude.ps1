$env:ANTHROPIC_AUTH_TOKEN = "lmstudio"
$env:ANTHROPIC_BASE_URL = "http://localhost:1234"
$env:ANTHROPIC_DEFAULT_HAIKU_MODEL = "omnicoder-qwen3.5-9b-claude-4.6-opus-uncensored-v2"
$env:ANTHROPIC_DEFAULT_OPUS_MODEL = "omnicoder-qwen3.5-9b-claude-4.6-opus-uncensored-v2"
$env:ANTHROPIC_DEFAULT_SONNET_MODEL = "omnicoder-qwen3.5-9b-claude-4.6-opus-uncensored-v2"
$env:CLAUDE_CODE_SUBAGENT_MODEL = "omnicoder-qwen3.5-9b-claude-4.6-opus-uncensored-v2"
$env:CLAUDE_CODE_ATTRIBUTION_HEADER = "0"
$env:CLAUDE_CODE_ENTRYPOINT = "cli"
$env:CLAUDE_CODE_EXECPATH = "C:\Program Files\nodejs\node.exe"
$env:CLAUDECODE = "1"
$env:CLAUDE_CODE_USE_POWERSHELL_TOOL = "1"

bunx @anthropic-ai/claude-code@latest --chrome