SECTIONS = ${wildcard sections/*.tex}
PYFIGS = ${wildcard figures/*.ps.py}
GFIGS = $(PYFIGS:.ps.py=.ps.py.tex)
FIGURES = ${wildcard figures/*.tex}
SETUP = ${wildcard setup/*.tex}
BIB = ${wildcard *.bib}
MAIN = poster

.PHONY: clean, figs

build: ${MAIN}.pdf

run: ${MAIN}.pdf
	open ${MAIN}.pdf

clean:
	rm -f *.blg *.log *.out *.aux *.bbl
	rm -f ${MAIN}.pdf
	rm -f ${GFIGS}

figs: ${GFIGS}
	echo ${GFIGS}

figures/%.ps.py.tex: figures/%.ps.py
	python $< > $@ 

${MAIN}.bbl: ${GFIGS} ${MAIN}.tex ${BIB}
	xelatex  ${MAIN}
	bibtex ${MAIN}

${MAIN}.pdf: ${MAIN}.tex ${BIB} ${MAIN}.bbl ${SETUP} ${FIGURES} ${SECTIONS} ${GFIGS}
	xelatex  ${MAIN}
	xelatex  ${MAIN}
