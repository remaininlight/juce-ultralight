from pathlib import Path
from itertools import chain
import re

rpc_path = Path('../dependencies/rpclib/include')
file_types = ['**/*.h', '**/*.hpp']
rpc_files = []
for file_type in file_types:
    files = rpc_path.glob(file_type)
    rpc_files += files

#print(rpc_files)
#rpc_files = {p.resolve() for p in rpc_path.glob("**/*") if p.suffix in [".h", ".hpp"]}
#rpc_files = {p.resolve() for p in Path(path).glob("**/*") if p.suffix in [".c", ".cc", ".cpp", ".hxx", ".h"]}
#ultralight_paths = ultralight_path.glob('**/*.h') 
#ultralight_paths = ultralight_paths.extend(ultralight_path.glob('**/*.hpp'))

for path in rpc_files:

    path_str = str(path)
    new_content = ""

    with open(path, 'r') as f:
        depth = len(path.relative_to(rpc_path).parts) - 2
        relative_path = ''.join(['../' for x in range(depth)])
        for line in f.readlines():
            (line, num_subs) = re.subn('(#\s*include\s*(<|")rpc/)', '#include "' + relative_path, line)
            if num_subs > 0:
                line = line.replace('>', '"')
            '''
            if line.startswith(('#include "rpc',)):
                line = line.replace('#include "', '#include "' + relative_path)
            if line.startswith(('#include <rpc',)):
                line = line.replace('#include <', '#include "' + relative_path)
                line = line.replace('>', '"')
            if line.startswith(('#   include <',)):
                line = line.replace('#   include <', '#include "' + relative_path)
                line = line.replace('>', '"')
            '''
            new_content += line

    with open(path, 'w') as f:
        f.write(new_content)