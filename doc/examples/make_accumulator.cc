//
// can be any other gate with numeric value_type
scrubb::channel<double> input_object;
//

//
// now make accumulator of size 5 with the input object.
// template parameter I can be omitted due to type deduction.
//
auto acc = scrubb::make_accumulator<5>(input_object);

// call `value()` to use
auto res = acc.value();  // sum of last five values of `input_object`


//
// LEGACY
//
// The following is also possible, however not preferable - thus the
// make_accumulator function.
scrubb::accumulator<decltype(input_object), 5> acc{input_object};
//
