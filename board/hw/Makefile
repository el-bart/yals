.SECONDARY:

ifeq ($(VERBOSE),1)
SILENT:=
else
SILENT:=@
endif

SCH:=$(wildcard *.kicad_sch) # expected to be 1 file
PCB:=$(wildcard *.kicad_pcb) # expected to be 1 file
OUT_DIR:=build
GERBER_DIR:=$(OUT_DIR)/gerber
GRAPHICS_DIR:=$(OUT_DIR)/graphics


.PHONY: all
all: gerber
all: stl
all: bom
all: graphics


.PHONY: gerber
gerber: $(GERBER_DIR).tar.gz
gerber: $(GERBER_DIR).zip

$(GERBER_DIR).tar.gz: Makefile
$(GERBER_DIR).tar.gz: $(GERBER_DIR)/version.txt
	$(SILENT)echo "GERBER $(GERBER_DIR).tar.gz"
	$(SILENT)cd "$(dir $@)" && fakeroot tar czf "$(notdir $@)" "gerber/"

$(GERBER_DIR).zip: Makefile
$(GERBER_DIR).zip: $(GERBER_DIR)/version.txt
	$(SILENT)echo "GERBER $(GERBER_DIR).zip"
	$(SILENT)cd "$(dir $@)" && fakeroot zip -r -q "$(notdir $@)" "gerber/"

$(GERBER_DIR)/version.txt: Makefile
$(GERBER_DIR)/version.txt: $(PCB)
	$(SILENT)echo "GERBER $(GERBER_DIR)"
	$(SILENT)mkdir -p "$(GERBER_DIR)"
	$(SILENT)print_on_error kicad-cli pcb export gerbers --output "$(GERBER_DIR)/" "$<"
	$(SILENT)print_on_error kicad-cli pcb export drill --format gerber --output "$(GERBER_DIR)/" "$<"
	$(SILENT)md5sum "$<" > "$(GERBER_DIR)/version.txt"


.PHONY: stl
stl: $(OUT_DIR)/pcb.stl

$(OUT_DIR)/pcb.stl: Makefile
$(OUT_DIR)/pcb.stl: /usr/local/bin/generate_stl
$(OUT_DIR)/pcb.stl: $(PCB)
	$(SILENT)echo "STL $<"
	$(SILENT)mkdir -p "$(OUT_DIR)"
	$(SILENT)generate_stl "$<" "$@"


.PHONY: bom
bom: $(OUT_DIR)/bom.csv

$(OUT_DIR)/bom.csv: Makefile
$(OUT_DIR)/bom.csv: /usr/local/bin/generate_bom
$(OUT_DIR)/bom.csv: $(PCB)
	$(SILENT)echo "BOM $<"
	$(SILENT)mkdir -p "$(OUT_DIR)"
	$(SILENT)generate_bom "$<" "$@"


.PHONY: graphics
graphics: $(GRAPHICS_DIR)/sch.pdf
graphics: $(GRAPHICS_DIR)/sch.svg
graphics: $(GRAPHICS_DIR)/pcb.done

$(GRAPHICS_DIR)/sch.pdf: Makefile
$(GRAPHICS_DIR)/sch.pdf: $(SCH)
	$(SILENT)echo "SCH PDF $<"
	$(SILENT)mkdir -p "$(GRAPHICS_DIR)"
	$(SILENT)print_on_error kicad-cli sch export pdf --no-background-color --exclude-drawing-sheet --output "$@" "$<"

$(GRAPHICS_DIR)/sch.svg: Makefile
$(GRAPHICS_DIR)/sch.svg: $(SCH)
	$(SILENT)echo "SCH SVG $<"
	$(SILENT)mkdir -p "$(GRAPHICS_DIR)"
	$(SILENT)print_on_error kicad-cli sch export svg --no-background-color --exclude-drawing-sheet --output "$(GRAPHICS_DIR)" "$<"
	$(SILENT)mv "$(GRAPHICS_DIR)/$(shell basename "$<" | sed 's/\.kicad_sch//').svg" "$@"

$(GRAPHICS_DIR)/pcb.done: Makefile
$(GRAPHICS_DIR)/pcb.done: /usr/local/bin/list_cu_layers
$(GRAPHICS_DIR)/pcb.done: $(PCB)
	$(SILENT)echo "PCB SVG $<"
	$(SILENT)mkdir -p "$(GRAPHICS_DIR)"
	$(SILENT)for l in $(shell list_cu_layers "$<") ; \
			do \
				print_on_error \
					kicad-cli pcb export svg -l "$$l" --output "$(GRAPHICS_DIR)/pcb_$$l.svg" --exclude-drawing-sheet "$<" ; \
			done
	$(SILENT)print_on_error kicad-cli pcb export drill --format gerber --generate-map --map-format svg --output "$(GRAPHICS_DIR)/" "$<"
	$(SILENT)rm -f "$(GRAPHICS_DIR)"/*drl.gbr
	$(SILENT)cd "$(GRAPHICS_DIR)" && \
			mv *-NPTH-drl-drl_map.svg NPTH-drl_map.svg && \
			mv *-PTH-drl-drl_map.svg  PTH-drl_map.svg
	$(SILENT)touch "$@"

.PHONY: clean
clean:
	rm -rfv "$(OUT_DIR)
