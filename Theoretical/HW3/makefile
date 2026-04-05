TARGET = main.pdf
OBJS = $(wildcard *.tex)

${TARGET}: ${OBJS}
	make doc

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
