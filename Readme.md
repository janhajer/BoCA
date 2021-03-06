# Readme

This code performs staged BDT analyses using root files as in- and output format.

## Dependencies

* [`ROOT`](https://root.cern.ch/) version ≥ 6.05.02 and compiled with [`TMVA`](http://tmva.sourceforge.net/) and [`MathMore`](https://root.cern.ch/mathmore-library)
* [`fastjet`](http://fastjet.fr/)
* [`fastjet contrib`](https://fastjet.hepforge.org/contrib/) (must be compiled with [`CXXFLAGS=-fPIC`](https://en.wikipedia.org/wiki/Position-independent_code))
* [`Boost`](http://www.boost.org/) ([`Optional`](http://www.boost.org/doc/libs/release/libs/optional/), [`Range`](http://www.boost.org/doc/libs/release/libs/range/), [`Units`](http://www.boost.org/doc/libs/release/libs/units/), [`Operators`](http://www.boost.org/doc/libs/release/libs/utility/operators.htm))

The BoCA code makes heavy use of modern C++ features.
The limiting factor for the minimal supportet compiler version is given by the use of return type deduction.
Therefore the compiler version must be
* GCC version ≥ 4.9
* Clang version ≥ 3.3

## Installation

* the following is tested on recent ubuntu and mac installations

~~~~
mkdir build
cd build
cmake ..
make
~~~~

* for quicker compilation (install ccache and ninja-build)

~~~~
mkdir build
cd build
cmake -G Ninja ..
ninja
~~~~

* for mac (still install ccache)

~~~~
mkdir build
cd build
cmake -G Xcode ..
xcodebuild
~~~~

* if you have trouble installing `ROOT` ≥ 6 with `GCC` ≥ 5 you can configure it with

~~~~
cmake -DCMAKE_CXX_FLAGS=-D_GLIBCXX_USE_CXX11_ABI=0 -Dmathmore=ON -Dbuiltin_gsl=ON -GNinja..
~~~~

## Usage

> Examples are listed [here](https://boostedcollideranalysis.github.io/BoCA/examples.html).

Each analysis needs a dedicated `Analysis` class, a set of `Tagger` classess and a `main.cpp` file.
The `Analysis` class contains the global information of the analysis, such as file names, path names, variables names etc.
Every `Tagger` class tags or reconstructs one specific particle or signature. Each analysis needs at least one dedicated `Tagger`.
The `main.cpp` defines the order in which each `Tagger` is passed to the `Analysis`.

## Source code documentation

The source code documentation including all classes and functions can be found [here](http://boostedcollideranalysis.github.io/BoCA).

## Standard model

The acceptances for Standard model particles at a future 100 TeV collider are presented [here](doc/standard-model.md).
These plots represent some examples of the power of this code.

## Reference

If you use this code to publish analyses please cite:
* [Jan Hajer, Ying-Ying Li, Tao Liu, John F. H. Shiu: *Heavy Higgs Bosons at 14 TeV and 100 TeV*, April 2015](https://inspirehep.net/record/1365110)
* [Nathaniel Craig, Jan Hajer, Ying-Ying Li, Tao Liu, Hao Zhang: *Heavy Higgs Bosons at Low tan β: from the LHC to 100 TeV*, May 2016](https://inspirehep.net/record/1466130)

## License

BoCa is licenced under the [GPL 3](doc/License.md).

## Git commands

* initial download of the project `git clone git@github.com:BoostedColliderAnalysis/BoCA.git`
* downloading updates from the server `git pull`
* save changes of specific files `git commit FILES`
* save changes in to all files `git commit -a`
* upload changes to server `git push`
* change branches `git checkout BRANCH`
* create new branch `git checkout -b NEWBRANCH`
* merge branches `git merge BRANCH`
* save unfinished work on stash `git stash`
* get stash changes back `git stash pop`

## Coding style

* use standard library when possible
* avoid heap allocation `new`
* prefer references `&` over pointers `*`
* Classes and Functions are written in CamelCase
* variables are written small_with_underscore
* member_variables_ and TemplateParamter_ end on an underscore
