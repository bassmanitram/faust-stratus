# faust-stratus
A Faust architecture file targeting the Chaos Stratus pedal

Your most important resources:

* [Faust](https://faust.grame.fr/) - [do the YouTube workshops!](https://ccrma.stanford.edu/~rmichon/faustWorkshops/course2015)
* [Stratus Developers Portal](https://github.com/chaosaudio/Dev-Portal)
* [The Faust Architecture File for The Chaos Strauss](./stratus.cpp)
* [A Python wrapper for any Faust-developed Stratus effect built using `stratus.cpp`](./stratus.py). See below for details

## Usage
For the moment you have to have the Faust compiler installed locally (but not on the Stratus)

* Download the [stratus.cpp](./stratus.cpp) file to a conveniently located local place
* Design and test your pipeline in the [Faust IDE](https://faustide.grame.fr/)
  * Represent Stratus knobs using `vslider` and/or `hslider`
  * ORDER your sliders by using a label with the syntax `[idx]MySlider` where idx is a non-negative
    integer. The actual label does not affect what your kob is called in the Stratus app is called. 
	Stratus knobs themselves are numbered left-to-right, top-to-bottom.
  * Stratus switches are not yet supported.
* Export the `.dsp` code from the IDE to a local file
* Generate the full C++ code using the Faust compiler:

   ```
   faust -a [where you put the architecture file]/stratus.cpp [path to the DSP file] -o [what you want the C++ file to be called]
   ```

* Copy the generated C++ file to the Stratus
* Compile that as described on the Stratus developers portal
* Do whatever else to get the effect loaded into the Stratus and testable (again, see the developer portal)

## Using the Python wrapper
If you want to use the [Python interface](./stratus.py) to test the effect without having to load it up in 
the pedal itself, you will need to add the `-DCINTERFACE` option to the C++ compile command line - and, 
of course, if that's what you are doing, you don't even need to compile the C++ code on the Stratus - just 
compile it locally to test it and only compile it on the Stratus itself when you are happy that it works.
