#!/usr/bin/env python3
import subprocess
import json
import os
import shlex

def generate():
    root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    result = subprocess.run(
        ["make", "-B", "-n"],
        cwd=root_dir,
        capture_output=True,
        text=True,
        check=True
    )
    
    entries = []
    seen = set()
    
    for line in result.stdout.splitlines():
        line = line.strip()
        if not line.startswith("g++ -c"):
            continue
        
        args = shlex.split(line)
        try:
            o_idx = args.index("-o")
            output_file = args[o_idx + 1]
            source_file = args[o_idx + 2]
        except (ValueError, IndexError):
            continue
        
        abs_source = os.path.abspath(os.path.join(root_dir, source_file))
        if abs_source in seen:
            continue
        seen.add(abs_source)
        
        entries.append({
            "directory": root_dir,
            "command": line,
            "file": abs_source,
            "output": output_file
        })
    
    output_path = os.path.join(root_dir, "compile_commands.json")
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(entries, f, indent=2)
    
    print(f"Generated {output_path} with {len(entries)} translation units.")

if __name__ == "__main__":
    generate()
