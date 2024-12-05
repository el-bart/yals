.SECONDARY:

ifeq ($(VERBOSE),1)
SILENT:=
else
SILENT:=@
endif

.PHONY: all
all: build

.PHONY: build
build: Makefile
	@echo "BUILD servo_ctrl"
	$(SILENT)SERVO_CTRL_NO_CLEAR=true ./compile_and_flash

.PHONY: flash
flash: Makefile
	@echo "FLASH servo_ctrl"
	$(SILENT)SERVO_CTRL_NO_CLEAR=true ./compile_and_flash "build/rp2040/servo_ctrl.elf"

.PHONY: clean
clean:
	@echo "CLEAN servo_ctrl"
	$(SILENT)rm -rf build
