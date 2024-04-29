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
	mkdir -p build/kernel && cd build/kernel \
	&& cmake -G"Unix Makefiles" ../../src
	mkdir -p target
	mkdir -p target/objs
	mkdir -p target/objs/asm-dump


.PHONY: build-programs
build-programs:
	mkdir -p target/objs/apps
	mkdir -p build/apps && cd build/apps \
	&& cmake -G"Unix Makefiles" ../../programs
	cd build/apps && cmake --build . -- -j 1


.PHONY: build-lib
build-lib:
	mkdir -p build/lib && cd build/lib \
	&& cmake -G"Unix Makefiles" ../../lib/src
	cd build/lib && cmake --build . -- -j 1
	mkdir -p target/objs
	cp build/lib/libv6pptongji.a target/objs/libv6pptongji.a


.PHONY: build-shell
build-shell:
	mkdir -p build/shell && cd build/shell \
	&& cmake -G"Unix Makefiles" ../../shell
	cd build/shell && cmake --build . -- -j 1
	mkdir -p target/objs
	cp build/shell/Shell.exe target/objs/
	objcopy --remove-section .comment target/objs/Shell.exe
	objdump -d target/objs/Shell.exe > target/asm-dump/Shell.exe.text.asm
	objdump -D target/objs/Shell.exe > target/asm-dump/Shell.exe.full.asm


.PHONY: build
build: prepare build-lib build-programs build-shell
	cd build/kernel && cmake --build . -- -j 1


.PHONY: deploy
deploy: build
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
QEMU += -m 32M 
QEMU += -rtc base=localtime 
QEMU += -d cpu_reset -D target/qemu.log 
QEMU += -machine pc 
QEMU += -cpu Icelake-Server 

QEMU_DISK := -boot c -drive file=target/c.img,if=ide,index=0,media=disk,format=raw


.PHONY: qemu-no-rebuild
qemu-no-rebuild:
	$(QEMU) $(QEMU_DISK)


.PHONY: qemug-no-rebuild
qemug-no-rebuild:
	$(QEMU) $(QEMU_DISK) -s -S


.PHONY: qemu
qemu: deploy qemu-no-rebuild


.PHONY: qemug
qemug: deploy qemug-no-rebuild


.PHONY: clean
clean:
	rm -rf ./target
	rm -rf ./build


.PHONY: all
all: deploy
	@echo -e "\033[32mbuild success (unix-v6pp-tongji).\033[0m"

