from pathlib import Path

ultralight_path = Path("../dependencies/ultralight-sdk/include")
ultralight_paths = ultralight_path.glob('**/*.h')

for path in ultralight_paths:

    path_str = str(path)
    new_content = ""

    with open(path, 'r') as f:
        depth = len(path.relative_to(ultralight_path).parts) - 1
        for line in f.readlines():
            if line.startswith(('#include <AppCore', '#include <JavaScriptCore', '#include <Ultralight')):
                relative_path = ''.join(['../' for x in range(depth)])
                line = line.replace('#include <', '#include "' + relative_path)
                line = line.replace('>', '"')
            new_content += line

    with open(path, 'w') as f:
        f.write(new_content)