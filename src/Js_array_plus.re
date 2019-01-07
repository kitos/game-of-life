open Js_array;

let init = (length, fn) => {
  let rec _make = (l: int, v: _ => 'a, a: array('a)): array('a) =>
    switch (l) {
    | 0 => a
    | l_ => _make(l_ - 1, v, concat(a, [|v()|]))
    };

  _make(length, fn, [||]);
};

let make = (length, value) => init(length, () => value);

let init_matrix = (dimx, dimy, fn) => init(dimx, () => init(dimy, fn));

let make_matrix = (dimx, dimy, value) => init_matrix(dimx, dimy, () => value);
