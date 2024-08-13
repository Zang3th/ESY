MD_FILES=$(wildcard *.md)
PDF_FILES=$(subst .md,.pdf,${MD_FILES})

FIGS=$(wildcard *.fig)
SVGS=$(wildcard *.svg)
F_PNGS=$(subst .fig,.png,${FIGS})
S_PNGS=$(subst .svg,.png,${PNGS})

all: $(PDF_FILES) $(F_PNGS) $(S_PNGS)

%.png: %.fig
	fig2dev -L png -m 4 $< $@

%.png: %.svg
	inkscape -z -T -E $@ $<

%.pdf: %.md
	pandoc $< -o $@ -s --toc --number-sections \
		-V lang=de-DE \
		-V breakurl -V hyphens=URL -V colorlinks \
		-V geometry=a4paper,left=3cm,right=2cm,top=2cm,bottom=2cm

