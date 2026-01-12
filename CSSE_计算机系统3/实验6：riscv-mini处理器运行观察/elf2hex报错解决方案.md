计系3实验运行elf2hex报错解决方案

实例：

user@ubuntu:~/Desktop/Exp6/riscv-mini$ elf2hex 16 4096 test > test.hex
elf2hex: error while loading shared libraries: libfesvr.so: cannot open shared object file: No such file or directory



#### 步骤 1：检查 `libfesvr.so` 的依赖（是否有其他缺失库）

`libfesvr.so` 自身可能依赖其他库，若依赖缺失，也会导致 `ldd` 显示 `not found`（误报）

```bash
ldd /home/user/Downloads/riscv-tools-priv1.7/riscv-tools-mini/lib/libfesvr.so
```

若输出中有 `not found` 的依赖（比如 `libstdc++.so.6`/`libgcc_s.so.1`）：

✅ 解决：安装缺失的依赖（以 `libstdc++.so.6` 为例）：

```
sudo apt install libstdc++6
```

若输出全是 `=> /lib/x86_64-linux-gnu/...`（无 not found）：说明库自身依赖正常，进入步骤 2。



#### 步骤 2：给 `elf2hex` 手动设置 RPATH（永久解决运行时库路径）

`elf2hex` 编译时未配置 RPATH（运行时自动查找库的路径），导致即使 `LD_LIBRARY_PATH` 配置了，也无法找到库。用 `patchelf` 工具修复：

```bash
# 1. 安装 patchelf（修改 ELF 文件的 RPATH 工具）
sudo apt install patchelf

# 2. 给 elf2hex 设置 RPATH（指向 libfesvr.so 所在目录）
patchelf --set-rpath /home/user/Downloads/riscv-tools-priv1.7/riscv-tools-mini/lib \
/home/user/Downloads/riscv-tools-priv1.7/riscv-tools-mini/bin/elf2hex

# 3. 验证 RPATH 是否设置成功
patchelf --print-rpath /home/user/Downloads/riscv-tools-priv1.7/riscv-tools-mini/bin/elf2hex
```

正常输出：`/home/user/Downloads/riscv-tools-priv1.7/riscv-tools-mini/lib`。

#### 步骤 3：重新检查 `elf2hex` 的依赖（此时应正常）

```bash
ldd /home/user/Downloads/riscv-tools-priv1.7/riscv-tools-mini/bin/elf2hex | grep fesvr
```

✅ 预期输出：`libfesvr.so => /home/user/.../libfesvr.so (0x00007fxxxxxx)`。

#### 步骤 4：执行 elf2hex 命令（此时必生效）

```bash
elf2hex 16 4096 test > test.hex
```