//
// can be any other gate with numeric value_type
scrubb::channel<double> input_object_1;
scrubb::channel<double> input_object_2;
//

//
// now make approximate object with tolerance of 3.0.
// template parameters can be omitted due to type deduction.
//
auto approx = scrubb::make_approximate(input_object_1, input_object_2, 3.0);
//

//
// The following calls are equivalent to the above, use whichever you prefer.
// Attention, however; this is not true for every gate in the library and only
// possible if your compiler is fully C++17 compliant (e.g. g++ 7 or later).
//
auto approx = scrubb::approximate(input_object_1, input_object_2, 3.0);
//
scrubb::approximate approx{input_object_1, input_object_2, 3.0};
//

// call `value()` to use
bool res = approx.value();  // (distance between input objects values') < 3.0


//
// LEGACY
//
// The following is also possible, however not preferable - thus the
// make_approximate function.
scrubb::approximate<decltype(input_object_1), decltype(input_object_2)> approx{
  input_object_1, input_object_2, 3.0};
//

