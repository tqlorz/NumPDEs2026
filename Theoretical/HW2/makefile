FIG_DIR = figure
PY_DIR = python
TARGET = main.pdf
OBJS = $(wildcard *.tex)
FIGS = $(wildcard $(FIG_DIR)/*.pdf)
PY = $(wildcard $(PY_DIR)/*.py)
FIGS = $(patsubst $(PY_DIR)/%.py, $(FIG_DIR)/%.pdf, ${PY})

${TARGET}: ${OBJS} ${FIGS}
	make doc

$(FIGS): $(FIG_DIR)/%.pdf: $(PY_DIR)/%.py
	@python3 $<

.PHONY: doc all clean

doc:
	@xelatex -synctex=1 ${OBJS}
	make clean

all: 
	@xelatex -synctex=1 ${OBJS}
	@bibtex main.aux
	@xelatex -synctex=1 ${OBJS}
	@xelatex -synctex=1 ${OBJS}
	make clean

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.run.xml *.blg 
