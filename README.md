# OCR
EPITA - S3 - API - O.C.R software by 4pm

## Dependencies
* libsdl-image1.2-dev
* libsdl1.2-dev
* libgtk2.0-dev

## Usage (for the first defense)
1. Clone the repository : `git clone https://github.com/4pm-nomnom/OCR.git`
2. Go to the OCR folder : `cd OCR`
3. Compile the project : `make all`
4. Test image segmentation : `./main samples/light_on_orange.png`
5. Test the xor implementation : `./xorgate/xor`
6. clean the repository using : `make clean`

## Progam Specifications
* Input : Image
* Output : Text

## Dataflow
1. Image loading
2. Color removal (gray-scale, then back/white)
3. Pre-processing
4. Text block detection
5. Character detection
6. Identification of extracted characters
7. Text reconstruction

# Work on the project
1. Go to the OCR folder : `cd OCR`
2. update your local version : `git pull`
3. Go to your branch : `git checkout branch-name`
4. Work on it <3
5. Commit your work:
    -   See changes done : `git status`
    -   add changes to be commited : `git add files-name`
    -   Commit your work : `git commit -s -m "nice description"`
    -   Push your work : `git push`

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