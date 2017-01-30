import sys
import re

if len(sys.argv) < 3:
    sys.exit(-1)

try:
    # First argument is input file name
    input_file_name = sys.argv[1]
    # Try opening input file
    input_file = open(input_file_name, "rt");
    # Try reading it, hope it is not too big
    input_source = input_file.read()
    # Close the file since we are done with it
    input_file.close()
except:
    # Handle opening fail
    print ("Can't open file " + input_file_name)
    sys.exit(-1)

# Now we are ready to process file
# Compile regular expression to
# match[0] == return type
# match[1] == function name
# match[2] == the rest
function_re = re.compile("extern\s+CL_API_ENTRY\s+(.+?)\s+CL_API_CALL\s+(.+?)(\(.+?)\s*CL_[AE]", flags=re.DOTALL)
match = re.findall(function_re, input_source)

if not match:
    # Handle parsing fail
    print ("Can't parse file, try changing regex at cldl.py:23 ")
    sys.exit(-1)

# Prepare header
header = '''#pragma once
#include <CL/cl.h>

struct _CL;

// Load all CL imports
// Return != 0 if succeded, 0 otherwise
extern "C" int LoadImports(struct _CL* cl);
extern "C" void ReleaseImports(struct _CL* cl);

// Structure containing all CL imports
struct _CL
{
    void* handle;

'''

# Start adding function pointers
for func in match:
    header += "\n    struct\n    {\n        union\n        {\n"
    header += "            " + func[0] + "(*" + func[1] + ")" + func[2] + ";\n"
    header += "            void* ptr_" + func[1] + ";\n        };\n    };\n"

# Finalize file
header += "\n};\n"

# Open new file
try:
    output_file_name = sys.argv[2] + ".h"
    output_file = open(output_file_name, "wt")
    output_file.write(header)
    output_file.close()
except:
    # Handle writing fail
    print ("Can't write file " + output_file_name)
    sys.exit(-1)

# Source file
source = "#include <dlfcn.h>\n"
source += "#include \"" + output_file_name + "\"\n\n\n"
source += '''int LoadImports(struct _CL* cl)
{
    cl->handle = dlopen("libOpenCL.so", RTLD_LAZY);

    if (!cl->handle)
    {
        return 0;
    }

'''

for func in match:
    source += "    cl->ptr_" + func[1] + " = dlsym(cl->handle, \"" + func[1] + "\");\n"
    source += "    if (!cl->ptr_" + func[1] + ") return 0;\n"

source += '''
    return 1;
}

void ReleaseImports(struct _CL* cl)
{
    dlclose(cl->handle);
}
'''

# Open new file
try:
    output_file_name = sys.argv[2] + ".cpp"
    output_file = open(output_file_name, "wt")
    output_file.write(source)
    output_file.close()
except:
    # Handle writing fail
    print ("Can't write file " + output_file_name)
    sys.exit(-1)

print("Successfully finished")
