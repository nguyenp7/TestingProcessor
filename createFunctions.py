# gen_funcs.py
num_funcs = 2048
nop_count = 40

with open("funcs.h", "w") as f:
    # 1. Write the function definitions
    for i in range(num_funcs):
        f.write(f'void __attribute__((aligned(64))) f{i}() {{ __asm__ volatile (')
        # Generate NOPs
        f.write('"nop\\n\\t" ' * nop_count)
        f.write('); }\n')
    
    # 2. Write the array definition inside the header
    f.write(f"\nvoid (*funcs[{num_funcs}])() = {{\n    ")
    for i in range(num_funcs):
        f.write(f"f{i}")
        if i < num_funcs - 1:
            f.write(", " if (i + 1) % 12 != 0 else ",\n    ")
    f.write("\n};")
    