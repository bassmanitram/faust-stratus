# faust-stratus
A toolkit that simplifies the use of Faust in designing algorithms to be used for
Chaos Stratus pedal effects.

Your most important resources:

* [Faust](https://faust.grame.fr/) - [do the YouTube workshops!](https://ccrma.stanford.edu/~rmichon/faustWorkshops/course2015)
* [Chaos Stratus Developers Portal](https://github.com/chaosaudio/Dev-Portal)

## Installation
Download the latest version of the SDK bundle to your local computer (NOT the Stratus pedal)
and un-zip it into a folder of your choosing.

You can install this SDK in either of two places - or both!

* Locally, on your standard computer.
  * You must have Faust installed on the same computer
  * To compile locally you also need C++ build tools installed
  * Use the [install-local](./install-local) command to install the Stratus-related components
  * After this you can follow the [Classic](#classic-mode) development life cycle for your Faust-designed Stratus effects libraries

* "Embedded" into your Stratus.
  * You must know the Stratus root password in order to do this
  * You must plug your Stratus into the computer via a USB C connection
  * Use the [install-sdk](./install-sdk) command to install Faust and the Stratus-related components onto the Stratus
  * After this you can follow the [Embedded](#embedded-mode) development life cycle for your Faust-designed Stratus effects libraries

## Usage

### Design in Faust
Obviously, since this is a toolkit to get Faust-designed effect algorithms working on the Chaos Stratus
pedal with as little fuss as possible, your gonna have to design your effect algorithm in Faust.

By FAR the easiest way to do this is using the [online IDE](https://faustide.grame.fr/), test there 
with sample audio extracts, then copy the DSP code to your computer (not the Stratus ... yet).

I'm not going to detail that process - Faust itself has some very complete doc, and some very useful online 
workshops, that will help you - _but_ there are certain principles that will help you build a Faust DSP
file that is easily adapted to the Stratus:

* **You get one input signal and must produce only one output signal**
  If you don't know what that means, read the Faust doc - but the Stratus is a mono pedal.

  _Within_ the algorithm you can split the signal any number of times you need, but you
  MUST merge it all back to one signal on output. 
  
  Here's a "blend" pattern that might help explain that:

  ```
  ...
  blend = vslider("[1]Blend", 0.5, 0, 1, 0.01);
  weird_effect = ...;

  // Splits the single input signal in two, applies the weird effect to one, scales the two signals
  // based upon the blend control such that blend = 0 gives only clean, blend = 1 gives only weird, 
  // and anything in between ... well, blends the two appropriately - and finally merges the internal
  // signals back to one
  process = _ <: (weird_effect : *(blend)),*(1-blend) :> _;
  ```

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

* **Represent Stratus switches using either `checkbox`, or `nvalue`** 
  While a rarer component of a Stratus UI, switches are also supported by this toolkit. Stratus supports
  both 2-state (on/off) and 3-state (on/mid/off) switches. Technically this toolkit doesn't distinguish 
  between the two - it's up to the Stratus firmware to use the switches correctly.
  
  To represent switches in the Faust IDE UI and the Faust DSP file, you can use:

  * `checkbox`, or
  * `nvalue`, with a minimum value of 0, a maximum value of 1 or 2, and a step value of 1.

  Again, organize them as you will in the Faust IDE UI, but only these two controls will be recognized

* **ORDER your sliders and switches correctly according to your Stratus UI**
  Without some hinting, Faust will "order" your controls in the code in alphabetical order by label.

  This is highly unlikely to be what you want.

  However, Faust _does_ take hints that cause the controls to be programmatically declared in the
  order you want no matter their label or how they appear in the IDE. You do this via the label
  (so, er, perhaps "...no matter their label...", above, is a little misleading :)). 
  
  Such a hint has the syntax:

  ```
  vslider("[0]This is my totally ignored label for knob 0", ...)
  ```

  With (evidently) the `[n]` bit providing the ordering you want. 
  
  In case it's not obvious, `n` should be a non-negative integer, but the `n` value itself doesn't 
  actually matter to this toolkit - only the ordering that it enforces is taken into account.
 
  Note that Stratus knobs themselves are numbered left-to-right, top-to-bottom. Switches too.
  Note also that within the toolkit the ordering of knobs is independent of the type of slider
  you declared to represent the knob, and the same for switches - _and_ that the orderings of
  knobs and of switches are independent of each other.

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

There are two modes of building - "Classic" and "Embedded". Embedded
will, HOPEFULLY become _the_ way one day - because it involves installing all the tools
on the Stratus, and, of course, since THAT is possible (and they don't take up much space) there is
no reason why they shouldn't become part of the default firmware in the future. For now, though, it
is separate - and I bear no responsibility if you break your Stratus!

### Classic mode
Classic mode means you ran `install-local` but didn't run `install-sdk` - OK, that's up to you, you
don't wanna take risks, I understand!

Actually "Classic" mode is perfectly appropriate, especially if your algorithm, as generated by 
the Faust compiler, isn't exactly what you want - i.e. you need to tweak the C++ code.

Whatever your reasons, this is the workflow for classic mode (remembering that it implies that
you have Faust installed on your computer):

* Generate the full C++ code using the Faust compiler:

   ```
   faust2Stratus [path to your DSP file] <any other options for the Faust compiler>
   ```

* Massage the algorithm in the generated C++ code at will if you need to
* Copy the generated C++ file (found in the same folder as the DSP file) to the Stratus
* Compile _that_ as described on the Stratus developers portal
* Do whatever else to get the effect loaded into the Stratus and testable.
  Basically that means copy the built `.so` file to the `/opt/firmware/effects` folder, 
  giving it a name that matches the UUID of the UI that will use it, and creating/updating
  its corresponding version file as necessary.

**NOTE** that the default Faust compiler options are perfectly appropriate - allowing you to add options
is a nice flexibility feature but for most use cases unnecessary.

If you have C++ build tools installed on your local computer, you will also get a shared library generated -
which is pretty useless on your computer - UNLESS you opt into the C interface in the build...

   ```
   faust2Stratus [path to your DSP file] --stratusc <any other options for the Faust compiler>
   ```

If you do THAT then you can use the installed Python interface to test your effect library outside of the Stratus -
see the [tester.py](./tests/tester.py) file for a basic example of using that interface.

### Embedded mode

Embedded mode means you don't need Faust or build tools on your computer - everything is on the Stratus. If you
trust us, this is easiest mode - but it doesn't allow you to modify the C++ code generated by the
Faust compiler before that is compiled and, optionally, installed

To recap, you now have a DSP file on your local computer. Since you have installed the SDK on the Stratus, 
all you need to do to get the effect implementation onto the stratus, AND to get local backups of the 
artifacts generated by the build process, is to run the command:

```
./sdk-build-effect <the-location--of-your-DSP-file> <any other options for the Faust compiler>
```

**NOTE** that the default Faust compiler options are perfectly appropriate - allowing you to add options
is a nice flexibility feature but for most use cases unnecessary.

Again, you'll need to have the Stratus connected to the computer by USB, and you'll need to 
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
* _and_ you run the `./sdk-build-effect` command like THIS:

  ```
  ./sdk-build-effect <the-location--of-your-DSP-file> --stratusinstall <any other options for the Faust compiler>
  ```

If you do _that_ then you are home free - the effect will be installed in the proper folder with a proper
version file - and any existing effect files for the same UUID will have been backed up in the `/root` folder
(it's up to _you_ to clean those up occasionally).

## Using the Python wrapper
If you want to use the [Python interface](./stratus.py) to test the effect without having to load it up in 
the pedal itself, you simply need to add the `--stratusc` option to the `faust2stratus` command line.

The python interface is installed on the Stratus by the `install-sdk` command, so you can even do that
on the Stratus itself (it has Python installed already).

But this is probably of more use in a local build, where you can devise tests for your effect that closely
emulate how the Stratus will interact with your effect without actually having to build the UI yet.

See the delivered [test script](../tester.py) for examples of how this can be done.