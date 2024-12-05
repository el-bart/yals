.SECONDARY:

ifeq ($(VERBOSE),1)
SILENT:=
else
SILENT:=@
endif

.PHONY: all
all: body sw hw

.PHONY: body
body: Makefile body/build/pcb.stl
	@echo "BUILD body"
	$(SILENT)cd body/ && ./make -j$(shell nproc)

body/build/pcb.stl: board/hw/build/pcb.stl
	@echo "CP $< $@"
	$(SILENT)mkdir -p "$(shell dirname "$@")"
	$(SILENT)cp -ap "$<" "$@"

.PHONY: sw
sw: Makefile
	@echo "BUILD baord's SW"
	$(SILENT)+$(MAKE) --no-print-directory -C board/sw/

.PHONY: hw
hw: Makefile
	@echo "BUILD baord's HW"
	$(SILENT)cd board/hw/ && ./make -j$(shell nproc)

board/hw/build/pcb.stl: hw

.PHONY: clean
clean:
	+$(MAKE) --no-print-directory -C body     clean
	+$(MAKE) --no-print-directory -C board/hw clean
	+$(MAKE) --no-print-directory -C board/sw clean
