# OCR CONTRIBUTING
EPITA - S3 - API - O.C.R software by 4pm

# Work on the project
The best way to keep a working executable all the time is by using tests
and branches. These tools are at your disposal, do not hesitate to push
your work using them. It is really not an issue to have something not
finished using those tools.

# Tests
0. Clone the repo if it wasn't already done
    -   `git clone https://username@github.com/4pm-nomnom/OCR.git`
    -   (see [README.md](../README.md))
1. Go to the OCR folder : `cd OCR`
2. update your local version : `git pull`
3. Go to the tests folder : `cd tests`
4. See in the Makefile of this folder for informations on the way to test
5. Work on your tests
6. Commit your work:
    -   See changes done : `git status`
    -   add changes to be commited : `git add files_name`
    -   Commit your work : `git commit -s -m "nice description"`
    -   Push your work : `git push`

# Branches
0. Clone or update the OCR repository
1. Create your branch using : `git branch branchName`
2. Go to this branch : `git checkout branchName`
3. Work on the project
4. Commit your work:
    -   See changes done : `git status`
    -   add changes to be commited : `git add files_name`
    -   Commit your work : `git commit -s -m "nice description"`
    -   Push your work & branch: `git push --set-upstream origin branchName`
