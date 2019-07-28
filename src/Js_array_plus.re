open Js_array;

let init = (length, fn) => {
  let rec _make = (l: int, v: int => 'a, a: array('a)): array('a) =>
    switch (l) {
    | 0 => a
    | l_ => _make(l_ - 1, v, concat(a, [|v(l_ - 1)|]))
    };

  _make(length, fn, [||]);
};

let make = (length, value) => init(length, _ => value);

let init_matrix = (dimx, dimy, fn) =>
  init(dimy, y => init(dimx, x => fn((x, y))));

let make_matrix = (dimx, dimy, value) => init_matrix(dimx, dimy, _ => value);
