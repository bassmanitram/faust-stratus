//
// MASSIVE epiphany - these programs are just applying algorithms to buffers of floats!
//
// While most of those algorithms will get extremely complex, whereby the semantics of 
// Faust really helps model them, to test the Stratus effect interface we don't need
// anything huge - or even anything that represents real sound! We just need say 10 
// hardcoded floats and a simple mathematical operation that produces an easily 
// derived (hardcoded) output to check against! 
//
// To make sure we have modeled knobs and switches correctly, we'll include one of each
// but effectively all this does is multiply the input by an easily derived factor! 
//
factor = vslider("Factor", 0, -2, 2, 1);
on_off = checkbox("On/Off");

process = *(factor * on_off);