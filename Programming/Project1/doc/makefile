OBJECT = $(wildcard *.tex)
PYTHON = $(wildcard python/*.py)
FIGURE = $(patsubst python/%.py, figure/%.pdf, ${PYTHON})

main.pdf: ${OBJECT} ${FIGURE}
	make doc

$(FIGURE): figure/%.pdf: python/%.py
	@python3 $<

.PHONY: doc all clean

doc:
	@xelatex -synctex=1 ${OBJECT}
	make clean

all: 
	@xelatex -synctex=1 ${OBJECT}
	@bibtex main.aux
	@xelatex -synctex=1 ${OBJECT}
	@xelatex -synctex=1 ${OBJECT}
	make clean

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.run.xml *.blg 
