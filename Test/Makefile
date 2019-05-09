ODIR = out
.PHONY : all
all: $(ODIR)/Makefile
	$(MAKE) -C $(ODIR) all

$(ODIR)/Makefile:
	mkdir -p $(ODIR)
	cd $(ODIR) && cmake ../

.PHONY : clean
clean:
	-rm -rf $(ODIR) *~

.PHONY: run
run: all
	$(ODIR)/test_apis -c -v
	$(ODIR)/test_tasks -c -v

.PHONY: style
style_FILES = $(wildcard *.c *.h *.cpp)
style:
	$(S)clang-format -i $(style_FILES)
