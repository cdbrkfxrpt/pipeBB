//
// initialize channel object using default constructor.
// name and unit will be empty strings, factor will be 1 and offset will be 0.
scrubb::channel<double> input_object_def;
//

//
// initialize channel object using name only
// unit will be an empty string, factor will be 1 and offset will be 0.
scrubb::channel<uint32_t> input_object_name{"input_object"};
//

//
// fully initialize channel object.
scrubb::channel<float> input_object_full{"full", "mm", 1.5, -5.0};
//

//
// now use it in your code:
while (true) { input_object_def << value_read_function(GPIO_PIN42); }
//
