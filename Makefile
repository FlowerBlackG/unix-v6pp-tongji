# Unix V6++ Tongji 顶层构建脚本
#
# 创建于 2024年4月28日 上海市嘉定区
# by 2051565 GTY


.DEFAULT_GOAL := all


.PHONY: help
help:
	@echo "unix-v6pp makefile"
	@echo "---------------"
	@echo "commands available:"
	@echo "- make bochs"
	@echo "    build and launch unix-v6pp using Bochs"
	@echo "- make qemu"
	@echo "    build and launch unix-v6pp using QEMU"
	@echo "- make qemug"
	@echo "    build and launch unix-v6pp using QEMU (with GDB)"
	@echo "- make"
	@echo "    alias for \"make all\""


.PHONY: prepare
prepare:
	mkdir -p target/objs/asm-dump


.PHONY: build-programs
build-programs: prepare
	mkdir -p target/objs/apps
	mkdir -p build/apps && cd build/apps \
	&& cmake -G"Ninja" ../../programs && \
	cmake --build . -- -j 4


.PHONY: build-lib
build-lib: prepare
	mkdir -p build/lib && cd build/lib \
	&& cmake -G"Ninja" ../../lib/src \
	&& cmake --build . -- -j 4
	mkdir -p target/objs
	cp build/lib/libv6pptongji.a target/objs/libv6pptongji.a


.PHONY: build-shell
build-shell: prepare
	mkdir -p build/shell && cd build/shell \
	&& cmake -G"Ninja" ../../shell && \
	cmake --build . -- -j 2
	mkdir -p target/objs
	cp build/shell/Shell.exe target/objs/
	objcopy --remove-section .comment target/objs/Shell.exe
	objdump -d target/objs/Shell.exe > target/asm-dump/Shell.exe.text.asm  # optional
	objdump -D target/objs/Shell.exe > target/asm-dump/Shell.exe.full.asm  # optional


.PHONY: build-kernel
build-kernel: prepare
	mkdir -p build/kernel && cd build/kernel \
	&& cmake -G"Ninja" ../../src && \
	cmake --build . -- -j 1


.PHONY: build-full
build-full: prepare build-lib build-programs build-shell build-kernel



.PHONY: deploy-full
deploy-full: build-full
	mkdir -p target/img-workspace
	mkdir -p target/img-workspace/programs/bin
	mkdir -p target/img-workspace/programs/etc
	cp target/objs/kernel.bin target/img-workspace/
	cp target/objs/boot/boot.bin target/img-workspace/
	cp target/objs/apps/* target/img-workspace/programs/bin/
	cp target/objs/Shell.exe target/img-workspace/programs/
	cp tools/unix-v6pp-filesystem-editor/bin/* target/img-workspace/
	cd target/img-workspace && ./filescanner | ./fsedit c.img c
	cp target/img-workspace/c.img target/



.PHONY: bochs
bochs:
	@echo 'not supported. use \"make qemu\" instead.'



QEMU := qemu-system-i386 
QEMU += -m 64M 
QEMU += -rtc base=localtime 
QEMU += -d cpu_reset -D target/qemu.log 
QEMU += -machine pc 
QEMU += -cpu Icelake-Server 
QEMU_GDB := -chardev socket,path=target/qemu-gdb.sock,server=on,wait=off,id=gdb0 
QEMU_GDB += -gdb chardev:gdb0 -S 

QEMU_DISK := -boot c -drive file=target/c.img,if=ide,index=0,media=disk,format=raw


.PHONY: qemu-no-rebuild
qemu-no-rebuild:
	$(QEMU) $(QEMU_DISK)


.PHONY: qemug-no-rebuild
qemug-no-rebuild:
	$(QEMU) $(QEMU_DISK) $(QEMU_GDB)


.PHONY: qemu
qemu: deploy-full qemu-no-rebuild


.PHONY: qemug
qemug: deploy-full qemug-no-rebuild


.PHONY: clean
clean:
	rm -rf ./target
	rm -rf ./build


.PHONY: all
all: deploy-full
	@echo -e "\033[32mbuild success (unix-v6pp-tongji).\033[0m"


.PHONY: full
full: deploy-full
