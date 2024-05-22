# faust-stratus
A toolkit that simplifies the use of Faust in designing algorithms to be used for
Chaos Stratus pedal effects.

Your most important resources:

* [Faust](https://faust.grame.fr/) - [do the YouTube workshops!](https://ccrma.stanford.edu/~rmichon/faustWorkshops/course2015)
* [Chaos Stratus Developers Portal](https://github.com/chaosaudio/Dev-Portal)

For now, you'll need to be using Linux to have the tools work for you, though there is no reason
why, in the future, we can't get the tools running on Windows and Mac. (Note, this has, in fact, been
tested on a Mac and appears to work! And Windows has [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) -
so, perhaps we are OK!)

The Linux-preference has sense, by the way: If you generate CPP from Faust on Linux then it will reference standard
Linux libraries. If you _then_ modify the code, it should be still be direct compilable and usable on the Stratus.

## Installation
Download the [latest version of the SDK bundle](../../releases/download/v0.4.1/faust-stratus-v0.4.1.zip) to your local computer 
(NOT the Stratus pedal) and un-zip it into a folder of your choosing.

You can install this SDK in either of two places - or both!

* Locally, on your standard computer.
  * You must have Faust installed on the same computer
  * To compile locally you also need C++ build tools installed
  * Use the [install-local](./srcs/install/install-local) command to install the Stratus-related components
  * After this you can follow the [Classic](#classic-mode) development life cycle for your Faust-designed Stratus effects libraries

* "Embedded" into your Stratus.
  * You must know the Stratus root password in order to do this
  * You must plug your Stratus into the computer via a USB C connection
  * Use the [install-stratus](./srcs/install/install-stratus) command to install Faust and the Stratus-related components onto the Stratus
  * After this you can follow the [Embedded](#embedded-mode) development life cycle for your Faust-designed Stratus effects libraries

## Usage

The usage is based on a few principles:

* Faust has good patterns for integrating `faust2something` build scripts and what Faust calls "architecture wrappers" - we exploit that to the full.
* There are simple effects and complex effects - simple effects can generally be fully described in Faust and built for use with the Stratus
  without any complications. Complex effects may require "tweaking" the CPP code generated by the Faust compiler before they are fully
  compatible with the Stratus. This toolkit allows for both scenarios, but, obviously, the latter requires more knowledge and intervention
  from the effect designer.
* "Local" development - i.e. building and testing effects on your "normal" computer is a great way to speed up the development cycle. For that
  the toolkit allows you to pass arbitrary sound sources through your effect and hear the results without having to load up the effect
  on the Stratus itself.
* On the other hand, once you are happy, you really do want your effect compiled and installed on the Stratus with the minimum of fuss. The
  toolkit facilitates that too.

### The `faust2stratus` command
As part of _both_ installation routes, a command named `faust2stratus` is installed into the Faust binary folder. This is the core of getting
your DSPs compiled for the Stratus.

The basic command line invocation is as follows:

```
faust2stratus <the location of your DSP file>
```

This will take your DSP file and

* Invoke the `Faust` command with the correct options for the compiler to generate CPP code suitable for use in the Stratus
* Compile that CPP code for the platform upon which the command is being executed, generating a shared library that implements the 
  Stratus binary interface.
* Save the CPP file and the shared library in the folder from which the originating DSP file was read.

If you run that on your "normal" computer, you will end up with a shared library that can be tested using the Python wrapper that
was also installed by the installation process. _But that shared library will not work on the Stratus_.

If, however, you run that on Stratus, you will end up with a shared library that DOES work on the Stratus and can be loaded by the
Stratus engine as an effect library.

#### Options
The `faust2stratus` command has a number of Stratus-specific options that you can add on the command line:

* **-nocppc** - this will skip the CPP compile step, only generating the Stratus-compatible CPP code.
* **-stratusinstall** - if the build is happening on the Stratus itself, _and_ you have followed the guidelines for DSP file structure
  described below, then the script will take the generated shared library and install it into the correct place under the correct
  name for immediate use in the Stratus. If you are currently using the effect in the active effects chain of the Stratus, you will 
  be alerted to this by the Stratus itself going through a short upgrade cycle accompanied by the standard flashing lights.

Apart from these options, you can add other Faust compiler options to the command line. This may be interesting if you are experimenting
with different CPP code generation strategies that may better suit your algorithm (e.g. improved vectorization).

Finally, you can affect the CPP compile itself by adding CPP compiler options to the `CXXFLAGS` environment variable before
running the command. For example, some users have reported that the following extra CPP optimization options make for faster-running
algorithms. You would run _this_ command before the build command in order to pick these options up:

```
export CXXFLAGS="-fprefetch-loop-arrays -funroll-loops -funsafe-loop-optimizations -fno-finite-math-only"
```

### The `stratus-build-effect` command
The `stratus-build-effect` command is simply a wrapper for the `faust2stratus` command that executes `faust2stratus` on your Stratus
pedal from your normal computer. It accepts the same `faust2stratus` arguments described above.

To use this command you must:

* Have installed the toolkit on the Stratus (`install-stratus`)
* Know the root password of the Stratus (if you installed the toolkit, you already know that)
* Have your Stratus connected to your normal computer via USB when you execute the command.

### Designing Effects in Faust
Obviously, since this is a toolkit to get Faust-designed effect algorithms working on the Chaos Stratus
pedal with as little fuss as possible, your gonna have to design your effect algorithm in Faust.

By FAR the easiest way to do this is using the [online IDE](https://faustide.grame.fr/), test there 
with sample audio extracts, then copy the DSP code to your computer (not the Stratus ... yet).

I'm not going to detail that process - Faust itself has some very complete doc, and some very useful online 
workshops, that will help you - _but_ there are certain principles that will help you build a Faust DSP
file that is easily adapted to the Stratus:

* **Represent Stratus knobs using `vslider` and/or `hslider`**. 

  This toolkit will recognize `hslider` and `vslider` UI components as Stratus *knobs*. For a nice UI
  in the Faust IDE you can add as many organizing containers as you want, but they will have no effect 
  on the final representation. Neither will the labels you give your knobs.

  Your parameterization of the sliders should match that which you will declare in the Stratus UI (not
  part of this toolkit). If you are using the tester "9 KNOB" effect, you are stuck with nine knobs 
  with the following parameterization:

  * Minimum value of 0
  * Maximum value of 10 (I tried for 11 in recognition of Spinal Tap, but Landon said no!)
  * Step value of 0.1

  In this situation, where the UI is a given but the values don't exactly match what you might want, you need
  to apply the necessary scaling of the values in your algorithm. When you move to a UI that is dedicated
  to your effect, you can have the knobs defined as you want and can remove the algorithmic scaling.

* **Represent Stratus switches using `button`, `checkbox`, or `nvalue`** 

  While a rarer component of a Stratus UI, switches are also supported by this toolkit. Stratus supports
  both 2-state (on/off) and 3-state (on/mid/off) switches. Technically this toolkit doesn't distinguish 
  between the two - it's up to the Stratus firmware to use the switches correctly.
  
  To represent switches in the Faust IDE UI and the Faust DSP file, you can use:

  * `button`, or
  * `checkbox`, or
  * `nvalue`, with a minimum value of 0, a maximum value of 1 or 2, and a step value of 1.

  Again, organize them as you will in the Faust IDE UI, but only these controls will be recognized

* **Mark your sliders and switches correctly according to your Stratus UI**

  As-is, your sliders, buttons, checkboxes, and/or nvalues will **not** be "attached" to the equivalent
  knobs and switches in the Stratus UI.

  To do _that_ you must exploit a feature of Faust UI labels - adding metadata.

  This is how you do it:

  ```
  vslider("This is my totally ignored label for my first knob[stratus:0]", ...)
  ```

  More generally, you add the `stratus` key with a value between 0 and 9 
  for knobs, and between 0 and 4 for switches (if you aren't familiar with
  zero-based indices, welcome to that world!).

  Obviously, those indices indicate to which control on the Stratus effect UI 
  a specific Faust control pertains. Stratus knobs themselves are numbered 
  left-to-right, top-to-bottom. Switches too.

  If you try to assign the same control number to two different Faust controls
  then the second control will be ignored (here I mean two different controls 
  of the same "class" - knob and switch numbering are independent of each other).

* **You get one input signal and must produce only one output signal**

  If you don't know what that means, read the Faust doc - but the Stratus is a mono pedal.

  _Within_ the algorithm you can split the signal any number of times you need, but you
  MUST merge it all back to one signal on output. 
  
  Here's a "blend" pattern that might help explain that:

  ```
  ...
  // The blend control is the SECOND knob on the Stratus UI
  blend = vslider("Blend[stratus:1]", 0.5, 0, 1, 0.01);
  weird_effect = ...;

  // Splits the single input signal in two, applies the weird effect to one, scales the two signals
  // based upon the blend control such that blend = 0 gives only clean, blend = 1 gives only weird, 
  // and anything in between ... well, blends the two appropriately - and finally merges the internal
  // signals back to one
  process = _ <: (weird_effect : *(blend)),*(1-blend) :> _;
  ```

* **Declare the UUID of your Stratus effect in the DSP code**

  That is, if you want it properly installed on the pedal. You do it like this (preferably at 
  the top of your DSP code, but technically it doesn't really matter where):

  ```
  declare stratusId "01234567-89ab-cdef-0123-456789abcdef"
  ```

  For the "9 KNOB" test effect, use `55631e3a-94f7-42f8-8204-f5c6c11c4a21`.

* **Declare the version of your Stratus effect in the DSP code**

  That is, if you want it properly installed on the pedal. You do it like this (preferably at 
  the top of your DSP code, but technically it doesn't really matter where):

  ```
  declare version "0.1.0"
  ```

So, now you have your Stratus-compatible DSP file - get it onto your computer (the one
where you originally unzipped this toolkit) and build away!

### Building

As discussed above, building an effect is based around the `faust2stratus` command installed
wherever you need to build your effect.

But, given that, we can roughly divide the subject of building into two modes - "Classic" and "Embedded".

#### Classic mode
Classic mode means you ran `install-local` but didn't (necessarily) run `install-stratus`.

"Classic" mode is perfectly appropriate, especially if your algorithm, as generated by 
the Faust compiler, isn't exactly what you want - i.e. you need to tweak the C++ code.

Whatever your reasons, this is the workflow for classic mode (remembering that it implies that
you have Faust installed on your computer):

* Generate the full C++ code using the Faust compiler:

   ```
   faust2Stratus [path to your DSP file] -nocppc <any optional flags for the Faust compiler>
   ```

* Massage the algorithm in the generated C++ code at will if you need to
* Copy the generated C++ file (found in the same folder as the DSP file) to the Stratus
* Compile _that_ as described on the Stratus developers portal
* Do whatever else to get the effect loaded into the Stratus and testable.
  
  Basically that means copy the built `.so` file to the `/opt/update/sftp/firmware/effects` folder, 
  giving it a name that matches the UUID of the UI that will use it, and creating/updating
  its corresponding version file as necessary.

  If you are already using an effect that you are updating, your Stratus should flash red and yellow 
  for a while - _don't panic_ - that's a good sign - it's updating your current chain to use the
  new effect library!

If you have C++ build tools installed on your local computer, you can also get a shared library generated 
that can be used on your local computer - which is useful for _testing_ your effect without 
having to install it to the pedal. To do that, omit the `-nocppc` option from the `faust2stratus` command line.

See [below](#using-the-python-wrapper) for usage notes.

#### Embedded mode

Embedded mode means you don't need Faust or build tools on your computer - everything is on the Stratus. 
This is the easiest mode - but it doesn't easily allow you to modify the C++ code generated by the
Faust compiler before that is compiled and, optionally, installed

To recap, you now have a DSP file on your local computer. Since you have installed the SDK on the Stratus, 
all you need to do to get the effect implementation onto the Stratus, AND to get local backups of the 
artifacts generated by the build process, is to run the command:

```
stratus-build-effect <the-location-of-your-DSP-file> <any optional options for the Faust compiler>
```

You'll need to have the Stratus connected to the computer by USB, and you'll need to 
enter the Stratus password when prompted.

If THAT works, you'll end up with a C++ file and a shared library object in the same
directory as your original DSP file, and in the `/root` directory on the Stratus.

(However, note that, unless your local machine is an arm-v7/cortex-8a system, the generated `so`
file will not work on your local machine - to have one that works locally, follow the
first few steps of the [classic build](#classic-mode) process, above).

At this point you need to follow the installation steps set out for Classic mode above...

**_UNLESS_**...

* You have followed the Stratus design principles (introduced above) in your DSP file (hint:
  particularly the `declare`s)
* _and_ you run the `sdk-build-effect` command like THIS:

  ```
  ./sdk-build-effect <the-location-of-your-DSP-file> -stratusinstall <any optional options for the Faust compiler>
  ```

If you do _that_ then you are home free - the effect will be installed in the proper folder with a proper
version file - and any existing effect files for the same UUID will have been backed up in the `/root` folder
(it's up to _you_ to clean those up occasionally).

If you are already using an effect that you are updating, your Stratus should flash red and yellow 
for a while - _don't panic_ - that's a good sign - it's updating your current chain to use the
new effect library!

## Using the Python wrapper
The Python interface is installed locally by the `install-local` command, and on the Stratus by the 
`install-stratus` command.

This is very useful for devising tests for your effect that closely emulate how the Stratus 
will interact with your effect without actually having to build the UI yet. In particular, you
can build the effect _locally_ and test it with Python without even requiring you Stratus pedal
to be accessed.

You don't have to do anything special to get the Python interface to work - the effect libraries
generated by the toolkit are by default compatible with the Python wrapper

See the delivered [test script](.tests/tester.py) for examples of how this can be used.