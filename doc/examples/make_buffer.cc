// can be any other gate with numeric value_type
scrubb::channel<double> input_object;

//
// now make buffer  with the input object.
// template parameter I can be omitted due to type deduction.
//
auto buf = scrubb::make_buffer(input_object);
//

//
// The following calls are equivalent to the above, use whichever you prefer.
// Attention, however; this is not true for every gate in the library and only
// possible if your compiler is fully C++17 compliant (e.g. g++ 7 or later).
//
auto buf = scrubb::buffer(input_object);
//
scrubb::buffer buf{input_object_1};
//

// call `value()` to use
auto res = buf.value();  // retrieves value from input object
//
auto next = buf.value();  // second call reproduces earlier retrieved value
//
buf.reset();  // resets the buffer
auto last = buf.value();  // this call again retrieves value from input object


//
// LEGACY
//
// The following is also possible, however not preferable - thus the
// make_accumulator function.
scrubb::buffer<decltype(input_object)> buf{input_object};
//
