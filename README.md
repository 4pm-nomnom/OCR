# OCR
EPITA - S3 - API - O.C.R software by 4pm

![Screenshot](https://raw.githubusercontent.com/4pm-nomnom/OCR/master/docs/screenshot.png)  

## Dependencies
* SDL : libsdl2-2.0-0 libsdl2-dev
* SDL-image : libsdl2-image-2.0-0 libsdl2-image-dev
* gtk : libgtk-3-dev libgtk3-0
* gspell : libgspell-1-1 libgspell-1-dev

## Usage
1. Clone the repository : `git clone https://github.com/4pm-nomnom/OCR.git`
2. Go to the OCR folder : `cd OCR`
3. Compile the project : `make all`
4. Launch the program : `./ocr`
5. Follow the instructions given on the screen
6. If you have any issue, do not hesitate to contact us
7. Clean your repository using : `make clean`

## Dataflow
1. Image loading
2. Color removal (gray-scale, then back/white)
3. Pre-processing
4. Text block detection
5. Character detection
6. Identification of extracted characters
7. Text reconstruction

## Contribution
See more in the [CONTRIBUTING](docs/CONTRIBUTING.md) file

## Links
* http://www.debug-pro.com/epita/prog/s3/index.html
* http://neuralnetworksanddeeplearning.com
* https://wiki-prog.infoprepa.epita.fr/index.php/Practical:GIT
* https://slashvar.github.io/2017/02/13/using-gnu-make.html
* https://wiki-prog.infoprepa.epita.fr/index.php/Programmation:C
* https://www.overleaf.com/read/yvqwvgtwzbyr

## Keywords
* supervised machine learning
* multi-layer perceptron
* retropropagation
* gradient optimization
* MLP with single hidden layer

## Notes
* C99 standard
* Compiler used : gcc
